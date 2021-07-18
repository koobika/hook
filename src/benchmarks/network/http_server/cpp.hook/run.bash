#!/usr/bin/env bash

if [[ ! -f ./httpserver ]]
then
    echo "You need to build the project first with koo."
fi

echo "Running app in background"
nohup ./httpserver &
sleep 2
echo "Running test"
docker run --rm -it \
    --log-driver none --net host \
    tools/wrk2 -t16 -c256 -d120s -R1000000 http://localhost:8080/plaintext > result.txt 2>&1
killall httpserver
rm nohup.out
echo "Benchmark finish! take a look to result.txt"