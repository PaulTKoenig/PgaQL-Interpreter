package compiler

import (
    "fmt"
)

type OpCode int

const (
    OP_SCAN OpCode = iota
    OP_LOAD_FIELD
    OP_LOAD_CONST
    OP_PUSH
    OP_EQ
    OP_FILTER
    OP_DEFINE_STAT
    OP_GROUP_BY
    OP_AGGREGATE
    OP_OUTPUT
    OP_PROJECT
)

type Instruction struct {
    Op   OpCode
    Args []interface{}
}

type Bytecode []Instruction

func Compile(query string) ([]Instruction, error) {

    return []Instruction{
        {Op: OP_SCAN, Args: []interface{}{"games"}},
        {Op: OP_LOAD_FIELD, Args: []interface{}{"season"}},
        {Op: OP_LOAD_CONST, Args: []interface{}{"2025"}},
        {Op: OP_EQ},
        {Op: OP_FILTER},
        {Op: OP_PROJECT, Args: []interface{}{"pts", "date"}},
        {Op: OP_OUTPUT},
	}, nil

    // cmd := exec.Command("./bin/compiler", query)
    // output, err := cmd.Output()
    // if err != nil {
    //     return nil, err
    // }
    // return output, nil // bytecode as raw []byte
}

func (op OpCode) String() string {
    switch op {
    case OP_SCAN:
        return "OP_SCAN"
    case OP_LOAD_FIELD:
        return "OP_LOAD_FIELD"
    case OP_LOAD_CONST:
        return "OP_LOAD_CONST"
    case OP_PUSH:
        return "OP_PUSH"
    case OP_EQ:
        return "OP_EQ"
    case OP_FILTER:
        return "OP_FILTER"
    case OP_DEFINE_STAT:
        return "OP_DEFINE_STAT"
    case OP_GROUP_BY:
        return "OP_GROUP_BY"
    case OP_AGGREGATE:
        return "OP_AGGREGATE"
    case OP_OUTPUT:
        return "OP_OUTPUT"
    case OP_PROJECT:
        return "OP_PROJECT"
    default:
        return fmt.Sprintf("UNKNOWN(%d)", int(op))
    }
}