#pragma once
#include "header.h"
node* build_program_tree(vector <PSK>& text) {
    i = 0;
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
                } else if (tk == "while") {//  while
                    p = build_while_tree(text);
                } else if (tk == "read" || tk == "write") {
                    p = build_exec_tree(text);
                } else {
                    throwError(ErrorCode::ProgramError, "非法关键字");
                }
            } else if (k == 5) {
                p = build_exec_tree(text);
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