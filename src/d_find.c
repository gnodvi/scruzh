/*******************************************************************************
 *                                                                             *
 *  Имя этого файла: d_find.c                                                  *
 *                                                                             *
  ******************************************************************************
 */ 


#include "a_comm.h"

#include "d_find.h" 
#include "d_tree.h" 

// *****************************************************************************
// 
// TODO:
// 
// - хеш-таблица транспозиций (оценка и оптимизация размеров);
// - передавать вместе с лучшей оценкой также и номер уровня и использовать его;
// - альфа-бета отсечение (начиная с крайних double);
// 
// - сделать DBG_PRINT макросом как-то, чтобы не вызывать в холостую в релизах;
// - оптимизация для последовательной игры: запоминать уже сделанный перебор (по уровням)!
// 
// - возможно переход на нейронную (нечеткую) реализацию алгоритма перебора (как в мозге);
// - для обслуживания деревьев можно использовать общую библ-ку для графов;
// 
// - все изменения фиксировать хорошими тестами на дереве;
// 
/*----------------------------------------------------------------------------*/
                                                                            
typedef struct {
  TR_TREE *tree; // тестовое дерево
  TR_NODE *node; // текущая позиция
} SE_MAIN;

/*----------------------------------------------------------------------------*/
long  
se_copy_posi (/* long pstr, */ long posi)
{ 
  TR_NODE *old_posi = (TR_NODE *) posi;

  TR_NODE *new_posi = old_posi;
  // в нашем простом случае позиция описывается только указателем, т.е.
  // не надо создавать место в памяти и реально копировать структуру

  return ((long)new_posi); 
}
/*----------------------------------------------------------------------------*/
int 
se_evaluate (long user, long posi, int *p_numlev/* , int gamer_color */)
{ 
  //TR_TREE *t = (TR_TREE*) user; // главная структура юзера
  TR_NODE *n = (TR_NODE *) posi;

  // нужно знать текущую позицию - это плохо!
  //int   eval = (t->cur)->nval;
  TR_NVAL   eval = n->nval;
 
  // если значения нет, то можно вернуть номер уровня
  if (eval == NO) eval = 0 /* numlev */; 

  return (eval);
}
/******************************************************************************/
/*----------------------------------------------------------------------------*/
//#define NEW_TREE
//#ifdef  NEW_TREE
/*----------------------------------------------------------------------------*/
int  
se_find_moves_new (long pstr, long posi, long *p_moves, long find_mode)
{ 
  TR_NODE *n, *cur = (TR_NODE *) posi;

  TR_MOVE *move;
  int i, num=0;  // не с нуля, а с некоторого уровня!!

  for (i = 0; (n = cur->next_pstr[i]) != NULL; i++) {
    move = malloc (sizeof (TR_MOVE)); 

    //fprintf (stderr, "%s \n", n->name);
    move->node = n;  

    (p_moves[num++]) = (long) move; 
  }

  return (num);
}
/*----------------------------------------------------------------------------*/
//#else
/*----------------------------------------------------------------------------*/
/* int   */
/* se_find_moves (long pstr, long posi, long *p_moves) */
/* {  */
/*   TR_NODE *cur = (TR_NODE *) posi; */

/*   TR_MOVE *move  */
/*   int num=0;   */

/*   if (cur->next1) { */
/*     move = malloc (sizeof (TR_MOVE));  */
/*     move->node=cur->next1;   */
/*     (p_moves[num++]) = (long) move;  */
/*   } */

/*   if (cur->next2) { */
/*     move = malloc (sizeof (TR_MOVE));  */
/*     move->node=cur->next2;   */
/*     (p_moves[num++]) = (long) move;  */
/*   } */

