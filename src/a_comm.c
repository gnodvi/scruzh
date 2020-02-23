/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: a_comm.c                                                  *
 *                                                                             *
  ******************************************************************************
 */ 
// abvgdeewziklmnoprstufxch[]'=`\yq  
// это тест на русские БУКВЫ  русскиe \\\/////                                                       

#include "a_comm.h"

/******************************************************************************/

BOOL    ERR_PRINT = FALSE /* TRUE */;
YT_BOOL debuging = FALSE;
BOOL    debug = FALSE;

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
int 
YInt (double f) 
{ 
  int     i; 
  double   o; 
 
  i = (int) f; 
  o = f - i; 
  if      (o > 0.5)   i++; 
  else if (o < -0.5)  i--; 
 
  return (i); // ближайшее целое
} 
/*----------------------------------------------------------------------------*/
void
Error (const char *msg)      
{

  fprintf (stderr, "Error: %s\n", msg);
  exit (EXIT_FAILURE);

}
/*----------------------------------------------------------------------------*/
double
YRandF (double fmin, double fmax)
{
  double   choise;

  choise = drand48 ();
  //void srand48(long int seedval);

  return (fmin + choise * (fmax - fmin));
}
/*----------------------------------------------------------------------------*/
int
Rand123 (double p1, double p2, double p3)
{
  int yrand;

  yrand = YRAND (1,100);

  if      (yrand < p1) return (1);
  else if (yrand > p3) return (3);
  else                 return (2);

  p2++;
}
/*----------------------------------------------------------------------------*/
BOOL
RandYes (double procent)
{

  if (Rand123 (procent, 0.0, 100.0-procent)==1)
    return (TRUE);
  else
    return (FALSE);

}
/*----------------------------------------------------------------------------*/
BOOL
FRavno (double d1, double d2, double eps)
{

  if (fabs(d1-d2) < eps) return TRUE;
  else                   return FALSE;

}
/*----------------------------------------------------------------------------*/
BOOL
str_cmp (const char *str1, const char *str2)
{
  if (!str1 || !str2) return (FALSE);


  return (!(strcmp(str1, str2)));
}
/*----------------------------------------------------------------------------*/
BOOL                                                                         
YStrCmp (char *str1, char *str2)                                                
{                                                                               
  if (!str1 || !str2) return (FALSE);                                           
                                                                                
  return (!(strcmp(str1, str2)));                                               
}                                                                               
/*----------------------------------------------------------------------------*/
int
Numer (int dim, int *ijk, int *IJK)
{
  int numer, i;

  numer = ijk[dim-1];

  for (i=0; i < dim-1; i++) {
    numer = numer + (ijk[i] * IJK[i]);
  }

  return (numer);
}
/*----------------------------------------------------------------------------*/
void 
srandom_set (int seed)
{

  if (seed < 0) srandom (time(0)); // переменная псевдослучайность
  else          srandom (seed);    // фиксированная 

  return;
}
/*----------------------------------------------------------------------------*/
// ищем среди параметров : "name", подразумеваем, что там стоит "name=val" и 
// возвращаем указатель на "val"
/*----------------------------------------------------------------------------*/
char*
parse_read (char *name, int argc, char *argv[], int  j)
{
  //int   j = 0;
  char *ptr;
  int   len, i;

  for (i=/* 0 */j; i < argc; i++) {
    ptr = argv[i];
    len = strlen(name);

    if (strncmp (ptr, name, len) == 0)
      return (ptr+len+1);    
  }

  return (NULL);
}
/*----------------------------------------------------------------------------*/
int
parse_find (char *name, int argc, char *argv[])
{
  //char *ptr;
  int   /* len, */ i;

  for (i=0; i < argc; i++) {
    //ptr = argv[i];
    //len = strlen(name);

    if (strcmp (/* ptr */argv[i], name) == 0)
      return (/* ptr+len+1 */i);    
  }

  return (-1);
}
/*----------------------------------------------------------------------------*/
void
YDataRW (int num, int act, void *ptr, int size, FILE *stream)
{

  if (act == YSEEK) { 
    if (fseek (stream, size, SEEK_CUR) != 0) {printf("%d ", num); Error ("YDataRW-fseek"); }
  }
  else if      (act == YREAD) { 
    if (fread (ptr, size, 1, stream) != 1)   {printf("%d ", num); Error ("YDataRW-read");}
  }
  else if (act == YWRITE) { 
    if (fwrite(ptr, size, 1, stream) != 1)   {printf("%d ", num); Error ("YDataRW-write");}
  }

  return;
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
int 
get_line (char *s, int size)
{
  char fmt[50];
  int  ret;

  // ?????
  sprintf (fmt, "%%%lu[^\n]%%*[^\n]", (long)size); // emgena: (long)
  *s = '\0';
  ret = scanf (fmt, s);

  getchar ();

  return (ret);
}
/******************************************************************************/
//   ВРЕМЯ: отсечка, формирование строк, печать
/******************************************************************************/

/*----------------------------------------------------------------------------*/
char*
time_calc_printa (int mode)
{
  static time_t    tim_beg, tim_end, tim_all, tim_min, tim_sec, tim_hor;
  static char   buffer[80];

  //-------------------------------
  if (mode==0) {
    tim_beg = time (NULL);

    printf ("Started Time = \n");
    return (buffer);
  }
  //-------------------------------

  tim_end = time (NULL);

  tim_all = tim_end - tim_beg;
  tim_hor = tim_all / 3600;
  tim_all = tim_all % 3600;
  tim_min = tim_all / 60;
  tim_sec = tim_all % 60;

  sprintf (buffer, "%ld:%2ld:%2ld", tim_hor, tim_min, tim_sec);

  //-------------------------------
  if (mode == -1) { // получить промежуточный результат в виде простой строки
    return (buffer);
  }
  //-------------------------------

  printf ("Elapsed Time = {%s} \n", buffer);
  printf ("\n");
  return (buffer);
}
/*----------------------------------------------------------------------------*/
long  
YTimeGet ()  
{			  
  struct timeval  tp;  
  struct timezone tz; 
 
  gettimeofday (&tp, &tz);  
  return (tp.tv_sec * 100 + tp.tv_usec / 10000);  

  return (100);  
}   
/*----------------------------------------------------------------------------*/
double
TimeSec () 
{	
  struct timeval tp; 
  struct timezone tz; 

  gettimeofday (&tp, &tz); 
  //printf ("tv_sec= %ld  tv_usec= %ld  \n", tp.tv_sec, tp.tv_usec);
  //return (tp.tv_sec * 100 + tp.tv_usec / 10000); 

  return (tp.tv_sec + tp.tv_usec / 1000000.0); 
} 
/*----------------------------------------------------------------------------*/
double
YSecOfDay ()
{
  struct timeb tb;

  ftime (&tb);
  return (tb.time + tb.millitm / 1000.0);

}
/*----------------------------------------------------------------------------*/
YT_TIMER*
timer_beg ()
{
  YT_TIMER *t = (YT_TIMER*) malloc (sizeof(YT_TIMER));

  t->tim_beg = time (NULL);

  return (t);
}
/*----------------------------------------------------------------------------*/
void
timer_hms (YT_TIMER *t, time_t *tim_hor, time_t *tim_min, time_t *tim_sec)
{
  time_t    tim_end, tim_all;

  tim_end = time (NULL);
  tim_all = tim_end - t->tim_beg;
  *tim_hor = tim_all / 3600;
  tim_all = tim_all % 3600;
  *tim_min = tim_all / 60;
  *tim_sec = tim_all % 60;

  return;
}
/*----------------------------------------------------------------------------*/
void
timer_end (YT_TIMER *t, char *hms_time)
{
  time_t /* tim_end, tim_all,  */tim_min, tim_sec, tim_hor;

  timer_hms (t, &tim_hor, &tim_min, &tim_sec);

  //sprintf (hms_time, "%d:%02d:%02d", (int)tim_hor, (int)tim_min, (int)tim_sec);
  sprintf (hms_time, "{%d:%02d:%02d}", (int)tim_hor, (int)tim_min, (int)tim_sec);

  //free (t); //???
  // ломается после записи 2-ных массивов (невыявленная ошибка);
  return;
}
/*----------------------------------------------------------------------------*/
BOOL
timer_hor (YT_TIMER *t)
{
  time_t  tim_min, tim_sec, tim_hor;
  static int save_count = 0;

  timer_hms (t, &tim_hor, &tim_min, &tim_sec);
  if (tim_hor > save_count) {		
    save_count++;
    return (TRUE);
  }

  return (FALSE);
}
/******************************************************************************/
//
//   Формирование символьной экранной информации
//   с дальнейшим выводом на терминал
//
/******************************************************************************/

/*-----------------------------------win_init----------------------------------*/
void
win_init (YT_WIN *win, CHAR ch)
{
  int i, j;

  for (i=0; i<win->h; i++)
    for (j=0; j<win->w; j++)
      win->p[i][j] = ch;

  for (i=0; i<win->h; i++)
    win->p[i][win->w] = '\0';

  return;
}
/*----------------------------------win_create---------------------------------*/
YT_WIN*
win_create (int w, int h)
{
  YT_WIN *win;

  win = (YT_WIN*) malloc (sizeof(YT_WIN));
  win->w = w;
  win->h = h;

  win_init (win, ' ');

  return (win);
}
/*-----------------------------------win_char----------------------------------*/
//YT_BOOL
void
win_char (YT_WIN *win, CHAR ch, int x, int y)
{

  if (x<0)       {printf ("win_char: x<0 \n"); return;}
  //if (x>=win->w) {printf ("win_char: x>=win->w \n"); return;}
  if (x>=win->w) {
    printf ("x=%d  y=%d  win->w=%d  win->h=%d  \n", x, y, win->w, win->h);
    Error ("win_char: x>=win->w");
    //return (FALSE);
  }

  if (y<0)       {printf ("win_char: y<0 \n"); return;}
  if (y>=win->h) {printf ("win_char: y>=win->h \n"); return;}

  win->p[y][x] = ch;

  return /* (TRUE) */;
}
/*----------------------------------win_text-----------------------------------*/
void
win_text (YT_WIN *win, const char *text, int x, int y)
{
  int  i;
  char ch;

  for (i=0; TRUE ;i++) {
    ch = text[i];
    if (ch == '\0')	break;
    win_char (win, ch, x+i, y);
  }

  return;
}
/*-----------------------------------------------------------------------------*/
void
win_draw (YT_WIN *win, int x, int y)
{
  int i, j;
  char buff[100] = "";
  char left[100] = "";

  for (i=0; i<y; i++)
    puts ("");

  for (j=0; j<x; j++)
    left[j] = ' ';
  left[j] = '\0';

  for (i=0; i<win->h; i++) {
    strcpy (buff, left);
    strcat (buff, (char*)(&(win->p[i][0])));
    puts (buff);
  }


  return;
}
/*-----------------------------------------------------------------------------*/
void
win_horz (YT_WIN *win, CHAR  ch, int y_horz, int x1, int x2)
{
  int x;

  for (x=x1; x<=x2; x++)
    win_char (win, ch, x, y_horz);

  return;
}
/*-----------------------------------------------------------------------------*/
void
win_vert (YT_WIN *win, CHAR ch, int x_vert, int my_y1, int y2)
{
  int y;

  for (y=my_y1; y<=y2; y++)
    win_char (win, ch, x_vert, y);

  return;
}
/*-----------------------------------------------------------------------------*/
void
win_rect (YT_WIN *win, CHAR ch_horz, CHAR  ch_vert, int x, int y, int w, int h)
{

  w--;
  h--;

  win_horz (win, ch_horz, y,   x, x+w);
  win_horz (win, ch_horz, y+h, x, x+w);

  win_vert (win, ch_vert, x,   y, y+h);
  win_vert (win, ch_vert, x+w, y, y+h);

  return;
}
/*-----------------------------------------------------------------------------*/
void
win_sgr (int par)
{

  printf ("\033[ %d m", par);

}
/*-----------------------------------------------------------------------------*/
void
win_sgr_stderr (int par)
{

  fprintf (stderr, "\033[ %d m", par);

}
/*----------------------------------win_delete---------------------------------*/
void
win_delete (YT_WIN *win)
{

  free (win);

  return ;
}
/*-----------------------------------------------------------------------------*/
BOOL
VHandPixel (YT_WIN *win, int x, int y, CHAR ch)
{

  win_char (win, ch, x, y);

  return (TRUE);
}
/*-----------------------------------------------------------------------------*/
BOOL
VHandLineFunc (VT_HANDLINE *p, BOOL flag, int *px1, int *pmy_y1,
                 int *px2, int *py2)
{
  int     x1, my_y1, x2, y2, delta;

  if (flag == FALSE) {
    x1 = *px1;
    my_y1 = *pmy_y1;
    x2 = *px2;
    y2 = *py2;

    p->delta_x = x2 - x1;
    p->delta_y = y2 - my_y1;
    p->x = x1;
    p->y = my_y1;

    if      (p->delta_x > 0)  p->incx = 1;
    else if (p->delta_x == 0) p->incx = 0;
    else                      p->incx = -1;

    if      (p->delta_y > 0)  p->incy = 1;
    else if (p->delta_y == 0) p->incy = 0;
    else                      p->incy = -1;

    p->delta_x = abs (p->delta_x);
    p->delta_y = abs (p->delta_y);
    if (p->delta_y > p->delta_x) {
      delta = p->delta_x;
      p->delta_x = p->delta_y;
      p->delta_y = delta;
      p->exchange = TRUE;
    } else
      p->exchange = FALSE;

    p->err = 2 * p->delta_y - p->delta_x;
    p->i = 1;
    return (TRUE);
  }

  if (p->i > p->delta_x)
    return (FALSE);
  *px1 = p->x;
  *pmy_y1 = p->y;
  while (p->err >= 0) {
    if (p->exchange)  p->x += p->incx;
    else              p->y += p->incy;
    p->err = p->err - 2 * p->delta_x;
  }

  if (p->exchange)  p->y += p->incy;
  else              p->x += p->incx;
  p->err += 2 * p->delta_y;
  p->i++;

  return (TRUE);
}
/*-----------------------------------------------------------------------------*/
void
VHandLine (YT_WIN *win, int x1, int my_y1, int x2, int y2, CHAR  ch)
{
  VT_HANDLINE line;
  int     x, y, dx, dy;
  BOOL by_y;

  dx = x2 - x1;
  dy = y2 - my_y1;
  if (!dx && !dy)
    return;

  if (abs (dy) > abs (dx))  by_y = TRUE;
  else                      by_y = FALSE;

  VHandLineFunc (&line, FALSE, &x1, &my_y1, &x2, &y2);

  while (VHandLineFunc (&line, TRUE, &x, &y, NULL, NULL)) {
    VHandPixel (win, x, y, ch/* , z */);
  }

}
/*-----------------------------------------------------------------------------*/
void
win_line (YT_WIN *win, CHAR ch, int x1, int my_y1, int x2, int y2)
{

  VHandLine (win, x1, my_y1, x2, y2, ch);

  return;
}
/*-----------------------------------------------------------------------------*/
void
win_triangle (YT_WIN *win, CHAR ch, int x1, int y1, int x2, int y2, int x3, int y3)
{

  win_line (win, ch,  x1,y1,  x2,y2);
  win_line (win, ch,  x2,y2,  x3,y3);
  win_line (win, ch,  x1,y1,  x3,y3);

  return;
}
/*-----------------------------------------------------------------------------*/
// Подпрограмма для черчения эллипса : 
//------------------------------------------------------------
void 
bj_drawellps (YT_WIN  *win, char ch,             
             //int obj,                 //  объект; 
             int x, int y,              //  точка черчения; 
             int xcentre, int ycentre,  //  центр эллипса; 
             int asp_ratio              //  коэффициент "сжатия" пиксела; 
             ) 
{ 
  //char ch = '@';
  int startx, starty, endx, endy, x1, y1; 
 
  starty = y*asp_ratio; 
  endy   = (y+1)*asp_ratio; 
  startx = x*asp_ratio; 
  endx   = (x+1)*asp_ratio; 
 
  for (x1 = startx; x1<endx; ++x1) { 

    win_char (win, ch, x1+xcentre, y+ycentre); 
    win_char (win, ch, x1+xcentre, ycentre-y); 
    win_char (win, ch, xcentre-x1, y+ycentre); 
    win_char (win, ch, xcentre-x1, ycentre-y); 
  } 
 
  for (y1 = starty; y1<endy; ++y1) { 
    win_char (win, ch, y1+xcentre, x+ycentre); 
    win_char (win, ch, y1+xcentre, ycentre-x); 
    win_char (win, ch, xcentre-y1, x+ycentre); 
    win_char (win, ch, xcentre-y1, ycentre-x); 
  } 

  return; 
}
//-----------------------------------------------------------------
int 
win_ellipse (YT_WIN  *win, char ch,               
                 //int obj,                //   объект; 
                 int left,  int top,       //   описание прямоугольника; 
                 int right, int bottom    // 
                 ) 
{ 
  int xcentre, ycentre, xradius, yradius; 
  //int xasp, yasp; 
  float asp_ratio; 
  /* register */int x, y, delta/* , size */; 
 
  xradius = (right-left)/2; 
  yradius = (bottom-top)/2; 
  xcentre = left+xradius; 
  ycentre = top+yradius; 
 
  //getaspectratio (&xasp, &yasp); 
  //asp_ratio=(float)(yasp) / (float)(xasp); 
  asp_ratio = 1.0;

  if (yradius!=0) 
    asp_ratio = asp_ratio * ((float)(xradius)/(float)(yradius)); 
  /* алгоритм Брезенхама */ 
  // size=max(xradius,yradius); 
  y = xradius; 
  delta = 3-2*xradius; 

  for (x=0; x<y; x++) { 
    bj_drawellps (/* obj, */win, ch,  x, y, xcentre, ycentre, asp_ratio); 
    if (delta<0) 
      delta += 4*x+6; 
    else { 
      delta += 4*(x-y)+10; 
      y--; 
    } 
  } 

  x = y; 
  if (y) bj_drawellps (/* obj, */win, ch, x, y, xcentre, ycentre, asp_ratio); 
 
  return TRUE; 
} 
/*-----------------------------------------------------------------------------*/
void
win_test0 ()
{ 

  win_sgr (SGR_BOLD);
  win_sgr (SGR_RED);
  printf ("SGR_BOLD - SGR_RED \n");
  win_sgr (SGR_DEFAULT);

  printf ("\n");
  return;
}
/*-----------------------------------------------------------------------------*/
void
win_test1 ()
{
  int iw=60,  ih=30;
  YT_WIN  *win;

  // создаем экранную форму и рисуем главную рамочку
  win = win_create (iw, ih);  
  win_rect (win, '=', '=', 0, 0, 60, 30);

  win_line (win, '*', 2, 2,  15, 15);
  win_line (win, '*', 2, 2,  15, 5);
  win_ellipse  (win, '@',  15,15, 25,25);             
  win_triangle (win, '$',  40, 5,   35, 20,  50, 15);
  
  win_draw (win, 0, 0); // выводим экранную форму
  return;
}
/*-----------------------------------------------------------------------------*/ 
void
win_test_old ()
{ 

  printf (" \n");
  //------------------------
  win_test0 ();
  win_test1 ();
  //ncurses_test ();
  //plot_test ();
  //return;   
  //------------------------

  //main_graf ();

  printf ("\n");
  return;
}
/******************************************************************************/
//
//   Печать отладочных отступов при выводе информации
//   Используется в файлах: a_find.c & x_comm.c  
//
/******************************************************************************/

/*----------------------------------------------------------------------------*/

char left_buf[80] = "\0                                          ";
int  left_num     = 0;

int   max_level = 0;
int   num_level = 0;
char  str_level[80][80];

/*----------------------------------------------------------------------------*/
void 
dbg_set_maxlevel (int mlevel)
{

  max_level = mlevel;

}
/*----------------------------------------------------------------------------*/
void 
dbg_proc_beg (char *str, int expression)
{
  assert (expression);

  num_level++;
  strcpy  (str_level[num_level], str);

  //win_sgr (sgr_level[num_level]); 
  // установка цвета для уровне, не работает в файле?!
  // и оказывается ОЧЕНЬ медленно !!

  DBG_PRINT ("BEG: %s \n", str);

  LEFT_BEG; 
  return;
}
/*----------------------------------------------------------------------------*/
void 
dbg_proc_end ()
{
  LEFT_END; 

  DBG_PRINT ("END: %s \n", str_level[num_level]);

  num_level--;

  //win_sgr (sgr_level[num_level]);

  return;
}
/*----------------------------------------------------------------------------*/
YT_BOOL 
DBG_IS_PRINT ()
{

  if (num_level > max_level) 
    return (FALSE);

  return (TRUE);
}
/*----------------------------------------------------------------------------*/
void 
DBG_PRINT (char *fmt, ...)
{
  va_list argp;

  //if (!DEBUGING) 
  //  return;
  //if (!DBG_IS_PRINT())
  if (num_level > max_level) 
    return;

  //OUTD (left_num);
  printf ("%s", LEFT);

  // стандартный прием вывода переменных <stdarg.h>
  va_start (argp, fmt);
  //vfprintf (STDERR, fmt, argp);
  vprintf (fmt, argp);
  va_end   (argp);

  return;
}
/******************************************************************************/
//
//   Простые  БОТЫ     БОТЫ     БОТЫ     БОТЫ     БОТЫ     БОТЫ     
//            БОТЫ     БОТЫ     БОТЫ     БОТЫ     БОТЫ     БОТЫ     
//
/******************************************************************************/

YT_BOOL BOTPRINT = FALSE;

/*----------------------------------------------------------------------------*/
YT_BOT *
bot_create () 
{
  YT_BOT *bot;

  bot = (YT_BOT*) malloc (sizeof (YT_BOT));

  return (bot);
}
/*----------------------------------------------------------------------------*/
void 
bot_init (YT_BOT *bot, /* char *name, */ YT_PROG prog, 
          long long1, long long2, long long3, /* long long4, */ double d1)
{

  //Bprog *bp = (Bprog*)bot;
  //b_prog_init (bp, name);
  //---------------------------------

  bot->prog  = prog;         // основная программа-бот
  bot->long1 = long1;
  bot->long2 = long2;
  bot->long3 = long3;
  //bot->long4 = long4;
  bot->d1 = d1;

  return;
}
/*----------------------------------------------------------------------------*/
YT_BOT *
bot_make (YT_PROG prog,  long long1, long long2, long long3, 
          /* long long4, */ double d1) 
{
  YT_BOT *bot = bot_create ();

  bot_init (bot, prog, long1, long2, long3, /* long4 */ d1);

  return (bot);
}
/******************************************************************************/
//
/******************************************************************************/
/*----------------------------------------------------------------------------*/
MF *
MFOR_create (int dim, int *ijk, int *IJK)
{

 MF *mf = malloc (sizeof (MF));

 int   i;

 for (i=0; i<dim; i++)
   ijk[i] = 0;

 mf->first = TRUE;
 mf->dim = dim;
 mf->ijk = ijk;
 mf->IJK = IJK;

 return (mf);
}
/*----------------------------------------------------------------------------*/
BOOL
MFOR_r (int cur, int *ijk, int *IJK)
{
  //printf ("cur= %d \n", cur);

  ijk[cur]++; // просто увеличиваем значение текущего индекса

  if (ijk[cur] != IJK[cur]) 
    return (TRUE); // и просто уходим, если нет еще границы

  //---------------------------------------------------------
  // дошли до максимума, значит надо к след. индексу переходить
  if (cur == 0)     // однако, если это был последний (левый) индекс,
    return (FALSE); // то конец

  ijk[cur] = 0;     // обнуляем текущий индекс

  return (MFOR_r (cur-1, ijk, IJK)); // и начинаем крутить индекс левее
  // а что же при этом с правыми происходит?  ну мы же возвращаемся каждый раз
}
/*----------------------------------------------------------------------------*/
BOOL
MFOR_todo (MF *mf)
{

  if (mf->first) { // самый первый этап с нулевыми значениями нужно выделить:
                   // просто ничего не делая уходим 
    mf->first = FALSE;
    return (TRUE);

  }else 
    // увеличиваем сначала правый индекс и при необходимости сдвигаемся влево
    return (MFOR_r (mf->dim-1, mf->ijk, mf->IJK)); 

}
/*----------------------------------------------------------------------------*/
void
MFOR_print_ijk (int *ijk, int dim)
{
  int d;

  printf ("ijk= "); 
  for (d=0; d < dim; d++) printf ("%d ", ijk[d]); 
  printf ("\n"); 

  return;
}
/******************************************************************************/
