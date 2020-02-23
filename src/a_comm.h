/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: a_comm.h                                                  *
 *                                                                             *
 *******************************************************************************
 */  
   
#ifndef __A_COMM_H__
#define __A_COMM_H__

#include <stdio.h>
#include <stdlib.h>   
#include <string.h>
#include <math.h> 
#include <time.h>  
#include <unistd.h> 
#include <ctype.h>  
#include <dirent.h> 
#include <sys/time.h> 
#include <sys/param.h>
#include <sys/timeb.h>
#include <assert.h>
#include <stdarg.h>
                                                                             
#include <glib.h>

/******************************************************************************/   
G_BEGIN_DECLS
/******************************************************************************/   

enum keywords_common {
  YREAD, YWRITE, YSEEK,
  GR_CONST, GR_RAND,
  YDONE_common
};

#define RETURN {fun++; long1++; long2++; return (1);}

#define OUTD(val)  (fprintf (stderr, "out_%d \n",(val)))                       
#define OUTF(val)  (fprintf (stderr, "out_%f \n",(val)))                       
#define OUTS(val)  (fprintf (stderr, "out_%s \n",(val))) 

#define YRAND_F  srand48(time(0))
double  YRandF (double fmin, double fmax);

#define YRAND(imin,imax) (imin+random()%(imax-imin+1)) 
void srandom_set (int seed);

#define YRAND_S  srandom_set(-1)
#define YRAND_C  srandom_set(2010) 

typedef int YT_BOOL;                                                          
#define BOOL YT_BOOL
#define CHAR int

#define EPS  0.000001

#define UNUSED(x) (x=x)
#define YERROR(str)  {fprintf (stderr, "ERROR: %s \n", (str)); exit(0);}

int     YInt (double); 
void    Error (const char *msg);      

#define STDERR (stdout) 

/*******************************************************************************/

#define YOVERFLOW(i) ( ((double)i) != (i) )

// calloc - с обнулением выделяемой памяти
#define CALLOC(p,conv,size) {if (YOVERFLOW(size)) p=NULL; else p=(conv)calloc(1,size);}
// malloc - без обнуления
#define MALLOC(p,conv,size) {if (YOVERFLOW(size)) p=NULL; else p=(conv)malloc(size);}

#define YMALLOC(p,type,num) {YMEMLOC(p,type*,(num)*sizeof(type));}
#define YMEMLOC(p,conv,size) {if (YOVERFLOW(size)) p=NULL; else p=(conv)calloc(1,size);}

#define  YMALLOC_NEW(label, p,type,num) {\
           p= (type*) malloc (num * sizeof(type)); \
           if (!p) {\
              fprintf (stderr, "Label=%s Num=%d Sizeof=%d  ", label, num, sizeof(type)); \
              ERROR ("YMALLOC_NEW"); \
           }\
         }

/*******************************************************************************/

#define  G_SUCCESS   0
#define  G_FAILURE  -1

extern BOOL ERR_PRINT;
extern BOOL debug;

int    Rand123 (double p1, double p2, double p3);
BOOL   RandYes (double procent);

BOOL   FRavno (double d1, double d2, double eps);

BOOL   str_cmp (const char *str1, const char *str2);
YT_BOOL  YStrCmp (char *str1, char *str2); 

#define SRAND_TIME -1
#define PRINT_LONGLINE printf ("------------------------------------------------------------------- \n");
  
#define FOR(i,ii) for ((i)=0; (i) < (ii); (i)++)

char* parse_read (char *name, int argc, char *argv[], int  j);
int   parse_find (char *name, int argc, char *argv[]);

void  YDataRW (int num, int act, void *ptr, int size, FILE *stream);

//------------------------------------
typedef struct {
  int  dim;
  int *ijk;
  int *IJK;

  BOOL first;
} MF;

MF   *MFOR_create (int dim, int *ijk, int *IJK);
BOOL  MFOR_todo (MF *mf);
void  MFOR_print_ijk (int *ijk, int dim);

//------------------------------------
int     get_line (char *s, int size);

#define SKIP while(getchar()!='\n')
#define SKIP_LINE  printf("\n")
  
/******************************************************************************/
//
//   ВРЕМЯ: отсечка, формирование строк, печать
//   
//
/******************************************************************************/

char*   time_calc_printa (int mode);
long    YTimeGet (void);
double  TimeSec ();
 
typedef struct {
	time_t    tim_beg;
} YT_TIMER;

YT_TIMER* timer_beg (void);
void      timer_end (YT_TIMER *t, char *hms_time);

/******************************************************************************/
//
//   Формирование символьной экранной информации
//   с дальнейшим выводом на терминал
//
/******************************************************************************/

//-----------------------------------------------------
// ANSI  color-change escape sequences.
// Xterm Control Sequences (this is the file ctlseqs.ms)
// man console_codes

#define ED  printf("\x1b[2J") // стереть экран
#define EL  printf("\x1b[K")  // стереть строку
#define CUP(row,col) printf("\x1b[%d;%dH", row,col) // переместить курсор

#define CUU printf("\x1b[A")  // на одну строку вверх
#define CUD printf("\x1b[B")  // на одну строку вниз
#define CUF printf("\x1b[C")  // на одну строку вправо
#define CUB printf("\x1b[D")  // на одну строку влево

#define SCP printf("\x1b[s")  // считать текущую позицию курсора
#define RCP printf("\x1b[u")  // восстановит позицию курсора

