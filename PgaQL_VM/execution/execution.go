package execution

import (
	"fmt"

    "github.com/PaulTKoenig/PgaQL_Backend/compiler"
)

func Execute(bytecode []compiler.Instruction) () {
	opTree, err := BuildOperatorTree(bytecode)
	if err != nil {
		panic(err)
	}

	for {
		row, ok := opTree.Next()
		if !ok {
			break
		}
		fmt.Println(row)
	}
}