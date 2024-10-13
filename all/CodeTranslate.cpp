#include "include/Program.h"


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