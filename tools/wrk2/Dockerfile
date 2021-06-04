FROM ubuntu:14.04

RUN apt-get update -y -q
RUN apt-get install -y git build-essential libssl-dev

RUN mkdir /opt/wrk2 && \
    git clone https://github.com/giltene/wrk2.git /opt/wrk2 && \
    cd /opt/wrk2 && \
    make

ENTRYPOINT ["/opt/wrk2/wrk"]