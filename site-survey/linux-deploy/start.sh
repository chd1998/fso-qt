#!/bin/sh
# should be the same  with your executable qt progame
#appname=`basename $0 | sed s,\.sh$,,
usage(){
        echo "Usage: `basename $0` \"exe file name \"  "
        echo "Example: ./`basename $0` ./tsi-linux-010 "
        exit 1
}

today=`date  +%Y%m%d`
ctime=`date  +%H:%M:%S`
[ "$#" -ne "1" ] && usage
appname=$1
cd `dirname $1`
mydir=`pwd`
export LD_LIBRARY_PATH=$mydir/lib:LD_LIBRARY_PATH
#export LD_LIBRARY_PATH
#$@ - original arguments
$mydir/$appname "$@"

