/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: x_game.c                                                  *
 *                                                                             *
 *******************************************************************************
 */  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

/* #ifdef _UNIX  */
#include <unistd.h>
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

#include <assert.h>

/*----------------------------------------------------------------------------*/

#include "a_comm.h"
#include "d_find.h" 

#include "x_comm.h"
#include "x_hash.h"
#include "x_work.h"
#include "x_game.h"

/******************************************************************************/



/*----------------------------------------------------------------------------*/
YT_GOBAN*
goban_create (int ii, int jj)
{
  YT_GOBAN *gb;
 
  gb = (YT_GOBAN*) malloc (sizeof (YT_GOBAN));
 
  gb->posi = xo_pos_create (/* stone */STONE_BLACK, ii, jj);

  gb->mywork = (long) work_begin_ (gb->posi->a);

  return (gb);
}
/*----------------------------------------------------------------------------*/
void
goban_showboard (YT_GOBAN  *goban) 
{

  aboard_show (goban->posi->a, TRUE, TRUE /*is_abcd*/);

}
/*----------------------------------------------------------------------------*/
void
goban_set_stone (YT_GOBAN *goban, int i_move, int j_move, GO_STONE stone)
{
  GO_BOARD *a = goban->posi->a;

  BOARD (a, i_move, j_move) = stone;

  return;
}
/*----------------------------------------------------------------------------*/
int
goban_set_move (YT_GOBAN *goban, int i_move, int j_move, GO_STONE stone)
{
  // дополнительная проверка гл. судьи (а надо ли?!)
  //if (!aboard_is_move_legal (ABOARD, i_move, j_move))
  //  return (FALSE); 

  // сделать ход на доске
  goban_set_stone (goban, i_move, j_move, stone);

  // запомнить последний ход  
  goban->posi->stone = STONE_NEXT(stone); 

  return (TRUE);
}
/*----------------------------------------------------------------------------*/
void
goban_set_board (YT_GOBAN *goban, int *d_board)
{
  GO_BOARD *a = goban->posi->a;

  int  i, j, stone;
  int  isize = a->isize; 
  int  jsize = a->jsize;  

  for (i = 0; i < isize; i++) 
  for (j = 0; j < jsize; j++) {

    stone = BOARD_PS (d_board, isize, jsize, i, j);
    goban_set_stone (goban, i, j, stone);
  }

  return;
}
/*----------------------------------------------------------------------------*/
void
goban_clean_board (YT_GOBAN *goban)
{
  GO_BOARD *a = goban->posi->a;

  int  i, j;
  int  isize = a->isize; 
  int  jsize = a->jsize;  

  for (i = 0; i < isize; i++) 
  for (j = 0; j < jsize; j++) {

    goban_set_stone (goban, i, j, STONE_NONE);
  }

  return;
}
/*----------------------------------------------------------------------------*/
void
goban_get_board (YT_GOBAN *goban, int *d_board)
{
  GO_BOARD *a = goban->posi->a;

  int i, j, stone;
  int  isize = a->isize; 
  int  jsize = a->jsize;  

  for (i = 0; i < isize; i++) {
  for (j = 0; j < jsize; j++) {
    stone = BOARD (a, i, j);
    d_board[i*(jsize)+j] = stone;
  }
  }

  return;
}
/*----------------------------------------------------------------------------*/
void 
goban_set_position (YT_GOBAN *goban, YT_POSITION *pp) 
{

  GO_POSI *posi = posi_from_position (pp); 

  // goban_set_board (goban, posi->a->board); // а зачем перекопировать?
  // может проще:
  goban->posi = posi;

  return;
}
/*----------------------------------------------------------------------------*/



