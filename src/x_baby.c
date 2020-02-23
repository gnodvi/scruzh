/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: x_baby.c                                                  *
 *                                                                             *
  ******************************************************************************
 */ 


/******************************************************************************/

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

//-----------------------------------------
                                                        
#include "a_comm.h"
#include "d_find.h"

#include "x_comm.h"
#include "x_hash.h"
#include "x_work.h"
#include "x_game.h"

/******************************************************************************/   

typedef struct {
  int       ii, jj;
  YT_GOBAN *goban;
  double    rating, rat;

  GO_GAMER *gamer_b, *gamer_w;
  int       game_end;
} YT_BABY;

YT_BABY *pa;

void  rating_read (YT_BABY *pa) ;
void  rating_write (double rating); 
void  rating_plus (); 
void  rating_minus (); 
void  set_rating_handicap (double rating); 

int   human_move (int i, int j) ;
int   computer_move (int stone) ;

/******************************************************************************/   

void      baby_begin (int ii, int jj);
//void      baby_set_rating (int rat); 
void      baby_newgame ();

char     *baby_text_caption (); 
char     *baby_text_status (); 
char     *baby_text_rating ();
GO_BOARD *baby_ptr_goban(); 

void      baby_menu_newgame (YT_BOOL); 
void      baby_menu_exit (); 
void      baby_menu_ratingset (double rating); 

YT_BOOL   baby_is_game_end ();
void      baby_onestep_moves (int i, int j); 


//****************************************************************************

  char *main_caption    = "  Т Е S Т  ";
  char *main_status     = "Process ...";
  char *main_youwins    = "Win ! ";
  char *main_youlose    = "Not ..";

char text_raiting[80];
char text_status [80];

#define SPRINT_RATING (sprintf (text_raiting, "%+3.1f",  rating_get ()))
#define SPRINT_STATUS(stat) (sprintf (text_status,  "%s",  (stat)))

