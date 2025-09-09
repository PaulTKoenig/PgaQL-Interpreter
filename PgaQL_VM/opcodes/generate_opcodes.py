import os

def generate_c_header(opcodes, filename="opcodes.h"):
	with open(filename, "w") as f:
		f.write("#ifndef OPCODES_H\n")
		f.write("#define OPCODES_H\n\n")
		f.write("typedef enum {\n")
		for i, op in enumerate(opcodes):
			f.write(f"    {op}")
			if i < len(opcodes) - 1:
				f.write(",\n")
			else:
				f.write("\n")
		f.write("} OpCode;\n\n")
		f.write("#endif\n")
	print(f"Generated C header: {filename}")


def generate_go_file(opcodes, filename="opcodes.go"):
	with open(filename, "w") as f:
		f.write("package compiler\n\n")

		f.write("import (\n")
		f.write("	\"fmt\"\n")
		f.write(")\n")
		f.write("\n")

		f.write("type OpCode int\n\n")
		f.write("const (\n")
		for i, op in enumerate(opcodes):
			f.write(f"    {op} OpCode = {i}\n")
		f.write(")\n")
		print(f"Generated Go file: {filename}")

		f.write("\n")
		f.write("func (op OpCode) String() string {\n")
		f.write("    switch op {\n")
		for op in opcodes:
			f.write(f"    case {op}:\n")
			f.write(f"        return \"{op}\"\n")
		f.write("    default:\n")
		f.write("        return fmt.Sprintf(\"UNKNOWN(%d)\", int(op))\n")
		f.write("    }\n")
		f.write("}\n")


def main():


	script_dir = os.path.dirname(os.path.abspath(__file__))
	input_file = os.path.join(script_dir, "opcodes.def")

	c_header_file = os.path.join(script_dir, "../ccompiler/src/opcodes.h")
	go_file = os.path.join(script_dir, "../compiler/opcodes.go")

	with open(input_file) as f:
		opcodes = [line.strip() for line in f if line.strip()]

	generate_c_header(opcodes, c_header_file)
	generate_go_file(opcodes, go_file)


if __name__ == "__main__":
	main()
