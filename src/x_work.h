/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: x_work.h                                                  *
 *                                                                             *
  ******************************************************************************
 */  


/******************************************************************************/                    
#ifdef __cplusplus                                                                
extern "C" {                                                                     
#endif                                                                                       
/******************************************************************************/   


typedef struct {
  int  i, j;
  int  move_stone;
} GO_MOVE;

GO_MOVE *xo_malloc_move (int i, int j);

/******************************************************************************/

typedef struct {
  GO_MOVE    moves[100]; // история ходов
  int        moves_num;
} GO_MOVES;

GO_MOVES *moves_create ();
void      moves_free (GO_MOVES *mvs);

void      moves_add (GO_MOVES *mvs, int i, int j, int stone);
GO_MOVE*  moves_get (GO_MOVES *mvs, int n);
void      moves_init (GO_MOVES *mvs);
int       moves_num (GO_MOVES *mvs);
void      moves_del (GO_MOVES *mvs, int n);

void      moves_print (GO_MOVES *mvs);

/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: gohash.h                                                  *
 *                                                                             *
  ******************************************************************************
 */  

#define EMPTY 0
#define WHITE 1
#define BLACK 2
#define NONE  6       /* for use with is_computer_player */

/****************************************************************************/   

Hashtable *
gohash_create (int board_isize, int board_jsize, /* int nodes, */ /* int size_node,  */
               int memory /* Megabytes of memory used for hash table. */) ;

void  aboard_to_board (GO_BOARD *b, board_t p[MAX_BOARD][MAX_BOARD]);
unsigned long
myhash_calc (Hashtable *ht, GO_BOARD *b, Hashposition *p_pos) ;

/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: gowork.h                                                  *
 *                                                                             *
  ******************************************************************************
 */  

#define BSIZE  7  // !!!
#define BSIZE2 49

typedef struct {
  int stone;     // здесь нули по умолчанию !!!!
  int ij_num, xy_num;        // количество камней и дамэ у группы
  int i[BSIZE2], j[BSIZE2];  // координаты камней этой группы
  int x[BSIZE2], y[BSIZE2];  // координаты дамэ этой группы
} GO_GROUP;

typedef struct {
  int       stone, dame; // здесь нули по умолчанию !!!!
  GO_GROUP *group;
} GO_POINT;

typedef struct {
  GO_BOARD  *aboard;
  int        isize, jsize;

  int        groups_num;
  GO_GROUP   groups[BSIZE2];       // нужно создавать массивы динамически !!  
  GO_POINT   goban[BSIZE][BSIZE];
} GO_WORK;

#define WBOARD(i, j) (((w->aboard)->board)[(i)*((w->aboard)->jsize)+(j)])

/****************************************************************************/   

GO_WORK  *work_begin_ (/* int, int, */ GO_BOARD *a);
GO_BOARD *work_set_board (GO_WORK *w, GO_BOARD *a); 
void      work_init (GO_WORK *w);
void      work_get_groups (GO_WORK */* , GO_BOARD *a */);

GO_GROUP* work_find_captured_group (GO_WORK *w, int stone);
int       work_is_this_good (GO_WORK *, int my_stone, int i, int j); 
void      work_get_captures (GO_WORK *, int is_set, int *p_ib_cap, int *p_iw_cap, GO_STONE last_move);
int       work_is_move_suicide (GO_WORK *, int stone, int i, int j); 

int       work_random_move (GO_WORK *, int stone, int *i_move, int *j_move); 

GO_GROUP* work_find_captured_group (GO_WORK *w, int stone);
int       work_calc_mindames (GO_WORK *w, GO_STONE stone); 
void      work_get_calcus (GO_WORK *w, GO_STONE stone, int *p_min_dames, int *p_num_capts) ;

int       work_set_move (GO_WORK *w, int stone, int i_move, int j_move);
  //int       work_del_move (GO_WORK *w);
int       work_move_mindames (GO_WORK *w, int stone, int i, int j); 

extern int  debug;

/******************************************************************************/

void  myeuler_test () ;
int   myeuler_calc (int *image, int ii, int jj); 

/******************************************************************************/

int  add_moves_defs (GO_BOARD *t, GO_WORK *w, GO_STONE stone,  long *p_moves, int num);
int  add_moves_cont (GO_BOARD *t,  GO_WORK *w,  long *p_moves, int num); 
int  add_moves_full (GO_BOARD *t, long *p_moves, int num);
int  add_moves_good (GO_BOARD *t, GO_WORK *w, GO_STONE stone,  long *p_moves, int num,
                int good);
int  find_moves_random (GO_BOARD *t, int numfind, long *p_moves, int num);
int  add_moves_caps (GO_BOARD *t, GO_WORK *w, GO_STONE stone,  long *p_moves, int num);

/******************************************************************************/
#ifdef __cplusplus                                                               
}                                                                                
#endif                                                                           
/******************************************************************************/

