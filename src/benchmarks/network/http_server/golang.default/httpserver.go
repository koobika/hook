package main

import (
    "fmt"
    "net/http"
)

func test(w http.ResponseWriter, req *http.Request) {
	w.Header().Set("Content-Type", "text/plain; charset=UTF-8")
	w.Header().Set("Server", "Example")
	w.Header().Set("Date", "Wed, 17 Apr 2013 12:00:00 GMT")
    fmt.Fprintf(w, "Hello, World!\r\n")
}

func main() {
    http.HandleFunc("/plaintext", test)
    http.ListenAndServe(":8080", nil)
}
