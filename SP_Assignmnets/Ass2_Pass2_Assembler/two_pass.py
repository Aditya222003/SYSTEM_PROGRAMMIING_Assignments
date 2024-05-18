class SymbolTable:
    def __init__(self):
        self.symbols = {}

    def add_symbol(self, label, address):
        if label in self.symbols:
            raise Exception(f"Duplicate label '{label}'")
        self.symbols[label] = address

    def get_address(self, label):
        if label not in self.symbols:
            raise Exception(f"Undefined symbol '{label}'")
        return self.symbols[label]

class SegmentTable:
    def __init__(self):
        self.segments = {"DATA": 0, "CODE": 0}  # Initial address for data and code segments

    def set_segment_address(self, segment, address):
        self.segments[segment] = address

class Assembler:
    def __init__(self):
        self.symbol_table = SymbolTable()
        self.segment_table = SegmentTable()
        self.intermediate_code = []

    def pass_one(self, code):
        current_address = 0
        current_segment = None

        for line in code:
            # Parse line to extract label, opcode, and operands
            label, opcode, operands = self.parse_line(line)

            # Handle segment directives
            if opcode == "DATA":
                current_segment = "DATA"
                self.segment_table.set_segment_address(current_segment, current_address)
                continue
            elif opcode == "CODE":
                current_segment = "CODE"
                self.segment_table.set_segment_address(current_segment, current_address)
                continue
            elif opcode == "END":
                break

            # Handle label definitions
            if label:
                self.symbol_table.add_symbol(label, current_address)

            # Calculate address for current instruction
            current_address += self.instruction_length(opcode)

            # Generate intermediate code
            self.intermediate_code.append((current_segment, current_address, opcode, operands))

    def pass_two(self):
        machine_code = []

        for segment, address, opcode, operands in self.intermediate_code:
            if opcode == "END":
                break

            # Translate opcode and operands to machine code
            translated_code = self.translate_instruction(opcode, operands)

            # Resolve symbolic addresses
            resolved_code = self.resolve_symbols(translated_code)

            machine_code.append((segment, address, resolved_code))

        return machine_code

    def parse_line(self, line):
        # Dummy implementation to parse assembly code line
        # Example implementation:
        parts = line.split()
        label = parts[0] if len(parts) > 1 else None
        opcode = parts[1] if len(parts) > 1 else parts[0]
        operands = parts[2:] if len(parts) > 2 else []
        return label, opcode, operands

    def instruction_length(self, opcode):
        # Dummy implementation to determine instruction length
        # Example implementation:
        if opcode in ["MOV", "ADD", "SUB"]:
            return 3
        elif opcode in ["JMP", "CALL"]:
            return 2
        else:
            return 1

    def translate_instruction(self, opcode, operands):
        # Dummy implementation to translate opcode and operands to machine code
        # Example implementation:
        if opcode == "MOV":
            return [0x8B]  # Example machine code for MOV instruction
        elif opcode == "ADD":
            return [0x03]  # Example machine code for ADD instruction
        else:
            return [0x00]  # Default machine code

    def resolve_symbols(self, code):
        # Dummy implementation to resolve symbolic addresses in machine code
        # Example implementation:
        resolved_code = []
        for byte in code:
            if byte == 0xFF:  # Placeholder for symbolic address
                resolved_code.append(0xAB)  # Resolved address
            else:
                resolved_code.append(byte)
        return resolved_code

    def assemble(self, code):
        try:
            self.pass_one(code)
            machine_code = self.pass_two()
            return machine_code
        except Exception as e:
            print(f"Error: {str(e)}")

# Example usage:
assembly_code = [
    "DATA",
    "LABEL1: MOV AX, 5",
    "CODE",
    "LOOP: ADD AX, BX",
    "END"
]

assembler = Assembler()
machine_code = assembler.assemble(assembly_code)
print(machine_code)
