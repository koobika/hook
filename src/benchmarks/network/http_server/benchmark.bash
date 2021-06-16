#!/usr/bin/env bash

if ! command -v docker &> /dev/null
then
    echo "Benchmarks requires docker for auto-run"
    exit 1
fi

mkdir -p ../../../../doc/benchmark/current

# Build the wrk2 tool if not present into the system
if [[ "$(docker images -q tools/wrk2 2> /dev/null)" == "" ]];  then
    docker build -t tools/wrk2 ../../../../tools/wrk2
fi

for folder in */ ; do
    echo "Benchark for $folder"
    cd $folder
    if [[ -f run.bash ]]
    then
        bash run.bash
        mv result.txt ../../../../../doc/benchmark/current/`basename $folder`.txt
    fi
    cd ..
done
