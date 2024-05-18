#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

void processIntermediateCode(ifstream& ic, unordered_map<string, string>& symbolTable, unordered_map<string, string>& literalTable) {
    string lc, ic1, ic2, ic3;
    while (ic >> lc >> ic1 >> ic2 >> ic3) {
        // Assuming ic1 contains labels, ic2 contains opcodes, and ic3 contains operands

        // If ic1 is not empty and is not a directive
        if (!ic1.empty() && ic1.substr(1, 2) != "AD" && !(ic1.substr(1, 2) == "DL" && ic1.substr(4, 2) == "02")) {
            // Add label to symbol table with address lc
            if (ic1 != "**") {
                symbolTable[ic1] = lc;
            }
        }

        // If ic3 is a literal
        if (!ic3.empty() && ic3[0] == '=') {
            // Add literal to literal table with address lc
            literalTable[ic3] = lc;
        }
    }
}

int main() {
    ifstream ic;
    ofstream symTableFile, litTableFile;

    ic.open("ic.txt");
    symTableFile.open("symtable.txt");
    litTableFile.open("littable.txt");

    unordered_map<string, string> symbolTable, literalTable;

    processIntermediateCode(ic, symbolTable, literalTable);

    // Write symbol table to file
    for (const auto& entry : symbolTable) {
        symTableFile << entry.first << "\t" << entry.second << endl;
    }

    // Write literal table to file
    for (const auto& entry : literalTable) {
        litTableFile << entry.first << "\t" << entry.second << endl;
    }

    // Close files
    ic.close();
    symTableFile.close();
    litTableFile.close();

    return 0;
}
