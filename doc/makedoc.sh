#!/bin/bash
###############################################################################
##                                                                           ##
##     Shell-script generates the source- code documentation of DocFsm       ##
##                                                                           ##
##---------------------------------------------------------------------------##
## File:    makedoc.sh                                                       ##
## Author:  Ulrich Becker                                                    ##
## Date:    21.03.2018                                                       ##
###############################################################################

FILE_LIST="df_attribute_reader.cpp df_preparser.cpp df_transition_finder.cpp"

check_installation()
{
   if [ ! -x "$(which $1)" ]
   then
     echo "ERROR: \"${1}\" seems not to be installed!" 1>&2
     exit 1
   fi
}

check_installation doxygen
check_installation dot

SRC_DIR="../src/"

DF_EXE=$(which docfsm)
if [ ! -x "$DF_EXE" ]
then
   DF_EXE="${SRC_DIR}docfsm"
   if [ ! -x "$DF_EXE" ]
   then
      check_installation cmake
      OLD_DIR=$(pwd)
      cd ${SRC_DIR}
      cmake .
      [ "$?" != "0" ] && exit 1
      make
      cd $OLD_DIR
   fi
fi

for i in $FILE_LIST
do
   $DF_EXE ${SRC_DIR}$i 1>"${i%cpp}gv"
done

doxygen

#=================================== EOF ======================================
