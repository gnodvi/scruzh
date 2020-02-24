// -*-  mode: c    ; coding: koi8   -*- ----------------------------------------

/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: a_plot.c                                                  *
 *                                                                             *
  ******************************************************************************
 */   
     
#include "a_comm.h"
#include "a_plot.h"


/******************************************************************************/
/*-----------------------------------------------------------------------------*/
void
plot_frames_init0 (YT_PLOT *plot)
{
  // начальная инициация списка фреймов/графиков:
  int wi, li;

  for (wi=0 ; wi < MAX_OUT; wi++) 
  for (li=0 ; li < MAX_OUT; li++) {
    plot->fr[wi][li] = -1; // вот это разумно!
  }

  return;
}
/*-----------------------------------------------------------------------------*/
void
plot_frames_init1 (YT_PLOT *plot)
{
  // пока формируем простой список фреймов, 
  // полагая, что в одном фрейме - один график..
  int fi;

  for (fi=0 ; fi < plot->fnum ; fi++) {
    plot->fr[fi][0] = fi; 
  }

  return;
}
/*-----------------------------------------------------------------------------*/
void
plot_frames_init9 (YT_PLOT *plot)
{
  int fi;
  // загоняем все графики в один фрейм

  for (fi=0 ; fi < plot->fnum ; fi++) {
    plot->fr[0][fi] = fi; 
  }

  return;
}
/*-----------------------------------------------------------------------------*/
int
plot_get_wnum (YT_PLOT *plot)
{
  int wi;

  for (wi=0 ; plot->fr[wi][0] != -1; wi++) {
    ;; 
  }

  return (wi);
}
/*-----------------------------------------------------------------------------*/
/* void */
/* plot_big_init (YT_PLOT* plot, */
/*                BIGPLOT_INIT  bigplot_init, */
/*                BIGPLOT_DRAW  bigplot_draw, */
/*                BIGPLOT_CLOSE bigplot_close) */
/* { */

/*   plot->bigplot_init  = bigplot_init; */
/*   plot->bigplot_draw  = bigplot_draw; */
/*   plot->bigplot_close = bigplot_close; */

