package vm

import (
    "errors"
    "github.com/PaulTKoenig/PgaQL_Backend/storage"
)

// type Instruction struct {
//     Op   string
//     Arg  string
// }

// func Execute(bytecode []byte, store storage.Store) ([]map[string]interface{}, error) {
//     // For now: fake decode
//     instructions := decodeBytecode(bytecode)

//     var results []map[string]interface{}
//     iter := store.Iterator()

//     for iter.Next() {
//         row := iter.Row()

//         // Minimal: only handle SCAN + PROJECT
//         out := make(map[string]interface{})
//         for _, instr := range instructions {
//             switch instr.Op {
//             case "PROJECT":
//                 out[instr.Arg] = row[instr.Arg]
//             default:
//                 return nil, errors.New("unsupported instruction: " + instr.Op)
//             }
//         }
//         results = append(results, out)
//     }
//     return results, nil
// }

// func decodeBytecode(b []byte) []Instruction {
//     // TODO: real bytecode parser
//     return []Instruction{
//         {Op: "PROJECT", Arg: "player"},
//         {Op: "PROJECT", Arg: "points"},
//     }
// }
