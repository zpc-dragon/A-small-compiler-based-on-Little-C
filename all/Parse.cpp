#include"include/Program.h"
int main() {
    string inPath = "input/lex_out";
    string outPath = "../task2/output/parse_out";
    for (int i = 1;i <= 10;i++) {
        string input = inPath + to_string(i) + ".txt";
        string output = outPath + to_string(i) + ".txt";
        ofstream f(output);
        try {
            if (!f.is_open()) {
                throwError(ErrorCode::FileNotFound, output);
            }
        }
        catch (const FileException& e) {
            cerr << e.what() << endl;
            exit(0);
        }
        parse_out(getVector(input), build_program_tree, f);
        f.close();
    }
}