/******************************************************************************/
//                                                                            //
/******************************************************************************/
/*----------------------------------------------------------------------------*/
int   
goban_random_handicap (YT_GOBAN *goban, int handicap, int *i_move, int *j_move) 
{ 
  GO_BOARD *b = goban->posi->a;

  YT_BOOL ret;
  int  imin, jmin, imax, jmax;
  
  if (b->isize <= 3) {
    imin = 0;
    imax = b->isize-1;
  } else {
    imin = 1;
    imax = b->isize-2;
  }

  if (b->jsize <= 3) {
    jmin = 0;
    jmax = b->isize-1;
  } else {
    jmin = 1;
    jmax = b->isize-2;
  }

  ret = board_random_rect_stone (b, imin, jmin, imax, jmax, i_move, j_move);

  //if (handicap <= 2) 
  //ret = board_random_rect_stone (b, 1, 1, b->isize-2, b->jsize-2, i_move, j_move);
  //ret = board_random_rect_stone (b, 0, 0, b->isize-1, b->jsize-1, i_move, j_move);
  //else
  //ret = board_random_stone (b, i_move, j_move);

  return (ret);
}
/*----------------------------------------------------------------------------*/
int   
goban_random_stone_new (YT_GOBAN *goban, int *i_move, int *j_move) 
{ 

  return (board_random_stone (goban->posi->a, i_move, j_move));

}
/******************************************************************************/
//                                                                             /
/******************************************************************************/

