#include "include/Program.h"



int main() {
    // string basepath = "../task2/";
    // string  filename = "input/lex_out10.txt";

    // filename = "test/while.txt";
    // out2terminal(getVector(filename), build_expr_tree);
    // vector<PSK> v = getVector(filename);
    // root = build_while_tree(v);

    // WhileGen(root);
    string inPath = "input/lex_out";
    string outPath = "output/semantic_out";
    code_translate(1, 10, inPath, outPath);



}