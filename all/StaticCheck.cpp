#include"include/Program.h"


int main() {

    string basepath = "../task2/";
    string  filename = basepath + "input/lex_out8.txt";
    node* root = nullptr;
    if (static_check(filename, root)) {
        cout << "静态语义检查成功" << endl;
        traverse3(root, cout);
    } else {
        cout << "静态语义检查失败" << endl;
    }
    free_tree(root);
    return 0;

}