#!/bin/bash

start_dir=$(pwd)

function walk_through_cur_dir() {
    cur_dir=$1
    parameter_to_search=$2
    header_string=$3
    for p in $cur_dir/*
    do
        if [ -d "$p" ] 
        then
            walk_through_cur_dir $p $parameter_to_search "$header_string"
        elif [ -f "$p" ] 
        then
            relative_filename=$(basename -- "$p")
            if [[ $relative_filename == $parameter_to_search* ]] 
            then
                relative_path="./${p#$start_dir/}"
                echo "$relative_path"

                first_line=$(head -n 1 "$p")
                if [[ "$first_line" == "$header_string" ]]
                then
                    echo "---- File: $relative_path ----"
                    nl "$p"
                    echo "------------------------------"
                fi
            fi
        fi
    done
}


# $0 - sh script name, $1 - header to search, $2-... - filename(s)
if [ $# -lt 1 ]
then
    echo "No parameters are provided."
elif [ $# -lt 2 ]
then
    echo "No search filename parameter is provided."
else
    header=$1 
    shift        # Сдвигаем аргументы, чтобы $2, $3, ... стали $1, $2, ...
    for parameter in $@
    do
        echo "------- PARAMETER: $parameter -------"
        walk_through_cur_dir $start_dir $parameter "$header"
    done
fi