/*   return (num); */
/* } */
/*----------------------------------------------------------------------------*/
//#endif
/*----------------------------------------------------------------------------*/
long  
se_make_move (long pstr, long old_posi, long move)
{ 
  //TR_TREE *t = (TR_TREE*) pstr;
  TR_MOVE *m = (TR_MOVE*) move;

  //t->cur = m->node;  // сделали ход (в дереве) - это лишнее!
  // а старая позиция в данном случае и не нужна!

  return ((long)(/* t->cur */m->node));  // вернули указатель на новую позицию
}
/*----------------------------------------------------------------------------*/
void  
se_undo_move (long pstr, long old_posi)
{ 
  //TR_TREE *t = (TR_TREE*) pstr;

  //TR_NODE *old_cur = (TR_NODE *) old_posi;

  // вернулись к запомненной старой позиции
  // используется "программный стек вызова функций"
  //cur = old_cur;
  //t->cur = old_cur;

  return;
}
/*----------------------------------------------------------------------------*/
char* 
se_name_move (/* long pstr, */ long move, long pstr_new)
{ 
  TR_TREE *tr = (TR_TREE *) pstr_new; 
  
  static char buffer[20];
  TR_MOVE *m;

  if (!move) return ("");

  m = (TR_MOVE*) move;

  //sprintf (buffer, "to_%s", (m->node)->name);
  sprintf (buffer, "%s to_%s", tr->name, (m->node)->name);

  return (buffer);
}
/*----------------------------------------------------------------------------*/
char* 
se_name_posi (long pstr, long posi)
{ 
  static char buffer[20];
  TR_NODE *n;

  if (!posi) return ("");

  n = (TR_NODE*) posi;

  //sprintf (buffer, "posi: %s", n->name);
  DBG_PRINT ("posi: %s  \n", n->name);

  return (buffer);
}
/******************************************************************************/
/*----------------------------------------------------------------------------*/
YT_MINMAX *
mm_create (MM_PROC_C copy_posi,
           MM_PROC_F find_moves, MM_PROC_E evaluate,
           MM_PROC_M make_move,  MM_PROC_U undo_move, MM_PROC_FP free_posi, 
           //int	(*end_of_game)(AB_POS *), 
           // необязательная, для отладки ..
           MM_PROC_N name_move,  MM_PROC_P name_posi, MM_PROC_MAP moves_map, 
           MM_PROC_H set_to_hashtable,
           int gamer_color/* , int maxply */
)
{
  YT_MINMAX *mm;

  mm = malloc (sizeof *mm); //??
  if (!mm) 
    Error ("mm_create");

  mm->copy_posi	  = copy_posi;
  mm->find_moves  = find_moves;
  mm->evaluate	  = evaluate;
  mm->make_move	  = make_move;
  mm->undo_move	  = undo_move;
  mm->free_posi	  = free_posi;

  mm->name_move	  = name_move;
  mm->name_posi	  = name_posi;
  mm->moves_map	  = moves_map;
  mm->set_to_hashtable = set_to_hashtable;

  mm->numlev      = -1;
  mm->posis_num   =  0;
  mm->gamer_color = gamer_color;

  return (mm);
}
/*----------------------------------------------------------------------------*/
void 
mm_init2 (YT_MINMAX *mm, int maxply, YT_BOOL is_alfa_beta/* , int alfa, int beta */)
{

  mm->maxply       = maxply;
  mm->is_alfa_beta = is_alfa_beta;

  //mm->alfa = alfa;
  //mm->beta = beta;

  return;
}
/*----------------------------------------------------------------------------*/
void 
mm_push_posi (YT_MINMAX *g, long p)
{ 

  g->posis[g->posis_num] = g->copy_posi (/* g->pstr, */ p);
  (g->posis_num)++;

}
/*----------------------------------------------------------------------------*/
long
mm_pop_posi (YT_MINMAX *g)
{ 

  (g->posis_num)--;
  return (g->posis[g->posis_num]);

}
/*----------------------------------------------------------------------------*/
void
free_moves (long moves [], int  num_moves, long bestmove)
{
  int   i;

  for (i = 0; i < num_moves; i++) {
    if (moves[i] == bestmove) continue;

    free ((void*) moves[i]);
  }

  return;
}
/*----------------------------------------------------------------------------*/
void
free_moves_all (long moves[], int num_moves)
{
  int   i;

  for (i = 0; i < num_moves; i++) {
    free ((void*) moves[i]);
  }

  return;
}
/******************************************************************************/
#define ISMY_MOVE(m) (!(m->numlev%2))
//
/*----------------------------------------------------------------------------*/
int 
mm_minimax (YT_MINMAX *m, int alfa, int beta, 
            long old_posi, long pstr,   
            long *p_bestmove, int *count_ends, int *p_levl /* в пару к eval - номер уровня */)
{ 
  int   i,  best_eval, eval, best_levl, levl;
  long  bestmove = 0;

  long moves [MM_MOVES_MAX];   // массив (указателей) возможных ходов
  int  evals [MM_MOVES_MAX];   // массив оценок для каждого хода (для хранения в базе)
  int  levls [MM_MOVES_MAX];   // массив уровней откуда подняты оценки
  int  num_moves; 

  DBG_PROC_BEG ("mm_minimax - INFO", TRUE);
  //if (m->name_posi) m->name_posi (pstr, old_posi); // DBG_PRINT
  DBG_PROC_BEG ("mm_minimax", TRUE);

  m->numlev++; // -1+1 = 0 уровень - я начинаю делать ходы и ищу максимальный

  // юзер генерирует список ходов
  num_moves = m->find_moves (pstr, old_posi, moves, /* find_mode */-10000);
 
  // по хорошему - хэштаблица должна быть здесь и сначала проверять ее
  eval = m->evaluate (pstr, old_posi, // оценивает позицию (как признак окончания игры) 
                      /* NULL */ &levl); 
                                      // обращаясь, если надо к хэш-таблице
  if (levl == -1000)  // не было значения в хэш-таблице
  levl = m->numlev;   // поэтому это не точно !!!!
  

  DBG_PRINT ("numlev   = %d (%d) \n", m->numlev, /* maxlev */ m->maxply);
  DBG_PRINT ("num_moves= %d \n", num_moves);

  if ( //-------- предварительная оценка: нужно ли идти дальше по дереву ? 
     (m->numlev == /* maxlev */m->maxply) ||  // достигли максималного уровня
     (num_moves == 0)      ||  // больше нет ходов  
     (eval >= MM_MAX_EVAL) ||  // это просто признак окончания игры (не альфа-бета)     
     (eval <= MM_MIN_EVAL)        
     ) 
  {  
    // посчитаем количество конечных позиций
    if (count_ends != NULL) (*count_ends)++;
    
    best_eval = eval;
    best_levl = levl /* m->numlev */;
    goto end;           // возвращаем оценку
  } //--------------------------------------------------------------------

  // чисто технический прием для начала поиска экстремумов в списке;
  if (ISMY_MOVE(m))  best_eval = MM_MIN_EVAL-1000 ; // на уровнях 0, 2, 4.. ищем максимум "для себя"
  else               best_eval = MM_MAX_EVAL+1000 ; // а на четных (после ходов соперника) - минимум
  best_levl = 0; //??

  for (i = 0; i < num_moves; i++) { //===================================
    DBG_PRINT ("move: %s \n", m->name_move (moves[i], pstr)); 

    long new_posi = m->make_move (pstr, old_posi, moves[i]);  // вносит изменения в позицию;
    if (m->name_posi) m->name_posi (pstr/* 0 */, new_posi); // DBG_PRINT

    // рекурсивно вызывает себя     
    eval = mm_minimax (m, alfa, beta, 
                       new_posi, pstr, NULL, count_ends, &levl);   
    DBG_PRINT ("minimax= % d \n", eval); 
    evals[i] = eval;
    levls[i] = levl - m->numlev; 

    // занесем найденную оценку для позиции в хэш-таблицу
    // если всей таблицы не хватит, то заносит надо первые уровни !!!!
    if (m->set_to_hashtable) m->set_to_hashtable (pstr, new_posi, eval, /* m->numlev */levl);

    // запоминает лучшую оценку
    // !bestmove  - в первый раз брать ход всегда
    if ( 
        (eval > best_eval &&  ISMY_MOVE(m)) || 
        (eval < best_eval && !ISMY_MOVE(m)) ||
        (eval== best_eval &&  (YRAND(0,100)<50 || !bestmove)) 
       ) 
    {
      best_eval = eval; 
      best_levl = levl; 
      bestmove = moves[i];
    }
    
    if (m->free_posi) m->free_posi (/* pstr */0, new_posi);
    //if (is_alfa_beta) {
    if (m->is_alfa_beta) {
    //if (is_alfa_beta > 0) {
      // скорректируем окошко для след. вызова минимакса
      if ( ISMY_MOVE(m)  &&  (best_eval > alfa))  alfa = best_eval;
      if (!ISMY_MOVE(m)  &&  (best_eval < beta))  beta = best_eval;

      // надо делать отсечение бесперспективных вариантов
      if (!ISMY_MOVE(m)  &&  (best_eval < alfa)) break;
      if ( ISMY_MOVE(m)  &&  (best_eval > beta)) break; // здесь потеря памяти!!!
    }

    //if (m->free_posi) m->free_posi (pstr, new_posi);
  } //================================================================

  // пусть пользователь напечатает "карту лучших ходов" для текущей позиции;
  if (m->moves_map) m->moves_map (/* pstr */0, old_posi, moves, evals, levls,  num_moves);

  end: 

  if (ISMY_MOVE(m)) DBG_PRINT ("BestMax= % d  Move= %s \n", best_eval, m->name_move (bestmove, pstr));
  else              DBG_PRINT ("BestMin= % d  Move= %s \n", best_eval, m->name_move (bestmove, pstr));  
                
  if (p_bestmove) {  // это только в самый первый вход в функцию!!
    // если вообще перебора не было - взять просто первый ход из списка!
    if (!bestmove) bestmove = moves[0]; 

    *p_bestmove = bestmove; // это указатель на структуру MOVE !!
  } else {
    free_moves_all (moves, num_moves);
  }

  m->numlev--;

  DBG_PROC_END ();
  if (count_ends) 
  DBG_PRINT ("CalcEnd= % d \n", *count_ends); 
  DBG_PROC_END ();

  if (p_levl) *p_levl = best_levl;
  return (best_eval);
}
/*----------------------------------------------------------------------------*/
//
/******************************************************************************/
void 
se_test_do (TR_NODE nodes[], int true_find, int true_nums, YT_BOOL is_alfa_beta) 
{
  int     alfa = MM_MIN_EVAL, beta = MM_MAX_EVAL;

  int   calc_nums = 0;
  int   maxply    = 10;
  int   calc_find;
  long  bestmove;

  TR_TREE *tr = tr_create (nodes);
  tr_rotate (tr->root);

  TR_NODE *n0 = tr->root; // будем искать лучший ход для начальной позиции

  YT_MINMAX *mm = mm_create (NULL, se_find_moves_new , 
                           se_evaluate, se_make_move, NULL, NULL,
                           se_name_move, se_name_posi, NULL, NULL, 0);

  mm_init2 (mm,  maxply, is_alfa_beta/* , alfa, beta */);

  calc_find = mm_minimax (mm, alfa, beta, 
                          (long)(n0), (long)(tr), &bestmove, &calc_nums, NULL);

  if ((calc_find==true_find) && (calc_nums==true_nums)) {
    printf (".......... OK \n");
  } else {
    printf ("ERROR: se_test_do: (T_FIND=%d C_FIND=%d) (T_NUMS=%d C_NUMS=%d) BESTMOVE=%s \n", 
             true_find, calc_find,  true_nums, calc_nums,
             se_name_move (bestmove, (long)tr));
  }

  return;
}
/*----------------------------------------------------------------------------*/
void 
minm_tests () 
{

  TR_NODE nodes_1[] = {

  //  +  -  +    =================================================
  {"A", NO, {"D","C","B", NULL}}, 


  {  "D", NO, {"K","J", NULL}}, 

  {     "K", NO, {"P","N",  NULL}},  
  {        "P",  3,  { NULL}},      // поэтому этот ход рассматривать он не будет уже!!     "X"  
  {        "N", 17,  { NULL}},      // этот мой ответ для соперника уже слишком

  {     "J", NO, {"M","L",  NULL}}, // <- max = 15 -  это мой лучший ход
  {        "M", 14,  { NULL}},  
  {        "L", 15,  { NULL}},  

  {  "C", NO, {"I","H","G", NULL}}, 

  {     "I",  4,  { NULL}},  // могу уже не рассматривать (хотя противник его бы сделал)    "X"   
  {     "H",  5,  { NULL}},  // этот уже не усторит: best_min < best_max (на пред. уровне)  
  {     "G", 10,  { NULL}},  // этот ход меня еще устроит

  {  "B", NO, {"F","E", NULL}}, 
  {     "F",  8,  { NULL}}, // лучший ответ на мой ход "B" - меня усторит 
  {     "E", 10,  { NULL}},  

       {NULL, 0, { NULL}},
  }; 
  // =============================================================

  SKIP_LINE;
  se_test_do (nodes_1,  /*8*/15 /*искомый узел*/, 9  /*проверенных узлов*/, /* FALSE */ FIND_SIMP_FULL); 
  se_test_do (nodes_1,  /*8*/15 /*искомый узел*/, 9-2/*проверенных узлов*/, /* TRUE */ FIND_BEST_FULL  /*alfa-beta*/); 
  SKIP_LINE;

  return;
}
/******************************************************************************/
