package api

import (
    "log"
    "net/http"
    "encoding/json"

    "github.com/PaulTKoenig/PgaQL_Backend/compiler"
    "github.com/PaulTKoenig/PgaQL_Backend/vm"
    "github.com/PaulTKoenig/PgaQL_Backend/execution"
)

type QueryRequest struct {
    Query string `json:"query"`
}

type QueryResponse struct {
    Data []map[string]interface{} `json:"data"`
}

func HandleQuery(w http.ResponseWriter, r *http.Request) {
    
    queryParams := r.URL.Query()
    queryString := queryParams.Get("query_string")

    if (queryString == "") {
        http.Error(w, "missing required query parameter: query_string", http.StatusBadRequest)
        return
    }

    log.Println(queryString)

    bytecode, err := compiler.Compile(queryString)
    if err != nil {
        http.Error(w, err.Error(), http.StatusInternalServerError)
        return
    }

    execution.Execute(bytecode)


    results, err := vm.Execute(bytecode)
    if err != nil {
        http.Error(w, err.Error(), http.StatusInternalServerError)
        return
    }

	resp := QueryResponse{
	    Data: results,
	}
	w.Header().Set("Content-Type", "application/json")
    json.NewEncoder(w).Encode(resp)
}