//------------------------------------------------------------------------------
int
find_bestmove_minimax (void *self, GO_POSI *posi, GO_MOVE **p_bestmove)
{
  GO_GAMER *gamer = (GO_GAMER *) self;
  int      best;
  long     bmove;
  int      calc_nums = 0; // нужно для  .. CalcEnd= 

  best = mm_minimax (gamer->mm, /* alfa */MM_MIN_EVAL, /* beta */MM_MAX_EVAL, 
                     (long)posi, (long)gamer, 
                     &bmove, &calc_nums, NULL);

  // здесь мы получаем "один лучший" ход после перебора
  // а хотелось бы список лучших, а потом уж делай с ним что хош..
  // мы же подаем предварительный список - вот для этих ходов и надо оценку получить
  //
  // но можно попробовать искать по одному ходу, но много раз - монтекарло перебор

  if (!bmove) 
    Error ("gamer_get_move"); // лучшей ход всегда возвращает, пусть и не "победный";

  *p_bestmove = (GO_MOVE *) bmove;

  return (best);
}
//------------------------------------------------------------------------------
int
find_bestmove_m_carlo (void *self, GO_POSI *posi, GO_MOVE **p_bestmove)
{
  GO_GAMER *gamer = (GO_GAMER *) self;
  int      best;
  long     bmove;
  GO_MOVE *bestmove;
  int  n, ii, jj, i, j;

  ii = posi->a->isize;
  jj = posi->a->jsize;
  GO_BOARD *a = aboard_begin (ii, jj);

  // играем (перебираем) случайные игры до самого конца
  for (n=0; n</* 10 */5; n++) {
    best = mm_minimax (gamer->mm, MM_MIN_EVAL, MM_MAX_EVAL, 
                       (long)posi, (long)gamer, 
                       &bmove, /* &calc_nums */NULL, NULL);
    bestmove = (GO_MOVE *) bmove;
    i = bestmove->i;
    j = bestmove->j;

    BOARD(a, i,j) = BOARD(a, i,j) + 1; // подсчитываем лучшие ходы
  }

  //  а теперь посмотрим накопленные результаты
  //aboard_show (a, FALSE, FALSE); 

  int val_max = -10000000, val;
  static GO_MOVE  move;

  for (i=0; i<ii; i++) {
  for (j=0; j<jj; j++) {
    val = BOARD(a, i, j);
    printf ("%4d  ", val);
    if (val < val_max)  continue;

    val_max = val;
    move.i = i;
    move.j = j;
  }
  printf ("\n");
  }

  *p_bestmove = /* bestmove */ &move; // не фига не работате вроде !!! :-(
  return (best);
}
//------------------------------------------------------------------------------
int
find_bestmove_simple (void *self, GO_POSI *posi, GO_MOVE **p_bestmove)
{
  //GO_GAMER *gamer = (GO_GAMER *) self;

  GO_MOVE bestmove = {2, 3};


  *p_bestmove = &bestmove;

  return (/* best */0);
}
//------------------------------------------------------------------------------
int
gamer_get_move (GO_GAMER *gamer, int *p_i, int *p_j)
{
  GO_MOVE *bestmove;

  // это надо сделать при инициализации бота
  //gamer->find_proc = find_bestmove_minimax;

  int best = gamer->find_proc (gamer, gamer->goban->posi, 
                                  &bestmove);

  if (p_i) *p_i = bestmove->i;  // может и не нужен сам ход, а только процесс..
  if (p_j) *p_j = bestmove->j;

  return (best);
}
/*----------------------------------------------------------------------------*/
YT_BOOL
gamer_todo_move (GO_GAMER *gamer, GO_STONE stone)
{
  int  i_move, j_move;

  // игрок подумал
  gamer_get_move (gamer, &i_move, &j_move);

  // и сделал (зафиксировал) этот ход
  goban_set_move (gamer->goban, i_move, j_move, stone/*??*/);

  return (/* ret */TRUE);
}
/*----------------------------------------------------------------------------*/
int 
aigo_who_win (YT_GOBAN *goban) 
{
  GO_WORK  *work  = (GO_WORK*) goban->mywork;
  int stone_old = STONE_NEXT(goban->posi->stone);

  int   who_win;
  int   b_cap, w_cap;

  work_get_captures (work, TRUE, &b_cap, &w_cap, /* OLD_MOVE */stone_old);

  if      (b_cap > 0)  who_win = STONE_BLACK;
  else if (w_cap > 0)  who_win = STONE_WHITE;
  else         
    who_win = 0;

  return (who_win);
}
/*----------------------------------------------------------------------------*/
int
game_autoplay_ (YT_GOBAN *goban, char *prog1, char *prog2, int is_print)
{

  int  is_moves_map = FALSE;
  int  who_win;

  GO_GAMER *white_gamer = gamer_make (prog1, is_moves_map, 0, STONE_WHITE, goban);
  GO_GAMER *black_gamer = gamer_make (prog2, is_moves_map, 0, STONE_BLACK, goban);

  int   stone   = STONE_BLACK;  // первый ход у черных
  GO_GAMER *gamer = black_gamer;

  while (1) { //-------------------------
    gamer_todo_move (gamer, stone);
    who_win = aigo_who_win (goban);

    if (is_print) goban_showboard (goban);  
    if (who_win) break; 
    
    stone   = STONE_NEXT (stone);
    if   (gamer == white_gamer) gamer = black_gamer;
    else                        gamer = white_gamer;
  } //-----------------------------------

  if (is_print) printf ("--------------> %s win! \n", STONE_NAME(who_win));

  return (who_win);
}
/******************************************************************************/
// 
/*----------------------------------------------------------------------------*/
GO_POSI *
xo_make_move (GO_POSI *old, GO_MOVE *move)
{
  GO_POSI   *new; // не статик ????????????
  int  i, j, me;
	
  //assert (old != NULL);
  //a = old->a;

  i  = move->i;
  j  = move->j;
  me = old->stone;

  // дополнительная проверка здесь нужна?
  //if (i < 0 || i > a->isize || j < 0 || j > a->jsize   || BOARD (a, i, j) != 0)
  //  return NULL;

  // выделение памяти и копирование старой позиции (долгая операция)
  new = xo_pos_copy (NULL, old);

  // ставим камень на доску (здесь бы и пересчитать инфу о группах!))
  BOARD (new->a, i, j) = me;

  // теперь ход другого игрока
  new->stone  = STONE_NEXT (me);

  return (new);
}
/*----------------------------------------------------------------------------*/
long  
go_make_move (long pstr, long old_posi, long move)
{ 


  return ((long)xo_make_move ((GO_POSI *)old_posi, (GO_MOVE *)move));

}
/*----------------------------------------------------------------------------*/
char* 
go_name_posi (long pstr, long posi)
{ 

  GO_POSI *pos = (GO_POSI*) posi;

  xo_show_board (pos); 
    
  return (NULL);
}
/*----------------------------------------------------------------------------*/
void 
go_moves_map_prep (GO_POSI *pos, long moves[], int evals[], int levls[],  int num_moves)
{ 
  int   i, eva;
  GO_MOVE *mov;
  int   stone = pos->stone;
  //char  lev[2];
  int  lev;

  for (i = 0; i < num_moves; i++) {
    mov = (GO_MOVE *) moves[i];

    if (!evals) { // пометить все ходы крестиками (нашел пользователь, пока без оценок каждого)
      BOARD (pos->a, mov->i, mov->j) = '+';
    } else {        
      eva = evals[i];
      //lev = '+'; 
      lev = 48 /* 65 */ + levls[i]; 
      //sprintf (lev, "%s", /* levls[i] */9);
      // здесь я порчу рассматриваемую позицию (занося плюсики)
      if ((stone==STONE_BLACK) && (eva>=MM_MAX_EVAL))  BOARD (pos->a, mov->i, mov->j) = lev; 
      if ((stone==STONE_WHITE) && (eva<=MM_MIN_EVAL))  BOARD (pos->a, mov->i, mov->j) = lev; 

    }
  }

  return;
}
/*----------------------------------------------------------------------------*/
//  напечатать исходную доску-позицию с "наложенной" "+"-картой
/*----------------------------------------------------------------------------*/
void 
go_moves_map1 (long pstr, long posi, long moves[], int evals[], int levls[], int num_moves)
{ 
  GO_POSI *pos = (GO_POSI*) posi;
  static GO_POSI *pos_new = NULL;

  // будем печатать только "позиции черных" ?
  if (pos->stone != STONE_BLACK) return;

  // чтобы не портить позицию - она еще понадобится
  pos_new = xo_pos_copy (pos_new, pos);

  go_moves_map_prep (pos_new, moves, evals, levls,  num_moves);
  posi_show (pos_new);

  return;
}
/*----------------------------------------------------------------------------*/
void
test_moves_map (int dbg_maxlevel, char *prog, int ii, int jj, int mb, int is_moves_map)
{
  YT_GOBAN *goban = goban_create (ii, jj);

  YRAND_S;  
  dbg_set_maxlevel (dbg_maxlevel);

  GO_GAMER *gamer = gamer_make (prog, is_moves_map, /*0*//*600*/mb, 
                                STONE_BLACK, goban);

  time_calc_printa (0);
  gamer_todo_move (gamer, STONE_BLACK); 
  printf ("\n");
  time_calc_printa (1);

  return;
}
/*----------------------------------------------------------------------------*/
// тест на симметрию и хэш-таблицу
/*----------------------------------------------------------------------------*/
void 
go_moves_map2 (long pstr, long posi, long moves[], int evals[], int levls[], int num_moves)
{ 
  GO_POSI *pos = (GO_POSI*) posi;
  int    stone = pos->stone;
  GO_BOARD *board = pos->a;

  printf ("%s \n", STONE_NAME (stone));

  aboard_show (board, FALSE, FALSE /* is_abcd */); 

  printf ("\n");

  return;
}
/*----------------------------------------------------------------------------*/
void 
xo_free_posi (long pstr, long posi)
{ 

  //xo_pos_copy (NULL, old);
  //xo_pos_free ((GO_POSI*) posi);
  xo_pos_delete ((GO_POSI*) posi);
   
  return;
}
/*----------------------------------------------------------------------------*/

