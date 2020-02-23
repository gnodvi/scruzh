/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: d_find.h                                                  *
 *                                                                             *
  ******************************************************************************
 */  
    
#include <limits.h>	/* for INT_MAX */

//******************************************************************************
#ifdef __cplusplus	/* let C++ coders use this library */
extern "C" {
#endif
//******************************************************************************

#define MM_MAX_EVAL   100
#define MM_MIN_EVAL  -100
#define MM_MOVES_MAX  50 //!!!

#define FIND_SIMP_FULL 0
#define FIND_BEST      1
#define FIND_BEST_FULL 2
#define FIND_BABY      3
#define FIND_KARL      4

#define MAXPLY_NOT    -1

/*----------------------------------------------------------------------------*/
typedef  long  (* MM_PROC_C) (long posi);
  //typedef  int   (* MM_PROC_F) (long pstr, long posi, long *p, long find_mode, void *p_work);   
typedef  int   (* MM_PROC_F) (long pstr, long posi, long *p, long find_mode/* , void *p_work */);   

// конечно здесь надо будет сделать в будущем  оценку в виде DOUBLE !!!!!!                         
typedef  int   (* MM_PROC_E) (long user, long posi,  int *p_numlev); // Evaluate position

typedef  long  (* MM_PROC_M) (long pstr, long posi, long move);
typedef  void  (* MM_PROC_U) (long pstr, long old_posi);
typedef  void  (* MM_PROC_FP) (long pstr, long posi);
typedef  char* (* MM_PROC_N) (/* long pstr, */ long p_move, long pstr_new);
typedef  char* (* MM_PROC_P) (long pstr, long posi);

typedef  void  (* MM_PROC_MAP) (long pstr, long posi, long moves[], int evals[], int levls[], int num);
typedef  void  (* MM_PROC_H) (long pstr, long posi, int eval, int numlev);

/*----------------------------------------------------------------------------*/
typedef struct {
  int     maxply;
  YT_BOOL is_alfa_beta;
  //int     alfa, beta; // этож переменное окошко!

  int     gamer_color;

  MM_PROC_C   copy_posi;
  MM_PROC_F   find_moves;
  MM_PROC_E   evaluate;
  MM_PROC_M   make_move;
  MM_PROC_U   undo_move;
  MM_PROC_FP  free_posi;

  MM_PROC_N   name_move;
  MM_PROC_P   name_posi;
  MM_PROC_MAP moves_map;
  MM_PROC_H   set_to_hashtable;

  int    numlev;
  long   posis[100];
  int    posis_num;

} YT_MINMAX;
/*----------------------------------------------------------------------------*/

YT_MINMAX* mm_create (/* long pstr,  */
                    MM_PROC_C copy_posi, 
                    MM_PROC_F find_moves, MM_PROC_E evaluate,
                    MM_PROC_M make_move,  MM_PROC_U undo_move, MM_PROC_FP free_posi,
                    MM_PROC_N name_move,  MM_PROC_P name_posi, MM_PROC_MAP moves_map, 
                    MM_PROC_H   set_to_hashtable,
                    int gamer_color/* , int maxply */);

void mm_init2 (YT_MINMAX *mm, int maxply, YT_BOOL is_alfa_beta/* , int alfa, int beta */);

int   mm_minimax (YT_MINMAX *g, int alfa, int beta, 
                  long posi, long param, 
                  long *p_bestmove, int *count_ends, int *p_levl);

void  free_moves_all (long moves[], int num_moves);
void  minm_tests (); 

//******************************************************************************
#ifdef __cplusplus
}
#endif
//******************************************************************************

  //#endif /* MMGAME_H */
/*----------------------------------------------------------------------------*/