/*   return; */
/* } */
/*-----------------------------------------------------------------------------*/
YT_PLOT*
plot_create (int fnum, int xnum, int tnum)
{

  YT_PLOT *plot = (YT_PLOT*) malloc (sizeof(YT_PLOT));

  plot->x = (double*) malloc (xnum * sizeof(double));

  plot->fnum = fnum;
  plot->xnum = xnum;
  plot->tnum = tnum;
  plot->tmax = tnum - 1;

  // начальная инициация (обнуление) списка фреймов:
  plot_frames_init0 (plot);

  // начальная инициация функций:
  int fi;
  for (fi=0 ; fi < plot->fnum ; fi++) {
    P_NAME(plot, fi) = "";
  }

  // пока формируем простой список фреймов - по одному графику 
  plot_frames_init1 (plot);

/*   plot->bigplot_init  = NULL; */
/*   plot->bigplot_draw  = NULL; */
/*   plot->bigplot_close = NULL; */

/*   plot_big_init (plot,  */
/*                  gnu_bigplot_init, gnu_bigplot_draw, gnu_bigplot_close); */

  plot->ti = 0;

  return (plot);
}
/*-----------------------------------------------------------------------------*/
void
plot_min_max_local (int fi, int ti, YT_PLOT *plot, double *p_xmin, double *p_ymin, 
						   double *p_xmax, double *p_ymax)
{
  double  xmin, ymin, xmax, ymax;
  int     i/* , li */;
  double  *x, *f_line;

  x = plot->x;

  xmin = ymin =  G_MAXDOUBLE;
  xmax = ymax = -G_MAXDOUBLE;

  for (i=0 ; i<plot->xnum ; i++) {
    if (x[i] < xmin) xmin = x[i];
    if (x[i] > xmax) xmax = x[i];

    f_line = P_LINE(plot, fi, ti);

    if (f_line[i] < ymin) ymin = f_line[i];
    if (f_line[i] > ymax) ymax = f_line[i];
  }

  *p_xmin = xmin;
  *p_ymin = ymin;
  *p_xmax = xmax;
  *p_ymax = ymax;

  return;
}
/*-----------------------------------------------------------------------------*/
void
plot_min_max (int fi, YT_PLOT *plot)
{

  double  xmin, ymin, xmax, ymax;
  int     ti;

  P_XMIN (plot, fi) = P_YMIN (plot, fi) = /* YMAXDOUBLE */  G_MAXDOUBLE;
  P_XMAX (plot, fi) = P_YMAX (plot, fi) = /* YMINDOUBLE */ -G_MAXDOUBLE;

  //for (ti=0; ti<=plot->tmax; ti++) { // 0..1 ??
  for (ti=0; ti<plot->tnum; ti++) { 
    plot_min_max_local (fi, ti, plot, &xmin, &ymin, &xmax, &ymax);

    P_XMIN (plot, fi) = MIN (xmin, P_XMIN (plot, fi));
    P_XMAX (plot, fi) = MAX (xmax, P_XMAX (plot, fi));

    P_YMIN (plot, fi) = MIN (ymin, P_YMIN (plot, fi));
    P_YMAX (plot, fi) = MAX (ymax, P_YMAX (plot, fi));
  }

  if (FRavno(P_YMIN (plot, fi), P_YMAX (plot, fi), EPS)) {
    // !!!!!!!!!!!!!!!????
    P_YMIN (plot, fi) -= 10.0;
    P_YMAX (plot, fi) += 10.0;
  }

  return;
}
/*-----------------------------------------------------------------------------*/
void
plot_get_minmax_frame (YT_PLOT *p, int wi,
                       double *p_xmin, double *p_xmax, double *p_ymin, double *p_ymax)
{
  double  xmin, xmax, ymin, ymax;
  YT_BOOL is_xrang_calc;
  int     fi, li;

  xmin = p->xmin_set;
  xmax = p->xmax_set;

  if (xmin >= xmax) { // условный сигнал - посчитать границы по иксам
    is_xrang_calc = TRUE;
    xmin = +100000;
    xmax = -100000;
  } else {
    is_xrang_calc = FALSE;
  }
  ymin = +100000;
  ymax = -100000;

  for (li=0; (fi = p->fr[wi][li]) != -1; li++) {
    if (is_xrang_calc) { // условный сигнал
      xmin = MIN (xmin, P_XMIN(p, fi));
      xmax = MAX (xmax, P_XMAX(p, fi));
    }
      	
    ymin = MIN (ymin, P_YMIN (p, fi));
    ymax = MAX (ymax, P_YMAX (p, fi));
  }

  *p_xmin = xmin;
  *p_xmax = xmax;
  *p_ymin = ymin;
  *p_ymax = ymax;

  return;
}
/*-----------------------------------------------------------------------------*/
void
plot_win_lines (YT_PLOT *plot, YT_WIN *win,
                    int /* fi */wi, int ti,  
                    int ix_0, int iy_0, int iw, int ih)
{
  int fi, li;
  char znak[] = {'+', 'o', '*', '.', '#'};

  int     i, ix, iy;
  double  x_step, y_step, xmin, ymin, xmax, ymax;
  double  *x, *y;

  for (li=0; (fi = plot->fr[wi][li]) != -1; li++) {

    // посчитаем габариты фрейма
    plot_get_minmax_frame (plot, wi,
                           &xmin, &xmax, &ymin, &ymax);

/*     if (ERR_PRINT) { */
/*       printf ("wi= %d  xmin= %f  xmax= %f  ymin= %f  ymax= %f \n",  */
/*               wi, xmin, xmax, ymin, ymax); */
/*     } */

    x_step = (xmax - xmin) / (iw - 1);
    y_step = (ymax - ymin) / (ih - 1);

    x = plot->x;              // массив иксов для графика
    y = P_LINE(plot, fi, ti); // массив игреков

    for (i=0; i < plot->xnum; i++) {
      ix = YInt ((x[i] - xmin) / x_step);
      iy = YInt ((y[i] - ymin) / y_step);

      win_char (win, znak[li], ix_0+ix, iy_0+ih-iy-1);
    }
  }

  return;
}
/*-----------------------------------------------------------------------------*/
void 
print_tabl_one (YT_WIN  *win, YT_PLOT *plot, int x, int y,
                double *f_0, double *f_1, int i)
{
  char str[80];

  // если есть 2-й график, то печатаем его таблицу
  // пока отключим эту сомнительную фичу .. !!

  //if (f_1)  sprintf (str, "%7.2f % f % f", plot->x[i], f_0[i], f_1[i]);
  /* else */   sprintf (str, "%7.2f % f",     plot->x[i], f_0[i]);

  win_text (win, str, x, y);

  return;
}
/*-----------------------------------------------------------------------------*/
void
win_plot_tabl_draw (int fi0, int fi1, 
                    int ti, YT_WIN  *win, YT_PLOT *plot, int x, int y, int h)
{
  int     i, j, h_all, h_beg, h_end;
  double  *f_0, *f_1;

  f_0 = P_LINE (plot, fi0, ti);

  if (fi1 == -1)  f_1 = NULL;
  else            f_1 = P_LINE (plot, fi1, ti);

  // печать таблицы значений графика
  h_all = h - 2;

  if (plot->xnum <= h_all) { // помещаются все значения
    for (i=0; i < plot->xnum; i++) 
      print_tabl_one (win, plot, x+2, y+i,  f_0, f_1, i);

  }else{
    h_end = h_all / 2 - 1; 
    h_beg = h_all - h_end - 1;

    for (i=0; i < h_beg; i++)  // верхняя часть таблицы
      print_tabl_one (win, plot, x+2, y+i,  f_0, f_1, i);
		
    win_text (win, "...............", x+2, y+i);

    y = y+i+1;
    for (i=0; i < h_end; i++) { // нижняя часть таблицы
      j = plot->xnum - h_end + i;
      print_tabl_one (win, plot, x+2, y+i,  f_0, f_1, j);
    }
  }

  return;
}
/*-----------------------------------------------------------------------------*/
void
plot_win_right (YT_PLOT *plot, YT_WIN *win,
                int /* fi */ wi, 
                int ti,  
                int x0, int y0, int hh)
{
  int fi0 =  plot->fr[wi][0]; // возьмем пока только 1-й график
  int fi1 =  plot->fr[wi][1]; 

  //char  str[80];
  // пока не будем печатать габариты (не понятно к каким функциям их относить)

  win_text (win, P_NAME(plot, fi0), x0+2, y0);
  // имена тоже хорошо бы напечатать все..

  //sprintf (str, "ymax =  % f", P_YMAX (plot, fi0));
  //win_text (win, str, x0+2, y0+1);

  win_plot_tabl_draw (fi0, fi1, 
                      ti, win, plot,  x0, y0+2, hh-1);

  //sprintf (str, "ymin =  % f", P_YMIN (plot, fi0));
  //win_text (win, str, x0+2, y0 + hh-1);

  return;
}
/*-----------------------------------------------------------------------------*/
void
plot_win_main (YT_PLOT *plot, YT_WIN *win, 
               int /* fi */wi, int ti,  
               int x, int y, int w, int h)
{
  // определяем размеры частей
  int w2 = 21 + 10;
  int w1 = w - w2+2;

  // рисуем левые графики
  plot_win_lines (plot, win,  /* fi */wi, ti,   x, y, w1-1, h);

  // рисуем перегородку
  win_vert (win, '|', w1, y, y+h);

  // рисуем правую часть (имя 1-го графика, мин, макс и таблицу 1-го графика)
  plot_win_right (plot, win,  /* fi */wi, ti,  w1, y, h);

  return;
}
/*-----------------------------------------------------------------------------*/
void
plot_win (int ti, YT_PLOT *plot, int iw, int ih, int ih_one)
{
  int  wi;
  int  yi = 1;

  // создаем экранную форму и рисуем главную рамочку
  YT_WIN  *win = win_create (iw, ih);
  win_rect (win, '=', '|', 0, 0, iw, ih);

  // здесь надо идти не по списку функций, а по фреймам!
  for (wi=0; plot->fr[wi][0] != -1; wi++) {

    plot_win_main (plot, win,  /* fi */wi, ti,  1, yi, iw-2, ih_one);

    yi += ih_one;
    win_horz (win, '=', yi, 2, iw-3);
    yi++;
  }

  // выводим экранную форму
  win_draw (win, 0, 0);

  return;
}
/*----------------------------------------------------------------------------*/
void
plot_print_info (YT_PLOT *p)
{

  int fi, li, wi, wnum = plot_get_wnum (p);

  printf ("\n");
  printf ("fnum=%d  xnum=%d  tnum=%d  tmax=%d   \n", 
          p->fnum, p->xnum, p->tnum, p->tmax);

  for (wi=0; wi < wnum; wi++) {
    printf ("wi=%d  ", wi);

    for (li=0; (fi = p->fr[wi][li]) != -1; li++) {
      //printf ("li=%d  ", li);
      printf ("%s  ", P_NAME (p, fi));
    }

    printf ("\n");
  }

  printf ("\n");
  return;
}
/*-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*/
void
winplot_init (T_PLOTSTEP *ps, BOOL is_win, BOOL is_big)
{

  //ps->plot->wnum = plot_get_wnum (ps->plot);

  int wnum = ps->plot->wnum;

  if (wnum == 1) ps->ih_one = 18; 
  if (wnum == 2) ps->ih_one = 13; 
  if (wnum == 3) ps->ih_one = 9; 
  if (wnum == 4) ps->ih_one = 8; 
  ps->ih = (wnum * ps->ih_one) + wnum + 1; //??
  ps->iw = 70;
		
  ps->first1 = TRUE;
  ps->first2 = TRUE;

  //ps->is_win = is_win;
  //ps->is_big = is_big;

  return;
}
/*-----------------------------------------------------------------------------*/
T_PLOTSTEP *
plotstep_create (/* BOOL is_win, BOOL is_big, */ YT_PLOT *plot)
{

  T_PLOTSTEP *ps = (T_PLOTSTEP *) malloc (sizeof (T_PLOTSTEP));
  ps->plot = plot;

  int   fi;
  for (fi=0 ; fi < plot->fnum ; fi++) 
    plot_min_max (fi, plot);
		
  ps->plot->wnum = plot_get_wnum (ps->plot);

  return (ps);
}
/*-----------------------------------------------------------------------------*/
T_PLOTSTEP *
plot_step_beg (/* BOOL is_win, BOOL is_big, */ YT_PLOT *plot)
{
  int is_win = TRUE;
  int is_big = FALSE;

  T_PLOTSTEP *ps;

  ps = plotstep_create (/* is_win, is_big *//* ,  */plot);

  winplot_init (ps, is_win, is_big);

/*   if (is_big) { */
/*     if (plot->bigplot_init)  ps->gp = plot->bigplot_init (plot); */
/*   } */

  return (ps);
}
/*-----------------------------------------------------------------------------*/
void
plot_step_end (T_PLOTSTEP *ps)
{

/*   if (ps->is_big) { */
/*     if (ps->plot->bigplot_close)  ps->plot->bigplot_close (ps->gp); */
/*   } */

  return;
}
/*-----------------------------------------------------------------------------*/
void
plot_step_do (T_PLOTSTEP *ps, int ti)
{
  int   i;
  char *title = P_TITLE (ps->plot, ti);
  //------------------------------------------------

  if (!(ps->first1)) CUU;

  //if (ps->is_win) {
    if (ps->first2) ps->first2 = FALSE;
    else for (i=0; i < ps->ih; i++) CUU;
    //}

  printf ("%s \n", title);
  //if (ps->is_win) {
    plot_win (ti, ps->plot, ps->iw, ps->ih, ps->ih_one);
    //}
			
  if (ps->first1) {
    printf ("\n"); CUU; 
    ps->first1 = FALSE;
  }
  printf ("Command: ");

  //------------------------------------------------
/*   if (ps->is_big) { */
/*     if (ps->plot->bigplot_draw)  ps->plot->bigplot_draw (ps->gp, ps->plot, title, ti); */
/*   } */

  return;
}
/*-----------------------------------------------------------------------------*/
T_PLOTSTEP *
plot_step (int ti, /* BOOL is_win, BOOL is_big, */ void *ptr1)
{
/*   int is_win = TRUE; */
/*   int is_big = FALSE; */

  static T_PLOTSTEP *ps;

  if (ti == -1) { // подготовительная часть 
    ps = plot_step_beg (/* is_win, is_big, */ ptr1);
    return (ps);
  }
	
  if (ti == -2) { // закрывающая часть 
    plot_step_end (ps);
    return NULL;
  }

  plot_step_do (ps, ti); // основная часть 
  return NULL;	
}
/*-----------------------------------------------------------------------------*/
void
plot_print (YT_PLOT *plot/* , BOOL is_win, BOOL is_big */)
{
  //int is_win = TRUE;
  //int is_big = FALSE;

  int  ti, ti_incr = +1;
  char buff[80];

  plot_step (-1, /* is_win, is_big, */ plot);
  ti = plot->tmax;
  //ti = 0;

  while (1) {
    plot_step (ti, /* is_win, is_big, */ P_TITLE (plot,ti));

    while (1) { 
      fgets (buff, 50, stdin); CUU;
      if      (buff[0] == '\n') ti += ti_incr;
      else if (buff[0] ==  'g') ti = atoi(&(buff[1]));
      else if (buff[0] ==  '[') ti = 0;
      else if (buff[0] ==  ']') ti = plot->tmax;
      else if (buff[0] ==  'q') goto end;
      else continue;

      if (ti > plot->tmax) ti = 0;
      break;
    }
  }

 end:
  plot_step (-2, /* is_win, is_big, */ NULL);

  return;
}
/*-----------------------------------------------------------------------------*/
void
plot_print_one (YT_PLOT *plot, /* BOOL is_win, BOOL is_big, */ int ti)
{

/*   BOOL is_win = 1;  */
/*   BOOL is_gnu = 0; */

  T_PLOTSTEP *ps;

  ps = plot_step_beg (/* is_win, is_gnu, */ plot);

  plot_step_do (ps, ti);

  plot_step_end (ps);

  return;
}
/*----------------------------------------------------------------------------*/
// 
/******************************************************************************/
YT_PLOT *
plot_test_prepare ()
{
  int     /* ti=0, */ fi=0/* , xi=0 */, i;
  YT_PLOT *plot;
  double  *f_line;

  int fnum = 2;
  int xnum = 10;
  int tnum = 1; // ??

  plot = plot_create (fnum, xnum, tnum/* , tmax */);
  sprintf (P_TITLE (plot,/* ti */0), "PLOT_TEST:");


  for (fi=0 ; fi < plot->fnum ; fi++) {
    P_NAME(plot, fi) = "TestPlot";

    YMALLOC (f_line, double, xnum);
    P_LINE (plot,fi,0) = f_line;

    for (i=0 ; i<plot->xnum; i++) {
      plot->x[i] = i;
      f_line[i]  = (fi+1)*i;
    }
  }
	
  return (plot);
}
/*----------------------------------------------------------------------------*/
void
plot_test ()
{

  YT_PLOT *plot = plot_test_prepare ();

  //plot_print_one (plot, /* is_win */TRUE, /* is_big */FALSE, /* ti */0);
  plot_print_one (plot, /* ti */0);

  return;
}
/*----------------------------------------------------------------------------*/
// 
/******************************************************************************/
YT_PLOT*
make_test (int tnum, 
           int xnum, double fi_min, double fi_max)
{
  YT_PLOT *plot;
  int     i;
  double  fi_step, *f_sin, *f_cos;

  int fnum = 2;
  //int tnum = 5;
  int ti;
  plot = plot_create (fnum, xnum, tnum);

  for (ti=0 ; ti < tnum ; ti++) {
    // выделяем память под графики
    f_sin = (double*) malloc (xnum * sizeof(double));
    f_cos = (double*) malloc (xnum * sizeof(double));

    fi_step = (fi_max - fi_min) / (xnum - 1);

    // формируем значения "x, y"
    for (i=0 ; i < xnum ; i++) {
      plot->x[i] = fi_min + fi_step * i;
      f_sin[i] = sin ((ti+1)*plot->x[i]);
      f_cos[i] = cos (plot->x[i]);
    }

    // теперь сами графики:
    P_LINE (plot, /*fi*/0, ti) = f_sin;
    P_NAME (plot, /*fi*/0) = "Sinus";

    P_LINE (plot, /*fi*/1, ti) = f_cos;
    P_NAME (plot, /*fi*/1) = "Cosin";

    // общий титл ..
    sprintf (P_TITLE (plot,ti), "ti = %d", ti);
  }

  return (plot);
}
/*----------------------------------------------------------------------------*/
YT_PLOT *
make_plot_test_3 ()
{ 

  YT_PLOT *plot = make_test (10,  
                             100, 0, 2*G_PI);

  // начальная инициация (обнуление) списка фреймов:
  plot_frames_init0 (plot);

  // в 1-м фрейме:
  plot->fr[0][0] = 0/*fi*/;  // "Sinus"
  plot->fr[0][1] = 1/*fi*/;  // "Cosin"

  // во 2-м фрейме:
  plot->fr[1][0] = 0/*fi*/;  // "Sinus" 

  return (plot);  
}
/*----------------------------------------------------------------------------*/
/*******************************************************************************/