/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: solver.c                                                  *
 *                                                                             *
 *******************************************************************************
 */  
/*----------------------------------------------------------------------------*/
YT_BOOL
posi_compare (GO_POSI *p_true, GO_POSI *p_calc) 
{
  int  i, j;
  GO_BOARD *a_true = p_true->a;
  GO_BOARD *a_calc = p_calc->a;

  // надо сравнивать "плюсики" - оценочные значения
  // с "циферками" - точные значения (хотя надо )

  for (i = 0; i < a_true->isize; i++) 
  for (j = 0; j < a_true->jsize; j++) {
    if (BOARD (a_true, i, j) == BOARD (a_calc, i, j)) 
      continue;
    if ((BOARD(a_true, i, j) >= 48) && (BOARD(a_calc, i, j) == '+'))
      continue;;   

    return (FALSE);
  }

  return (TRUE);
}
/*----------------------------------------------------------------------------*/
void
read_posi (FILE *fp, char *buffer, GO_POSI *p_true, int ii, int jj) 
{

  // читаем запись оценки-позиции из файла (p_true)
  //  if (strncmp(buffer, "WHITE", 5)==0) p_true->stone = STONE_WHITE;
  //  else
  //    if (strncmp(buffer, "BLACK", 5)==0) p_true->stone = STONE_BLACK;
  //   else
  //     continue; // пропускаются пробелы и разные рабочие записи

  // read_board (fp, p_true->a, ii, jj);


  return;
}
/*----------------------------------------------------------------------------*/
YT_BOOL
posi_add_moves (GO_POSI *posi, GO_WORK *p_work) 
{

  long moves [MM_MOVES_MAX]; // массив (указателей) возможных ходов
  int  num_moves;

  // юзер генерирует список ходов (лучших с его точки зрения) - без перебора !
  num_moves = findmoves (FIND_BEST, p_work, posi, /**/moves);

  // пометить найденные ходы на доске (просто крестиками, поскольку оценок для них нет)
  go_moves_map_prep (posi, moves, NULL /*evals*/, NULL /*levls*/, num_moves);

  free_moves_all (moves, num_moves);  
  return (TRUE);
}
/*----------------------------------------------------------------------------*/
double
solver_check (int ii, int jj, YT_BOOL is_print_err) 
{
  char  fname[20];
  FILE *fp;
  char  buffer[80];
  int   i, j, num_true=0, num_full=0;
  GO_BOARD *a;
  GO_POSI  *p_calc=NULL, *p_true;


  sprintf (fname, "D/x%dx%d.dat", ii, jj);
  if (!(fp = fopen (fname, "r"))) {
    printf ("File not found: %s \n", fname);
    return (0);
  }

  printf ("==================================\n\n");
  printf ("SOLVER_CHECK : %s \n\n", fname);

  p_true = xo_pos_create (STONE_NONE, ii, jj);
  GO_WORK *p_work = work_begin_ (/* ii, jj, */ p_true->a);
  YRAND_S;  


  //----------------------------------------------------------
  while (fgets (buffer, 70, fp) != NULL) {

    //-----------------------------------------------------------
    // читаем запись оценки-позиции из файла (p_true)
    if (strncmp(buffer, "WHITE", 5)==0) p_true->stone = STONE_WHITE;
    else
    if (strncmp(buffer, "BLACK", 5)==0) p_true->stone = STONE_BLACK;
    else
      continue; // пропускаются пробелы и разные рабочие записи

    read_board (fp, p_true->a, ii, jj);
    //-----------------------------------------------------------

    // сделаем копию и очистим доску от оценок--плюсиков (p_calc)
    p_calc = xo_pos_copy (/* NULL */p_calc, p_true);

    a = p_calc->a;
    for (i = 0; i < a->isize; i++) 
    for (j = 0; j < a->jsize; j++) {
      //if (BOARD (a, i, j) == '+')  BOARD (a, i, j) = STONE_NONE;

      if (BOARD (a, i, j) == STONE_NONE)  continue;
      if (BOARD (a, i, j) == STONE_WHITE) continue;
      if (BOARD (a, i, j) == STONE_BLACK) continue;

      BOARD (a, i, j) = STONE_NONE;
    }


    //------------------------------------------------------
    // MAIN CALC FUNCTION // Где-то здесь набегает память !!!
    //
    posi_add_moves (p_calc, p_work); 
    //------------------------------------------------------

    if (posi_compare (p_true, p_calc)) {
      num_true++;
    } else {
      if (is_print_err) {
      // печатать позицию
      posi_show (p_true);
      posi_show (p_calc);
      printf ("==================================\n\n");
      }
    }
    num_full++;

  } //while end
  //----------------------------------------------------------

  // MAIN CALC RESULT
  double fitness = (float)num_true / num_full * 100.0;

  fclose (fp);

  printf ("TRUE = %5.2f %% \n", fitness);

  return (fitness);
}
/******************************************************************************/


