package main

import (
    "log"
    "net/http"

    "github.com/PaulTKoenig/PgaQL_Backend/api"
)

func main() {
    mux := http.NewServeMux()
    mux.Handle("GET /query", http.HandlerFunc(api.HandleQuery))

    log.Println("Server running on :8080")
    if err := http.ListenAndServe(":8080", mux); err != nil {
        log.Fatal(err)
    }
}
