#pragma once
#include"func.h"
// 这里的报错需要返回值，不需要try catch，直接抛出异常即可，由上层函数处理
bool pushID(node* T, int kind) {
    if (T->firstchild) {
        if (T->firstchild->type == 5) {
            string s = T->firstchild->token;
            if (symbol_table.find(s) != symbol_table.end()) {
                throwError(ErrorCode::DeclareError, s + "重复定义");
                return false;
            } else {
                symbol_table[s] = kind;
            }
        } else {
            throwError(ErrorCode::DeclareError, "缺少标识符");
            return false;

        }

    }
    if (T->nextsibling) {
        return pushID(T->nextsibling, kind);
    }
    return true;
}
bool static_check_decl(node* T) {
    // 这里的T是声明语句的根节点，不需要考虑其他语句情况
    if (T) {
        if (T->type == 1) {
            int kind;
            if (T->token == "int") {
                kind = 1;
            } else if (T->token == "bool") {
                kind = 2;
            } else {
                throwError(ErrorCode::DeclareError, "关键字错误");
                return false;
            }
            try {
                bool flag = pushID(T, kind);
            }
            catch (const DeclareException& e) {
                cerr << e.what() << endl;
                return false;
            }


        } else {
            throwError(ErrorCode::DeclareError, "缺少关键字");
            return false;

        }
    }
    return true;
}
node* build_decl_tree(vector <PSK>& v) {
    // 构建语法树，不检查语法错误
    node* root, * cur;
    try {
        if (v[i].first != "int" && v[i].first != "bool") {// 根节点是关键字
            throwError(ErrorCode::DeclareError, "缺少关键字");
        }
        root = new node(v[i].second, v[i++].first);
        cur = root;
        node* p = new node(v[i].second, v[i++].first); // 第一个标识符
        cur->firstchild = p;
        symbol_table[p->token] = root->token == "int" ? 1 : 2;
        for (;i < v.size(); i++) {
            string s = v[i].first;
            int k = v[i].second;
            node* p = new node(k, s);
            if (k == 3) {// 分隔符
                if (s == ";") {
                    break;
                }
                continue;
            }
            if (k == 5) {   // 标识符
                if (cur->firstchild) {
                    node* t = create_node("decl");
                    cur->nextsibling = t;
                    t->firstchild = p;
                    string s = p->token;
                    if (symbol_table.find(s) != symbol_table.end()) {
                        throwError(ErrorCode::DeclareError, s + "重复定义");
                    } else {
                        symbol_table[s] = root->token == "int" ? 1 : 2;
                    }
                    cur = t;
                }
            }
        }
    }
    catch (const DeclareException& e) {
        cerr << e.what() << endl;
    }
    // 静态检查
    // try {
    //     bool flag = static_check_decl(root);
    //     if (!flag) {
    //         root = nullptr;
    //         throwError(ErrorCode::DeclareError, "声明语句静态检查失败");
    //     }
    // }
    // catch (const DeclareException& e) {
    //     cerr << e.what() << endl;
    // }

    return root;
}
