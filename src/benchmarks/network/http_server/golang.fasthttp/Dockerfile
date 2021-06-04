FROM golang:alpine as build

WORKDIR /src
COPY . .
ENV CGO_ENABLED=1
ENV GOOS=linux
ENV GOARCH=amd64

RUN go mod download github.com/klauspost/compress && \
	go mod download github.com/valyala/fasthttp && \
	go mod download github.com/andybalholm/brotli && \
	go mod download github.com/valyala/bytebufferpool 

RUN go build -o httpserver
