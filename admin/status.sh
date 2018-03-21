#!/bin/bash
# It counts ToBePorted tags in source tree and
# prints file names with number of ocurrences

C1="TB"
C2="P"

for D in `find . -type f`
do
    NM=`grep "$C1$C2" $D | wc -l`
    if [ $NM -ne 0 ] ; then echo $D $NM; fi
done


