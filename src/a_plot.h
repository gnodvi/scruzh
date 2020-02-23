/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: a_plot.h                                                  *
 *                                                                             *
 *******************************************************************************
 */  

   
/******************************************************************************/                    
#ifdef __cplusplus                                                                
extern "C" {                                                                     
#endif                                                                                         
/******************************************************************************/   

/*******************************************************************************/
/*                                                                             */
#define FI_MAX 20
#define TI_MAX 2000

/*-----------------------------------------------------------------------------*/

typedef struct {
  char    *l_name;
  double   xmin, ymin, xmax, ymax; 
} TT_FUNC;
  
typedef struct {
  char     title[80];
  double  *f[FI_MAX]; 
} TT_STEP;

#define MAX_OUT 50

/* typedef void* (*BIGPLOT_INIT)  (void *plot); */
/* typedef void  (*BIGPLOT_CLOSE) (void *); */
  ///* typedef void  (*BIGPLOT_DRAW)  (void *p, /* YT_PLOT */void *plot, char *title, int ti); */

typedef struct {
  int      ti; // текущее значение (иногда надо, например в GTK+ ?)
  int      fnum, xnum, tnum, tmax;

  TT_FUNC  funcs[FI_MAX];
  double   xmin_set, xmax_set; // принудительные границы

  double  *x;
  TT_STEP  steps[TI_MAX]; 

  // -----------------------------------------------------------
  // список фреймов - это вспомогательная структура НАД списком графиков
  // и отвечает только за внешнее расположение уже готовых графиков по фреймам
  int      fr[MAX_OUT][MAX_OUT]; 
  int      wnum; // вычисляется перед отрисовкой
  // -----------------------------------------------------------
  // (по умолчанию спимок фреймов идентичен списку графиков)
  // однако в тесте надо задавать этот список индивидуально !

/*   BIGPLOT_INIT  bigplot_init; */
/*   BIGPLOT_CLOSE bigplot_close; */
/*   BIGPLOT_DRAW  bigplot_draw; */

} YT_PLOT;

#define P_XMIN(p,fi) (((p)->funcs[(fi)]).xmin)
#define P_XMAX(p,fi) (((p)->funcs[(fi)]).xmax)
#define P_YMIN(p,fi) (((p)->funcs[(fi)]).ymin)
#define P_YMAX(p,fi) (((p)->funcs[(fi)]).ymax)

#define P_NAME(p,fi) (((p)->funcs[(fi)]).l_name)
#define P_TITLE(p,ti) (((p)->steps[(ti)]).title)
#define P_LINE(p,fi,ti) (((p)->steps[(ti)]).f[(fi)])

YT_PLOT* plot_create (int fnum, int xnum, int tnum);
/* void */
/* plot_big_init (YT_PLOT* plot, */
/*                BIGPLOT_INIT  bigplot_init, */
/*                BIGPLOT_DRAW  bigplot_draw, */
/*                BIGPLOT_CLOSE bigplot_close); */

typedef struct {

  YT_PLOT *plot;
  int      ih_one, iw, ih;
  void    *gp;
  BOOL     first1;
  BOOL     first2;

/*   BOOL     is_win; */
/*   BOOL     is_big; */

} T_PLOTSTEP;
  
T_PLOTSTEP *plot_step_beg (/* BOOL is_win, BOOL is_big, */ YT_PLOT *plot);
void        plot_step_end (T_PLOTSTEP *ps);
void        plot_step_do (T_PLOTSTEP *ps, int ti);
void
plot_min_max (int fi, YT_PLOT *plot);
void
winplot_init (T_PLOTSTEP *ps, BOOL is_win, BOOL is_big);
T_PLOTSTEP *
plotstep_create (/* BOOL is_win, BOOL is_big, */ YT_PLOT *plot);

T_PLOTSTEP * plot_step (int ti, /* BOOL is_win, BOOL is_big, */ void *ptr1);

void     plot_print     (YT_PLOT *plot/* , BOOL is_win, BOOL is_gnu */);
  //void     plot_print_one (YT_PLOT *plot, BOOL is_win, BOOL is_gnu, int ti);
void     plot_print_one (YT_PLOT *plot, /* BOOL is_win, BOOL is_gnu, */ int ti);

void     plot_frames_init0 (YT_PLOT *plot);
void     plot_frames_init9 (YT_PLOT *plot);
int      plot_get_wnum (YT_PLOT *plot);
/*-----------------------------------------------------------------------------*/

typedef long  YT_SPACE_NULL;

void
plot_get_minmax_frame (YT_PLOT *p, int wi,
                       double *p_xmin, double *p_xmax, double *p_ymin, double *p_ymax);

/*-----------------------------------------------------------------------------*/

/* YT_PLOT* */
/* make_plot_test_old (int xnum, double fi_min, double fi_max); */

void plot_main (YT_WIN *win, int, int, YT_PLOT *plot, int x, int y, int w, int h);
void plot_test (void);
/* void plot_test_SCS_1 (); */
/* void plot_test_SCS_2 (); */
YT_PLOT *
make_plot_test_3 ();
YT_PLOT*
make_test (int tnum, 
           int xnum, double fi_min, double fi_max);

/******************************************************************************/   
#ifdef __cplusplus                                                               
}                                                                                
#endif                                                                           
/******************************************************************************/   
