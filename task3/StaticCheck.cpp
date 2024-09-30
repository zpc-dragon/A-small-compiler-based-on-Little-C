#include"include/Program.h"


int main() {

    string basepath = "../task2/";
    string  filename = basepath + "input/lex_out8.txt";
    node* root = nullptr;
    if (static_check(filename, root)) {
        cout << "静态语义检查成功" << endl;
        // cout << "中序遍历" << endl;
        // inorder(root);
        // cout << endl;
    } else {
        cout << "静态语义检查失败" << endl;
    }
    delete root;
}