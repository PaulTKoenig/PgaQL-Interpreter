package api

import (
    "log"
    "net/http"
    "encoding/json"

    "github.com/PaulTKoenig/PgaQL_Backend/compiler"
    // "github.com/PaulTKoenig/PgaQL_Backend/vm"
)

type QueryRequest struct {
    Query string `json:"query"`
}

type QueryResponse struct {
    Data []compiler.Instruction `json:"data"`
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

    // // 2. Initialize storage (CSV backend for now)
    // store := storage.NewCSVStore("data/player_stats.csv")

    // // 3. Run VM with bytecode
    // results, err := vm.Execute(bytecode, store)
    // if err != nil {
    //     http.Error(w, err.Error(), http.StatusInternalServerError)
    //     return
    // }

	resp := QueryResponse{
	    Data: bytecode,
	}
	w.Header().Set("Content-Type", "application/json")
    json.NewEncoder(w).Encode(resp)
}
