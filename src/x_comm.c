/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: x_comm.c                                                  *
 *                                                                             *
  ******************************************************************************
 */  

#include "a_comm.h"

#include "x_comm.h"
#include "x_hash.h"
#include "x_work.h"

/******************************************************************************/


char horz_arr[19] = 
{'A','B','C','D','E','F','G','H','J','K','L','M','N','O','P','Q','R','S','T'};


/*----------------------------------------------------------------------------*/
void
aboard_clean (GO_BOARD *a)
{
  int  i, j;

  for (i=0; i < a->isize; i++) 
  for (j=0; j < a->jsize; j++) {
    BOARD (a, i, j) = STONE_NONE;        
  }

  return;
}
/*----------------------------------------------------------------------------*/
GO_BOARD *
aboard_begin (int ii, int jj)
{
  int  i, j;
  GO_BOARD *a = malloc (sizeof (GO_BOARD));
  if (!a) Error ("aboard_begin1");

  a->board = calloc (ii*jj, sizeof (int));
  if (!(a->board)) Error ("aboard_begin2");

  a->isize = ii;
  a->jsize = jj;

  for (i=0; i < ii; i++) 
  for (j=0; j < jj; j++) {
    BOARD (a, i, j) = STONE_NONE;        
  }

  return (a);
}
/*----------------------------------------------------------------------------*/
void
aboard_delete (GO_BOARD *a)
{

  free (a->board);
  free (a);

  return;
}
/*----------------------------------------------------------------------------*/
void
board_to_from (GO_BOARD *to, GO_BOARD *from)
{

  memcpy (to->board, from->board, from->isize * from->jsize * sizeof(int));

  return;
}
/*----------------------------------------------------------------------------*/
GO_BOARD *
board_copy (GO_BOARD *from)
{

  GO_BOARD *to = aboard_begin (from->isize, from->jsize);

  board_to_from (to, from);

  return (to);
}
/*----------------------------------------------------------------------------*/
GO_BOARD *
board_copy2 (GO_BOARD *to, GO_BOARD *from)
{
  if ((to == NULL) || (from->isize!=to->isize) || (from->jsize!=to->jsize)) {
    to = aboard_begin (from->isize, from->jsize);
    //OUTD (111);
  }

  //board_to_from (to, from);
  int i, j;

  for (i=0; i < from->isize; i++) 
  for (j=0; j < from->jsize; j++) {
    BOARD (to, i, j) = BOARD (from, i, j);        
  }

  return (to);
}
/*----------------------------------------------------------------------------*/
void
aboard_del_stone (GO_BOARD *a, int i, int j)
{

  BOARD (a, i, j) = STONE_NONE;

}
/*----------------------------------------------------------------------------*/
void
aboard_set_stone_ (GO_BOARD *a, int stone, int i_move, int j_move)
{

  if (BOARD(a, i_move, j_move) != STONE_NONE)
    printf ("board_set_move: %d %d stone_old=%d \n", 
             i_move, j_move, BOARD(a, i_move, j_move));

  BOARD (a, i_move, j_move) = stone;
  return;
}
/*----------------------------------------------------------------------------*/
int
aboard_is_move_legal (GO_BOARD *a, int i, int j)
{

  if (i<0 || j<0 || i>a->isize-1 || j>a->jsize-1) 
    return (FALSE);

  if (BOARD(a,i,j) == STONE_NONE)  return (TRUE);
  else                             return (FALSE);

}
/*----------------------------------------------------------------------------*/
void 
board_name_to_index (GO_BOARD *a, char *name, int *i_ptr, int *j_ptr) 
{
  int isize = a->isize;
  int jsize = a->jsize;
  char horz, vert[10];
  int  i, j;

  horz = name[0];
  strcpy (vert, name+1);

  for (j=0; j<jsize; j++)
    if (toupper(horz) == horz_arr[j]) break;
      
  i = isize - atoi (vert); 

  *i_ptr = i;
  *j_ptr = j;

  return;
}
/*----------------------------------------------------------------------------*/
char* 
board_name_from_index (GO_BOARD *a, /* int nn, */ int i, int j) 
{
  //int isize = a->isize;
  //int jsize = a->jsize;
  int nn = a->isize;

  static char name[10];
  char  *name1 = NULL;

  strcpy (name, "");

  if (j ==  0) name1 = "a";
  if (j ==  1) name1 = "b";
  if (j ==  2) name1 = "c";
  if (j ==  3) name1 = "d";
  if (j ==  4) name1 = "e";
  if (j ==  5) name1 = "f";
  if (j ==  6) name1 = "g";
  if (j ==  7) name1 = "h";
  if (j ==  8) name1 = "j";

  if (j ==  9) name1 = "k";
  if (j == 10) name1 = "l";
  if (j == 11) name1 = "m";
  if (j == 12) name1 = "n";
  if (j == 13) name1 = "o";
  if (j == 14) name1 = "p";
  if (j == 15) name1 = "q";
  if (j == 16) name1 = "r";
  if (j == 17) name1 = "s";
  if (j == 18) name1 = "t";

  sprintf (name, "%s%d", name1, nn-i);

  return (name);
}
/*----------------------------------------------------------------------------*/
int  
int_from_char (char c) 
{
  char str[2];
  int  ret;

  str[0] = c;
  str[1] = '\0';
  
  ret = atoi (str);
  printf ("%c -- %d \n", c, ret);
  return (ret);
}
/*----------------------------------------------------------------------------*/
void
board_borders (YT_WIN  *win, int x0, int  y0, int ww, int hh, YT_BOOL is_plus) 
{
  int  x, y, j, n;

  // горизонтальные бортики
  for (j=x0+2, n=0; j < ww-2; j++, n++) {
    if (!is_plus && n%2) continue;
    x = j;
    win_char (win, '-', x, 0);
    win_char (win, '-', x, hh-1);
  }

  // вертикальные бортики
  for (y = y0+1; y < hh-1; y++) {
    win_char (win, '|', 0,    y);
    win_char (win, '|', ww-1, y);
  }

  if (is_plus) {
    // уголки
    win_char (win, '+', 0, 0);
    win_char (win, '+', 0, hh-1);
    win_char (win, '+', ww-1, 0);
    win_char (win, '+', ww-1, hh-1);
  }

  return;
}
/*----------------------------------------------------------------------------*/
void
board_mainreg (YT_WIN  *win, GO_BOARD *a, int x0, int  y0) 
{
  int  i, j;
  CHAR ch;
  int  isize = a->isize;
  int  jsize = a->jsize;

  // основная область доски с камнями
  for (i=0; i < isize; i++) 
  for (j=0; j < jsize; j++) {
    if      (BOARD (a,i,j) == STONE_WHITE)       ch = 'O';
    else if (BOARD (a,i,j) == STONE_BLACK)       ch = 'X';      
    else if (BOARD (a,i,j) == STONE_WHITE_MARK)  ch = 'o';
    else if (BOARD (a,i,j) == STONE_BLACK_MARK)  ch = 'x';      
    else if (BOARD (a,i,j) == STONE_WHITE_DEAD)  ch = '@';
    else if (BOARD (a,i,j) == STONE_BLACK_DEAD)  ch = '#';      

    else if (BOARD (a,i,j) == STONE_NONE)        ch = '.';      
    else 
      ch = BOARD (a,i,j);
    //ch = '?';  
    //else ch = '.';

    win_char (win, ch, x0 + 2*j, y0 + i);
  }

  return;
}
/*----------------------------------------------------------------------------*/
void
board_abcd (YT_WIN  *win, GO_BOARD *a) 
{
  int  x0 = 2;
  int  y0 = win->h-1;
  int  j;
  int  jsize = a->jsize;
  CHAR ch;

  for (j=0; j < jsize; j++) {
    ch = tolower (horz_arr[j]);
    win_char (win, ch, x0 + 2*j, y0);
  }

  return;
}
/*----------------------------------------------------------------------------*/
void
board_1234 (YT_WIN  *win, GO_BOARD *a) 
{
  int  x0 = win->w-1;
  int  y0 = 1;
  int  i;
  int  isize = a->isize;
  CHAR ch;

  //printf ("x0= %d \n", x0);

  for (i=0; i < isize; i++) {
    ch = toascii (48+ isize-i);
    win_char (win, ch, x0, y0+i);
  }

  return;
}
/*----------------------------------------------------------------------------*/
void
aboard_show (GO_BOARD *a, YT_BOOL is_debug, YT_BOOL is_plus) 
{
  int  i;
  int  hh =   a->isize + 2;
  int  ww = 2*a->jsize - 1 + 4;
  int  hb = hh;  // высота рамки бортика 
  int  wb = ww;  // ширина рамки бортика 

  if (is_debug && !DBG_IS_PRINT()) return;

  // для больших досок принудительно установить показ координат
  //if (a->isize>=4 && a->jsize>=4) is_plus = /* 1 */ TRUE;

  if (is_plus) {
    hh++; 
    ww+=2;
  }

  //printf ("ww= %d \n", ww);
  YT_WIN  *win = win_create (ww, hh);

  board_borders (win, 0, 0, wb, hb, is_plus); 
  board_mainreg (win, a, 2, 1); 
  if (is_plus) {
    board_abcd (win, a); 
    board_1234 (win, a); 
  }

  // -------------- win_draw (win, 0, 0);
  for (i=0; i < win->h; i++) {
    if (is_debug) DBG_PRINT ("%s \n", (char*)(&(win->p[i][0])));
    else          printf ("%s \n", (char*)(&(win->p[i][0])));
  }

  if (is_plus) {
    if (is_debug) DBG_PRINT ("\n");
    else          printf ("\n");
  }
  // -----------------------------------

  win_delete (win);

  return;
}
/*----------------------------------------------------------------------------*/
int   
board_random_rect_stone (GO_BOARD *b, int  imin, int  jmin, int  imax, int  jmax, 
                         int *i_move, int *j_move) 
{ 
  int  i, j, n0, n;

  n = 0;
  for (i=imin; i <= imax; i++) 
  for (j=jmin; j <= jmax; j++) {
    if (BOARD(b,i,j) == STONE_NONE) 
      n++;
  }

  // посчитали количество пустых полей
  if (n==0) return (FALSE);

  n0 = YRAND (1, n); // выбрали случайное

  n = 0;
  for (i=imin; i <= imax; i++) 
  for (j=jmin; j <= jmax; j++) {
    if (BOARD(b,i,j) == STONE_NONE) 
      n++;

    if (n0 == n) { // нашли это случайное      
      *i_move = i;
      *j_move = j;
      return (TRUE);
    }
  }

  return (FALSE);
}
/*----------------------------------------------------------------------------*/
int   
board_random_stone (GO_BOARD *b, int *i_move, int *j_move) 
{ 

  int  imin = 0;
  int  jmin = 0;
  int  imax = b->isize-1;
  int  jmax = b->isize-1;

  return (board_random_rect_stone (b, imin, jmin, imax, jmax, 
                                   i_move, j_move)); 
}
/*----------------------------------------------------------------------------*/
/* int    */
/* board_get_random_moves (GO_BOARD *b, int *i_move, int *j_move)  */
/* {  */
/*   int num; */



