package execution

import (
	"errors"
	"fmt"

    "github.com/PaulTKoenig/PgaQL_Backend/compiler"
    "github.com/PaulTKoenig/PgaQL_Backend/storage"
)

func BuildOperatorTree(instructions []compiler.Instruction) (Operator, error) {
	var child Operator	
	var pendingStackOps []compiler.Instruction

	for _, instr := range instructions {
        switch instr.Op {
            case compiler.OP_SCAN:
            	table := instr.Args[0].(string)
    			store := storage.NewCSVStore(table)
    			iter := store.Iterator()
            	child = &Scan{iter: iter}
            case compiler.OP_LOAD_FIELD, compiler.OP_LOAD_CONST,
				compiler.OP_EQ, compiler.OP_AND, compiler.OP_OR:
				pendingStackOps = append(pendingStackOps, instr)

            case compiler.OP_FILTER:
            	pred := buildPredicateFunc(pendingStackOps)
            	child = &Filter{child: child, pred: pred}
            	pendingStackOps = nil

            case compiler.OP_GROUP_BY:
            	groupField := instr.Args[0].(string)
            	child = &GroupBy{child: child, groupField: groupField, aggs: make(map[string]AggFunc)}

            case compiler.OP_AGG_AVG:
				field := instr.Args[0].(string)
				alias := instr.Args[0].(string) + "_avg"
				if gb, ok := child.(*GroupBy); ok {
					gb.aggs[alias] = Avg(field)
				} else {
					return nil, errors.New("AGG_AVG must follow a GROUP_BY")
				}

            case compiler.OP_PROJECT:
            	fields := []string{}
        		for _, arg := range instr.Args {
                    fields = append(fields, arg.(string))
                }
                child = &Project{child: child, fields: fields}

            case compiler.OP_OUTPUT:
            	return child, nil

            default:
                return nil, errors.New("unsupported instruction while building operator tree: " + instr.Op.String())
        }   
    }

    return child, nil
}

// func buildPredicateFunc(stackOps []compiler.Instruction) func(Row) bool {
// 	return func(row Row) bool {
//         stack := []interface{}{}

// 		for _, instr := range stackOps {
//             switch instr.Op {
//                 case compiler.OP_LOAD_FIELD:
//                     col := instr.Args[0].(string)
//                     val, exists := row[col]
//                     if !exists {
//                         return nil, fmt.Errorf("Field '%s' not found", col)
//                     }
//                     stack = append(stack, val)

//                 case compiler.OP_LOAD_CONST:
//                     val := instr.Args[0]
//                     stack = append(stack, val)

//                 case compiler.OP_EQ:
//                     if len(stack) < 2 {
//                         return nil, errors.New("EQ requires 2 operands")
//                     }
//                     b := stack[len(stack)-1]
//                     a := stack[len(stack)-2]
//                     stack = stack[:len(stack)-2]

//                     stack = append(stack, a == b)

//                 case compiler.OP_AND:
//                     if len(stack) < 2 {
//                         return nil, errors.New("AND requires 2 boolean operands")
//                     }
//                     b := stack[len(stack)-1].(bool)
//                     a := stack[len(stack)-2].(bool)
//                     stack = stack[:len(stack)-2]
//                     stack = append(stack, a && b)

//                 case compiler.OP_OR:
//                     if len(stack) < 2 {
//                         return nil, errors.New("OR requires 2 boolean operands")
//                     }
//                     b := stack[len(stack)-1].(bool)
//                     a := stack[len(stack)-2].(bool)
//                     stack = stack[:len(stack)-2]
//                     stack = append(stack, a || b)

//                 default:
//                     return nil, errors.New("unsupported instruction: " + instr.Op.String())
//             }   
//         }

//         return stack[0].(bool)
// 	}
// }

func buildPredicateFunc(stackOps []compiler.Instruction) func(Row) bool {
    return func(row Row) bool {
        stack := []interface{}{}

        for _, instr := range stackOps {
            switch instr.Op {
            case compiler.OP_LOAD_FIELD:
                col := instr.Args[0].(string)
                val, exists := row[col]
                if !exists {
                    panic(fmt.Sprintf("Field '%s' not found", col))
                }
                stack = append(stack, val)

            case compiler.OP_LOAD_CONST:
                val := instr.Args[0]
                stack = append(stack, val)

            case compiler.OP_EQ:
                if len(stack) < 2 {
                    panic("EQ requires 2 operands")
                }
                b := stack[len(stack)-1]
                a := stack[len(stack)-2]
                stack = stack[:len(stack)-2]
                stack = append(stack, a == b)

            case compiler.OP_AND:
                if len(stack) < 2 {
                    panic("AND requires 2 boolean operands")
                }
                b := stack[len(stack)-1].(bool)
                a := stack[len(stack)-2].(bool)
                stack = stack[:len(stack)-2]
                stack = append(stack, a && b)

            case compiler.OP_OR:
                if len(stack) < 2 {
                    panic("OR requires 2 boolean operands")
                }
                b := stack[len(stack)-1].(bool)
                a := stack[len(stack)-2].(bool)
                stack = stack[:len(stack)-2]
                stack = append(stack, a || b)

            default:
                panic("unsupported instruction: " + instr.Op.String())
            }
        }

        return stack[0].(bool)
    }
}



