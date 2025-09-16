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
    OP_FILTER OpCode = 5
    OP_PROJECT OpCode = 6
    OP_OUTPUT OpCode = 7
    OP_GROUP_BY OpCode = 8
    OP_AGG_AVG OpCode = 9
    OP_OR OpCode = 10
    OP_AGG_SUM OpCode = 11
    OP_AGG_MAX OpCode = 12
    OP_AGG_MIN OpCode = 13
    OP_AGG_COUNT OpCode = 14
    OP_SORT OpCode = 15
    OP_LIMIT OpCode = 16
    OP_ADD OpCode = 17
    OP_SUB OpCode = 18
    OP_MUL OpCode = 19
    OP_DIV OpCode = 20
    OP_DEFINE_FIELD OpCode = 21
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
    case OP_FILTER:
        return "OP_FILTER"
    case OP_PROJECT:
        return "OP_PROJECT"
    case OP_OUTPUT:
        return "OP_OUTPUT"
    case OP_GROUP_BY:
        return "OP_GROUP_BY"
    case OP_AGG_AVG:
        return "OP_AGG_AVG"
    case OP_OR:
        return "OP_OR"
    case OP_AGG_SUM:
        return "OP_AGG_SUM"
    case OP_AGG_MAX:
        return "OP_AGG_MAX"
    case OP_AGG_MIN:
        return "OP_AGG_MIN"
    case OP_AGG_COUNT:
        return "OP_AGG_COUNT"
    case OP_SORT:
        return "OP_SORT"
    case OP_LIMIT:
        return "OP_LIMIT"
    case OP_ADD:
        return "OP_ADD"
    case OP_SUB:
        return "OP_SUB"
    case OP_MUL:
        return "OP_MUL"
    case OP_DIV:
        return "OP_DIV"
    case OP_DEFINE_FIELD:
        return "OP_DEFINE_FIELD"
    default:
        return fmt.Sprintf("UNKNOWN(%d)", int(op))
    }
}
