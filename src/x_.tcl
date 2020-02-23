#!/bin/sh
# the next line restarts using tclsh \
exec wish "$0" "$@"

#-------------------------------------------------------------------------------
#set win [info exists env(WINDIR)]
#if $win {
#    set libname aigo.dll    ;# для Виндоса
#} else {
    set libname ./T/x_.tcl.so   ;# для Линукса
#}
#-----------------------------------------------------------------
# процедура загрузки библиотеки
#
proc loadlib {name initproc} {
    if {[info commands load] != ""} {
	load $name $initproc   ;# загружаем библиотеку
                                # и вызываем инициализирующую процедуру
    } else {
	dlopen $name init ${initproc}_Init
    }
}
#-------------------------------------------------------------------------------
proc paint_stone {w x0 y0 c_fill c_line t_player} {
 global size 
 global xmin xmax ymin ymax  xbeg ybeg xend yend
 
#  if {$x0 < $xmin} return
#  if {$x0 > $xmax} return
#  if {$y0 < $ymin} return
#  if {$y0 > $ymax} return

 set x0 [expr $x0]

 set siz [expr $size/2.0]

 $w create oval [expr $x0-$siz] [expr $y0-$siz] [expr $x0+$siz] [expr $y0+$siz] \
    -fill $c_fill -outline $c_line -tags {mv $t_player} 
}
#-------------------------------------------------------------------------------
proc paint_white_stone {w x0 y0} {

  paint_stone  $w $x0 $y0 white black player1
}
#-------------------------------------------------------------------------------
proc paint_black_stone {w x0 y0} {

  paint_stone  $w $x0 $y0 black black player2
}
#-------------------------------------------------------------------------------
proc index_to_coord {xbeg index} {
  global grid

  set x  [expr $xbeg + $grid * $index];

  return $x
}
#-------------------------------------------------------------------------------
proc coord_to_index {x0 x} {
  #int index, o;
  global grid

  set index [expr ($x - $x0) / $grid]
  set o     [expr ($x - $x0) % $grid]

  if {$o > $grid / 2} {incr index}

  #fprintf (stderr, "x0=%d x=%d index=%d \n", x0, x, index);
  return $index
}
#-------------------------------------------------------------------------------
proc draw_white_stone {i j} {
  global xbeg ybeg

  set x0  [index_to_coord  $xbeg $j]
  set y0  [index_to_coord  $ybeg $i]

  #puts "$x0 $y0"
  paint_white_stone .c $x0 $y0
}
#-------------------------------------------------------------------------------
proc draw_black_stone {i j} {
  global xbeg ybeg

  set x0  [index_to_coord  $xbeg $j]
  set y0  [index_to_coord  $ybeg $i]

  paint_black_stone .c $x0 $y0
}
#-------------------------------------------------------------------------------
proc reset {w} {
  global grid size colors title

  wm title . $title ;# ообнулили заголовок
  $w delete mv      ;# удалит уже созданные объекты-камни

  #aigo_init
}
#-------------------------------------------------------------------------------
proc board {grid colors linewidth} {
  global N 
  global xmin xmax ymin ymax  xbeg ybeg xend yend

  set c_fill [lindex $colors 0]
  set c_line [lindex $colors 1]

  # рисуем прямоугольник (как фон для гобана?)
  .c create rect $xmin $ymin $xmax $ymax -fill $c_fill

  set x $xbeg
  set y $ybeg
  for {set i 0} {$i < $N} {incr i} {  ;# create playfield:
      .c create line $x  $ybeg $x  $yend -fill $c_line -width $linewidth
      .c create line $xbeg $y  $xend $y  -fill $c_line -width $linewidth

     incr x $grid 
     incr y $grid
  }

  reset .c
}
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
proc mouse_click {x0_ckick y0_ckick} {
  global xbeg ybeg

  set j  [coord_to_index  $xbeg $x0_ckick]
  set i  [coord_to_index  $ybeg $y0_ckick]

  if {$i < 0} return
  if {$j < 0} return
  if {$i > 5} return
  if {$j > 5} return

  draw_black_stone  $i $j
  #winfo

  set ret [aigo_move $i $j]

  set arg_list [split $ret { }]
  set i [lindex $arg_list 0]
  set j [lindex $arg_list 1]

  draw_white_stone $i $j

}
#-------------------------------------------------------------------------------

set title "AtariGo-2.0"
set size 18   ;# определяет все остальные размеры - это размер камней?
set colors {beige brown white black} ;# 2 for board, 2 for men
set linewidth 2
set grid [expr int($size*1.2)]
set N 5

  # координаты самого гобана
  set xmin 20
  set ymin 50
  set xmax [expr $xmin + $grid*($N+1)]
  set ymax [expr $ymin + $grid*($N+1)]

  # координаты расчерченного поля
  set xbeg [expr $xmin + $grid]
  set ybeg [expr $ymin + $grid]
  set xend [expr $xbeg + $grid * ($N-1)]
  set yend [expr $ybeg + $grid * ($N-1)]

#---------------------------------------------
proc main_old {} {
  global grid size colors title linewidth N
  set W 185
  set H 220

  # создать поле графического вывода - задавая размеры, которые примитив канвы должен 
  # запросить у своего графического менеджера
  #canvas .c -width [expr $grid*($N+1)] -height [expr $grid*($N+1) + 2*$size] -background yellow
  canvas .c -width $W -height $H -background yellow
  pack   .c
  wm resizable . 0 0

  #bind . <1> {paint_white_stone .c %x %y}
  bind . <1> {mouse_click %x %y}
  bind . <3> {paint_black_stone .c %x %y}

  # рисуем гобан (с чашами для камней?)
  board $grid $colors $linewidth

  # создать кнопку
  button .c.b -text "New Game" -command {reset .c} -padx 0 -pady 0
  # выводим КНОПКУ в указанном положении на канве (по верхней центральной точке);
  #.c create window [expr $grid*10] [expr $grid*21-$size] -window .c.b -anchor n
  .c create window 55 20 -window .c.b 

  #draw_white_stone 2 2 
  #draw_black_stone 3 3 
}
#---------------------------------------------

loadlib $libname Demo

aigo_init $N $N

main_old 

#-------------------------------------------------------------------------------


