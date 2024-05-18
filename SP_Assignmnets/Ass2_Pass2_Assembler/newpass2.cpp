#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

string tableLookup(const unordered_map<string, string>& table, const string& key) {
    auto it = table.find(key);
    if (it != table.end()) {
        return it->second;
    }
    return "NAN"; // Not found in table
}

int main() {
    ifstream ic, symTableFile, litTableFile;
    ofstream mc;

    ic.open("ic.txt");
    symTableFile.open("symtable.txt");
    litTableFile.open("littable.txt");
    mc.open("machine_code.txt");

    unordered_map<string, string> symTable, litTable;

    // Read symbol table
    string label, address;
    while (symTableFile >> label >> address) {
        symTable[label] = address;
    }

    // Read literal table
    string literal;
    while (litTableFile >> literal >> address) {
        litTable[literal] = address;
    }

    string lc, ic1, ic2, ic3;

    cout << "\n -- ASSEMBLER PASS-2 OUTPUT --" << endl;
    cout << "\n LC\t <INTERMEDIATE CODE>\t\t\tLC\t <MACHINE CODE>" << endl;

    while (ic >> lc >> ic1 >> ic2 >> ic3) {
        string MC;

        if (ic1.substr(1, 2) == "AD" || (ic1.substr(1, 2) == "DL" && ic1.substr(4, 2) == "02")) {
            MC = " -No Machine Code-";
        } else if (ic1.substr(1, 2) == "DL" && ic1.substr(4, 2) == "01") {
            MC = "00\t0\t00" + ic2.substr(3, 1);
        } else {
            if (ic1 == "(IS,00)") {
                MC = ic1.substr(4, 2) + "\t0\t000";
            } else if (ic2.substr(1, 1) == "S") {
                MC = ic1.substr(4, 2) + "\t0\t" + tableLookup(symTable, ic2.substr(4, 1));
            } else {
                if (ic3.substr(1, 1) == "S") {
                    MC = ic1.substr(4, 2) + "\t" + ic2.substr(1, 1) + "\t" + tableLookup(symTable, ic3.substr(4, 1));
                } else {
                    MC = ic1.substr(4, 2) + "\t" + ic2.substr(1, 1) + "\t" + tableLookup(litTable, ic3.substr(4, 1));
                }
            }
        }

        if (ic1 == "(AD,03)") {
            cout << " " << lc << "\t" << ic1 << "\t" << ic2 << " " << ic3 << "\t\t\t" << lc << "\t" << MC << endl;
            mc << lc << "\t" << MC << endl;
            continue;
        }

        cout << " " << lc << "\t" << ic1 << "\t" << ic2 << "\t " << ic3 << "\t\t\t" << lc << "\t" << MC << endl;
        mc << lc << "\t" << MC << endl;
    }

    // Close files
    ic.close();
    symTableFile.close();
    litTableFile.close();
    mc.close();

    return 0;
}