/*   return (num); */
/* } */
/*----------------------------------------------------------------------------*/
GO_MOVE *
xo_malloc_move (int i, int j)
{
  GO_MOVE *mv;

  mv = malloc (sizeof (*mv));
  //YMALLOC_NEW ("11", m->maz3_points[l], MAZ3_POINT, ii*jj*kk);
  if (!mv) Error ("xo_malloc_move");

  mv->i = i;
  mv->j = j;

  return (mv);
}
//------------------------------------------------------------------------------
void 
xo_show_board (GO_POSI *pos)
{

  if (pos == NULL) 
    Error ("board == NULL");

  aboard_show (pos->a, TRUE, FALSE /*is_abcd*/);

  return;
}
/*----------------------------------------------------------------------------*/
void 
posi_show (GO_POSI *pos)
{ 
  int  stone = pos->stone;

  printf ("%s \n", STONE_NAME (stone));
  aboard_show (pos->a, FALSE, FALSE /* is_abcd */); 

  printf ("\n");

  return;
}
/*----------------------------------------------------------------------------*/
GO_POSI *
xo_pos_create (int stone, int ii, int jj)
{
  GO_POSI *pos; 

  pos = calloc (1, sizeof (*pos));
  if (!pos) {
    Error ("xo_pos_create");
  }

  pos->a = aboard_begin (ii, jj);
  pos->stone = stone;

  return (pos);
}
/*----------------------------------------------------------------------------*/
void
xo_pos_delete (GO_POSI *pos)
{

  aboard_delete (pos->a);

  free (pos);

  return;
}
/*----------------------------------------------------------------------------*/
GO_POSI *
xo_pos_copy (GO_POSI *dst, GO_POSI *src)
{
  GO_BOARD *sa = src->a;

  if (!dst) 
    dst = xo_pos_create (0, sa->isize, sa->jsize); // здесь не важен игрок, все равно закопируется

  // а если уже создано, то надо честно копировать все поля!!!
  // и аккуратно сложную вложенную структуру GO_BOARD

  //dst->next   = src->next;

  memcpy ((dst->a)->board, (src->a)->board, (sa->isize)*(sa->jsize)*sizeof (int));
  (dst->a)->isize = sa->isize;
  (dst->a)->jsize = sa->jsize;

  dst->stone = src->stone;

  return (dst);
}
/*----------------------------------------------------------------------------*/
int
stone_from_char (char ch)
{
  int stone= STONE_NONE;

  switch (ch) {
  case 'X':
    stone = STONE_BLACK;
    break;
  case 'O':
    stone = STONE_WHITE;
    break;
  case '@':
    stone = STONE_WHITE_DEAD;
    break;
  case '#':
    stone = STONE_BLACK_DEAD;
    break;
  case '.':
    stone = STONE_NONE;
    break;
  default: 
    stone = ch;
    //MyError ("stone");
    break;
  }

  return (stone);
}
/*--------------------------------------------------------------------------*/
int
board_symm_calc (GO_BOARD *bs[], GO_BOARD *b)
{ 
  int  i, j, num;
  int  ii = b->isize;
  int  jj = b->jsize;

  if ((ii==1) || (jj==1))  num = 2;
  else if (ii == jj)       num = 8;
  else                     num = 4;

  //for (i=0; i</* NUM_SYM */num; i++) 
  //  bs[i] = aboard_begin (ii, jj);  /// ????!!!!!

  board_to_from (bs[0], b);

  for (i = 0; i < ii; i++) 
  for (j = 0; j < jj; j++) {
    if (jj > 1) BOARD (bs[1], i, j) = BOARD (bs[0], i,         SYM(jj,j));
    if (ii > 1) BOARD (bs[2], i, j) = BOARD (bs[0], SYM(ii,i), j);

    if (num == 2) continue;

    BOARD (bs[3], i, j) = BOARD (bs[0], SYM(ii,i), SYM(jj,j));

    // при не квадратной матрице не все симметрии возможны!
    // поскольку нельзя менять местами индексы 
    if (num == 4) continue;

    BOARD (bs[4], i, j) = BOARD (bs[0], j, i);

    BOARD (bs[5], i, j) = BOARD (bs[0], j,         SYM(ii,i));
    BOARD (bs[6], i, j) = BOARD (bs[0], SYM(jj,j), i);
    BOARD (bs[7], i, j) = BOARD (bs[0], SYM(jj,j), SYM(ii,i));    
  }

  return (num);
}
/*----------------------------------------------------------------------------*/
GO_POSI*
posi_from_position (YT_POSITION *pp) 
{
  GO_POSI  *posi = xo_pos_create (pp->stone, pp->ii, pp->jj);
  GO_BOARD *a = posi->a;

  int   i, j;
  char *line;

  for (i=0; i < pp->ii; i++) {
    line = pp->pos[i];

    for (j=0; j < pp->jj; j++) {
      if      (line[j]=='O')  BOARD (a, i, j) = STONE_WHITE; 
      else if (line[j]=='X')  BOARD (a, i, j) = STONE_BLACK;
      else                    BOARD (a, i, j) = STONE_NONE; 
   }
  }

  return (posi);
}
/*----------------------------------------------------------------------------*/
void
read_board (FILE *fp, GO_BOARD *b, int ii, int jj) 
{
  int   i, j;
  char  ch;
  char  buffer[80];
 
  // здесь бы еще читать размерность доски для этой позиции ?!
  fgets (buffer, 70, fp);

  i = 0;
  while (fgets (buffer, 70, fp) != NULL) {

    for (j = 1; j <= jj; j++) {
      ch = buffer[2*j];
      BOARD (b, i, j-1) = /* ch */ stone_from_char (ch);        
    }

    i++;
    if (i >= ii) break;
  }

  fgets (buffer, 70, fp);
  return;
}
/*----------------------------------------------------------------------------*/
int
read_file_posi (char *fn1, GO_POSI *posis[], int max_num, int ii, int jj) 
{
  int  n = 0;

  FILE  *fp1;
  char   buffer[80];
  GO_POSI *p_true=NULL;
 

  if (!(fp1 = fopen (fn1, "r"))) {
    printf ("File not found: %s \n", fn1);
    return (0);
  }

  //----------------------------------------------------------
  while (fgets (buffer, 70, fp1) != NULL) {

    //-----------------------------------------------------------
    // читаем запись оценки-позиции из файла (p_true)

    if        (strncmp(buffer, "WHITE", 5)==0) {
      p_true = xo_pos_create (STONE_WHITE, ii, jj);
    } else if (strncmp(buffer, "BLACK", 5)==0) {
      p_true = xo_pos_create (STONE_BLACK, ii, jj);
    } else
      continue; // пропускаются пробелы и разные рабочие записи

    // читает, к сожалению, без ограничений по размеру!!!
    read_board (fp1, p_true->a, ii, jj); 
    //-----------------------------------------------------------

    posis[n] = p_true; 
    n++;

    if (n == /* MAX_NUM_POSI-10 */max_num) {
      printf ("MAX_NUM \n");
      break;
    }
  }

  return (n);
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
//                                                                             /
/******************************************************************************/
