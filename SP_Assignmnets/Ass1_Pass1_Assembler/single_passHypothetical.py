opcode_map = {
    "MOV": "00",
    "ADD": "01",
    "SUB": "02",
    "JMP": "03",
    "HLT": "04"
}

register_map = {
    "AX": "00",
    "BX": "01",
    "CX": "02",
    "DX": "03"
}

symbol_table = {}
address_counter = 0

def assemble(input_file, output_file):
    global address_counter

    with open(input_file, "r") as file:
        for line in file:
            line = line.strip()
            if not line:
                continue
            
            tokens = line.split()

            if tokens and ':' in tokens[0]:
                label = tokens[0][:-1]
                symbol_table[label] = address_counter
                tokens = tokens[1:]

            if not tokens:
                continue

            mnemonic = tokens[0].upper()
            opcode = opcode_map.get(mnemonic)

            if opcode:
                if mnemonic == "JMP":
                    if len(tokens) < 2:
                        print("Error: Incomplete JMP instruction")
                        return
                    if tokens[1] in symbol_table:
                        offset = symbol_table[tokens[1]]
                        machine_code = opcode + format(offset, '04X')
                    else:
                        print(f"Error: Undefined label '{tokens[1]}'")
                        return
                elif mnemonic == "HLT":
                    machine_code = opcode + "0000"
                else:
                    if len(tokens) != 3:
                        print("Error: Invalid instruction format")
                        return
                    register = tokens[1]
                    value = tokens[2]
                    machine_code = opcode + register_map.get(register) + format(int(value), '04X')
                write_to_output(machine_code, output_file)
            else:
                print(f"Error: Unknown mnemonic '{mnemonic}'")
                return

def write_to_output(machine_code, output_file):
    global address_counter
    with open(output_file, "a") as file:
        file.write(machine_code + "\n")
        address_counter += 1

# Example usage
assemble("input.asm", "output.txt")
