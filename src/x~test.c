/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: x~test.c                                                  *
 *                                                                             *
 *******************************************************************************
 */  

// *****************************************************************************
// 
// TODO: 
// 
// - создание "пищи" для NEAT - в отдельный файл (goneat.c);
// - возможна там же - простое тестирование;

/******************************************************************************/
                                                                            
#include  <stdio.h>
#include  <stdlib.h>   
#include  <string.h>
#include  <math.h>
#include  <time.h>  
#include  <unistd.h> 
#include  <ctype.h>  
#include  <dirent.h> 
#include  <sys/time.h> 
#include  <sys/param.h>

/* #ifdef _UNIX  */
//#include  <values.h>  
#include <unistd.h>
//#include <time.h>
/* #else */

/* #include <windows.h> */
/* #include <io.h> */
/* #include <fcntl.h> */
/* #include <process.h> */

/* #include <process.h> */
/* #include <memory.h> */
/* #include <string.h> */
/* #include <stdio.h> */
/* #include <fcntl.h> */
/* #include <io.h> */
/* #endif      */
    
//-----------------------------------------
                                                        
#include "a_comm.h"

#include "d_tree.h" 
#include "d_find.h" 

#include "x_comm.h"
#include "x_hash.h" 
#include "x_work.h"
#include "x_game.h"

/******************************************************************************/   

#define TEST_1x2_0   20
#define TEST_1x3_0   30
#define TEST_1x2_1   21

typedef  int (*GO_EVALPROC) (YT_POSITION *p);                             
void  test_eval_all (GO_EVALPROC eval_one, int level);


typedef struct {
  YT_POSITION position;
  GO_STONE    win_true;
} GOTEST_POSI;

  //-----------------
  GOTEST_POSI test_1x2_0[] = {
    {{ 1,2 , { "XO" },  STONE_BLACK}, STONE_WHITE}, 
    {{ 1,2 , { "OX" },  STONE_BLACK}, STONE_WHITE}, 
    {{ 0,0 , { NULL }}, 0},
  };

  GOTEST_POSI test_1x3_0[] = {
    {{ 1,3 , { "XOO" },  STONE_BLACK}, STONE_WHITE}, 
    {{ 1,3 , { "OOX" },  STONE_BLACK}, STONE_WHITE}, 
    {{ 0,0 , { NULL }}, 0},
  };

  //-----------------
  GOTEST_POSI test_1x2_1[] = {
    {{ 1,2 , { "X." },  STONE_WHITE}, STONE_WHITE},   
    {{ 1,2 , { ".X" },  STONE_WHITE}, STONE_WHITE},   
    {{ 0,0 , { NULL }}, 0},
  };


GOTEST_POSI *test_eval;
GO_EVALPROC  evalproc;


/******************************************************************************/
/*----------------------------------------------------------------------------*/
void
gotest_init (GO_EVALPROC eproc, int level)
{

  evalproc = eproc;

  if (level==TEST_1x2_0) test_eval = &(test_1x2_0[0]);
  if (level==TEST_1x2_1) test_eval = &(test_1x2_1[0]);

  return;
}
/*----------------------------------------------------------------------------*/
GOTEST_POSI *
gotest_get_posi (int p)
{
  GOTEST_POSI *tp = &(test_eval[p]);

  if ((tp->position).ii == 0)
    return (NULL);

  return (tp);
}
/*----------------------------------------------------------------------------*/
void
test_eval_all (GO_EVALPROC eval_one, int level)
{
  int e, p;
  GOTEST_POSI *tp;
  YT_POSITION *pstn;

 
  gotest_init (eval_one, level);

  for (p = 0; (tp=gotest_get_posi(p)) != NULL ; p++) {
    pstn = &(tp->position);

    e = evalproc (pstn);

/*     if (debug) */
/*     fprintf (stderr, "e= %d \n", e); */

    if ( 
        ((e > 0) && (/* разные камни    */pstn->stone != tp->win_true)) ||
        ((e < 0) && (/* совпадают камни */pstn->stone == tp->win_true)) 
       ) {
      Error (".......... ERROR \n");
    }
  } 
  printf (".......... OK \n");

  return;
}
/******************************************************************************/
/******************************************************************************/

