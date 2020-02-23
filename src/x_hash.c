/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: x_hash.c                                                  *
 *                                                                             *
  ******************************************************************************
 */  


/*----------------------------------------------------------------------------*/
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <stdarg.h>
#include <assert.h>
//#include <values.h>  

/* #ifdef _UNIX  */
#include <unistd.h>
//#include <time.h>
//#include <values.h>  
/* #else */

//#include <windows.h>
//#include <io.h>
//#include <fcntl.h>
//#include <process.h>

//#include <process.h>
//#include <memory.h>
//#include <string.h>
//#include <stdio.h>
//#include <fcntl.h>
//#include <io.h>
/* #endif      */

// emgena ----------------------------------------------------------------------

//-----------------------------------------------------------------------------

#include "a_comm.h"
#include "x_hash.h"

//------------------------------------------------------------------------------

/*
 * This file, together with engine/hash.h implements hashing of go positions
 * using a method known as Zobrist hashing.  See the Texinfo documentation
 * (Reading/Hashing) for more information.  
 */

//------------------------------------------------------------------------------
void
hashtable_clear (Hashtable *table)
{
  int  i;

  /* Initialize all hash buckets to the empty list. */
  for (i = 0; i < table->hashtablesize; ++i)
    table->hashtable[i] = NULL;
    //(table->voidtable)[i] = NULL;

  table->free_node = 0;
  //table->free_result = 0;
}
//------------------------------------------------------------------------------
void
hashposition_dump (Hashposition *pos/* , FILE *outfile */)
{
  int  i;

  printf ("Board:  ");
  for (i = 0; i < COMPACT_BOARD_SIZE; ++i)
     printf (" %lx", pos->board[i]);

  printf ("\n");
  return;
}
//------------------------------------------------------------------------------
int
hashposition_compare (Hashposition *pos1, Hashposition *pos2)
{
  int  i;

  /* We need only compare to board_size.  MAX_BOARD is not necessary. */
  for (i = 0; i < COMPACT_BOARD_SIZE; i++)
    if (pos1->board[i] != pos2->board[i])
      return 1;

  // This adheres (almost) to the standard compare function semantics 
  // which are used e.g. by the comparison functions used in qsort().
  return 0;
}
//------------------------------------------------------------------------------
void
hashnode_dump (Hashnode *node/* , FILE *outfile */)
{

  printf ("-------------  \n");
  printf ("Hash value: %d \n", (int) node->hashval);
  hashposition_dump (&(node->position)/* , outfile */);
  printf ("Result    : %d \n", node->myresult);

  // Это следущий шаг - если нужено хранить много данных (results)
  printf ("------------- \n");

  return;
}
//------------------------------------------------------------------------------
Hashnode *
hashtable_enter_position (Hashtable *table, 
			 Hashposition *pos, unsigned long hash)
{
  Hashnode  *node, *lll_nodes;
  int         bucket;
  static    YT_BOOL is_full = FALSE;

  /* If the position is already in the table, return a pointer to it. */
  node = hashtable_search (table, pos, hash);
  if (node != NULL) {
    return node;
  }

  /* If the table is full, return NULL */
  if (table->free_node == table->num_nodes) {
    if (!is_full) {

      printf ("Table is Full !\n");
      is_full = TRUE;
    }
    return NULL;
  }

  /* It wasn't there and there is still room.  Allocate a new node for it... */
  //node = &(table->all_nodes[table->free_node++]);
  lll_nodes = (Hashnode *) (table->lll_nodes);
  node = &(lll_nodes[table->free_node++]);

  node->hashval = hash;
  node->position = *pos;

  //node->results = NULL;

  /* ...and enter it into the table. */
  bucket = hash % table->hashtablesize;
  //if (table->hashtable[bucket] != NULL) fprintf (stderr, "Collision! \n");

  node->next = table->hashtable[bucket]; // первй становится вторым (т.е. это стек)
  table->hashtable[bucket] = node;       // новый узел вставляется первым в список

  
  //stats.position_entered++;
  return node;
}
//------------------------------------------------------------------------------
/* 
 * Given a Hashposition and a Hash value, find the hashnode which contains
 * this very position with the given hash value.  
 *
 * We could compute the hash value within this functions, but later
 * when we have incremental calculation of the hash function, this 
 * would be dumb. So we demand the hash value from outside from the 
 * very beginning.
 */
