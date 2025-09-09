package compiler

import (
	"fmt"
)

type OpCode int

const (
    OP_SCAN OpCode = 0
    OP_LOAD_FIELD OpCode = 1
    OP_LOAD_CONST OpCode = 2
    OP_EQ OpCode = 3
    OP_AND OpCode = 4
    OP_OR OpCode = 5
    OP_FILTER OpCode = 6
    OP_PROJECT OpCode = 7
    OP_OUTPUT OpCode = 8
)

func (op OpCode) String() string {
    switch op {
    case OP_SCAN:
        return "OP_SCAN"
    case OP_LOAD_FIELD:
        return "OP_LOAD_FIELD"
    case OP_LOAD_CONST:
        return "OP_LOAD_CONST"
    case OP_EQ:
        return "OP_EQ"
    case OP_AND:
        return "OP_AND"
    case OP_OR:
        return "OP_OR"
    case OP_FILTER:
        return "OP_FILTER"
    case OP_PROJECT:
        return "OP_PROJECT"
    case OP_OUTPUT:
        return "OP_OUTPUT"
    default:
        return fmt.Sprintf("UNKNOWN(%d)", int(op))
    }
}