/*----------------------------------------------------------------------------*/
//
/*----------------------------------------------------------------------------*/
char* 
go_name_move (long move, long pstr_new)
{ 
  static char buf[80];
  char       *name;
  GO_MOVE    *mov;
  GO_BOARD   *board; 

  if (!move) return ("");

  mov = (GO_MOVE *) move;

  GO_GAMER *gamer = (GO_GAMER*) pstr_new;
  board = GMDESK; 

  if (!board) /* My */Error ("board");

  name = board_name_from_index (board, mov->i, mov->j);
  sprintf (buf, "%s", name);

  return (buf);
}
/*----------------------------------------------------------------------------*/
int get_name_num (char *string, char *name)
{
  int num;
  int offset = strlen (name);

  if (strlen(string) == offset)
    return (MAXPLY_NOT); // нет числа, т.е. глубина не задана явно

  num = atoi (string + offset);

  return (num);
}
/*----------------------------------------------------------------------------*/
GO_FIND_BESTMOVE_PROC
gamer_parse_name (char *name, 
                  int  *p_maxply, YT_BOOL *p_is_alfa_beta, int *p_find_mode)
{
  int     maxply;
  YT_BOOL is_alfa_beta;
  int     find_mode;
  GO_FIND_BESTMOVE_PROC find_proc = find_bestmove_minimax;

  if (!strncmp(name, "karl", 4)) {
    maxply       = MAXPLY_NOT/* get_name_num (name, "aigo") */;
    is_alfa_beta = TRUE;
    find_mode    = FIND_KARL;
    find_proc    = find_bestmove_m_carlo;
    //OUTD (2222222);
  } else 

  if (!strncmp(name, "aigo", 4)) {
    maxply       = get_name_num (name, "aigo");
    is_alfa_beta = TRUE;
    find_mode    = FIND_BEST_FULL;
  } else 

  if (!strncmp(name, "migo", 4)) {
    maxply       = get_name_num (name, "migo");
    is_alfa_beta = FALSE;
    find_mode    = FIND_SIMP_FULL;    //  ~6c
  } else 

  if (!strncmp(name, "baby", 4)) {
    maxply       = get_name_num (name, "baby");
    is_alfa_beta = FALSE;
    find_mode    = FIND_BABY;    //  ~6c
  } else 
    Error ("prog_name");

  *p_maxply       = maxply;
  *p_is_alfa_beta = is_alfa_beta;
  *p_find_mode    = find_mode;

  return (find_proc);
}
/*----------------------------------------------------------------------------*/
void 
gamer_init (GO_GAMER *gamer, char *name, int is_moves_map,  int memory_hash,
            GO_STONE color, YT_GOBAN *goban)
{

  int ii = goban->posi->a->isize;
  int jj = goban->posi->a->jsize;

  gamer->color = color;
  if (name) strcpy (gamer->name, name);
  else  Error ("prog_name");

  gamer->goban = goban;

  //----------------------------------------
  MM_PROC_MAP maps_proc;
  MM_PROC_H   hash_proc;

  int     maxply;
  YT_BOOL is_alfa_beta;
  int     find_mode;
  GO_FIND_BESTMOVE_PROC find_proc;

  find_proc = gamer_parse_name (name, &maxply, &is_alfa_beta, &find_mode);

  gamer->find_proc = find_proc;
  gamer->find_mode = find_mode;

  if (maxply == MAXPLY_NOT) {
    maxply = (ii * jj)*2 /* на всякий случай */;
  }

  if      (is_moves_map==1) maps_proc = go_moves_map1;
  else if (is_moves_map==2) maps_proc = go_moves_map2;
  else                      maps_proc = NULL; 

  if  (memory_hash) { 
    hash_proc = aigo_set_to_hashtable;
    gamer->movehash = gohash_create (ii, jj, memory_hash);
  } else  { // если указано 0 памяти, то хэш не нужен
    hash_proc = NULL; 
    gamer->movehash = NULL;
  }

  gamer->mm = mm_create (NULL, aigo_findmoves, 
                        aigo_evaluate, go_make_move, NULL, xo_free_posi,
                        go_name_move, go_name_posi, maps_proc, hash_proc, 
                        (int) color);

  mm_init2 (gamer->mm,  maxply, is_alfa_beta/* , MM_MIN_EVAL, MM_MAX_EVAL */);

  return;
}
/*----------------------------------------------------------------------------*/
GO_GAMER * 
gamer_create ()
{

  GO_GAMER *gamer = (GO_GAMER*) malloc (sizeof (GO_GAMER));

  return (gamer);
}
/*----------------------------------------------------------------------------*/
GO_GAMER * 
gamer_make (char *prog_name, int is_moves_map,  int memory_hash,
            GO_STONE color, YT_GOBAN *goban)
{

  GO_GAMER *gamer = gamer_create ();

  gamer_init (gamer, prog_name, is_moves_map, memory_hash, color, goban);

  return (gamer);
}
/******************************************************************************/

