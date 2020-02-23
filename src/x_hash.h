/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: x_hash.h                                                  *
 *                                                                             *
  ******************************************************************************
 */  

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 * This is GNU GO, a Go program. Contact gnugo@gnu.org, or see   *
 * http://www.gnu.org/software/gnugo/ for more information.      *
 *                                                               *
 * Copyright 1999 by the Free Software Foundation.               *
 *                                                               *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/****************************************************************************/                    
#ifdef __cplusplus                                                                
extern "C" {                                                                     
#endif                                                                                       
/****************************************************************************/   

//#ifndef _MYHASH_H_
//#define _MYHASH_H_

// --------------------------------- gohash.h --------------------------------*/
//#define EMPTY 0
//#define WHITE 1
//#define BLACK 2
//#define NONE  6       /* for use with is_computer_player */


//#define MIN_BOARD 3          /* minimum supported board size        */
//#define MAX_BOARD 21         /* maximum supported board size        */
#define MAX_BOARD 6         /* maximum supported board size        */

typedef unsigned char board_t;

/* for testing: Enables a lot of checks. */
//#define CHECK_HASHING 0

/* Dump (almost) all read results. */
#define TRACE_READ_RESULTS 0

/*
 * We define a special compact representation of the board for the 
 * positions.  In this representation each location is represented
 * by 2 bits with 0 meaning EMPTY, 1 meaning WHITE and 2 meaning 
 * BLACK as defined in liberty.h.
 *
 * COMPACT_BOARD_SIZE is the size of such a compact representation
 * for the maximum board size allowed.
 */

#define COMPACT_BOARD_SIZE ((MAX_BOARD) * (MAX_BOARD) / 16 + 1)


//------------------------------------------------------------------------------
/*
 * A go position consists of the board, possibly a ko point but NOT of
 * which player is to move.  The ko point is defined as the point where,
 * on the last move, one stone was captured.  It is illegal for he 
 * player to move to place a stone on this point.  To do so would either
 * be suicide, which is illegal anyhow, or a violation of the ko rule.
 * If there is no ko going on, ko_i == -1;
 */

typedef struct hashposition_t {
  unsigned long  board[COMPACT_BOARD_SIZE];
  //int            ko_i;
  //int            ko_j;
  /*  int            to_move;   DISABLED  */
} Hashposition;


/*
 * The hash table consists of hash nodes.  Each hash node consists of
 * The hash value for the position it holds, the position itself and
 * the actual information which is purpose of the table from the start.
 *
 * There is also a pointer to another hash node which is used when
 * the nodes are sorted into hash buckets (see below).
 */

typedef struct hashnode_t {
  unsigned int  hashval;  /* The hash value... */
  Hashposition  position; /* ...for this position. */

  //Read_result  *results;  /* And here are the results of previous readings */
  int           myresult;
  int           mynumlev;

  struct hashnode_t  * next;
} Hashnode;


/*
 * The hash table consists of three parts:
 * - The hash table proper: a number of hash buckets with collisions
 *   being handled by a linked list.

 * - The hash nodes.  These are allocated at creation time and are 
 *   never removed or reallocated in the current implementation.

 * - The results of the searches.  Since many different searches can
 *   be done in the same position, there should be more of these than
 *   hash nodes.
 */

typedef struct hashtable {
  int            hashtablesize;	/* Number of hash buckets */
  Hashnode    ** hashtable;	/* Pointer to array of hashnode lists */
  void         * voidtable;

  int            num_nodes;	/* Total number of hash nodes */
  //Hashnode     * all_nodes;	/* Pointer to all allocated hash nodes. */
  void         * lll_nodes;
  int            free_node;	/* Index to next free node. */

  //int            num_results;	/* Total number of results */
  //Read_result  * all_results;	/* Pointer to all allocated results. */
  //int            free_result;	/* Index to next free result. */
  //------------------
  //int board_size;

  double    koeff_tab;
} Hashtable;


//------------------------------------------------------------------------------
Hashtable *hashtable_new (int tablesize, int num_nodes, int size_node, double);
int        hashtable_init( Hashtable *table, int tablesize, int num_nodes, int size_node, double);

int        hashposition_compare (Hashposition *pos1, Hashposition *pos2);
void       hashposition_dump (Hashposition *pos/* , FILE *outfile */);

void       hashtable_clear (Hashtable *table);

Hashnode  *hashtable_enter_position (Hashtable *table,
				     Hashposition *pos, unsigned long hash);
void       hashtable_delete (Hashtable *table, Hashposition *pos, unsigned long hash);
Hashnode  *hashtable_search (Hashtable *table, Hashposition *pos, unsigned long hash);
void       hashtable_dump (Hashtable *table, /* FILE *outfile, */ YT_BOOL is_dada_print);

void       hashnode_dump (Hashnode *node/* , FILE *outfile */);
//void          hash_init (void);
//unsigned long board_hash (Hashtable *ht, board_t board[MAX_BOARD][MAX_BOARD]);
//void          board_to_position (board_t board[MAX_BOARD][MAX_BOARD],  Hashposition *pos);


//------------------------------------------------------------------------------

//extern int       board_isize, board_jsize;
//extern GO_BOARD *bs[NUM_SYM];

/* static *///extern  int  is_initialized/*  = 0 */;

/* Random values for the hash function.  For stones and ko position. */
/* static */ //extern unsigned long  white_hash[MAX_BOARD][MAX_BOARD];	
/* static */ //extern  unsigned long  black_hash[MAX_BOARD][MAX_BOARD];	
/* static */ //extern  unsigned long  ko_hash[MAX_BOARD][MAX_BOARD];

/* static *///extern   unsigned long white_patterns[];
/* static *///extern   unsigned long black_patterns[];

//------------------------------------------------------------------------------

Hashnode *
myhash_set_result (Hashtable *tt, unsigned long hash, Hashposition *p_pos, 
                   int result, int numlev); 
int
myhash_get_result (Hashtable *ht, unsigned long hash, Hashposition *p_pos, 
                   int *result, int *numlev) ;


/****************************************************************************/   
#ifdef __cplusplus                                                               
}                                                                                
#endif                                                                           
/****************************************************************************/   
//------------------------------------------------------------------------------

//#endif
//------------------------------------------------------------------------------

