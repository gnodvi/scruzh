/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: x_work.c                                                  *
 *                                                                             *
  ******************************************************************************
 */  


/*----------------------------------------------------------------------------*/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <conio.h>
#include <ctype.h>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
//#include <values.h>  

/* #ifdef _UNIX  */
#include <unistd.h>
//#include <time.h>
#define HAVE_RANDOM 1
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

#include  <stdarg.h>
#include  <math.h>
/*----------------------------------------------------------------------------*/

#include "a_comm.h"
//#include "a_draw.h"

#include "x_comm.h"
#include "x_hash.h"
#include "x_work.h"

/*----------------------------------------------------------------------------*/
//#define HAVE_CONFIG_H
/* Define if you have the random function.  */

//#define HAVE_RANDOM 1

//                                                                             /
/******************************************************************************/

/*----------------------------------------------------------------------------*/
GO_MOVES*
moves_create ()
{
  GO_MOVES *mvs = malloc (sizeof (GO_MOVES));
  if (!mvs) Error ("moves_create");

  mvs->moves_num = 0;
  return (mvs);
}
/*----------------------------------------------------------------------------*/
void
moves_free (GO_MOVES *mvs)
{

  free (mvs);
  return;
}
/*----------------------------------------------------------------------------*/
void
moves_init (GO_MOVES *mvs)
{

  mvs->moves_num = 0;

  return;
}
/*----------------------------------------------------------------------------*/
void
moves_add (GO_MOVES *mvs, int i, int j, int stone)
{
  int num = mvs->moves_num;

  if (num == 100) YERROR ("moves_add");

  ((mvs->moves)[num]).i = i;
  ((mvs->moves)[num]).j = j;

  mvs->moves_num++;
  return;
}
/*----------------------------------------------------------------------------*/
GO_MOVE*
moves_get (GO_MOVES *mvs, int n)
{
  GO_MOVE *mv = &((mvs->moves)[n]);

  return (mv);
}
/*----------------------------------------------------------------------------*/
void
moves_del (GO_MOVES *mvs, int n_del)
{
  int n;

  if (n_del >= mvs->moves_num)
    YERROR ("n_del >= mvs->moves_num");

  for (n=n_del; n<mvs->moves_num; n++) {
    ((mvs->moves)[n]).i = ((mvs->moves)[n+1]).i;
    ((mvs->moves)[n]).j = ((mvs->moves)[n+1]).j;
    ((mvs->moves)[n]).move_stone = ((mvs->moves)[n+1]).move_stone;
  }

  mvs->moves_num--;
  return;
}
/*----------------------------------------------------------------------------*/
void
moves_print (GO_MOVES *mvs)
{
  int n;

  for (n=0; n < mvs->moves_num; n++) {
    printf ("(");
    printf ("%d ", ((mvs->moves)[n]).i);
    printf ("%d ", ((mvs->moves)[n]).j);
    printf ("%d ", ((mvs->moves)[n]).move_stone);
    printf (")");
  }
  printf ("\n");

  return;
}
/*----------------------------------------------------------------------------*/
int 
moves_num (GO_MOVES *mvs)
{

  return (mvs->moves_num);

}
/*----------------------------------------------------------------------------*/

/******************************************************************************/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/* static *//* extern */  int  is_initialized = 0;


/* Random values for the hash function.  For stones and ko position. */
/* static */ unsigned long  white_hash[MAX_BOARD][MAX_BOARD];	
/* static */ unsigned long  black_hash[MAX_BOARD][MAX_BOARD];	
/* static */ unsigned long  ko_hash[MAX_BOARD][MAX_BOARD];

/* Bit patterns for white stones in the compact board representation. */
/* static */ unsigned long white_patterns[] = {
  0x00000001, 0x00000004, 0x00000010, 0x00000040, 
  0x00000100, 0x00000400, 0x00001000, 0x00004000,
  0x00010000, 0x00040000, 0x00100000, 0x00400000,
  0x01000000, 0x04000000, 0x10000000, 0x40000000,
};

/* Bit patterns for black stones in the compact board representation. */
/* static */ unsigned long black_patterns[] = {
  0x00000002, 0x00000008, 0x00000020, 0x00000080, 
  0x00000200, 0x00000800, 0x00002000, 0x00008000,
  0x00020000, 0x00080000, 0x00200000, 0x00800000,
  0x02000000, 0x08000000, 0x20000000, 0x80000000,
};

int       board_isize, board_jsize;
GO_BOARD *bs[NUM_SYM];


//------------------------------------------------------------------------------
/*
 * Initialize the entire hash system.
 */
//------------------------------------------------------------------------------
void
gohash_init ()
{
  int  i;
  int  j;

  //#if !TRACE_READ_RESULTS

#ifdef HAVE_RANDOM
  srandom (time(0));
#else
  srand (time(0));
#endif

  //#else

/* #ifdef HAVE_RANDOM */
/*   srandom (0); */
/* #else */
/*   srand (0); */
/* #endif */

/* #endif */

  for (i = 0; i < MAX_BOARD; ++i)
  for (j = 0; j < MAX_BOARD; ++j) {

#ifdef HAVE_RANDOM
    white_hash[i][j] = random();
    black_hash[i][j] = random();
    ko_hash[i][j]    = random();
#else
    white_hash[i][j] = rand();
    black_hash[i][j] = rand();
    ko_hash[i][j]    = rand();
#endif
    }
  is_initialized = 1;
}
//------------------------------------------------------------------------------
/*
 * Take a go position consisting of the board and a possible ko,
 * and store this in a compact form into *pos.
 */
