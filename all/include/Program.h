#pragma once
#include "header.h"
void init_global_var() {
    i = 0;
    nextId = 1;
    qua.clear();
    symbol_table.clear();
    initial_table.clear();
    NoError = true;
}
node* build_program_tree(vector <PSK>& text) {
    init_global_var();
    node* root, * cur;
    try {
        if (text[i].first != "{") {
            throwError(ErrorCode::ProgramError, "程序缺少{");
        }
        root = new node(-1, "program");
        cur = root;
        i++;
        cur->firstchild = build_decls_tree(text);
        cur->nextsibling = build_stmts_tree(text, 1);
        if (cur->nextsibling->firstchild == nullptr) {
            throwError(ErrorCode::ProgramError, "程序不合法：缺少执行语句");
        }
        if (text[i].first != "}") {
            throwError(ErrorCode::ProgramError, "程序缺少}");
        }
    }
    catch (const BaseException& e) {
        std::cerr << e.what() << '\n';
    }
    return root;
}
bool static_check(string file, node*& root) { // 静态语义检查，引用传递root
    auto v = getVector(file);
    root = build_program_tree(v);
    return NoError;
}

// 驱动模块
void tran2File(int start, int end, string inPath, string outPath) {
    for (int i = start;i <= end;i++) {
        node* root = nullptr;
        string input = inPath + to_string(i) + ".txt";
        string output = outPath + to_string(i) + ".txt";
        ofstream f(output); // 创建输出文件流对象
        try {
            if (!f) {
                throwError(ErrorCode::FileNotFound, output);
            }
        }
        catch (const BaseException& e) {
            std::cerr << e.what() << '\n';
            exit(1);
        }
        // 将 cerr 重定向到文件流对象
        std::streambuf* cerr_backup = std::cerr.rdbuf(); // 备份 cerr 原始缓冲区
        std::cerr.rdbuf(f.rdbuf()); // 将 cerr 重定向到文件流对象
        if (static_check(input, root)) {
            cout << "静态语义检查成功" << endl;
            Fprint(f);// 代码翻译结果输出到文件  
        } else {
            cout << "静态语义检查失败" << endl;
            // 输出错误消息到 cerr，但实际上输出到文件中
            std::cerr << "静态检查失败，编译不通过！" << std::endl;
        }
        // 恢复 cerr 的原始缓冲区
        std::cerr.rdbuf(cerr_backup);
        delete root;
    }

}
void tran2Terminal(int start, int end, string inPath, string outPath) {
    for (int i = start;i <= end;i++) {
        node* root = nullptr;
        string input = inPath + to_string(i) + ".txt";
        string output = outPath + to_string(i) + ".txt";
        ofstream f(output); // 创建输出文件流对象
        try {
            if (!f) {
                throwError(ErrorCode::FileNotFound, output);
            }
        }
        catch (const BaseException& e) {
            std::cerr << e.what() << '\n';
            exit(1);
        }
        cout << "--------------------------------------\n";
        cout << "文件" << input << "\n";
        if (static_check(input, root)) {
            cout << "静态语义检查成功" << endl;
            cout << "四元式：" << endl;
            print_qua();
        } else {
            cout << "静态语义检查失败" << endl;
        }
    }

}