#!/bin/bash

function random()
{
    min=$1
    max=$(($2-$min+1))
    num=$(date +%s%N)
    echo $(($num%$max+$min))
}

basedir=`cd $(dirname $0); pwd -P`
label_file="label.txt"
train_file="train.txt"
test_file="test.txt"
train_num=$2

if [ $# = 2 ];then
    # rm old file
    if [ -f $label_file ];then
        rm $label_file
    fi

    if [ -f $train_file ];then
        rm $train_file
    fi

    if [ -f $test_file ];then
        rm $test_file
    fi
    # end rm old file

    label=0
    cd $1
    for file in *;do
            if [ -d $file ];then
                count=`ls $file | wc -w`
                if [ $count -gt 2 ];then
                    for image in `ls $file`;do
                        num=$(random 0 $train_num)
                        if [ $num = 0 ];then
                            echo $basedir"/"$1"/"$file"/"$image $label>> $basedir"/"$test_file
                            echo "test: "$basedir"/"$1"/"$file"/"$image $label
                        else
                            echo $basedir"/"$1"/"$file"/"$image $label>> $basedir"/"$train_file
                            echo "train: "$basedir"/"$1"/"$file"/"$image $label
                        fi
                        done
                    echo $label ':' $file >> $basedir"/"$label_file
                    label=`expr $label + 1`
                fi
            fi
        done
else
    echo "./create_dataset.sh path train_num"
fi