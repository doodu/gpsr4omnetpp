#!/bin/bash

for i in Simple*
do
echo $i

# 修改文件
sed "s/Simple\(NetwLayer\)/GPSR\1/g" $i > temp
mv temp $i
sed "s/SIMPLE/GPSR/g" $i > temp
mv temp $i

mv $i `echo $i | sed 's/Simple/GPSR/g' -` # 修改文件名
done