/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: evalut.c                                                  *
 *                                                                             *
  ******************************************************************************
 */   
// *****************************************************************************
// 
// TODO:
// 
// - сделать оц. функцию от нескольких факторов с коэффициентами;
// - использовать "myeuler" и другие факторы (из разных источников);
// - подсчитывать съеденные камни, в дальнейшем включать их в оценку;
// - можно, наоборот, использовать нейронный выбор лучших ходов и затем минимакс;
// 
// - но в идеале использовать "мозг" не только для оценки позиции, но в комплексе
//   для выбора хода (минимакс используется неявно);

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
int  
findmoves (long find_mode, GO_WORK *aigo_work, GO_POSI *pos, /* GO_BOARD *a,  */
           long *p_moves)
{  
  GO_BOARD *a     = pos->a;
  int num  = 0; 
  GO_BOARD *tmp  = NULL;

  GO_BOARD *old_a = work_set_board (aigo_work, a); 

  // здесь можно похоже значительно оптимизировать по времени
  // не просчитывать каждый раз все заново, а делать поправки на каждом ходе
  work_get_groups (aigo_work);  


  switch (find_mode) {
  case FIND_BEST:
  case FIND_BEST_FULL:

    // здесь очень важен порядок выбора ходов
    tmp = board_copy2 (tmp, a); 

    // да и здесь можно поработать на улучшение !!
    num = add_moves_defs (tmp, aigo_work, pos->stone, p_moves, num);
    num = add_moves_caps (tmp, aigo_work, pos->stone, p_moves, num);

    num = add_moves_cont (tmp, aigo_work, p_moves, num);
    num = add_moves_good (tmp, aigo_work, pos->stone, p_moves, num, 1); 

    if (find_mode == FIND_BEST_FULL)
    num = add_moves_full (tmp, p_moves, num); 

    break;
  case FIND_SIMP_FULL:
    num = add_moves_full (a, p_moves, num); 
    break;

  case FIND_BABY:
    tmp = board_copy2 (tmp, a); 

    num = add_moves_defs (tmp, aigo_work, pos->stone, p_moves, num);
    num = add_moves_caps (tmp, aigo_work, pos->stone, p_moves, num);

    num = add_moves_good (tmp, aigo_work, pos->stone, p_moves, num, 2); 
    num = add_moves_good (tmp, aigo_work, pos->stone, p_moves, num, 1); 
    num = find_moves_random (tmp, /* num_rand */2, p_moves, num);

    break;

  case FIND_KARL: // для монтекарло-перебора
    tmp = board_copy2 (tmp, a); 
    num = find_moves_random (tmp, /* num_rand *//* 2 */1, p_moves, num);
    //OUTD (num);
    //moves_print ((GO_MOVES *)p_moves);
    break;
  }

  work_set_board (aigo_work, old_a); 
  work_init (aigo_work); // для чистоты эксперимента

  return (num);
}
/*----------------------------------------------------------------------------*/
int  
aigo_findmoves (long /* pstr */l_gamer, long l_posi, long *p_moves, long /* param */find_mode)
{  
  GO_GAMER *gamer = (GO_GAMER*) l_gamer;
  GO_POSI  *pos   = (GO_POSI *) l_posi;
  GO_WORK  *work  = (GO_WORK *) (GMWORK); 

  int num = findmoves (gamer->find_mode, work, pos, /**/p_moves);

  return (num);
}
//*****************************************************************************/

