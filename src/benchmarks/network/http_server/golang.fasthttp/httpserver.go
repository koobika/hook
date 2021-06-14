package main

import (
	"flag"
	"fmt"
	"log"
	"github.com/valyala/fasthttp"
)

var (
	addr     = flag.String("plaintext", ":8080", "TCP address to listen to")
)

func main() {
	flag.Parse()
	h := requestHandler
	if err := fasthttp.ListenAndServe(*addr, h); err != nil {
		log.Fatalf("Error in ListenAndServe: %s", err)
	}
}

func requestHandler(ctx *fasthttp.RequestCtx) {
	ctx.SetContentType("text/plain; charset=UTF-8")
	ctx.Response.Header.Set("Server", "Example")
	ctx.Response.Header.Set("Date", "Wed, 17 Apr 2013 12:00:00 GMT")
	fmt.Fprintf(ctx, "Hello, World!\r\n")
}
