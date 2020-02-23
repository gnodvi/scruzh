/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: x_comm.h                                                  *
 *                                                                             *
  ******************************************************************************
 */  


/******************************************************************************/                    
#ifdef __cplusplus                                                                
extern "C" {                                                                     
#endif                                                                                       
/******************************************************************************/   


typedef  short  GO_STONE;                                                          


#define STONE_NONE  0 // 0 // почему с другими значениями ошибка?

/* #define STONE_NONE   100 */
/* #define STONE_WHITE  1 */
/* #define STONE_BLACK  2 */
#define STONE_WHITE  200
#define STONE_BLACK  300

#define STONE_WHITE_MARK  11
#define STONE_BLACK_MARK  22

#define STONE_WHITE_DEAD  111
#define STONE_BLACK_DEAD  221

#define STONE_CONTACT_DAME 99

#define STONE_OTHER(stone) (stone==STONE_WHITE ? STONE_BLACK : STONE_WHITE)
#define STONE_NEXT(stone)  (stone==STONE_WHITE ? STONE_BLACK : STONE_WHITE)

#define STONE_CHAR(stone)  (stone==STONE_WHITE ? '0' : 'X')
#define STONE_NAME(stone)  (stone==STONE_WHITE ? "WHITE" : "BLACK")

/******************************************************************************/

typedef struct {
  int   isize, jsize;  
  int  *board;
} GO_BOARD;

#define BOARD(b, i, j) (((b)->board)[(i)*((b)->jsize)+(j)])
#define BOARD_PS(p, ii, jj, i, j) (p[(i)*(jj)+(j)])

typedef  struct  {
  GO_BOARD *a;
  int       stone; 
} GO_POSI;

void     xo_show_board     (GO_POSI *pos);
GO_POSI *xo_pos_copy (GO_POSI *dst, GO_POSI *src);
GO_POSI *xo_pos_create (int stone, int ii, int jj);
void     xo_pos_delete (GO_POSI *pos);

void  posi_show (GO_POSI *pos);

void read_board (FILE *fp, GO_BOARD *b, int ii, int jj); 
int  read_file_posi (char *fn1, GO_POSI *posis[], int max_num, int ii, int jj); 


/******************************************************************************/

typedef struct {
   int    ii, jj;
   char  *pos[5];

  GO_STONE stone;
} YT_POSITION;
  
GO_POSI* posi_from_position (YT_POSITION *pp) ;

/******************************************************************************/

GO_BOARD *aboard_begin (int isize, int jsize);
void      aboard_clean (GO_BOARD *a);
void      aboard_set_stone_ (GO_BOARD *a, int stone, int i_move, int j_move);
void      aboard_del_stone (GO_BOARD *a, int i, int j);

int       aboard_is_move_legal (GO_BOARD*, int i, int j);
void      aboard_show (GO_BOARD*, YT_BOOL is_debug, YT_BOOL is_plus); 

void      aboard_show_old (GO_BOARD *a); 

char*     board_name_from_index (GO_BOARD *a, int i, int j); 
void      board_name_to_index (GO_BOARD *a, char *name, int *i_ptr, int *j_ptr); 

int       board_random_stone (GO_BOARD *b, int *i_move, int *j_move); 
int       board_random_rect_stone (GO_BOARD *b, int, int, int, int, int *i_move, int *j_move); 
GO_BOARD *board_copy (GO_BOARD *from);
GO_BOARD *board_copy2 (GO_BOARD *to, GO_BOARD *from);

void      board_to_from (GO_BOARD *to, GO_BOARD *from);

/******************************************************************************/

int       stone_from_char (char ch);
int       int_from_char (char c); 

enum symm {SYMM_VERT, SYMM_HORZ, SYMM_DIA1, SYMM_DIA2};
#define SYM(kk, k) (kk-k-1)
#define NUM_SYM 8

int  board_symm_calc (GO_BOARD *bs[], GO_BOARD *b);


/******************************************************************************/   
#ifdef __cplusplus                                                               
}                                                                                
#endif                                                                           
/******************************************************************************/   

/******************************************************************************/
//                                                                             /
/******************************************************************************/
