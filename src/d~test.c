/*******************************************************************************
 *                                                                             *
 *  ��� ����� �����: d~test.c                                                  *
 *                                                                             *
  ******************************************************************************
 */ 
                                                                             
#include "a_comm.h"

//#include "d_sort.h"
#include "d_find.h"  
#include "d_tree.h"  
//#include "d_unts.h"  

/*----------------------------------------------------------------------------*/
/* void  */
/* a_sort_main ()  */
/* { */

/*   int Orign[] = {44, 55, 12, 42, 94, 18,  6, 67}; */
/*   int Sorts[] = { 6, 12, 18, 42, 44, 55, 67, 94}; */

/*   int Num = sizeof (Orign)/sizeof (Orign[0]); */

/*   printf ("\n"); */
/*   ai_print (Orign, Num);      */
/*   ai_print (Sorts, Num);      */
/*   printf ("\n"); */

/*   qsort_AF (Orign, Num); */

/*   ai_print (Orign, Num);      */
/*   printf ("\n"); */

/*   return; */
/* } */
/*----------------------------------------------------------------------------*/
// 
/*============================================================================*/
int 
main (int argc, char** argv) 
{
  char *name;
  int   j;

  printf ("\n");
  j = 1;
  name = argv[j++];

  //-------------------------
  //if (!strcmp (name, "sort"))     a_sort_main ();

  //-------------------------
/*   if (!strcmp (name, "ants"))  { */
/*     int  seed  = -1 ; // ������������ �� �������  */
/*     if (argc == 3)  seed   = 2009; // ����� ������, ������� : @  */
/*     main_ants (seed); */
/*   } */
  //-------------------------

  if (!strcmp (name, "minm")) {   
      printf ("minmax tests \n"); 
      minm_tests (); 
  }

  if (!strcmp (name, "tree")) {   
      tree_tests (); 
  }

  //-------------------------

  printf ("\n");
  return (0);
}
/*============================================================================*/

