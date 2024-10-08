#include "include/Program.h"

void inputLex(int start, int end) {
    for (int index = start;index <= end;index++) {
        string inname = "source/sourceProgram" + to_string(index) + ".txt";
        string outname = "input/lex_out" + to_string(index) + ".txt";
        // 如果存在outname文件，说明已经进行过词法分析
        if (filesystem::exists(outname)) {
            cout << outname << " exists" << endl;
            continue;
        }
        single_process(index, "source/sourceProgram", "input/lex_out"); // 词法分析
    }

}
void outputCode(int start, int end, string inPath, string outPath, int flag = 0) {
    if (flag == 0)
        tran2Terminal(start, end, inPath, outPath);
    else
        tran2File(start, end, inPath, outPath);
}
int main() {
    string inPath = "input/lex_out";
    string outPath = "output/semantic_out";
    // CodeTranslate(index, len, inPath, outPath);
    string filename = "input/lex_out8.txt";
    // out2terminal(getVector(filename), build_program_tree);

    inputLex(1, 12); // 词法分析，将sourceProgram文件转换为lex_out文件
    outputCode(1, 12, inPath, outPath, 1); // 语法+语义分析，将lex_out文件转换为semantic_out文件

    return 0;
}