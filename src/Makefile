# =============================================================================# 
#                               M A K E F I L E
# =============================================================================# 

#GCC_o64   = gcc -D_OPT64
#GCC_p32   = gcc -malign-double -Wall

CC    = gcc -Wall -g -static $(GLIB_INCS) #-D_UNIX  
CPP   = c++ -O2
FLAGS = -I../

ICC_f11.ch70.chel.su  = icc #-openmp 
ICC_is7  = icc -D_OPT64 #-openmp -w0 -w1
ICC = $(ICC_$(HOSTNAME))

X_LIBS_f11.ch70.chel.su = -L/usr/X11R6/lib  -lX11 -lXext 
X_LIBS_is7 = -L/usr/X11R6/lib64 -lX11 -lXext 
X_LIBS = $(X_LIBS_$(HOSTNAME))

GLIB_INCS = `pkg-config --cflags glib-2.0`
GLIB_LIBS = `pkg-config --libs   glib-2.0`
GLIB_ALLS = `pkg-config --libs --cflags glib-2.0`

T = T

#-------------------------------------------------------------------------------
# =============================================================================#
A  = .

A_COMM_C = $(A)/a_comm.c
A_COMM_H = $(A)/a_comm.h
A_COMM_O = $(A)/T/a_comm.o

$(A_COMM_O): $(A_COMM_C) $(A_COMM_H)                              
	$(CC) $(FLAGS) -o $@ -c $(A_COMM_C) $(GLIB_INCS)                                

A_PLOT_C = $(A)/a_plot.c
A_PLOT_H = $(A)/a_plot.h
A_PLOT_N = $(A_COMM_H) $(A_PLOT_H) 
A_PLOT_O = $(A)/T/a_plot.o

$(A_PLOT_O): $(A_PLOT_C) $(A_PLOT_N)                              
	$(CC) $(FLAGS) -o $@ -c $(A_PLOT_C)                                

A_TEST_B = $(A)/a_test
A_TEST_C = $(A)/a_test.c

$(A_TEST_B): $(A_TEST_C) $(A_COMM_O) $(A_PLOT_O) #$(B_GNUP_O)
	$(CC) $(FLAGS) -o $@ $^ -lm 

# =============================================================================#

TXZ_LIB = $(A_PLOT_O) 

#-------------------------------------------------------------------------------
D  = .

B_FIND_C = $(D)/d_find.c
B_FIND_H = $(D)/d_find.h
B_FIND_N = $(A_COMM_H) $(B_TREE_H) $(B_FIND_H) 
B_FIND_O = $(D)/T/d_find.o   

$(B_FIND_O) : $(B_FIND_C) $(B_FIND_N) 
	$(CC) $(FLAGS) -o $@ -c $(B_FIND_C)

B_TREE_C = $(D)/d_tree.c
B_TREE_H = $(D)/d_tree.h
B_TREE_N = $(A_COMM_H) $(B_TREE_H) #a_draw.h 
B_TREE_O = $(D)/T/d_tree.o

$(B_TREE_O) : $(B_TREE_C) $(B_FIND_N)
	$(CC) $(FLAGS) -o $@ -c $(B_TREE_C) 

B_TEST_OBJS =  $(A_COMM_O) $(B_FIND_O) $(B_TREE_O) #$(B_UNTS_O)  $(B_SORT_O) 

B_TEST_B = $(D)/d~test
B_TEST_C = $(D)/d~test.c

$(B_TEST_B): $(B_TEST_C) $(B_TEST_OBJS)
	$(CC) $(FLAGS) -o $@ $^ -lm  

#-------------------------------------------------------------------------------
X  = .

X__TCL_C = $(X)/x_.tcl.c
X__TCL_N = $(A_COMM_H) $(B_FIND_H)  $(X_HASH_H) $(X_COMM_H) $(X_WORK_H) $(X_GAME_H)
X__TCL_O = $(X)/T/x_.tcl.so

X_TEST_B = $(X)/x~test
X_TEST_C = $(X)/x~test.c

X_BABY_B = $(X)/x_baby
X_BABY_C = $(X)/x_baby.c

X-SRCS = $(X_COMM_C) $(X_WORK_C) $(X_GAME_C) $(X_HASH_C) 
XO-SRC = $(A_COMM_C) $(B_TREE_C) $(B_FIND_C)  $(X-SRCS)  # a_draw.c  

#----------------------------------------------

X_COMM_C = $(X)/x_comm.c
X_COMM_H = $(X)/x_comm.h
X_COMM_N = $(A_COMM_H) $(X_COMM_H) $(X_HASH_H) $(X_WORK_H)
X_COMM_O = $(X)/T/x_comm.o

X_WORK_C = $(X)/x_work.c
X_WORK_H = $(X)/x_work.h
X_WORK_N = $(A_COMM_H) $(X_COMM_H) $(X_HASH_H) $(X_WORK_H)
X_WORK_O = $(X)/T/x_work.o

X_GAME_C = $(X)/x_game.c
X_GAME_H = $(X)/x_game.h
X_GAME_N = $(A_COMM_H) $(B_FIND_H) $(X_COMM_H) $(X_HASH_H) $(X_WORK_H) $(X_GAME_H)
X_GAME_O = $(X)/T/x_game.o

X_HASH_C = $(X)/x_hash.c
X_HASH_H = $(X)/x_hash.h
X_HASH_N = $(A_COMM_H) $(X_HASH_H)
X_HASH_O = $(X)/T/x_hash.o

$(X_COMM_O) : $(X_COMM_C) $(X_COMM_N)
	$(CC) $(FLAGS) -o $@ -c $(X_COMM_C) 
$(X_WORK_O) : $(X_WORK_C) $(X_WORK_N)    
	$(CC) $(FLAGS) -o $@ -c $(X_WORK_C)
$(X_GAME_O) : $(X_GAME_C) $(X_GAME_N)
	$(CC) $(FLAGS) -o $@ -c $(X_GAME_C) 
$(X_HASH_O) : $(X_HASH_C) $(X_HASH_N)
	$(CC) $(FLAGS) -o $@ -c $(X_HASH_C) 

$(X__TCL_O): $(X__TCL_C) $(XO-SRC) $(X__TCL_N)
	gcc $(FLAGS) -shared -fpic  -o $@ $(X__TCL_C) $(XO-SRC) -ltcl $(GLIB_ALLS)

X-OBJS = $(A_COMM_O) $(B_FIND_O) $(B_TREE_O) $(X_COMM_O) $(X_WORK_O) $(X_GAME_O) $(X_HASH_O)

$(X_TEST_B) : $(X_TEST_C) $(X-OBJS) 
	$(CC) $(FLAGS) -o $@ $^ -lm 
$(X_BABY_B) : $(X_BABY_C) $(X-OBJS) 
	$(CC) $(FLAGS) -o $@ $^ -lm 

# =============================================================================#


PROGS = $(A_TEST_B) $(B_TEST_B) $(X_BABY_B) $(X_TEST_B) $(X__TCL_O)  

all : $(PROGS) 

# =============================================================================#
clean :
	rm -f  T/*.o T/*.a $(PROGS) 

# =============================================================================#

SLIST = *.c *.h *.t *.txt *.tcl *.sh E/*.sh E/OUT Makefile*  L  

ANAME = scruzh
save :  
	tar --ignore-failed-read -cvf $(ANAME).tar $(SLIST)
	gzip -6 $(ANAME).tar  
	(cd ../../A/scruzh; mdat) 

# ============================================================================#