//------------------------------------------------------------------------------
void
board_to_position (board_t board[MAX_BOARD][MAX_BOARD]/* ,  */
/*                    int koi, int koj */,  Hashposition *pos)
{
  int            index;
  int            subindex;
  unsigned long  bits;
  int            i, j;

  /* Initialize the compact board. */
  for (i = 0; i < COMPACT_BOARD_SIZE; i++)
    pos->board[i] = 0;

  /* Put 16 locations into one long using 2 bits per location. */
  index    = 0;
  subindex = 0;
  bits     = 0;

  for (i = 0; i < board_isize; i++)
  for (j = 0; j < board_jsize; j++) {

    if (board[i][j] != EMPTY) {
      if      (board[i][j] == WHITE)  bits |= white_patterns[subindex];
      else if (board[i][j] == BLACK)  bits |= black_patterns[subindex];
    }

    if (++subindex == 16) {
      pos->board[index++] = bits;
      bits = 0;
      subindex = 0;
    }
  }

  /* Store the last long into the compact board. 
     We only need to do this if we didn't just store it inside the loop. */
  if (subindex != 0)
    pos->board[index] = bits;

  /* The rest of the info, beside the board. */
  //pos->ko_i    = koi;
  //pos->ko_j    = koj;

  return;
}
/*----------------------------------------------------------------------------*/
double
calc_sizes (int isize, int jsize, int memory_free, /* Mb */
            unsigned long *p_table_num, unsigned long *p_nodes_num) 
{ 
  unsigned long   nodes_num=0, table_num;
  unsigned long   nodes_mem;
  //Hashtable *table;
  double   koeff_tab = 1.0;
  int      ij = isize*jsize;
  double   dnodes_num = pow (3, ij);

  if (isize==jsize) dnodes_num = dnodes_num / 8;
  else              dnodes_num = dnodes_num / 4;

  if (ij > 6)  // эмпирический масштабный коэф. (нереальности позиций)
  dnodes_num = dnodes_num / 3.0;

  //if (dnodes_num > MAXLONG) {
  if (dnodes_num > G_MAXLONG) {
    //#ifdef _UNIX
    printf ("dnodes_num= %f  MAXLONG= %ld (*10^6) \n", dnodes_num/1000000, G_MAXLONG/1000000);
    //#endif
    Error ("OVERFLOW");
  } else {
    nodes_num = dnodes_num;
  }

  //if (memory_free == 0) { // брать из учета размеров доски
  //  ;;;;
  //} else {
  printf ("nodes_num = %ld \n", nodes_num);
  nodes_mem = (memory_free*1024*1024) / (sizeof (Hashnode) + sizeof(Hashnode*)) / 2.0 ;
  printf ("nodes_mem = %ld \n", nodes_mem);

  //if (nodes_mem < 0) { // условный сигнал (отрицательная память - в точности ее задавать)
  //  
  //} else
  if (nodes_num > nodes_mem) {
    nodes_mem = fabs (nodes_mem);// условный сигнал (отрицательная память - в точности ее задавать)

    // и здесь надо запомнить сколько от необходимого выделено !!!
    // и записывать в хэштаблицу только первые уровни (?)
    // вполне возможно что это - фигня !!!!!
    koeff_tab = (double) nodes_mem / (double) nodes_num;
    printf ("koeff_tab = %f \n", koeff_tab);
    // надо сразу посчитать макс. глубину записей в хэш
    //maxlevhash = 

    nodes_num = nodes_mem;
  }
    //}

  table_num = /* 1.5 *  */nodes_num; // чем больше кофф, тем меньше коллизий

  *p_table_num = table_num;
  *p_nodes_num = nodes_num;

  return (koeff_tab);
}
/*----------------------------------------------------------------------------*/
Hashtable *
gohash_create (int isize, int jsize, 
               int memory_free /* Megabytes of memory used for hash table. */) 
{
  unsigned long nodes_num, table_num;
  Hashtable *table;
  double   koeff_tab;

  if (memory_free == 0)  return (NULL);

  koeff_tab = calc_sizes (isize, jsize, memory_free, &table_num, &nodes_num);

  gohash_init ();
  table = hashtable_new (table_num, nodes_num, sizeof (Hashnode), koeff_tab);

  if (!table) Error ("gohash_create");
  board_isize = isize;
  board_jsize = jsize;

  int i;
  for (i=0; i<NUM_SYM; i++) 
    bs[i] = aboard_begin (isize, jsize); 

  return (table);
}
/*---------------------------------------------------------------------------*/
void
aboard_to_board (GO_BOARD *b, board_t p[MAX_BOARD][MAX_BOARD])
{
  int i, j;

  for (i=0; i < b->isize; i++) 
  for (j=0; j < b->jsize; j++) {
    //p[i][j] = NONE;

    if      (BOARD (b, i, j) == STONE_NONE)   p[i][j] = NONE;
    else if (BOARD (b, i, j) == STONE_BLACK)  p[i][j] = BLACK;
    else if (BOARD (b, i, j) == STONE_WHITE)  p[i][j] = WHITE;
    else {
      printf ("i=%d j=%d ch=%c d=%d \n", i, j, BOARD (b, i, j), BOARD (b, i, j));
      Error ("aboard_to_board");
    }
  }

  return;
}
//------------------------------------------------------------------------------
/*
 * Take a go position consisting of the board and a possible k,
 * and generate a hash value from it.
 * 
 * See the Texinfo documentation (Reading/Hashing) for more information.
 */
