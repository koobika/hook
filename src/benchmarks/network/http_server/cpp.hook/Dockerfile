FROM ubuntu

RUN apt-get update && apt-get -y install build-essential

WORKDIR /src
COPY output/httpserver .
RUN chmod +x httpserver
EXPOSE 8542
ENTRYPOINT ["/src/httpserver"]
