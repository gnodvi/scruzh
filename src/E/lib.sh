#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------

#DOTSIM=',' # ����������� ���������� � ����� ��������� �����
#TMP_DIR=~/tmp
TMP_DIR=./T


#-------------------------------------------------------------------------------
tst () 
{

  LL=$1   # ������� ��������� �������
  OUT=$2
  DIR_CMD=$3
  CMD=$4  # ���� ������� (�������)

  CMD_TRUE=$( echo ${CMD//'^'/'/'} ) 
  #echo "$CMD_TRUE"

  # ��������� ��� �����, ������� ������� ������������� 
  T_FILE=$( echo ${CMD// /,} ) 

  N_FILE=$( echo "$LL:$T_FILE" ) 
  echo "$LL: $CMD ..."

  eval ${DIR_CMD}/$CMD_TRUE > $OUT/$N_FILE  2> /dev/null
  # eval $CMD &> $OUT/$N_FILE 

}
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
run_tests () 
{

  LEVEL=$1
  DIR_TST=$2
  DIR_CMD=$3

for TRUE_FILE in ${DIR_TST}/*               # ���� �� ���� ������ � ��������� ����������
  do
    T_FILE=`basename $TRUE_FILE`       # �������� ��� ����� �� �����

#    LL=`echo "$T_FILE" | awk 'BEGIN{FS=":"} {print $1}'` # �������� ����� ������    
#    RR=`echo "$T_FILE" | awk 'BEGIN{FS=":"} {print $2}'` # �� ��������������� �������

    LL=`echo "$T_FILE" | awk 'BEGIN{FS="%"} {print $1}'` # �������� ����� ������    
    RR=`echo "$T_FILE" | awk 'BEGIN{FS="%"} {print $2}'` # �� ��������������� �������

    # ���� �� ��� �������, �� ���������� ��������� ..
    if [ "$LL" != "$LEVEL" ]
    then
        continue
    fi

    echo -n "$T_FILE ... "  # �������� ���������� ���

    # � ��������� �������, ������� ��� ����������� ���������:
    # CMD=$( echo ${RR//$DOTSIM/ } )  # ������ �� F11 : bad substitution
    # CMD=`echo $RR | sed -e "s/,/ /g"` # � ��� ��������!

    #--------------!!!!!
    CMD=$( echo ${RR//'^'/'/'} )   # ������� (!) ��������� ����-������� ����������
    CMD=$( echo ${CMD//','/' '} )  # � ����� ����-������� ��������
    #--------------!!!!!

    # ���������� $$ ������ id �������� ��������
    #CALC_FILE=$TMP_DIR/$$.$T_FILE.calc   # ��� ���������� ��������� ����� 
    #DIFF_FILE=$TMP_DIR/$$.$T_FILE.diff   # ��� ���������� ����� ��������� 
    CALC_FILE=$TMP_DIR/a_CALC   # ��� ���������� ��������� ����� 
    DIFF_FILE=$TMP_DIR/a_DIFF   # ��� ���������� ����� ��������� 
                                  
    # ��������� �������, ������������� ������ ����� �� ��������� ����
    # &>  �������������� ��� ������ �.�. stdout + stderr
    #eval $CMD &> $CALC_FILE

    #echo ${DIR_CMD}/$CMD
    #exit
    eval ${DIR_CMD}/$CMD > $CALC_FILE  2> /dev/null

    # --------- ������ ���� ������� ��������� ����� � ������� ������ {}
    #CALC_FILE_SED=$CALC_FILE.sed
    #TRUE_FILE_SED=$TMP_DIR/$$.$T_FILE.true.sed
    CALC_FILE_SED=$TMP_DIR/a_CALC_SED
    TRUE_FILE_SED=$TMP_DIR/a_TRUE_SED

    #cp $CALC_FILE $CALC_FILE_SED
    #cp $TRUE_FILE $TRUE_FILE_SED

    sed -e 's/{[^ ]*}/{-----}/g' $CALC_FILE > $CALC_FILE_SED
    sed -e 's/{[^ ]*}/{-----}/g' $TRUE_FILE > $TRUE_FILE_SED

    # --ignore-all-space --ignore-blank-lines --ignore-space-change --minimal
    #diff  $TRUE_FILE $CALC_FILE  &>  $DIFF_FILE 
    #diff --ignore-all-space  $TRUE_FILE_SED  $CALC_FILE_SED  &>  $DIFF_FILE 
    diff --ignore-all-space  $TRUE_FILE_SED  $CALC_FILE_SED  1> $DIFF_FILE 2>&1

    # $?  �������� ���� �������� ������� (�������, �������)
    if [ $? -eq 1 ]  # ���������� ������� ��������� ���������� ������������
    then
      echo "" 
      echo "------------------------------------------------------DIFF-----------" 
      more $DIFF_FILE
      echo "---------------------------------------------------------------------" 
      echo "" 
      #
      # -s     ��������� ���-�����
      # -n N   ���� ����������� ������������� ����� N-�� �������
      # -p     ������ ��� ������ ���������
      #
      # read -s -n1 -p "FAILED: do you wish to overwrite (y or [n]) ? " keypress
      # �� �� �������� ��  F11 !

      echo "FAILED: do you wish to overwrite (y or [n]) ? ";
      read keypress; # � ��� �������� � �������� �������� "������� � ������������"!

      echo; 
 
      case "$keypress" in
      "Y" | "y" )
         cp $CALC_FILE $TRUE_FILE # ��������� ����� ����� ��� �������� !
         echo "YES   : overwrite CALC -> TRUE !!"
      ;;
              * )
         echo "NOT   : continue with this CALC !"
      esac

      echo    
    else
      echo "PASSED"  # ���� ���� ������
    fi

    rm $CALC_FILE  # ������� ��������� ��������� ����
    rm $DIFF_FILE  # 
    rm $CALC_FILE_SED
    rm $TRUE_FILE_SED
  done

}
#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------



