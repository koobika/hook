FROM golang:alpine as build

WORKDIR /src
COPY . .
ENV CGO_ENABLED=1
ENV GOOS=linux
ENV GOARCH=amd64

RUN go build -o httpserver