typedef struct {
  char *pos_old_new[10];
} YT_PROB_2x5x5;

void  myhash_test (); 

/*----------------------------------------------------------------------------*/
void
test_play_one (int wins_true, char *prog1, char *prog2, int ii, int jj)
{

  YRAND_S;  
  YT_GOBAN *goban = goban_create (ii, jj);

  int wins_calc = game_autoplay_ (goban, prog1, prog2, /* is_print */FALSE);

  if (wins_calc == wins_true)  printf (".......... OK \n");
  else {
    printf ("\n.......... ERROR \n");
    goban_showboard (goban);
    puts ("Enter any key .."); getchar ();
  }

  return;
}
/*----------------------------------------------------------------------------*/
void
test_play_all (char *prog1, char *prog2)
{
  int BlackWin = STONE_BLACK;
  int WhiteWin = STONE_WHITE;

  printf ("\n");

  //test_play_one (WhiteWin, prog1, prog2, 0, 0);
  // желательно этот крайний случай тоже рассмотреть потом!!!!!

  test_play_one (WhiteWin, prog1, prog2, 1, 1);
  test_play_one (WhiteWin, prog1, prog2, 1, 2);
  test_play_one (BlackWin, prog1, prog2, 1, 3);
  test_play_one (BlackWin, prog1, prog2, 1, 4);
  test_play_one (BlackWin, prog1, prog2, 1, 5);
  test_play_one (BlackWin, prog1, prog2, 1, 6); 
  test_play_one (BlackWin, prog1, prog2, 1, 7);
  //test_play_one (BlackWin, prog1, prog2, 1, 8);
  //test_play_one (BlackWin, prog1, prog2, 1, 9);

  test_play_one (WhiteWin, prog1, prog2, 2, 2);
  test_play_one (WhiteWin, prog1, prog2, 2, 3);
  //test_play_one (WhiteWin, prog1, prog2, 2, 4);
  //ame_test_one (BlackWin, prog1, prog2, 2, 5);

  return;
}
//==============================================================================
/*----------------------------------------------------------------------------*/
void
test_prob_lines_to_ints (GO_WORK *w, char *pos_lines[], int *pos_old, int *pos_new)
{
  int   i, j;
  char *line_old, *line_new;

  for (i=0; i < w->isize; i++) {
    line_old = pos_lines[2*i];
    line_new = pos_lines[2*i+1];

    for (j=0; j<w->jsize; j++) {
      BOARD_PS (pos_old, w->isize, w->jsize, i, j) =  stone_from_char (line_old[j]);
      BOARD_PS (pos_new, w->isize, w->jsize, i, j) =  stone_from_char (line_new[j]);
   }
  }

  return;
}
/*----------------------------------------------------------------------------*/
int
test_prob_compare (GO_WORK *w, int *poscalc, int *postrue)
{
  int   i, j;

  for (i=0; i<w->isize; i++) 
  for (j=0; j<w->jsize; j++) {
    if (BOARD_PS (poscalc, w->isize, w->jsize, i,j) != BOARD_PS (postrue, w->isize, w->jsize, i,j))
      return (FALSE);
  }
  
  return (TRUE);
}
/*----------------------------------------------------------------------------*/
void
test_prob_one (char *prog_string, YT_PROB_2x5x5 *prob)
{
  GO_STONE stone = STONE_BLACK;
 
  YT_GOBAN *goban = goban_create (5, 5);
  GO_WORK  *work  = (GO_WORK*) goban->mywork;

  YRAND_S;  

  GO_GAMER *gamer = gamer_make (prog_string, 0, 0, stone, goban);

  int  b_cap, w_cap;
  int  pos_old[25], pos_new_true[25], pos_new_calc[25];

  test_prob_lines_to_ints (work, prob->pos_old_new, pos_old, pos_new_true);
  
  goban_set_board (goban, pos_old);      // устанавливаем нужную позицию   
  gamer_todo_move (gamer, stone);        // делаем на заданной позиции ход 
  work_get_captures (work, TRUE, &b_cap, &w_cap, stone); // оцениваем захваченные камни  
  goban_get_board (goban, pos_new_calc); // забираем полученную позицию

  if (test_prob_compare (work, pos_new_calc, pos_new_true))
    printf (".......... OK \n");
  else {
    printf ("\n");
    printf (".......... ERROR \n");

    goban_showboard (goban);
    puts ("Enter any key .."); getchar ();
  }

  return;
}
//==============================================================================
/*----------------------------------------------------------------------------*/
void
test_prob_all (char *prog_string)
{

  YT_PROB_2x5x5 prob1 = {{
    ".....",   ".....",
    ".....",   ".....",
    ".....",   ".....",
    "X....",   "X....",
    "O....",   "@X...", 
  }}; 

  YT_PROB_2x5x5 prob3 = {{
    ".....",   ".....",   
    ".....",   ".....",   
    "OOOOO",   "OOOOO",   
    "XXXX.",   "XXXXX",   
    "XXXOO",   "XXX@@",   
  }};

  YT_PROB_2x5x5 prob4 = {{
    ".....",   ".....",   
    ".....",   ".....",   
    "XXXXX",   "XXXXX",   
    "OOOO.",   "@@@@X",   
    "OOOXX",   "@@@XX",   
  }};

  YT_PROB_2x5x5 prob5 = {{
    ".....",   ".....",
    ".....",   ".....",
    ".....",   ".....",
    "XXXXX",   "XXXXX",
    "OO.OO",   "@@X@@", 
  }};  

  YT_PROB_2x5x5 prob7 = {{
    "XXXXX",   "XXXXX",  
    "XOOOO",   "X@@@@", 
    "XXXXO",   "XXXX@", 
    ".XOOO",   ".X@@@", 
    ".XO.O",   ".X@X@", 
  }};
  
  YT_PROB_2x5x5 prob8 = {{
    "XO.O.",   "XO.O.",  
    "XOOOX",   "XOOOX",  
    "XXXOX",   "XXXOX",  
    "XOOXX",   "X@@XX",  
    "X.OXX",   "XX@XX",  
  }}; 
  
  YT_PROB_2x5x5 prob9 = {{
    ".XOOO",   ".X@@@",  
    "..XOO",   "..X@@",  
    "..XOO",   "..X@@",  
    "...XO",   "...X@",  
    "...O.",   "...OX",  
  }};  
 

  printf ("\n");
  
  test_prob_one (prog_string, &prob1);
  //return;
  test_prob_one (prog_string, &prob3);
  test_prob_one (prog_string, &prob4);
  test_prob_one (prog_string, &prob5);
  test_prob_one (prog_string, &prob7);    
  test_prob_one (prog_string, &prob8);
  test_prob_one (prog_string, &prob9);

  return;
}
/*----------------------------------------------------------------------------*/
int
test_eval_one (YT_POSITION *p)
{
  int e = 0;
  //GO_POSI  posi;

  YT_GOBAN *goban = goban_create (p->ii, p->jj);
  YRAND_S;  

  GO_GAMER *gamer = gamer_make ("aigo2", /* is_moves_map */ 0, 0, STONE_BLACK, goban);

  goban_set_position (goban, p);  // это здесь не надо ?! 

  //posi.a     = goban->posi->a;    // ??
  //posi.stone = p->stone;

/*   if (debug) */
/*     xo_show_board (goban->posi);  */

  e = aigo_evaluate ((long) gamer , (long)/* (&posi) */goban->posi,  NULL);

  if (debug)
  fprintf (stderr, "e= %d \n", e);

  return (e);
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
void
test_resolve (char *prog, int ii, int jj, int megabytes, int mlevel)
{
  int i, j;
  int /* i_move, j_move, */ best;

  YT_GOBAN *goban = goban_create (ii, jj);
  YRAND_S;  
  dbg_set_maxlevel (mlevel);

  GO_GAMER *gamer = gamer_make (prog, 0, megabytes, STONE_WHITE, goban);

  hashtable_dump (gamer->movehash, FALSE);
  time_calc_printa (0);

  for (i = 0; i < ii; i++)   // симметрия вертикальная
  for (j = 0; j < jj; j++) { // симметрия горизонтальная    

    //goban_set_stone (goban, i, j, STONE_BLACK); // ставим черный камень
    goban_set_move (goban, i, j, STONE_BLACK); // ставим черный камень
    best = gamer_get_move (gamer, NULL, NULL);  // и думаем белыми в ответ

    //-----------------------------------
    printf ("\n");
    goban_showboard (goban);
    if (best <= -100)  printf ("WIN \n"); //??
    else               printf ("LOX \n");
    time_calc_printa (1);
    printf ("\n");
    //-----------------------------------

    goban_set_stone (goban, i, j, STONE_NONE);
  }

  hashtable_dump (gamer->movehash, /* TRUE */ FALSE);
  return;
}
/*---------------------------------------------------------------------------*/
short
board_calc_index (GO_BOARD *b)
{ 
  short index = 10000 /* 70000 */;
  int i, j;

  for (i=0; i < b->isize; i++) 
  for (j=0; j < b->jsize; j++) {

  }

  return (index);
}
/*----------------------------------board_test--------------------------------*/
void
board_test ()
{ 

  OUTD (1);
  int a3x3[9] = { STONE_BLACK, STONE_NONE,   STONE_NONE, 
                  STONE_NONE,  STONE_WHITE,  STONE_BLACK, 
                  STONE_NONE,  STONE_WHITE,  STONE_NONE  };
  GO_BOARD b3x3 = {3, 3, &a3x3[0]};

  int a2x3[6] = { STONE_BLACK, STONE_NONE,   STONE_NONE, 
                  STONE_NONE,  STONE_WHITE,  STONE_NONE  };
  GO_BOARD b2x3 = {2, 3, &a2x3[0]};

  int a1x3[3] = { STONE_BLACK, STONE_NONE,   STONE_NONE };
  GO_BOARD b1x3 = {1, 3, &a1x3[0]};

  GO_BOARD *bs[NUM_SYM];
  int i;

  OUTD (2);
  UNUSED (b3x3);
  UNUSED (b1x3);
  UNUSED (b2x3);

  if (0) {
  //---------------------------------
  printf ("short  = %d \n", (int) sizeof (short));
  printf ("int    = %d \n", (int) sizeof (int));
  printf ("long   = %d \n", (int) sizeof (long));
  printf ("double = %d \n", (int) sizeof (double));
  printf ("\n");
  printf ("MAXSHORT = %d \n", G_MAXSHORT);
  //#ifdef _UNIX
  printf ("MAXLONG  = %ld \n", G_MAXLONG);
  //#endif

  char *p;
  YMALLOC (p, char, /* MAXLONG */G_MAXSHORT);
  if (!p)  printf ("Not memory! \n");
  else     printf ("Yes memory! \n");
  //---------------------------------
  }

  OUTD (3);
  //((long)(&b3x3))++;
  int num_sym = board_symm_calc (bs, &b1x3);
  //int num_sym = board_symm_calc (bs, &b2x3);
  //int num_sym = board_symm_calc (bs, &b3x3);

  OUTD (4);

  for (i=0; i<num_sym; i++) {
    aboard_show (bs[i], FALSE,  TRUE); 

    short index = board_calc_index (bs[i]);
    printf ("Index = %d \n", index);

    printf ("\n");
  }
  OUTD (5);

  printf ("\n");
  return;
}
/*---------------------------------------------------------------------------*/
void
myhash_test () 
{
  Hashposition    pos;
  unsigned long   hash;
  int  result;
  Hashtable  *movehash;
  int  i, j;
  
  //------------
  GO_BOARD *b;

  //------------
  int isize = 2;
  int jsize = 3;
  //int boardsize = 3;
  //int size_node = sizeof (Hashnode);
  //------------
  printf ("\n");
  //UNUSED (b3x3);

  movehash = gohash_create (isize, jsize, 10  /* 0  *//*MEMORY*/);
  for (i = 0; i < isize; i++) 
  for (j = 0; j < jsize; j++) {

    b = aboard_begin (isize, jsize);
    BOARD (b, i, j) = STONE_BLACK;

    hash = myhash_calc (movehash, b, &pos); 

    if (!myhash_get_result (movehash, hash, &pos, &result, NULL)) {
      result = (i+1)*10 + (j+1);
      myhash_set_result (movehash, hash, &pos,  result, i+j) ;
    }
  }

  hashtable_dump (movehash, /* STDERR, */ /* TRUE */ FALSE);

  for (i = 0; i < isize; i++) 
  for (j = 0; j < jsize; j++) {

    b = aboard_begin (isize, jsize);
    BOARD (b, i, j) = STONE_BLACK;
    hash = myhash_calc (movehash, b, &pos); 

    if (myhash_get_result (movehash, hash, &pos, &result, NULL)) 
      printf ("result = %d \n\n", result);
    else 
      printf ("result not found \n\n");
  }

  return;
}
/******************************************************************************/
/*----------------------------------------------------------------------------*/
#define MAX_ALL_POSI 30000

/*----------------------------------------------------------------------------*/
int
calc_difficult (GO_POSI *pos) 
{ 
  int  d, lmin=+100, s=0;
  int  i, j;
  GO_BOARD *board = pos->a;
  int  isize = board->isize;
  int  jsize = board->jsize;

  // основная область доски с камнями
  for (i=0; i < isize; i++) 
  for (j=0; j < jsize; j++) {
    s = BOARD (board, i,j);
    if      (s == STONE_WHITE)   continue;
    else if (s == STONE_BLACK)   continue; 
    else if (s == STONE_NONE)    continue; 

    s = s-48;
    //OUTD (s);
    lmin = MIN (lmin, s);
  }

  if (lmin == +100) d = 0;
  else              d = lmin;
  //OUTD (d);

  return (d);
} 
/*----------------------------------------------------------------------------*/
void
make_baza_move (YT_GOBAN *goban, GO_GAMER *gamer,
                int h,   int i1, int j1,   int i2, int j2) 
{  

  if ((h == 1) || (h == 2))
    //goban_set_stone (goban, i1, j1, STONE_WHITE); 
    goban_set_move (goban, i1, j1, STONE_WHITE); 

  if (h == 2)
    //goban_set_stone (goban, i2, j2, STONE_WHITE);
    goban_set_move (goban, i2, j2, STONE_WHITE);

  printf ("--------------- h = %d ------------------ \n", h); 
  gamer_todo_move (gamer, STONE_BLACK); 

  return;

}
/*----------------------------------------------------------------------------*/
GO_GAMER*
make_baza_game_new (int ii, int jj, int mb) 
{  

  YT_GOBAN *goban = goban_create (ii, jj);

  YRAND_S;  

  GO_GAMER *gamer = gamer_make ("migo50", 1, mb, STONE_BLACK, goban);

  return (gamer);
}
/*----------------------------------------------------------------------------*/
void
make_baza_h (int h,  int ii, int jj,  int mb, 
             int i1, int j1,  int i2, int j2) 
{  

  GO_GAMER *gamer = make_baza_game_new (ii, jj, mb);

  YT_GOBAN *goban = gamer->goban;
  make_baza_move (goban, gamer, h,  i1, j1,  i2, j2); 

  return;
}
/*----------------------------------------------------------------------------*/
#ifdef OLDTEST
void
make_stderr_ (YT_BOOL is_make, char *pref, int ii, int jj, char *post) 
{ 
  // здесь файл вообще не простой, а с именем специального вида..
  // для вывода:
  //char *pref = "B/";
  //char *post = "all";

  static FILE *fout=NULL; // куда будем выводить stderr

  char  fname[80];

  //-------------------------------------------------------
  // закрываем вывод от "stderr" в файл и возвращаем стандарт:

  if (!is_make) {
    if (fout) fclose (fout);    
    my_stderr_set_ (stderr);
    fout = NULL;
    return;
  }

  //-------------------------------------------------------
  // задаем вывод от "stderr" в файл:

  sprintf (fname, "%s%dx%d%s.pos", pref, ii, jj, post);

  fout = fopen (fname, "w");
  if (!fout) Error ("make_stderr");
  my_stderr_set_ (fout);

  return;
} 
#endif
/*----------------------------------------------------------------------------*/
void
make_baza (YT_BOOL to_files, int ii, int jj) 
{  
  int   sizeij = ii*jj;
  int   i1, j1, i2, j2;
  int   mb = -600 /* 0 */; // не нужны симметричные позиции

  dbg_set_maxlevel (0);
  //---------------------------
  // вот эту фигню с формированием и затем чтением файлов вместо стандартных потоков
  // как раз и надо делать с помощью скриптов, а не здесь !!!!!!!!!!!!!!!!!!!!!!!!!
  // Поэтому:
  //make_stderr_ (FALSE, 0,0,0,0); 
  //make_stderr_ (to_files, "B/", ii, jj, "all"); 
  //---------------------------

  GO_GAMER *gamer = make_baza_game_new (ii, jj, mb);
  YT_GOBAN *goban = gamer->goban;

  make_baza_move (goban, gamer,  /* h */0,  0,0, 0,0); 

  for (i1 = 0; i1 < ii; i1++)  
  for (j1 = 0; j1 < jj; j1++) {    
    //make_baza_h (1,  ii,jj, mb, i1,j1, 0,0); 
    //g = make_baza_game (ii, jj, mb,  &gamer2); 
    goban_clean_board (goban);
    make_baza_move (goban, gamer,  /* h */1,  i1,j1, 0,0); 
  }

  if (sizeij >= 2) { //-------------------------
  for (i1 = 0; i1 < ii; i1++)  
  for (j1 = 0; j1 < jj; j1++) {    
    for (i2 = 0; i2 < ii; i2++)  
    for (j2 = 0; j2 < jj; j2++) {  

      if ((i2 == i1) && (j2 == j1))
        continue;
      //make_baza_h (2,  ii,jj, mb, i1,j1, i2,j2); 
      //g = make_baza_game (ii, jj, mb,  &gamer2); 
      goban_clean_board (goban);
      make_baza_move (goban, gamer,  /* h */2,  i1,j1, i2,j2); 
    }
  }
  } //------------------------------------------

  //---------------------------
  //make_stderr_ (FALSE, 0,0,0,0); 
  //---------------------------
  return;
}
/*----------------------------------------------------------------------------*/
void
make_levl (YT_BOOL to_files,  int ii, int jj,  int lev) 
{ 

  GO_POSI *l[MAX_ALL_POSI];
  int      n, in, d;
  char     fname[80];

  GO_BOARD *a;
  int  i, j;

  //---------------------------
  //char     post[80];
  //sprintf (post, "-l%d", lev);
  //make_stderr_ (FALSE, 0,0,0,0); 
  // в дальнейшем сделать этло через скрипты Tcl..

  //make_stderr_ (to_files, "B/", ii, jj, post); 

  //sprintf (fname, "B/%dx%dall.pos", ii, jj);
  //fprintf (stderr, "\nExtract %d-nd level from File: %s \n", lev, fname); 
  //---------------------------

  // читаем файл позиций в массив
  n = read_file_posi (fname, l, MAX_ALL_POSI, ii, jj);

  // просматриваем массив вычисляя рейтинг сложности для каждой позиции
  for (in = 0; in < n; in++) {
    d = calc_difficult (l[in]);
    if (d == lev) { // подходящий уровень сложности

      // сначала надо убрать  рабочую информацию (цифры) из позиции
      // сделаем копию и очистим доску от оценок--плюсиков (p_calc)
      //p_copy = xo_pos_copy (/* NULL */p_copy, l[i]);
      //a = p_copy->a;
      a = (l[in])->a;
      for (i = 0; i < a->isize; i++) 
      for (j = 0; j < a->jsize; j++) {
        if (BOARD (a, i, j) == STONE_NONE)  continue;
        if (BOARD (a, i, j) == STONE_WHITE) continue;
        if (BOARD (a, i, j) == STONE_BLACK) continue;
        BOARD (a, i, j) = STONE_NONE;
      }

      posi_show (l[in]);
      //num_lev++; // считаем выход для этого уровня
      //if (num_lev == MAX_LEV_POSI)  break;
    }
  }

  printf ("\n"); 

  //---------------------------
  //make_stderr_ (FALSE, 0,0,0,0); 
  //---------------------------
  return;
} 
/******************************************************************************/
/*----------------------------------------------------------------------------*/
void 
main_leveling (int num_var, char **ptr_var) 
{ 

// TODO: подготовить вход. данные для брейн-тестов до 3х3 (не дальше):
//
//  - не получается использовать общую хэш-память (для всех фор) !
//  - нестабильный результат создания базы !! :-(
//
//  - правильнее форы выставлять путем серии пасов в начале партии
//  - правильнее, после полного перебора, записывать таблицу транспозиций в файл!
//
//  - не делить на файлы-уровни, а структурировать общий выходной файл (L=5, например)
//  - отмечать также уровни пригрышных ходов (A, B, C, ..)
//  - продумать правильный алгортим определения сложности позиции
//  - сивольные значения для ходов 1, 2.. могут означать 10, 20..(учитывать глубину)
//  - записывать ли на ходах белых?
// 
/*----------------------------------------------------------------------------*/
  int    lev, ii, jj;
  char  *mode;

  if   (num_var == 0)  {
    //game_main (3, 3);
    make_baza (FALSE, 1, 3);
  } else { 
    if (num_var < 2) Error ("main parsing_1");
    ii   = atoi (ptr_var[0]);
    jj   = atoi (ptr_var[1]);

    if (num_var < 3) Error ("main parsing_2");
    mode =       ptr_var[2];
    if       (strcmp(mode, "-test") == 0)  make_baza (FALSE, ii, jj);
    else if  (strcmp(mode, "-make") == 0)  make_baza (TRUE,  ii, jj);
    else if  (strcmp(mode, "-levl") == 0)  {
      if (num_var == 4) {
        lev  = atoi (ptr_var[3]);
        make_levl (FALSE, ii, jj, lev); 
      } else {
        for (lev = 1; lev <= ii*jj; lev++) 
          make_levl (TRUE,  ii, jj, lev); 
      }
    }
    //else if  (strcmp(mode, "-play") == 0)  game_main (ii, jj);
    else    
      Error ("main parsing");
  }

  return;
} 
/*----------------------------------------------------------------------------*/
int 
main_switch_test (int num_var, char **ptr_var) 
{
  int mode;

/*   if (num_var == 0) mode = -1; */
/*   else              mode = atoi (ptr_var[0]); */
  if (num_var == 0) mode = 0; // печатаем меню
  else              mode = atoi (ptr_var[0]);

  //my_stderr_set (stdout);

  switch (mode) {
  case  0:
    printf ("MENU : \n\n");

  case  1: //--------------------------------------------------------
    if (mode==0)  printf ("1 - ALL TESTS [@ == debug] \n");
    else {
      printf ("test_eval_all \n");
      printf ("\n");
      test_eval_all (test_eval_one, TEST_1x2_0); 
      test_eval_all (test_eval_one, TEST_1x3_0); 
      test_eval_all (test_eval_one, TEST_1x2_1); 
      printf ("\n"); 

      printf ("test_prob_all [aigo1] \n");  
      test_prob_all ("aigo1"); 
      printf ("\n"); 

      printf ("test_play_all [aigo aigo] \n"); 
      test_play_all ("aigo", "aigo"); 
      printf ("\n"); 

      //printf ("minmax tests \n"); // перенесено в СOMMON
      //minmax_tests (); 
      break;
    }

  case  91: //--------------------------------------------------------
    if (mode==0)  ;
    else {
      debug = TRUE;
      printf ("\n");
      test_eval_all (test_eval_one, TEST_1x2_0); 
      printf ("\n");
      test_eval_all (test_eval_one, TEST_1x3_0); 
      printf ("\n");
      test_eval_all (test_eval_one, TEST_1x2_1); 
      printf ("\n"); 
      break;
    }

  case 2: //--------------------------------------------------------
    if (mode==0)  printf ("2 - test_prob_all [prog] \n");
    else {
      char *prog = ptr_var[1];
      test_prob_all (prog); 
      break;
    }

  case 3: //--------------------------------------------------------
    if (mode==0)  {
      printf ("3 - auto_play [prog1 prog2  ii jj] \n");
    } else {
      char *prog1 = ptr_var[1];
      char *prog2 = ptr_var[2];
      int   ii    = atoi (ptr_var[3]);
      int   jj    = atoi (ptr_var[4]);

      srand (0); // установим одинаковые условия для теста
                 // после  work_begin->YRAND_S ..

      YT_GOBAN *goban = goban_create (ii, jj);

      game_autoplay_ (goban, prog1, prog2, /* is_print */TRUE);
      break;
    }

  case 5: //--------------------------------------------------------
    if (mode==0)  printf ("5 - resolve [pname ii jj megabytes mlevel] \n");
    else {
      char *pname = ptr_var[1];        // aigo  migo
      int  ii  = atoi (ptr_var[2]);
      int  jj  = atoi (ptr_var[3]);
      int  mb  = atoi (ptr_var[4]);
      int  mlevel = atoi (ptr_var[5]); // 14  1  2 

      test_resolve (pname, ii, jj, mb, mlevel); 
      //test_resolve ("migo", ii, jj, mb, mlevel); //mini-max
      //test_resolve ("aigo", ii, jj, mb, mlevel); //alfa-beta
      break;
    }

  case 7: //--------------------------------------------------------
    if (mode==0)  printf ("7 - make Baza [ii jj megabytes] \n");
    else {
      int  ii  = atoi (ptr_var[1]);
      int  jj  = atoi (ptr_var[2]);
      int  mb  = atoi (ptr_var[3]);
      test_moves_map (0, "migo50", ii, jj, /* 600 */mb, /* is_moved_map = */ 1); 
      break;
    } //-------------------------------------------------------------

  case 8: //--------------------------------------------------------
    if (mode==0)  printf ("8 - read Baza [ii jj]  \n");
    else {
      int  ii  = atoi (ptr_var[1]);
      int  jj  = atoi (ptr_var[2]);
      solver_check (ii, jj, TRUE);
      break;
    } //-------------------------------------------------------------

  case 9: //--------------------------------------------------------
    if (mode==0)  printf ("9 - read Baza [ii jj] (NOT PRINT ERR) \n");
    else {
      int  ii  = atoi (ptr_var[1]);
      int  jj  = atoi (ptr_var[2]);
      solver_check (ii, jj, FALSE);
      break;
    } //-------------------------------------------------------------

  case 10: //--------------------------------------------------------
    if (mode==0)  printf ("10 - LEVELING \n");
    else {
      main_leveling (--num_var, ++ptr_var); 
      break;
    } //-------------------------------------------------------------

  case 11: //--------------------------------------------------------
    if (mode==0)  printf ("11 - myhash_test \n");
    else {
      myhash_test () ; 
      break;
    } //-------------------------------------------------------------

  case 12: //--------------------------------------------------------
    if (mode==0)  printf ("12 - myeuler_test \n");
    else {
      //win_test ();
      myeuler_test (); 
      break;
    } //-------------------------------------------------------------

  default: //--------------------------------------------------------
    if (mode!=0) 
    printf ("Sorry, what you want ??\n");
  }

  //fprintf (SERR, "\n");
  printf ("\n");
  return 1;
} 
/*--------------------------------------main----------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
int
main (int argc, char **argv) 
{
  int    num_var = argc-1;
  char **ptr_var = argv+1; 
  
  //board_test ();

  //GO_BOARD *board = aboard_begin (4, 4);
  //aboard_show (board, TRUE, TRUE);
  //return 1;

  if ((num_var > 1) && (ptr_var[0][0] == '@')) { // ввести "звездочку" не получается!

    dbg_set_maxlevel (2);
    //dbg_set_maxlevel (10);
    // надо сделать более гибким управление такой отладкой, т.е.
    // указывать интервалы уровней и/или их имена...

    num_var--;
    ptr_var++;
  } 
  
  printf ("\n");

  main_switch_test (num_var, ptr_var); 

  return 1;
} 
//------------------------------------------------------------------------------
//******************************************************************************
