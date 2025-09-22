package execution

import (
	"fmt"

    "github.com/PaulTKoenig/PgaQL_Backend/compiler"
)

func Execute(bytecode []compiler.Instruction) ([]map[string]interface{}, error) {
	opTree, err := BuildOperatorTree(bytecode)
	if err != nil {
		panic(err)
	}

	var results []map[string]interface{}

	for {
		row, ok := opTree.Next()
		if !ok {
			break
		}
		results = append(results, row)
		fmt.Println(row)
	}
	return results, nil
}