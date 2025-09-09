package vm

import (
    "errors"
    "fmt"

    "github.com/PaulTKoenig/PgaQL_Backend/storage"
    "github.com/PaulTKoenig/PgaQL_Backend/compiler"
)

func Execute(instructions []compiler.Instruction) ([]map[string]interface{}, error) {

    var results []map[string]interface{}

    if len(instructions) == 0 || instructions[0].Op != compiler.OP_SCAN {
        return nil, errors.New("program must begin with SCAN")
    }

    table := instructions[0].Args[0].(string)
    store := storage.NewCSVStore(table)
    iter := store.Iterator()

    for iter.Next() {
        row := iter.Row()
        stack := []interface{}{}
        out := make(map[string]interface{})

        for _, instr := range instructions[1:] {
            switch instr.Op {
                case compiler.OP_SCAN:
                    continue // Already handled
                case compiler.OP_LOAD_FIELD:
                    col := instr.Args[0].(string)
                    val, exists := row[col]
                    if !exists {
                        return nil, fmt.Errorf("Field '%s' not found", col)
                    }
                    stack = append(stack, val)

                case compiler.OP_LOAD_CONST:
                    val := instr.Args[0]
                    stack = append(stack, val)

                case compiler.OP_EQ:
                    if len(stack) < 2 {
                        return nil, errors.New("EQ requires 2 operands")
                    }
                    b := stack[len(stack)-1]
                    a := stack[len(stack)-2]
                    stack = stack[:len(stack)-2]

                    stack = append(stack, a == b)

                case compiler.OP_AND:
                    if len(stack) < 2 {
                        return nil, errors.New("AND requires 2 boolean operands")
                    }
                    b := stack[len(stack)-1].(bool)
                    a := stack[len(stack)-2].(bool)
                    stack = stack[:len(stack)-2]
                    stack = append(stack, a && b)

                case compiler.OP_OR:
                    if len(stack) < 2 {
                        return nil, errors.New("OR requires 2 boolean operands")
                    }
                    b := stack[len(stack)-1].(bool)
                    a := stack[len(stack)-2].(bool)
                    stack = stack[:len(stack)-2]
                    stack = append(stack, a || b)

                case compiler.OP_FILTER:
                    if len(stack) < 1 {
                        return nil, errors.New("FILTER requires condition value")
                    }
                    cond := stack[len(stack)-1].(bool)
                    stack = stack[:len(stack)-1]
                    if !cond {
                        goto skipRow
                    }

                case compiler.OP_PROJECT:
                    for _, arg := range instr.Args {
                        col := arg.(string)
                        if val, exists := row[col]; exists {
                            out[col] = val
                        } else {
                            return nil, fmt.Errorf("Cannot project missing field '%s'", col)
                        }
                    }

                case compiler.OP_OUTPUT:
                    if out != nil {
                        results = append(results, out)
                    }

                default:
                    return nil, errors.New("unsupported instruction: " + instr.Op.String())
            }   
        }

    skipRow:
    }

    return results, nil
}