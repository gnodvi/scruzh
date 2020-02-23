#!/bin/sh
#
################################################################################

# подцепить библиотеку "тестовых" утилит
source ./lib.sh

# перейти в корневую директорию для выполнения заданий
cd ..

echo ""
#-------------------------------------------------------------------------------
# tst  K1 E/OUT . "k~test rsb_min 2010 1"
# tst  K2 E/OUT . "k~test rsb_big 2010 1"

#-------------------------------------------------------------------------------

 tst D1 E/OUT . "d~test tree"

#-------------------------------------------------------------------------------

# tst X1 E/OUT . "x_baby"
# tst X1 E/OUT . "x~test"

# tst X1 E/OUT . "x~test 1"
# tst X1 E/OUT . "x~test 2 aigo2"
# tst X1 E/OUT . "x~test 2 migo2"
# tst X1 E/OUT . "x~test 2 baby2"
# tst X1 E/OUT . "x~test @ 2 aigo2"
# tst X1 E/OUT . "x~test 3 aigo2 aigo2 3 3"
# tst X1 E/OUT . "x~test @ 3 aigo2 aigo2 1 3"
# tst X1 E/OUT . "x~test 5 aigo 2 2 100  2"
# tst X1 E/OUT . "x~test 7  1 3  100"  
# tst X1 E/OUT . "x~test 7  1 4  100"  
# tst X1 E/OUT . "x~test 7  1 5  100"  
# tst X1 E/OUT . "x~test 9  1 3"
# tst X1 E/OUT . "x~test 9  1 4"
# tst X1 E/OUT . "x~test 8  1 5"
# tst X1 E/OUT . "x~test 10  1 3 -test"
# tst X1 E/OUT . "x~test 11"
# tst X1 E/OUT . "x~test 12"

#-------------------------------------------------------------------------------
echo ""
#-------------------------------------------------------------------------------

