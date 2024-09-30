// 运行环境: vscode c++-17
#include"include/Program.h"
void inputLex(int index) {
    string inname = "../source/sourceProgram" + to_string(index) + ".txt";
    string outname = "input/lex_out" + to_string(index) + ".txt";
    // 如果存在outname文件，退出
    if (filesystem::exists(outname)) {
        cout << outname << " exists" << endl;
        return;
    }
    // std::cout << "input from " << inname << endl;
    single_process(index, inname, outname); // 词法分析

}
void outputParse(int index, string type, node* (*build)(vector <PSK>&)) {
    cout << "---------------------------------" << endl;
    cout << "Test " << type << " tree" << endl;
    // string num = "";
    string file = "input/lex_out" + to_string(index) + ".txt";
    string name = "output/parse_out" + to_string(index) + ".txt";
    // cout << "输出到" << name << endl;
    // out2termianl(getVector(file), build);
    out2file(getVector(file), build, name);
}
int main() {

    string filename;
    filename = "test/expr.txt";
    // out2terminal(getVector(filename),build_expr_tree);

    filename = "test/exe1.txt";
    // out2terminal(filename, "exec", build_exec_tree);

    filename = "test/cond.txt";
    // out2terminal(filename, "cond", build_cond_tree);

    filename = "test/decl.txt";
    // out2terminal(getVector(filename), build_decl_tree);
    // getVector(filename);
    filename = "test/while.txt";
    // out2terminal(getVector(filename), build_while_tree);

    // for (int j = 1;j <= 10;j++) {
    //     inputLex(j);
    //     outputParse(j, "program", build_program_tree);
    // }
    // filename = "test/test.txt";
    filename = "input/lex_out10.txt";
    out2terminal(getVector(filename), build_program_tree);

    return 0;
}