//------------------------------------------------------------------------------
Hashnode *
hashtable_search (Hashtable *table, Hashposition *pos, unsigned long hash)
{
  Hashnode     * node;
  int            bucket;

  bucket = hash % table->hashtablesize;

  node = table->hashtable[bucket];

  // идем по связному списку в поисках нужной позиции
  while (node != NULL
	 && (node->hashval != hash
	     || hashposition_compare (pos, &(node->position)) != 0))
    node = node->next;

  return node;
}
//------------------------------------------------------------------------------
void
hashtable_dump (Hashtable *table, /* FILE *outfile,  */YT_BOOL is_dada_print)
{
  int  i;
  Hashnode * hn;

  if (!table) return;

  /* Data about the table itself. */
  printf ("------------- Dump of hashtable ------------- \n\n");
  printf ("Num_table_All : %d \n",   table->hashtablesize);
  printf ("Num_nodes_All : %d \n",   table->num_nodes);
  printf ("Num_nodes_Use : %d \n",   table->free_node);
  printf ("\n");

  if (!is_dada_print) return;

  /* Data about the contents. */
  for (i = 0; i < table->hashtablesize; ++i) {
    printf ("Bucket %5d: ", i);
    hn = table->hashtable[i];

    if (hn == NULL)
      printf ("empty \n");
    else {
      printf ("\n");
      while (hn) {
	hashnode_dump (hn/* , outfile */);
	hn = hn->next;
      }
    }
    //fprintf (outfile, "\n");
  }
  printf ("\n");


  return;
}
/******************************************************************************/

//------------------------------------------------------------------------------
/*
 * Initialize a hash table for a given total size and size of the
 * hash table.
 *
 * Return 0 if something went wrong.  Just now this means that there
 * wasn't enough memory available.
 */
//------------------------------------------------------------------------------
int
hashtable_init (Hashtable *table, 
	       int tablesize, int num_nodes, int size_node, double koeff_tab)
{
  unsigned long size_table = tablesize * sizeof(Hashnode *);
  unsigned long size_nodes = num_nodes * size_node;

  /* If the hash system is not initialized, do so now. */
  //if (!is_initialized)
  //  gohash_init ();

  //fprintf (stderr, "SIZE = %f (Mb) \n", (size_table + size_nodes)/(float)(1024*1024));

  table->koeff_tab = koeff_tab;
  /* Allocate memory for the pointers in the hash table proper. */
  table->hashtablesize = tablesize;
  table->voidtable = 
  table->hashtable = (Hashnode **) malloc (size_table/* tablesize * sizeof(Hashnode *) */);
  if (table->hashtable == NULL) {
    free (table);
    return 0;
  }

  /* Allocate memory for the nodes. */
  table->num_nodes = num_nodes;
  table->lll_nodes = 
  (Hashnode *) malloc (size_nodes);
  //if (table->all_nodes == NULL) {
  if (table->lll_nodes == NULL) {
    free (table->hashtable);
    free (table);
    return 0;
  }

  /* Allocate memory for the results. */

  /* Initialize the table and all nodes to the empty state . */
  hashtable_clear (table);

  return 1;
}
//------------------------------------------------------------------------------
Hashtable *
hashtable_new (int tablesize, int num_nodes, int size_node, double koeff_tab)
{
  //int size_node = sizeof (Hashtable);
  Hashtable  *table;

  /* Allocate the hashtable struct. */
  table = (Hashtable *) malloc (sizeof (Hashtable)/* size_node */);
  if (table == NULL)
    return NULL;

  /* Initialize the table. */
  if (!hashtable_init (table, tablesize, num_nodes, size_node, koeff_tab)) {
    free (table);
    return NULL;
  }

  return (table);
}
//------------------------------------------------------------------------------
Hashnode *
myhash_set_result (Hashtable *ht, unsigned long hash, Hashposition *p_pos, 
                   int result, int numlev) 
{
  Hashnode  *hashnode;

  hashnode = hashtable_enter_position (ht, p_pos, hash);
  if (hashnode) {
    hashnode->myresult = result;
    hashnode->mynumlev = numlev;
  }

  return (hashnode);
}
//------------------------------------------------------------------------------
int
myhash_get_result (Hashtable *ht, unsigned long hash, Hashposition *p_pos, 
                   int *result, int *numlev) 
{
  Hashnode  *hashnode;

  /* Find this position in the table.  If it wasn't found, enter it. */
  hashnode = hashtable_search (ht, p_pos, hash);
  
  if (hashnode) {
    //*result = hashnode_search (hashnode, routine, *si, *sj);  //?????
    *result = hashnode->myresult;
    if (numlev) 
    *numlev = hashnode->mynumlev;
    return (1);
  }

  return (0);
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
