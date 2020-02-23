//------------------------------------------------------------------------------

/******************************************************************************/
                                                                            
#include  <stdio.h>
#include  <stdlib.h>  
#include  <string.h>
#include  <math.h>
#include  <time.h>  
#include  <unistd.h> 
#include  <ctype.h>  
#include  <sys/time.h> 
#include  <sys/param.h>
                                                                             
/******************************************************************************/

#include "a_comm.h"
#include "d_find.h"

#include "x_hash.h"
#include "x_comm.h"
#include "x_work.h"
#include "x_game.h"

//------------------------------------------------------------------------------
#include "tcl.h"

//------------------------------------------------------------------------------

//GO_GAME  *game;
//int       /* gamer_black, */ gamer_white;
GO_GAMER *gamer_w;
YT_GOBAN *goban;
//------------------------------------------------------------------------------
/*
  Следующее определение необходимо для всех интерфейсных функций
  (которые вызываются непосредственно из интерпретатора Tcl)
*/
#ifdef WIN32
#define DllExport __declspec( dllexport )
#else
#define DllExport
#endif

#ifdef __cplusplus
#define EXPFUN extern "C" DllExport
#else
#define EXPFUN extern DllExport
#endif

//------------------------------------------------------------------------------
/*
 Далее описаны все экспортируемые процедуры, т.е. процедура
 инициализации пакета и командные процедуры.
*/
EXPFUN int Demo_Init (Tcl_Interp *interp);
EXPFUN int demoCmdProc (ClientData cData, Tcl_Interp *interp, int argc,	char *argv[]);

//------------------------------------------------------------------------------
/* For Tcl 8.4 */
//./Scruzh/x_.tcl.c:147: warning: passing arg 3 of `Tcl_CreateCommand' from incompatible pointer type

//#define _CONST84  
#define _CONST84 CONST84

//------------------------------------------------------------------------------
//  Пример командной процедуры.
//  Выдает в качестве результата строку вида:  demo_cmd: arg1 arg2
//------------------------------------------------------------------------------
int 
demo_cmd_proc (ClientData cData, Tcl_Interp *interp, int argc, 
                _CONST84 char *argv[])
{
  char buf[80];

  printf ("====== \n");

  if (argc != 3) {
    sprintf (buf, "usage: %s arg1 arg2", argv[0]);
    Tcl_SetResult (interp, buf, TCL_VOLATILE);

    return TCL_ERROR;
  }

  sprintf (buf, "%s: %s %s", argv[0], argv[1], argv[2]);
  Tcl_SetResult (interp, buf, TCL_VOLATILE);

  return TCL_OK;
}
//------------------------------------------------------------------------------
int 
aigo_init_proc (ClientData cData, Tcl_Interp *interp, int argc, _CONST84 char *argv[])
{
  int ii, jj;
  char buf[80];

  printf ("aigo_init \n");
  YRAND_S;

  if (argc != 3) {
    printf ("%s - argc= %d ii= %s jj=%s \n", argv[0], argc, argv[1], argv[2]);
    exit (0);
    //sprintf (buf, "usage: aigo_init 6 6");
    //Tcl_SetResult (interp, buf, TCL_VOLATILE);
    //return TCL_ERROR;
  }
  ii = atoi (argv[1]);
  jj = atoi (argv[2]);

  //game =  game_begin_ (ii, jj);
  goban = goban_create (ii, jj);
  //gamer_white = game_create_gamer_ (game, "aigo2", 0, 0, STONE_WHITE);
  gamer_w = gamer_make ("aigo2", 0, 0, STONE_WHITE, /* game-> */goban);

  Tcl_SetResult (interp, /* buf */"aigo was init !!", TCL_VOLATILE);
  return TCL_OK;
}
//------------------------------------------------------------------------------
int 
aigo_move_proc (ClientData cData, Tcl_Interp *interp, int argc, _CONST84 char *argv[])
{
  char buf[80];

  int i = atoi (argv[1]);
  int j = atoi (argv[2]);
  printf ("aigo_move %d %d \n", i, j);
  //sleep (1);
  goban_set_move (goban, i, j, STONE_BLACK);

  int i_move, j_move;

  gamer_get_move (gamer_w, &i_move, &j_move);
  goban_set_move (goban, i_move, j_move, /* stone */STONE_WHITE);

  sprintf (buf, "%d %d", i_move, j_move);
  Tcl_SetResult (interp, buf, TCL_VOLATILE);

  return TCL_OK;
}
//------------------------------------------------------------------------------
//  Процедура инициализации библиотеки.
//  Добавляет новые команды в интерпретатор Tcl.
//  Имя процедуры ДОЛЖНО иметь вид "Пакет_Init".
//------------------------------------------------------------------------------
int 
Demo_Init (Tcl_Interp *interp)
{
  //Tcl_CreateCommand (interp, "demo_cmd", demo_cmd_proc, (ClientData)0, (Tcl_CmdDeleteProc *)0);

  //Tcl_CmdProc  *proc
  Tcl_CreateCommand (interp, "demo_cmd", demo_cmd_proc, NULL, NULL);

  Tcl_CreateCommand (interp, "aigo_init", aigo_init_proc, NULL, NULL);
  Tcl_CreateCommand (interp, "aigo_move", aigo_move_proc, NULL, NULL);

  //aigo_init_proc ();

  interp->result = "Demo lib was successfully loaded";
  return TCL_OK;
}
//------------------------------------------------------------------------------