/******************************************************************************/
//  
/*----------------------------------------------------------------------------*/
void  
set_rating_handicap (double rating) 
{
  int h, handicap, i, j;
  YT_GOBAN *goban = pa->goban;

  handicap = (int) (fabs(rating));

  if (rating >= 0) { // первый ход - компутер
    for (h = 0; h < handicap; h++) {
      //computer_move (); 
      //if (goban_random_stone_new (goban, &i, &j))
      if (goban_random_handicap (goban, handicap, &i, &j))
        goban_set_move (goban, i, j, STONE_WHITE);
    }
  ////////////
  } else {
  ////////////
    for (h = 0; h < handicap; h++) {
      //if (goban_random_stone_new (goban, &i, &j)) // не сработает только, если нет места
      if (goban_random_handicap (goban, handicap, &i, &j)) // не сработает только, если нет места
        human_move (i, j); 
    }
    computer_move (STONE_WHITE); 
  }

  return;
}
/*----------------------------------------------------------------------------*/
void  
baby_menu_newgame (YT_BOOL is_rand) 
{

  if (is_rand) YRAND_S;  

  if (!pa->game_end) 
    rating_minus (); 

  pa->goban = goban_create (pa->ii, pa->jj);

  pa->gamer_b = gamer_make ("baby1", 0, 0, STONE_BLACK/*??*/, pa->goban);
  pa->gamer_w = gamer_make ("aigo2", 0, 0, STONE_WHITE, pa->goban);

  pa->game_end = FALSE;
  SPRINT_STATUS (main_status);

  set_rating_handicap (pa->rating);  

  return;
}
/*----------------------------------------------------------------------------*/
void
baby_begin (int ii, int jj) 
{

  pa = (YT_BABY*) malloc (sizeof (YT_BABY));

  rating_read (pa);
  pa->rat = /* 0.25 */0.5;

  pa->ii    = ii;
  pa->jj    = jj;

  pa->game_end = TRUE;
  
  return;
}
/*----------------------------------------------------------------------------*/
void 
baby_menu_ratingset (double rating) 
{

  pa->rating = rating; 

}
/*----------------------------------------------------------------------------*/
double 
rating_get () 
{

  return (pa->rating); 

}
/*----------------------------------------------------------------------------*/
void 
draw_status (char *status) 
{
  if (status == NULL) return; 

  SPRINT_STATUS (status);

  return;
}
/*----------------------------------------------------------------------------*/
int
human_move (int i, int j) 
{
  int  ret;

  if (pa->game_end) return (FALSE); //??

  // ход человека
  ret = goban_set_move (pa->goban, i, j, STONE_BLACK);

  if (!ret) {
    return (FALSE);
  }

  return (TRUE);
}
/*----------------------------------------------------------------------------*/
int 
computer_move (int stone) 
{

  if (pa->game_end) return FALSE;

  GO_GAMER *gamer;
  if (stone == STONE_WHITE) gamer = pa->gamer_w;
  else                      gamer = pa->gamer_b;

  // ход компутера
  if (!gamer_todo_move (gamer, stone)) {
    //?? TRUE !!
    draw_status ("сдались !");
    pa->game_end = TRUE;
    //pa->rating  += pa->rat; 
    rating_plus (); 

    return FALSE;
  }

  return TRUE;
}
/*----------------------------------------------------------------------------*/
int 
update_check_end () 
{
  int       b_cap, w_cap;
  char     *mes;

  YT_GOBAN *goban = pa->goban;
  GO_WORK  *work  = (GO_WORK*) goban->mywork;
  int old_move = STONE_NEXT(goban->posi->stone);

  work_get_captures (work, TRUE, &b_cap, &w_cap, old_move);

  if  (b_cap==0 && w_cap==0)
    return FALSE;

  // есть захват - конец игре!
  if      (b_cap>0)  {
    rating_plus (); 
    mes = main_youwins;
  }
  else if (w_cap>0)  {
    rating_minus () ;
    mes = main_youlose;
  }
  else YERROR ("update_check_end");

  draw_status (mes);
  pa->game_end = TRUE;

  return TRUE;
}
/*----------------------------------------------------------------------------*/
void  
baby_onestep_moves (int i, int j) 
{

  if (i == -1 && j == -1) {
    // комп играет сам с собой, т.е. делает ход за человека
    if (! computer_move (STONE_BLACK)) return; 
  }
  else {
    if (! human_move (i, j)) {return; OUTD (555);}
  }

  if (update_check_end ()) return;

  //draw_status ("Я думаю.."); 
  if (! computer_move (STONE_WHITE)) return; 

  if (update_check_end ()) return;
  //draw_status ("Ваш ход.."); 

  return;
}
/*----------------------------------------------------------------------------*/
char* 
baby_text_caption () 
{ 

  return (main_caption);
}
/*----------------------------------------------------------------------------*/
char* 
baby_text_status () 
{ 

  return (text_status);
}
/*----------------------------------------------------------------------------*/
char* 
baby_text_rating () 
{ 

  SPRINT_RATING;

  return (text_raiting);
}
/*----------------------------------------------------------------------------*/
GO_BOARD *
baby_ptr_goban() 
{ 
  YT_GOBAN *goban = pa->goban;

  return (goban->posi->a);
}
/*----------------------------------------------------------------------------*/
YT_BOOL 
baby_is_game_end () 
{

  return (pa->game_end);

}
/******************************************************************************/
// 
/******************************************************************************/