/*----------------------------------------------------------------------------*/
YT_BOOL 
aigohash_get (GO_GAMER *gamer, GO_POSI *pos, int *p_result, int *p_numlev)
{
  GO_BOARD *a   = pos->a;
  Hashposition    p;
  unsigned long   hash;

  hash = myhash_calc (gamer->movehash, a, &p); 

  if (!myhash_get_result (gamer->movehash, hash, &p,  p_result, p_numlev))
    return (FALSE);

  return (TRUE);
}
/*----------------------------------------------------------------------------*/
YT_BOOL 
aigohash_set (GO_GAMER *gamer, GO_POSI *pos, int result, int numlev)
{
  GO_BOARD *a   = pos->a;
  Hashposition   p;
  unsigned long  hash;
  Hashtable *table = gamer->movehash;
  int  max_numlev;

  max_numlev = YInt ((a->isize*a->jsize) * table->koeff_tab);
  //OUTD (max_numlev);

  if (numlev > max_numlev)  return (FALSE);
  // 3 - очень долго
  // 4 - приемлимо, но дольше раза в 2
  // 5 - самое оптимальное значение для доски 3х4 (при коефф.табл = 0.4)
  // 6 - совсем долго

  hash = myhash_calc (table, a, &p); 

  if (myhash_set_result (table, hash, &p,  result, numlev))
    return (TRUE);
  
  return (FALSE);
}
/*----------------------------------------------------------------------------*/
void 
aigo_set_to_hashtable (long pstr, long posi, int eval, 
                       int numlev /* чтобы не все записывать */)
{
  GO_GAMER *gamer = (GO_GAMER*) pstr;
  GO_POSI  *pos   = (GO_POSI *) posi;

  aigohash_set (gamer, pos, eval,  numlev);

  return;
}
/*----------------------------------------------------------------------------*/
int 
aigo_evaluate (long pstr, long l_posi, int *p_numlev)
{
  // оценочная функция для позиции (за того, чей текущий ход)

  GO_GAMER *gamer = (GO_GAMER*) pstr;
  GO_POSI  *posi  = (GO_POSI *) l_posi;
  GO_WORK  *work  = (GO_WORK *) (GMWORK);

  GO_STONE  me   = gamer->color; // с чьей позиции оценивается !
 
  GO_STONE  he   = STONE_NEXT (me);
  GO_STONE  last = STONE_NEXT (posi->stone);

  int MAX_SCORE = MM_MAX_EVAL;
  int /* s, */ min_score = -MAX_SCORE, max_score = MAX_SCORE;
  int he_dames, me_dames, he_capts=0, me_capts=0;
  int s, l=-1000;

  //-----------------------------------
  // проверка хэш-таблицы, в дальнейшем ее надо вынести в MINMAX.C

  if (gamer->movehash)
  if (aigohash_get (gamer, posi, &s, &l)) {
    if (p_numlev) *p_numlev = l;
    return (s);
  }
  //-----------------------------------

  // это все уже должно расчитаться сразу после сделанного хода !?
  GO_BOARD *old_a = work_set_board (work, posi->a); 

  work_get_groups (work/* , posi->a */);  

  work_get_calcus (work, me, &me_dames, /* NULL */ &me_capts); 
  work_get_calcus (work, he, &he_dames, /* NULL */ &he_capts); 

  // будем учитывать и захваченных!
  // work_get_captures (work, TRUE, &b_cap, &w_cap);
/*   if (debug) */
/*     fprintf (stderr, "last= %d  he_dames= %d  me_dames= %d \n",  */
/*              last, he_dames, me_dames); */

  if      (last==me && he_dames==0) s = max_score;   // атака удалась,  я  его убил
  else if (last==me && me_dames==0) s = min_score-1; // я сам себя убил (суицид-плохо)

  else if (last==he && me_dames==0) s = min_score;   // атака удалась, он меня убил
  else if (last==he && he_dames==0) s = max_score+1; // он сам себя убил (суицид-плохо)

  else {
    work_set_board (work, old_a); 
    work_init (work); // для чистоты эксперимента

    s = me_dames - he_dames;
    return (s);
  }
  //-----------------------------------

  //??????????
  //aigohash_set (gamer, posi, s);
  work_set_board (work, old_a); 
  work_init (work); // для чистоты эксперимента

  if (p_numlev) *p_numlev = l;

/*   if (debug) */
/*     fprintf (stderr, "s= %d \n", s); */

  return (s);
}
/******************************************************************************/
/******************************************************************************/
//                                                                            //
/******************************************************************************/
