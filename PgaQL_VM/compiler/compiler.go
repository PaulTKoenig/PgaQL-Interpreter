package compiler

type OpCode int

const (
    OP_SCAN OpCode = iota
    OP_LOAD
    OP_PUSH
    OP_EQ
    OP_FILTER
    OP_DEFINE_STAT
    OP_GROUP_BY
    OP_AGGREGATE
    OP_OUTPUT
)

type Instruction struct {
    Op   OpCode
    Args []interface{}
}

type Bytecode []Instruction

func Compile(query string) ([]Instruction, error) {

    return []Instruction{
        {Op: OP_SCAN, Args: []interface{}{"players"}},
        {Op: OP_LOAD, Args: []interface{}{"season"}},
        {Op: OP_PUSH, Args: []interface{}{2025}},
        {Op: OP_EQ},
        {Op: OP_FILTER},
        {Op: OP_PROJECT, Args: []interface{}{"pts"}},
        {Op: OP_PROJECT, Args: []interface{}{"date"}},
        {Op: OP_OUTPUT},
	}, nil

    // cmd := exec.Command("./bin/compiler", query)
    // output, err := cmd.Output()
    // if err != nil {
    //     return nil, err
    // }
    // return output, nil // bytecode as raw []byte
}
