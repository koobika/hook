#!/usr/bin/env bash

if [[ ! -f httpserver ]]
then
    echo "Building app"
    docker build -t build/go-fasthttp .
    docker create --name go-fasthttp build/go-fasthttp
    docker cp go-fasthttp:/src/httpserver .
    docker rm go-fasthttp
fi

echo "Running app in background"
nohup ./httpserver &
sleep 2
echo "Running test"
docker run --rm -it \
    --log-driver none --net host \
    tool/wrk2 -t16 -c256 -d120s -R1000000 http://localhost:8542/foo/bar > result.txt 2>&1
killall httpserver
rm nohup.out
echo "Benchmark finish! take a look to result.txt"