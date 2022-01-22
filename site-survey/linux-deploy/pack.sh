#!/bin/sh
usage(){
        echo "Usage: `basename $0` \"exe file name \" \"exe file dir \" "
        echo "Example: ./`basename $0` ./tsi-linux-010 ./ "
        exit 1
}

today=`date  +%Y%m%d`
ctime=`date  +%H:%M:%S`


[ "$#" -ne "2" ] && usage
exe=$1 #你需要发布的程序名称，也就是上一步的可执行文件的文件名
des=$2 #步骤1中的目录即本文件所在目录
deplist=$(ldd $exe | awk '{if (match($3,"/")){ printf("%s "),$3 } }')
cp -f $deplist $des
export LD_LIBRARY_PATH=$des:$LD_LIBRARY_PATH

