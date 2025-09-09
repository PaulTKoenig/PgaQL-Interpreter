package compiler

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
