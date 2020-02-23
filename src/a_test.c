/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: a_test.c                                                  *
 *                                                                             *
  ******************************************************************************
 */ 
                                                                             
 
#include "a_comm.h"
#include "a_plot.h"


/*----------------------------------------------------------------------------*/
// 
/******************************************************************************/
void 
glib_test ()
{

  printf ("G_MININT= %d  G_MAXINT= %d  \n", G_MININT, G_MAXINT);
  printf ("MIN= %d  MAX= %d  \n", MIN (2, 5), MAX (2, 5));
  printf ("G_DIR_SEPARATOR_S= %s  \n", G_DIR_SEPARATOR_S);

  return;
}
/*----------------------------------------------------------------------------*/
void 
rand_test_0 ()
{

  int imin = 1;
  int imax = 4;
  int i;

  for (i=0; i<10; i++) {
    printf ("YRAND(1 4)= %ld  \n", YRAND(imin, imax));    
  }

}
/*----------------------------------------------------------------------------*/
void 
rand_test_7 ()
{

  int imin = -7;
  int imax = +7;
  int i;

  srandom_set (2010);  

  for (i=0; i<10; i++) {
    printf ("YRAND(-7 +7)= %ld  \n", YRAND(imin, imax));    
  }

}
/*----------------------------------------------------------------------------*/
void 
rand_test ()
{

  double v, vmin=-10, vmax=7;

  YRAND_F;
  v = YRandF (vmin, vmax);
  printf ("v= %f  \n", v);

  YRAND_F;
  v = YRandF (vmin, vmax);
  printf ("v= %f  \n", v);

  return;
}
/*----------------------------------------------------------------------------*/
void
FOR_ALL_test ()
{
#define MAX_DIM 1000 

  int    d, dim;
  int    ijk[MAX_DIM], IJK[MAX_DIM]; // индекс и его граница

  dim = 3;
  for (d=0; d < dim; d++) {
    IJK[d] = 2; 
  }

  MF *mf = MFOR_create (dim, ijk, IJK);

  while (MFOR_todo (mf))  {
    MFOR_print_ijk (ijk, dim);
  }

  return;
}
/******************************************************************************/
int 
main (int argc, char** argv) 
{
  char *name;
  int   j;
  YT_PLOT *plot;

  printf ("\n");

  //-------------------------
  j = 1;
  name = argv[j++];
  //-------------------------

  if (!strcmp (name, "glib"))    glib_test ();
  if (!strcmp (name, "rand"))    rand_test ();
  if (!strcmp (name, "rand_0"))  rand_test_0 ();
  if (!strcmp (name, "rand_7"))  rand_test_7 ();
  if (!strcmp (name, "wint"))    win_test_old ();

  if (!strcmp (name, "fora"))    FOR_ALL_test ();


  if (!strcmp (name, "plot"))    plot_test ();
  if (!strcmp (name, "plot3"))  {
    plot = make_plot_test_3 ();
    plot_print (plot);
  }


  printf ("\n");
  return (0);
}
/*============================================================================*/