//------------------------------------------------------------------------------
unsigned long
board_hash (Hashtable *ht, board_t board[MAX_BOARD][MAX_BOARD]/* , int koi, int koj */)
{
  unsigned int  hash;
  int           i;
  int           j;

  /* If the hash system is not initialized, do so now. */
  if (!is_initialized)
    gohash_init();

  /* Get the hash value for all the stones on the board. */
  hash = 0;

  for (i = 0; i < board_isize; ++i)
  for (j = 0; j < board_jsize; ++j) {
    switch (board[i][j]) {
    case WHITE:
      hash = hash ^ white_hash[i][j];
      break;
    case BLACK:
      hash = hash ^ black_hash[i][j];
      break;
    default:
      break;
    }
  }

  /* If there is a ko going on, take this into consideration too. */
  //if (koi != -1) 
  //  hash ^= ko_hash[koi][koj];

  return hash;
}
/*---------------------------------------------------------------------------*/
unsigned long
myhash_calc (Hashtable *ht, GO_BOARD *b, Hashposition *p_pos) 
{
  unsigned long hash, hash_min = 0x88888888;
  board_t  p[MAX_BOARD][MAX_BOARD];

  //GO_BOARD *bs[NUM_SYM];
  int i, i_min=0, num_sym;

  for (i=0; i<NUM_SYM; i++) 
    aboard_clean (bs[i]); 

  num_sym = board_symm_calc (bs, b);

  for (i=0; i<num_sym; i++) {
    aboard_to_board (bs[i], p);

    board_to_position (p, p_pos);
    hash = board_hash (ht, p);

    if (hash < hash_min) {
      hash_min = hash;
      i_min = i;
    }
  }

  aboard_to_board (bs[i_min], p);
  board_to_position (p, p_pos);

  return (hash_min);
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: gowork.c                                                  *
 *                                                                             *
  ******************************************************************************
 */  
// *****************************************************************************
// 
// TODO:
// 
// + убрать запоминание ходов "work_set_move";
// + последний ход для вычисл. брать как параметр;
// 

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
void
work_init (GO_WORK *w)
{
  int  i, j;
  int  bsize  = BSIZE;

  for (i=0; i < bsize; i++) 
  for (j=0; j < bsize; j++) 
    ((w->goban)[i][j]).stone = STONE_NONE;

  for (i=0; i < BSIZE2; i++) 
   ((w->groups)[i]).stone = STONE_NONE;      

  return;
}
/*----------------------------------------------------------------------------*/
GO_BOARD * 
work_set_board (GO_WORK *w, GO_BOARD *a) 
{ 
  GO_BOARD *a_old;

  if (!a)  return (NULL);

  a_old = w->aboard;

  w->aboard = a;

  w->isize  = a->isize;
  w->jsize  = a->jsize;

  return (a_old);
}
/*----------------------------------------------------------------------------*/
GO_WORK *
work_begin_ (GO_BOARD *a)
{

  GO_WORK *w = (GO_WORK*) malloc (sizeof (GO_WORK));
  if (!w) Error ("work_begin");

  //w->aboard = a;
  //w->isize  = a->isize;
  //w->jsize  = a->jsize;
  work_set_board (w, a) ;

  work_init (w);

  return (w);
}
/*----------------------------------------------------------------------------*/
char *
get_stone_name (int stone) 
{

  if      (stone == STONE_WHITE)  return ("White");  
  else if (stone == STONE_BLACK)  return ("Black");
  else if (stone == STONE_NONE )  return ("None" );
  else                           return ("Don't defined stone");

}
/*----------------------------------------------------------------------------*/
void   
work_groups_print (GO_WORK *w) 
{ 
  //GO_WORK *w = ptr_work;
  int n;
  GO_GROUP *g;
  char *stone;

  printf ("groups_num = %d \n", w->groups_num);

  for (n=0; n < w->groups_num; n++) {
    g = &(w->groups[n]);
    stone = (g->stone==STONE_WHITE) ? "White" : "Black";
    printf ("%s group: stones=%d dame=%d \n", stone, g->ij_num, g->xy_num); 
  }

  return;
}
/*----------------------------------------------------------------------------*/

/******************************************************************************/
//                                                                             /
/******************************************************************************/


/*----------------------------------------------------------------------------*/
GO_GROUP*
work_find_captured_group (GO_WORK *w, int stone)
{
  GO_GROUP *gr=NULL;
  int n;

  for (n=0; n < w->groups_num; n++) {
    gr = &(w->groups[n]);
    if (gr->xy_num==0) { // группа с нулевым дамэ
      //printf ("captured group: %d   move_stone: %d \n", g->stone, move_stone); //?????
      if (gr->stone==stone)  
      break; // нашли захваченную группу данного цвета
    }
  }

  if (n == w->groups_num) 
    return (NULL); // нет ни одной захваченной группы
  else 
    return (gr);
}
/*----------------------------------------------------------------------------*/
void   
work_add_dame_to_group (GO_WORK *w, GO_GROUP *gr, int i, int j) 
{ 
  int d; 

  for (d=0; d < gr->xy_num; d++) {
    if ((gr->x[d]==i) && (gr->y[d]==j)) 
      return; // уже есть такое дамэ в группе
  }

  gr->x[gr->xy_num] = i;
  gr->y[gr->xy_num] = j;
  gr->xy_num++;

  return;
}
/*----------------------------------------------------------------------------*/
void   
work_this_is_dame (GO_WORK *w, GO_GROUP *gr, int i, int j) 
{ 
  int DAME = w->goban[i][j].dame;

  // добавим это дамэ к группе
  work_add_dame_to_group (w, gr, i, j); 

  if (DAME==gr->stone) return; // уже посчитали это даме за свое (НО ЭТОЙ ЛИ ГРУППЫ????!)
  if (DAME==STONE_CONTACT_DAME) return; // а это за общее  (НО ЭТОЙ ЛИ ГРУППЫ????!)

  if (DAME==STONE_NONE) DAME = gr->stone;
  else                  DAME = STONE_CONTACT_DAME;

  return;
}
/*----------------------------------------------------------------------------*/
void   
work_check_iteration (GO_WORK *w, GO_GROUP *gr, int i, int j) 
{ 
  // проверка на выход за границу
  if (i<0 || j<0 || i>w->isize-1 || j>w->jsize-1)
    return;

  if (WBOARD(i,j) == STONE_NONE) {
    work_this_is_dame (w, gr, i, j); 
    return;
  } 

  if (WBOARD(i,j) != gr->stone) 
    // это - камень противника, здесь делать нечего
    return;

  //----------------------------------------------
  //----------------------------------------------
  // а это значит камень нашего цвета

  if (/* ptr_work */w->goban[i][j].stone != STONE_NONE)
  return; // здесь мы уже были

  // отметим камень на схеме
  /* ptr_work */w->goban[i][j].stone = gr->stone;
  /* ptr_work */w->goban[i][j].group = gr;
  // добавим камень в группу
  gr->i[gr->ij_num] = i;
  gr->j[gr->ij_num] = j;
  gr->ij_num++;
  
  work_check_iteration (w, gr, i,  j-1);
  work_check_iteration (w, gr, i,  j+1);
  work_check_iteration (w, gr, i+1,  j);
  work_check_iteration (w, gr, i-1,  j);

  return;
}
/*----------------------------------------------------------------------------*/
void   
work_get_group (GO_WORK *w, int i, int j) 
{ 
  // описать группу для этого пункта-камня
  //GO_GROUP *gr = &(ptr_work->groups[ptr_work->groups_num]);
  GO_GROUP *gr = &(w->groups[w->groups_num]);

/*   if (debug) */
/*   fprintf (stderr, "work_get_group: i= %d   j= %d  stone= %s \n", */
/*            i, j, get_stone_name (WBOARD (i, j))); */

  gr->stone = WBOARD(i,j);
  gr->ij_num = 0;
  gr->xy_num = 0;

  work_check_iteration (w, gr, i, j);

  return;
}
/*----------------------------------------------------------------------------*/
void   
work_get_groups (GO_WORK *w/* , GO_BOARD *a */) 
{ 
  int i, j;

  // здесь переустановка указателя на доску ?!
  //work_set_board (w, a); 
  //if (a)  w->aboard = a;
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  for (i=0; i < w->isize; i++) 
  for (j=0; j < w->jsize; j++) {
    w->goban[i][j].dame  = STONE_NONE;
    w->goban[i][j].stone = STONE_NONE;
    w->goban[i][j].group = NULL;
  }

  w->groups_num = 0;

  for (i=0; i < w->isize; i++) 
  for (j=0; j < w->jsize; j++) {
    if (WBOARD(i,j) == STONE_NONE) 
    continue;

    //(format t "wboard_i_j= ~s ~%" (get_stone_name (WBOARD i j)))
/*     if (debug) */
/*     fprintf (stderr, "wboard_i_j= %s \n", */
/*              get_stone_name (WBOARD (i, j))); */

    if (w->goban[i][j].stone != STONE_NONE) 
    continue;// здесь мы уже были

    work_get_group (w, i, j);
    w->groups_num++; 
  }

  return;
}
/*----------------------------------------------------------------------------*/
void  
work_get_calcus (GO_WORK *w, GO_STONE stone, int *p_min_dames, int *p_num_capts) 
{ 
  GO_GROUP *gr;
  int n;

  int dames, min_dames = 1000; 
  int num_capts = 0;
  
/*   if (debug)  */
/*     work_groups_print (w); */
 

  for (n=0; n < w->groups_num; n++) {
    gr = &(w->groups[n]);
    if (gr->stone != stone) continue;
 
    dames = gr->xy_num;
    if (dames == 0) num_capts++;

    min_dames = MIN (min_dames, dames);
  }


  if (min_dames==1000) { // а если нет еще групп (т.е. вообще камней??!!)
    min_dames = -1;
  }

  if (p_min_dames) *p_min_dames = min_dames;
  if (p_num_capts) *p_num_capts = num_capts;

  return;
}
/*----------------------------------------------------------------------------*/
int   
work_calc_mindames (GO_WORK *w, GO_STONE stone) 
{ 
  GO_GROUP *gr;
  int n;

  int dames, min_dames = 1000; 
  
  for (n=0; n < w->groups_num; n++) {
    gr = &(w->groups[n]);
    if (gr->stone != stone) continue;
 
    dames = gr->xy_num;

    min_dames = MIN (min_dames, dames);
  }

  if (min_dames==1000) { // а если нет еще групп (т.е. вообще камней??!!)
    min_dames=-1;
  }

  return (min_dames);
}
/*----------------------------------------------------------------------------*/
int   
work_move_mindames (GO_WORK *w, int stone, int i, int j) 
{ 
  int min_dames;
  GO_BOARD *a = w->aboard;

  // ставим камень на доску и расчитываем инфу о группах
  aboard_set_stone_ (w->aboard, stone, i, j);
  work_get_groups (w/* , NULL */);

  min_dames = work_calc_mindames (w, stone);

  // убираем камень с доски и пересчитываем инфу
  aboard_del_stone (a, i, j);
  work_get_groups (w/* , NULL */);

  return (min_dames);
}
/*----------------------------------------------------------------------------*/
int   
work_is_move_suicide (GO_WORK *w, int stone, int i, int j) 
{ 
  int min_dames;

  min_dames = work_move_mindames (w, stone, i, j); 

  if (min_dames==0) return (TRUE);
  else              return (FALSE);
}
/*----------------------------------------------------------------------------*/
int   
work_is_this_good (GO_WORK *w, int my_stone, int i, int j) 
{ 
  int min_dames;

  min_dames = work_move_mindames (w, my_stone, i, j); 

  if (min_dames<=1) return (FALSE);
  else              return (TRUE);
}
/*----------------------------------------------------------------------------*/
int   
work_random_stone (GO_WORK *w, int stone, int *i_move, int *j_move) 
{ 
  int i, j, n, nmax=30;
  int num=0;

  for (n=0; n<nmax; n++) {
    i = YRAND (0, w->isize-1);
    j = YRAND (0, w->jsize-1);
    
    if (aboard_is_move_legal(w->aboard, i, j) && !work_is_move_suicide (w, stone, i, j) ) {
      *i_move = i;
      *j_move = j;
      num++;
      break;
    }
  }

  return (num);
}
/*----------------------------------------------------------------------------*/
/* int */
/* work_del_move (GO_WORK *w) */
/* { */
/*   GO_BOARD *a = w->aboard; */
/*   int i, j; */

/*   if (w->moves_num==0) YERROR ("board_del_move"); */

/*   i =  (w->moves)[w->moves_num-1].i; */
/*   j =  (w->moves)[w->moves_num-1].j; */

/*   aboard_del_stone (a, i, j); */

/*   w->moves_num--;   */
/*   return (TRUE); */
/* } */
/*----------------------------------------------------------------------------*/
/* int */
/* work_set_move (GO_WORK *w, int stone, int i_move, int j_move) */
/* { */

/*   (w->moves)[w->moves_num].i = i_move; */
/*   (w->moves)[w->moves_num].j = j_move; */
/*   (w->moves)[w->moves_num].move_stone = stone; */

/*   w->moves_num++; */
  
/*   return (TRUE); */
/* } */
/*----------------------------------------------------------------------------*/
int   
work_random_move_dame (GO_WORK *w, int stone, int *i_move, int *j_move, int dame) 
{ 
  int i, j, n, nmax=30;
  int min_dames;

  for (n=0; n<nmax; n++) {

    if (!board_random_stone (w->aboard, &i, &j))
      break;

    min_dames = work_move_mindames (w, stone, i, j);
 
    if (min_dames >= dame) {
      *i_move = i;
      *j_move = j;
      return (TRUE);
    }    
  }

  return (FALSE);
}
/*----------------------------------------------------------------------------*/
int   
work_random_move (GO_WORK *w, int stone, int *i_move, int *j_move) 
{ 

  if (work_random_move_dame (w, stone, i_move, j_move, 2))
    return (TRUE);

  if (work_random_move_dame (w, stone, i_move, j_move, 1))
    return (TRUE);

  if (work_random_move_dame (w, stone, i_move, j_move, 0)) // если нет ходов, то делать суицид !!
    return (TRUE);

  return (FALSE);
}
/*----------------------------------------------------------------------------*/
void
work_get_captures (GO_WORK *w, int is_set_captures, int *p_bcap, int *p_wcap, GO_STONE last_move)
{
  GO_STONE last_stone;

  int bcap=0, wcap=0;
  int n, dead_stone, enemy_stone, k, stone_cap/* , stone */;

  GO_GROUP *gr;
  //GO_BOARD *a = w->aboard;

  work_get_groups (w/* , NULL */);

  // необходимо знать цвет последнего хода !!!!!! ??????
  // но это известно при заданной позиции !!!
  //last_stone = (w->moves)[w->moves_num-1].move_stone;
  last_stone = last_move;

  enemy_stone = STONE_OTHER (last_stone);

  if (!(gr = work_find_captured_group (w, enemy_stone)))
    if (!(gr = work_find_captured_group (w, last_stone))) {
    *p_bcap = 0;
    *p_wcap = 0;
    return; // нет ни одной захваченной группы
  }
 
  stone_cap = gr->stone;
  // Группа этого камня - захвачена!!
 
  if (stone_cap==STONE_WHITE) {
    bcap = TRUE;
    dead_stone = STONE_WHITE_DEAD;
  } else {
    wcap = TRUE;
    dead_stone = STONE_BLACK_DEAD;
  }

  if (is_set_captures) {
    for (n=0; n < w->groups_num; n++) {
      gr = &(w->groups[n]);
      if (gr->xy_num!=0) continue;
      if (gr->stone!=stone_cap) continue;

      for (k = 0; k < gr->ij_num; k++) {
        WBOARD (gr->i[k], gr->j[k]) = dead_stone;
      }
    }
  }

  *p_bcap = bcap;
  *p_wcap = wcap;
  return;
}
/******************************************************************************/
//                                                                             /
/******************************************************************************/


/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: myeuler.c                                                 *
 *                                                                             *
  ******************************************************************************
 */  
/******************************************************************************/

#define MATRIX_REF(PR, NUMROWS, R, C) (*((PR) + (NUMROWS)*(C) + (R)))

int weights2[2][2] = {{1, 4}, {2, 8}};

/*----------------------------------------------------------------------------*/
int
Nhood2Offset (int *pBWin, int II, int JJ, int r, int c) 
{
  int maxR, maxC;
  int rr, cc;
  int result = 0;

  /* Figure out the neighborhood extent that does not go past */
  /* image boundaries */
  if (r == (II-1)) maxR = 0;
  else             maxR = 1;
  

  if (c == (JJ-1)) maxC = 0;
  else             maxC = 1;
 

  for (rr = 0; rr <= maxR; rr++) 
  for (cc = 0; cc <= maxC; cc++) {
    result += weights2[rr][cc] *
      (MATRIX_REF(pBWin, II, r+rr, c+cc) != 0);
   
  }

  return(result);
}
/*----------------------------------------------------------------------------*/
void 
Compute2by2DoubleResult (double *BWout, int *BWin, double *lut, int II, int JJ) 
{    
  int r, c, off;

  for (c = 0; c < JJ; c++) 
  for (r = 0; r < II; r++) {
    off = Nhood2Offset (BWin, II, JJ, r, c);

    MATRIX_REF (BWout, II, r, c) = *(lut + off);  
  }
   
  return;
}
/*----------------------------------------------------------------------------*/
void  
dmatrix_print (double *image, int II, int JJ) 
{ 
  // печатаем матрицу
  double b;
  int    i, j;

  printf ("\n");

  for (i = 0; i < II; i++) { 
  for (j = 0; j < JJ; j++) {
    b = MATRIX_REF (image, II, i, j);  
    printf ("%.1f  ", b);
  }
  printf ("\n");
  }

}   
/*----------------------------------------------------------------------------*/
void  
imatrix_print (int *image, int II, int JJ) 
{ 
  // печатаем матрицу
  int  b;
  int  i, j;

  printf ("\n");

  for (i = 0; i < II; i++) { 
  for (j = 0; j < JJ; j++) {
    b = MATRIX_REF (image, JJ, i, j);  
    printf ("%3d  ", b);
  }
  printf ("\n");
  }
 
}   
/*----------------------------------------------------------------------------*/
int 
myeuler_calc (int *image, int ii, int jj) 
{   
  YT_BOOL is_print = FALSE;
  int     II = ii + 2;
  int     JJ = jj + 2;
  int     e = 999;
  double  o[16] = {0, 0.25, 0.25, 0, 0.25, 0, 0.5, -0.25, 0.25, 0.5, 0, -0.25, 0, -0.25, -0.25, 0};
  double  lut[16];
  int     i, j;
  double  sum;

  int    *image_big;    
  double *image_out; 

  image_big = malloc (II*JJ*sizeof(int));
  image_out = malloc (II*JJ*sizeof(double));

  // формируем "правильную" просмотровую таблицу
  for (i = 0; i < 16; i++) { 
    lut[i] = 4 * o[i] + 2;
    if (is_print) printf ("%.1f  ", lut[i]);
  }
  if (is_print) printf ("\n");

  // дополнительная проверка
  //a = bwcheck (a);

  // инициализация нулями (можно только края расширения!)
  for (i = 0; i < II; i++)  
  for (j = 0; j < JJ; j++) 
    MATRIX_REF (image_big, II, i, j) = 0;  
 
  // копируем в расширенную матрицу (с краями)
  for (i = 0; i < ii; i++)  
  for (j = 0; j < jj; j++) {
    MATRIX_REF (image_big, II, i+1, j+1) = MATRIX_REF (image, ii, i, j);  
  }

  // "продолжаем" граничные камни на бортик!
/*   for (i = 0; i < 5; i++) {  */
/*     MATRIX_REF (image_big, 7, i+1, 0) = MATRIX_REF (image, 5, i, 0);  */
/*     MATRIX_REF (image_big, 7, i+1, 6) = MATRIX_REF (image, 5, i, 4);  */
/*   }  */
/*   for (j = 0; j < 5; j++) {  */
/*     MATRIX_REF (image_big, 7,  0, j+1) = MATRIX_REF (image, 5,  0, j);  */
/*     MATRIX_REF (image_big, 7,  6, j+1) = MATRIX_REF (image, 5,  4, j);  */
/*   }  */

  if (is_print) imatrix_print (image_big, II, JJ); 

  // это почти функция:  weights = applylut (b, lut);
  Compute2by2DoubleResult (image_out, image_big, lut, II, JJ);
   
  if (is_print) dmatrix_print (image_out, II, JJ); 

  sum = 0;
  for (i = 0; i < II*JJ; i++)  
    sum += image_out[i];

  e = (sum - 2 * (II*JJ)) / 4;
             
  return (e);
}
/*----------------------------------------------------------------------------*/
void 
myeuler_test () 
{ 
  int e;

  int test_image[5*5] = {
    1, 1, 0, 0, 0,
    1, 1, 0, 0, 0,
    0, 0, 1, 1, 1,
    0, 0, 1, 0, 1,
    0, 0, 1, 1, 1, 
  };

  // Почему нельзя сделать дырку в углу ?
  // И что тогда делать, ведь бортик - важная часть игры?
  // Нужно расширять камни на бортик!!!!

  e = myeuler_calc (test_image, 5, 5);

  printf ("\n");
  printf ("Euler's Number = %d \n\n", e);

  return;
}
/*----------------------------------------------------------------------------*/


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
// goeval.c 
/******************************************************************************/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
void   
work_get_contact_dames (GO_WORK *w, GO_MOVES *mvs) 
{ 
  int i, j, num=0;

  for (i = 0; i < w->isize; i++) 
  for (j = 0; j < w->jsize; j++) {
    if (w->goban[i][j].dame != STONE_CONTACT_DAME) 
      continue;

    (mvs->moves)[num].i = i;
    (mvs->moves)[num].j = j;

    num++;
  }

  mvs->moves_num = num;
  return;
}
/*----------------------------------------------------------------------------*/
int   
work_calc_mindames_diff (GO_WORK *w, GO_STONE stone) 
{ 
  int  my, he, diff;

  // оцениваем новую позицию и запоминаем
  my  = work_calc_mindames (w, stone);
  he  = work_calc_mindames (w, STONE_OTHER(stone));

  //if (my == 0 ) diff = VALUE_MIN; //???!
  //else          diff = my - he;

  diff = my - he;

  return (diff);
}
/*----------------------------------------------------------------------------*/
void   
weakes_calc (GO_WORK *w, GO_STONE stone, GO_MOVES *mvs) 
{ 
  int  i, j, n, mindames;

  for (n=0; n < mvs->moves_num; n++) {

    // а надо ли мне все это?? (только чтобы продолжить серию 'simple'!)
    i = (mvs->moves)[n].i;
    j = (mvs->moves)[n].j;

    // пробуем ход 
    WBOARD (i, j) = stone;
    work_get_groups (w/* , NULL */);

    mindames = work_calc_mindames_diff (w, stone);

    (mvs->moves)[n].move_stone = mindames;

    // возвращаем ход назад
    WBOARD(i, j) = STONE_NONE;
    work_get_groups (w/* , NULL */);

    work_init (w); // для чистоты эксперимента
  }

  return;
}
/*----------------------------------------------------------------------------*/
int   
play_by_weakes_diff (GO_WORK *w, int *i_move, int *j_move, GO_STONE stone) 
{   
  GO_MOVES cdames;

  int n, n_best=-1;
  int diff, max_diff;

  // находим все контактные дамэ
  work_get_contact_dames (w, &cdames);

  // пробуем туда ходы и вычисляем "разницу дыхалок" = my - he
  weakes_calc (w, stone, &cdames);

  // выбираем ход, где у меня лучшая `разница дыхалок
  // ------------------------------------------------
  max_diff = /* VALUE_MIN */-1000;

  for (n=0; n < cdames.moves_num; n++) {
    diff = (cdames.moves)[n].move_stone;

    if (diff > max_diff) { 
      max_diff = diff;
      n_best   = n;
    }
  }
  if (n_best == -1) // вообще не было контактных даме
    return (FALSE); 

  if (i_move) *i_move = (cdames.moves)[n_best].i;
  if (j_move) *j_move = (cdames.moves)[n_best].j;
  // ------------------------------------------------

  return (TRUE);
}
/*----------------------------------------------------------------------------*/
int   
find_try_captures (GO_WORK *w, GO_STONE stone, GO_MOVES *mvs) 
{ 
  // попробуем захватить слабую чужую группу!
  int  n, num_find;
  GO_GROUP *g;

  num_find = 0;

  for (n=0; n < w->groups_num; n++) {
    g = &(w->groups[n]);
    if (stone==g->stone) continue; // свои группы нас не интересуют

    if (g->xy_num==1) { // нашли чужую группу с одним дамэ
      moves_add (mvs, g->x[0], g->y[0], stone);
      num_find++;
    }
  }

  return (num_find);
}
/*----------------------------------------------------------------------------*/
int    
find_try_defends (GO_WORK *w, GO_STONE stone, GO_MOVES *mvs) 
{ 
  // попробуем защитить свою слабую группу!
  int i=0, j=0, n, num_find;
  GO_GROUP *g;

  num_find = 0;

  for (n=0; n < w->groups_num; n++) {
    g = &(w->groups[n]);
    if (stone != g->stone) continue; // чужих не защищаем 

    if (g->xy_num==1) { // нашли группу с одним дамэ
      i = g->x[0];
      j = g->y[0];
      if (!work_is_move_suicide (w, stone, i, j)) {
        // и ход туда не будет суицидным
        moves_add (mvs, i, j, stone);
        num_find++;
        break; // пока ограничемся одним найденным ходом!
      }
    }

  }
  if (num_find==0)
    return (FALSE);

  // все эти проверки надо делать потом, и вообще в алгоритме МиниМакс !
  // нет, абстрактные такие проверки не пойдут, нужно конкретно!
 
  if (!work_is_this_good (w, stone, i, j))
     return (FALSE);

  return (TRUE);
}
//*****************************************************************************/
//*****************************************************************************/

//*****************************************************************************/
//*****************************************************************************/
/*----------------------------------------------------------------------------*/
int 
add_moves_caps (GO_BOARD *t, GO_WORK *w, GO_STONE stone,  long *p_moves, int num)
{ 

  GO_MOVES *mvs = moves_create (); // слишком частой маллок !!!!!
  moves_init (mvs);

  int  my_num = find_try_captures (w, stone, mvs); 
  int  n, i, j;

  for (n=0; n < my_num; n++) {
    i = ((mvs->moves)[n]).i;
    j = ((mvs->moves)[n]).j;

    BOARD (t, i, j) = STONE_BLACK;
    (p_moves[num++]) = (long) xo_malloc_move (i, j); 
  }

  moves_free (mvs);
  return (num);
}
/*----------------------------------------------------------------------------*/
int 
add_moves_defs (GO_BOARD *t, GO_WORK *w, GO_STONE stone,  long *p_moves, int num)
{ 

  GO_MOVES *mvs = moves_create (); // слишком частой маллок !!!!!
  moves_init (mvs);

  int  my_num = find_try_defends (w, stone, mvs); 
  int  n, i, j;

  for (n=0; n < my_num; n++) {
    i = ((mvs->moves)[n]).i;
    j = ((mvs->moves)[n]).j;

    BOARD (t, i, j) = STONE_BLACK;
    (p_moves[num++]) = (long) xo_malloc_move (i, j); 
  }

  moves_free (mvs);
  return (num);
}
/*----------------------------------------------------------------------------*/
int   
add_moves_cont (GO_BOARD *t,  GO_WORK *w,  long *p_moves, int num) 
{   
  GO_MOVES cdames;
  int  n, i, j;

  // находим все контактные дамэ
  work_get_contact_dames (w, &cdames);

  for (n=0; n < cdames.moves_num; n++) {
    i = (cdames.moves)[n].i;
    j = (cdames.moves)[n].j;

    BOARD (t, i, j) = STONE_BLACK;
    (p_moves[num++]) = (long) xo_malloc_move (i, j); 
  }

  return (num);
}
/*----------------------------------------------------------------------------*/
int 
find_moves_random (GO_BOARD *t, int numfind, long *p_moves, int num)
{ 
  int  n, i, j;

  for (n=0; n < numfind; n++) {
    if (!board_random_stone (t, &i, &j) )
      break; // нет даже случайных, т.е. - никаких!

    BOARD (t, i, j) = STONE_BLACK;
    (p_moves[num++]) = (long) xo_malloc_move (i, j); 
  }

  return (num);
} 
/*----------------------------------------------------------------------------*/
int 
add_moves_full (GO_BOARD *t, long *p_moves, int num)
{ 
  int  i, j;

  for (i=0; i < t->isize; i++) 
  for (j=0; j < t->jsize; j++) {
    if (BOARD (t, i, j) != STONE_NONE) continue;

    (p_moves[num++]) = (long) xo_malloc_move (i, j);  

    // потом надо это все освобождать !!!!!
    // т.е. вызывать функцию  "aigo_freemoves"
  }

  return (num);
} 
/*----------------------------------------------------------------------------*/
int 
add_moves_good (GO_BOARD *t, GO_WORK *w, GO_STONE stone,  long *p_moves, int num,
                int good)
{ 
  int  i, j;

  for (i=0; i < t->isize; i++) 
  for (j=0; j < t->jsize; j++) {
    if (BOARD (t, i, j) != STONE_NONE) 
      continue;

    int mindames = work_move_mindames (w, stone, i, j);
    if (mindames <= good) 
      continue;

    BOARD (t, i, j) = STONE_BLACK;
    (p_moves[num++]) = (long) xo_malloc_move (i, j);  

  }

  return (num);
} 
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/******************************************************************************/


/******************************************************************************/
//                                                                             /
/******************************************************************************/
