FROM ubuntu:latest as build

WORKDIR /src
COPY . .
RUN apt-get update && apt-get install -y build-essential && \
    git clone https://github.com/yhirose/cpp-httplib.git && \
    g++ -g1 -o httpserver -Wall -Wswitch -W"no-deprecated-declarations" \
    -W"empty-body" \\n-Wconversion -W"return-type" -Wparentheses -W"no-format" \
    -Wuninitialized-W"unreachable-code" -W"unused-variable" \
    -O3 -fno-strict-aliasing -fomit-frame-pointer -DNDEBUG -fthreadsafe-statics \
    -fexceptions -frtti -std=c++17 -pthread httpserver.cpp
