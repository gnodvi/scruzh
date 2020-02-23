/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: d_tree.c                                                  *
 *                                                                             *
  ******************************************************************************
 */    


#include "a_comm.h"
#include "d_tree.h" 

/******************************************************************************/

/*----------------------------------------------------------------------------*/
TR_TREE * 
tr_create_beg ()
{
  TR_TREE *t = (TR_TREE *) malloc (sizeof(TR_TREE));

  t->name = "TREE";

  return (t);
}
/*----------------------------------------------------------------------------*/
void
tr_init_from_txt (TR_TREE *tr, TR_NODE nodes[])
{
  TR_NODE  *n_beg, *n_end, *n;
  char **name_end;
  int    i;

  
  for (n=nodes; n->name; n++) {
    // n->pstr = n; // свой (этого узла) физический адрес
    n->prev = NULL; // "корректность" дерева: не больше одного предка
  }

  // каждый узел - возможно начало новой ветки !!
  for (n_beg=nodes; n_beg->name; n_beg++) { 

    i = 0;
    // идем по списку имен-указателей
    for (name_end=&(n_beg->next_name[0]); *name_end!=NULL; *name_end++) {

      // и снова сканируем весь список в поисках имени конца
      for (n_end=nodes; n_end->name; n_end++) { 

        if (!strcmp (n_end->name, *name_end)) { // нашли пару-ветку
          n_beg->next_pstr[i++] = /* n_end->pstr */n_end;
          if (n_end->prev != NULL) Error ("init_from_txt: Too many PREV");
          n_end->prev = n_beg; // возможно понадобится для чего-нибудь
        }
      }
    }
    n_beg->next_pstr[i]=NULL;
  }


  tr->root = tr->curn = &(nodes[0]);

  return;
}
/*----------------------------------------------------------------------------*/
TR_TREE *
tr_create (TR_NODE nodes[])
{
  TR_TREE *tr = tr_create_beg ();

  tr_init_from_txt (tr, nodes);
  
  return (tr);
}
/*----------------------------------------------------------------------------*/
void
tr_rotate (TR_NODE *cur)
{
  TR_NODE *n1, *n2, *n;
  int i, j, num;

  for (i = 0; (n = cur->next_pstr[i]) != NULL; i++) ;; // считаем количество веток
  num = i;

  for (i = 0; i < num; i++) {
    j = num-i-1;
    if (i >= j) break;

    n1 = cur->next_pstr[i];
    n2 = cur->next_pstr[j];

    cur->next_pstr[i] = n2;
    cur->next_pstr[j] = n1;
  }
  
  for (i = 0; (n = cur->next_pstr[i]) != NULL; i++)
    tr_rotate (n); 

  return;
}
/******************************************************************************/
/*----------------------------------------------------------------------------*/
void 
l_to_n (char *lines[], TR_NODE nodes[], int lev, int *nod) 
{

  lev++;
  
  //for (i = 0; (n = cur->next_pstr[i]) != NULL; i++)

  lev--;
  return;
}
/*----------------------------------------------------------------------------*/
void 
lines_to_nodes (char *lines[], TR_NODE nodes[], int *px, int y)
{
  char /* **pline, */ *line;

  //int x = *px;

  *px += 2;
   y  += 1;

  line = lines[y];

/*   if (line[*px+2] == '=') { */
/*     fprintf (stderr, "%s \n", line); */
/*     *px = x; */
/*     return; */

/*   } */

  printf ("Node: %c ", line[*px]);

  if (line[*px+2] == '=') {
    printf ("%c ", line[*px+4]);
  } else {
    printf ("NO ");
  }

  printf ("\n");
  

  //for (pline=lines+y+1; TRUE; *pline++) {
  //   if (*pline == NULL) break;
  //   line = *pline+x+2;
  //   if (line[0] == ' ') break;

    //fprintf (stderr, "%s \n", line);
    //lines_to_nodes (lines, nodes, &x, y+1);
  //}

  //x  -= 2;
  //*px = x;
  return;
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
TR_TREE *
tr_create_new (char *lines[])
{

  TR_NODE nodes[50];
  //TR_TREE *tr;
  int x = -2;

  lines_to_nodes (lines, nodes,   &x, -1  /* 0, 0  */  /* 2, 1 */);

  //tr = tr_create (nodes);  
  //tr_rotate (tr->root);

  return (/* tr */NULL);
}
/******************************************************************************/
/*----------------------------------------------------------------------------*/
void 
tree_draw_simp_iter (TR_NODE *cur, int level) 
{
  TR_NODE *n;
  int      i, num;
  char     margin[100];

  level++;
  strcpy (margin, "                                          ");
  margin[2*level] = '\0';

  printf ("%s%s", margin, cur->name);
  if (cur->nval != NO)
  printf (" = %d ", cur->nval);
  printf ("\n");

  for (i = 0; (n = cur->next_pstr[i]) != NULL; i++) ;; // считаем количество веток
  num = i;

  for (i = num-1; i>=0; i--) {
    n = cur->next_pstr[i];
    tree_draw_simp_iter (n, level); 
  }


  level--;
  return;
}
/*----------------------------------------------------------------------------*/
void 
tree_draw_simp (TR_TREE *tr) 
{
  int level = -1;

  printf ("----- Tree Draw Simp ----- \n");

  tree_draw_simp_iter (tr->root, level); 

  printf ("-------------------------- \n");
  
  return;
}
/*----------------------------------------------------------------------------*/
void 
tree_tests () 
{
/******************************************************************************/
//                    _________A__________                 
//                    |                  |
//               _____B_____       ______C______            
//               |         |       |           |
//              D=7       E=6     F=10        G=9          
//                                             ^
/******************************************************************************/
  TR_NODE nodes[] = {
    {"A",      NO,  {"C", "B", NULL}}, 
    {  "C",    NO,  {"G", "F", NULL}}, 
    {    "G",   9,  { NULL}}, 
    {    "F",  10,  { NULL}}, 
    {  "B",    NO,  {"E", "D", "H", NULL}}, 
    {    "E",   6,  { NULL}}, 
    {    "D",   7,  { NULL}}, 
    //    {    "H",   5,  { NULL}}, 
    {NULL, 0,  { NULL}},
  };

  //{    "G",  NO,  { "B", NULL}}, // тест кооректности дерева

/*   char *lines[] = { */
/*     "A            ",  */
/*     "  C          ",  */
/*     "    G = 9    ",  */
/*     "    F = 10   ",  */
/*     "  B          ",  */
/*     "    E = 6    ",  */
/*     "    D = 7    ",  */
/*     NULL, */
/*   }; */

  TR_TREE *tr = tr_create (nodes);
  tr_rotate (tr->root);

  //TR_TREE *tr = tr_create_new (lines);

  SKIP_LINE;   
  tree_draw_simp (tr); 


  return;
}
/*----------------------------------------------------------------------------*/
/******************************************************************************/

/******************************************************************************/

