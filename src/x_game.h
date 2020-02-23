/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: x_game.h                                                  *
 *                                                                             *
  ******************************************************************************
 */  


/******************************************************************************/                  
#ifdef __cplusplus                                                               
extern "C" {                                                                     
#endif                                                                                     
/******************************************************************************/   



/******************************************************************************/   
#define  GPROC_PAR   int, long, long, long, long, long 
#define  GPROC_VAR   int message, long mes1, long mes2, long mes3, long mes4, long mes5  
typedef  long (*YT_GPROC) (GPROC_PAR);                              
 
enum y_enum { 
   YM_CREATE, YM_SETSTONE, YM_SETMOVE, YM_GETMOVE 
}; 

/******************************************************************************/   
typedef struct {
  GO_POSI  *posi;
  long      mywork; // GO_WORK 
} YT_GOBAN;

/******************************************************************************/   
typedef int
  (*GO_FIND_BESTMOVE_PROC) (void *self, GO_POSI *posi, GO_MOVE **p_bestmove);

typedef struct { 
  char       name[150];
  GO_STONE   color;      // это текущий ход ?!! он же.. 

  //void  (*bot_data) (void *self, void *mp);
  //void  (*bot_todo) (void *self);
  //int (*find_bestmove) (void *self, GO_POSI *posi, GO_MOVE **p_bestmove);
  GO_FIND_BESTMOVE_PROC find_proc;

  YT_GOBAN  *goban;
  int        find_mode;  // форм. списка лучших ходов

  YT_MINMAX *mm;         // механизм перебора
  Hashtable *movehash;
} GO_GAMER;

int      gamer_get_move (GO_GAMER *gamer, int *p_i, int *p_j);
YT_BOOL  gamer_todo_move (GO_GAMER *gamer, GO_STONE stone);
GO_GAMER * 
gamer_make (char *prog_name, int is_moves_map,  int memory_hash,
            GO_STONE color, YT_GOBAN *goban);

/* YT_MINBOT *minbot_create ();  */

/* YT_MINBOT * */
/* minbot_make ( */
/*              void  (*bot_init) (void *self, long long1, long long2, long long3, double d1), */
/*              void  (*bot_data) (void *self, void *mp), */
/*              void  (*bot_todo) (void *self), */
/*              long long1, long long2, long long3, double d1);  */

/* void  botperebor_init (void *b, long long1, long long2, long long3, double d1); */
/* void  botperebor_data (void *bot, void *mp); */
/* void  botperebor_todo (void *bot); */

/******************************************************************************/   

#define GAMER_NEXT(gamer)  (gamer==0 ? 1 : 0) 

#define GMWORK (gamer->goban->mywork) //??
#define GMDESK (gamer->goban->posi->a)

/******************************************************************************/ 
YT_GOBAN* goban_create (int ii, int jj);

void     goban_showboard (YT_GOBAN *goban); 
void     goban_set_stone (YT_GOBAN *goban, int i_move, int j_move, GO_STONE stone);
int      goban_set_move  (YT_GOBAN *goban, int i_move, int j_move, GO_STONE stone);
void     goban_set_board (YT_GOBAN *goban, int *d_board);
void     goban_clean_board (YT_GOBAN *goban);
void     goban_get_board (YT_GOBAN *goban, int *d_board);
  //void     goban_set_posi (YT_GOBAN *goban, GO_POSI *posi); 
void     goban_set_position (YT_GOBAN *goban, YT_POSITION *pp);
int      goban_random_stone_new (YT_GOBAN *goban, int *i_move, int *j_move); 
int      goban_random_handicap (YT_GOBAN *goban, int, int *i_move, int *j_move); 

int      game_autoplay_ (YT_GOBAN *goban, char *prog1, char *prog2, int is_print);

/******************************************************************************/  
 
void  go_moves_map (long pstr, long posi, long moves[], int evals[], int num_moves);
void  go_moves_map_prep (GO_POSI *pos, long moves[], int evals[], int levls[], int num_moves);
 
void
test_moves_map (int dbg_maxlevel, char *prog, int ii, int jj, int mb, int is_moves_map);

/*----------------------------------------------------------------------------*/

double solver_check (int ii, int jj, YT_BOOL is_print_err); 

/******************************************************************************/ 
// goeval.h  
/******************************************************************************/   

/*----------------------------------------------------------------------------*/

  //void  myeuler_test () ;
  //int   myeuler_calc (int *image, int ii, int jj); 

/*----------------------------------------------------------------------------*/
int  
findmoves (long find_mode, GO_WORK *aigo_work, GO_POSI *pos, /* GO_BOARD *a,  */
           long *p_moves);

int  aigo_findmoves (long gamer, long posi, long *p_moves, long find_mode/* , void *p_work */);
int  aigo_evaluate  (long user, long posi, int *p_numlev);
void aigo_set_to_hashtable (long pstr, long posi, int eval, int numlev);

//int  aigo_who_win (GO_GAME *g); 

/******************************************************************************/   


/******************************************************************************/   
#ifdef __cplusplus                                                               
}                                                                                
#endif                                                                           
/******************************************************************************/   