#define SGR_DEFAULT  0
#define SGR_BOLD     1
#define SGR_BLACK   30    //set black foreground
#define SGR_RED     31    //set red foreground
#define SGR_GREEN   32    //set green foreground
#define SGR_BROWN   33    //set brown foreground
#define SGR_BLUE    34    //set blue foreground
#define SGR_MAGENTA 35    //set magenta foreground
#define SGR_CYAN    36    //set cyan foreground
#define SGR_WHITE   37    //set white foreground

//-----------------------------------------------------

typedef struct {
	int  w, h;
	char p[100][100];
} YT_WIN;

YT_WIN* win_create (int w, int h);
void    win_char (YT_WIN *win, CHAR ch, int x, int y);
void    win_sgr (int par);
void    win_text (YT_WIN *win, const char *text, int x, int y);
void    win_draw (YT_WIN *win, int x, int y);
void    win_rect (YT_WIN *win, CHAR ch_horz, CHAR  ch_vert, int x, int y, int w, int h);
void    win_vert (YT_WIN *win, CHAR ch, int x_vert, int my_y1, int y2);
void    win_horz (YT_WIN *win, CHAR  ch, int y_horz, int x1, int x2);
void    win_delete (YT_WIN *win);

void    win_sgr_stderr (int par);

#define ERR_CYAN     /* win_sgr_stderr (SGR_BOLD); */win_sgr_stderr (SGR_CYAN);
#define ERR_GREEN    win_sgr_stderr (SGR_GREEN);
#define ERR_DEFAULT  win_sgr_stderr (SGR_DEFAULT);

typedef struct {
  int     i, x, y, delta_x, delta_y, exchange, err, incx, incy;
  int     x_old, y_old, x_left, x_right;
  BOOL    begin, end;
  int     align;
} VT_HANDLINE;

void    win_test_old ();

/******************************************************************************/
//
//   Печать отладочных отступов при выводе информации
//   Используется в файлах: a_find.c & x_comm.c  
//
/******************************************************************************/

void    dbg_proc_beg (char *str, int exp);
#define DBG_PROC_BEG(str, exp) dbg_proc_beg (str, exp)

void    DBG_PRINT (char *fmt, ...);
YT_BOOL DBG_IS_PRINT ();

void    dbg_proc_end ();
#define DBG_PROC_END(str) dbg_proc_end (str)

void    dbg_set_maxlevel (int mlevel);

#define LEFT left_buf                                                           
#define LEFT_BEG {LEFT[left_num]=' '; left_num+=2; LEFT[left_num]='\0';}        
#define LEFT_END {LEFT[left_num]=' '; left_num-=2; LEFT[left_num]='\0';}                       

/******************************************************************************/
//
//   Простые  БОТЫ     БОТЫ     БОТЫ     БОТЫ     БОТЫ     БОТЫ     
//            БОТЫ     БОТЫ     БОТЫ     БОТЫ     БОТЫ     БОТЫ     
//
/******************************************************************************/

extern YT_BOOL BOTPRINT;

#define fw         4       /* field width for printed numbers */
#define NAMELENG  18

#define BOT_RETURN return (1)
#define BOT_VAR   int message, void *m, long long1, long long2, long long3, double d1
#define T_BOT_RETURN int // пусть пока так будет а то в Рошамбо много менять ..
typedef T_BOT_RETURN (*YT_PROG) (BOT_VAR); // любые параметры ??

#define PROC_VAR  int message, double *xyz, double *fun, long long1, long long2, void *ptr, double *ret
#define T_PROC_RETURN int
typedef T_PROC_RETURN (*YT_PROC) (PROC_VAR);

typedef int (*YT_SIMP) (); // любые параметры ??

enum keywords_1 {
  YINIT, YFREE, YCALC, YTRUE,  

  YGINT, YRINT, YQINT, 

  Y_BEG, YTODO, 
  Y_PEREBOR, YGSLSPUSK, 
  Y_GENALGO, 
};

typedef struct {
  //Bprog  parent;
  //----------------------------------

  YT_PROG prog;               // основная счетная прога (бот)
  long    long1, long2, long3 /* , long4 */;
  double  d1;
} YT_BOT; 

YT_BOT *bot_create (); 

YT_BOT *
bot_make (YT_PROG prog,  long long1, long long2, long long3, 
          /* long long4, */ double d1); 

/******************************************************************************/
//
//    Временные определения:
//
//
/******************************************************************************/

/* typedef struct { */
/*   int     numw; */
/*   char    *legend; */
/*   BOOL    val; */
/*   double  *f; */
/* } ZT_OUT; */

/* ZT_OUT *Out (ZT_OUT *p_out, const char *legend); */
/* void  Do_Open_Close (ZT_OUT *p_out, int mod, int xnum); */

#define YMAX(a,b)  MAX(a,b)
#define YMIN(a,b)  MIN(a,b) 

#define MAXVAL  G_MAXINT
#define MINVAL  G_MININT

#define YMAXSPACE  G_MAXDOUBLE
#define YMINSPACE -G_MAXDOUBLE

#define YMAXDOUBLE  G_MAXDOUBLE
#define YMINDOUBLE -G_MAXDOUBLE

#define YPI G_PI

/******************************************************************************/
G_END_DECLS

#endif /* __A_COMM_H__ */
/******************************************************************************/