/*----------------------------------------------------------------------------*/
void  
rating_read (YT_BABY *pa) 
{
  FILE *inifile;
  inifile = fopen ("u_baby.i", "r");

  if (!inifile) {
    pa->rating = /* 0 */1; 
  } else {
    fscanf (inifile, "%lf", &(pa->rating));
  }

  return;
}
/*----------------------------------------------------------------------------*/
void  
rating_write (double rating) 
{
  FILE *inifile;
  inifile = fopen ("u_baby.i", "w");

  if (!inifile)
    Error ("rating_write");
  else {
    fprintf (inifile, "%f", rating);
  }

  return;
}
/*----------------------------------------------------------------------------*/
void 
rating_plus () 
{

  pa->rating += pa->rat; 

  //if (pa->rating > max_rating)  pa->rating = max_rating;
  //if (pa->rating < min_rating)  pa->rating = min_rating;

  return;
}
/*----------------------------------------------------------------------------*/
void 
rating_minus () 
{

  pa->rating -= pa->rat; 

  //if (pa->rating > max_rating)  pa->rating = max_rating;
  //if (pa->rating < min_rating)  pa->rating = min_rating;

  return;
}
/*******************************************************************************/
/*----------------------------------------------------------------------------*/
void
baby_final (YT_BABY *pa) 
{

  rating_write (pa->rating);

  free (pa);
  return;
}
/*----------------------------------------------------------------------------*/
void 
baby_menu_exit () 
{

  //endgame (); 
  baby_final (pa); 

  exit (0);
  return;
}
//******************************************************************************
/*----------------------------------------------------------------------------*/
void 
draw_all () 
{ 

  printf ("\n\n"); 
  printf ("======================= \n"); 
  printf ("%s \n", baby_text_caption ()); 
  printf ("======================= \n"); 
  printf ("\n"); 

  printf ("Status: %s \n", baby_text_status ()); 
  printf ("Rating: %s \n", baby_text_rating ()); 
  printf ("\n"); 
  
  aboard_show (baby_ptr_goban (), TRUE, TRUE /*is_abcd*/);
  
  printf ("======================= \n"); 
  printf ("\n"); 

  return;
} 
/*----------------------------------------------------------------------------*/
YT_BOOL 
next_step () 
{ 
  char move[128] = {0};
  int  x, y;

  printf ("Command: ");
  get_line (move, sizeof (move));

  if      (!strncmp (move, "undo", 4)) { OUTD (11);
  }
  else if (!strncmp (move, "help", 4)) { OUTD (22);
  }
  else if (!strncmp (move, "game", 4)) { baby_menu_newgame (TRUE);
  }
  /*     else if (!strncmp (move, "pass", 4)) { */
  /*       rating_minus ();  */
  /*       baby_menu_newgame (); */
  /*     } */
  else if (!strncmp (move, "null", 4)) {
    baby_menu_ratingset (0); 
    baby_menu_newgame (TRUE);
  }
  else if (!strncmp (move, "exit", 4) || !strncmp(move, "quit", 4)) {
    baby_menu_exit (); 
    //break;
    return (FALSE);
  }

  // ------------ введены координаты хода с клавы --------------
  else if ( isalpha(move[0]) && isdigit(move[1]) ) { 

    //board_name_to_index ((pa->game)->aboard, move, &y, &x); 
    //name_to_index (move, &y, &x);
    board_name_to_index (baby_ptr_goban (), move, &y, &x); 

    baby_onestep_moves (y, x); 
  }
  // нажали Enter - просьба сделать ход компом
  else if (strlen (move) == 0) { 
    if (baby_is_game_end ())  baby_menu_newgame (TRUE); // menu_game_NEW
    else                      baby_onestep_moves (-1, -1); 
  }

  return (TRUE);
} 
/*----------------------------------------------------------------------------*/
void 
inf_loop () 
{

  for (;;) {     
    draw_all (); 

    if (!next_step ()) 
      break; 
  }

  return;
}  
/*----------------------------------------------------------------------------*/
void 
num_loop (int num) 
{
  int n = 1;

  for (;;) {     
    draw_all (); 

    if (baby_is_game_end ())  {
      if (n == num) break;
      n++;
      baby_menu_newgame (/* TRUE */FALSE); 
    }
    else  baby_onestep_moves (-1, -1); 
  }

  return;
}  
/*----------------------------------------------------------------------------*/
int 
asci_main_loop (int ii, int jj, int num) 
{  

  baby_begin (ii, jj);
  baby_menu_newgame (/* TRUE */FALSE);

  if (num == 0) inf_loop (); 
  else          num_loop (num); 

  printf ("\n"); 
  return 1;                                                             
}  
/*----------------------------------------------------------------------------*/
/******************************************************************************/
int 
main (int argc, char **argv) 
{  

  asci_main_loop (3, 2,  2 /* 0 - интерактивно*/);

  return 1;
}  
/*----------------------------------------------------------------------------*/
/******************************************************************************/
 
