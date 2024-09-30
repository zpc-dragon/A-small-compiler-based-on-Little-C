#pragma once
#include "header.h"
node* build_program_tree(vector <PSK>& text) {
    i = 0;
    nextId = 1;
    qua.clear();
    symbol_table.clear();
    initial_table.clear();
    NoError = true;
    node* root, * cur;
    try {
        if (text[i].first != "{") {
            // err("程序缺少{");
            throwError(ErrorCode::ProgramError, "程序缺少{");
        }
        root = new node(-1, "program");
        cur = root;
        i++;
        for (;i < text.size();i++) {
            string tk = text[i].first;
            int k = text[i].second;
            node* p = new node(k, tk);
            if (k == 1) {
                if (tk == "int" || tk == "bool") {
                    p = build_decl_tree(text);
                } else if (tk == "if") {   // if
                    p = build_cond_tree(text);
                    IFGen(p);
                } else if (tk == "while") {//  while
                    p = build_while_tree(text);
                    WhileGen(p);
                } else if (tk == "read" || tk == "write") {
                    p = build_exec_tree(text);
                    execGen(p);
                } else {
                    throwError(ErrorCode::ProgramError, "非法关键字");
                }
            } else if (k == 5) {
                p = build_exec_tree(text);
                execGen(p);
            } else if (k == 3) {
                if (tk == "{" || tk == "}") {   //由于我们限定了只有在while中才有{}，所以这里不用递归
                    continue;
                } else {
                    // err("分隔符使用");
                    throwError(ErrorCode::ProgramError, "分隔符使用");
                }
            } else {
                // err("缺少运算符/操作数");
                throwError(ErrorCode::ProgramError, "缺少运算符/操作数");
            }
            if (root->firstchild) {
                cur->nextsibling = new node(-1, "stmts");
                cur = cur->nextsibling;
                cur->firstchild = p;
            } else {
                root->firstchild = p;

            }
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
vector<node*> program_post_vector(node* t, vector<node*>& v) {
    if (t == nullptr) {
        return v;
    }
    if (t->firstchild) {
        v = program_post_vector(t->firstchild, v);
    }
    if (t->nextsibling) {
        v = program_post_vector(t->nextsibling, v);
    }
    if (t->type == 1) { // 对于if while int bool read write等节点 
        v.push_back(t);
    } else if (t->type == 5) {
        // 对于执行语句比较麻烦，需要判断是单独的执行语句还是while和if中的执行语句
        v.push_back(t);
    }
    return v;


}
// 驱动模块
void code_translate(int start, int len, string inPath, string outPath) {
    for (int i = start;i <= len;i++) {
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
        }
        // 将 cerr 重定向到文件流对象
        std::streambuf* cerr_backup = std::cerr.rdbuf(); // 备份 cerr 原始缓冲区
        std::cerr.rdbuf(f.rdbuf()); // 将 cerr 重定向到文件流对象

        if (static_check(input, root)) {
            cout << "静态语义检查成功" << endl;
            // cout << "四元式：" << endl;
            // print_qua();
            Fprint(f);// 代码翻译结果输出到文件   

        } else {
            cout << "静态语义检查失败" << endl;
            // 输出错误消息到 cerr，但实际上输出到文件中
            std::cerr << "静态检查失败，编译不通过！" << std::endl;
        }
        // 恢复 cerr 的原始缓冲区
        std::cerr.rdbuf(cerr_backup);
        free_tree(root);  // 释放树的内存
    }

}
