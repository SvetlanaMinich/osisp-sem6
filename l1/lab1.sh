#!/bin/bash

start_dir=$(pwd)

function walk_through_cur_dir() {
    cur_dir=$1
    parameter_to_search=$2
    for p in $cur_dir/*
    do
        if [ -d "$p" ] 
        then
            walk_through_cur_dir $p $parameter_to_search
        elif [ -f "$p" ] 
        then
            relative_filename=$(basename -- "$p")
            if [[ $relative_filename == $parameter_to_search* ]] 
            then
                relative_path="./${p#$start_dir/}"
                echo "$relative_path"
            fi
        fi
    done
}

if [ $# -lt 1 ]
then
    echo "No search filename parameter is provided."
else
    for parameter in $@
    do
        echo "------- PARAMETER: $parameter -------"
        walk_through_cur_dir $start_dir $parameter
    done
fi