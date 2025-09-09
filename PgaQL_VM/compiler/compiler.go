package compiler

import (
    "log"
    "os/exec"
    "encoding/json"
)

type Instruction struct {
    Op   OpCode
    Args []interface{}
}

type Bytecode []Instruction

func Compile(query string) ([]Instruction, error) {

    cmd := exec.Command("./ccompiler/bin/compiler", query)
    output, err := cmd.Output()
    if err != nil {
        return nil, err
    }

    var instructions []Instruction
    err = json.Unmarshal(output, &instructions)
    if err != nil {
        log.Fatal(err)
    }

    log.Println("1")
    log.Println(instructions)
    log.Println("2")
    // return output, nil // bytecode as raw []byte

    return []Instruction{
        {Op: OP_SCAN, Args: []interface{}{"games"}},
        {Op: OP_LOAD_FIELD, Args: []interface{}{"season"}},
        {Op: OP_LOAD_CONST, Args: []interface{}{"2025"}},
        {Op: OP_EQ},
        {Op: OP_FILTER},
        {Op: OP_PROJECT, Args: []interface{}{"pts", "date"}},
        {Op: OP_OUTPUT},
	}, nil
}