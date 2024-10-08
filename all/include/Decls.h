#pragma once
#include"func.h"
node* build_decl_tree(vector <PSK>& v) {
    // 构建语法树
    node* root, * cur;
    try {
        if (v[i].first != "int" && v[i].first != "bool") {// 根节点是关键字
            throwError(ErrorCode::DeclareError, "缺少关键字");
        }
        root = new node(v[i].second, v[i++].first);
        node* p = new node(v[i].second, v[i++].first); // 第一个标识符
        // qua.emplace_back(root->token, "_", "_", p->token);
        cur = root;
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
                continue; //,继续下一个
            }
            if (k == 5) {   // 标识符
                if (cur->firstchild) {
                    node* t = create_node("var_decl");
                    cur->nextsibling = t;
                    t->firstchild = p;
                    string s = p->token;
                    if (symbol_table.find(s) != symbol_table.end()) {
                        throwError(ErrorCode::DeclareError, s + "重复定义");
                    } else {
                        symbol_table[s] = root->token == "int" ? 1 : 2;
                    }
                    cur = t;
                    // qua.emplace_back(root->token, "_", "_", s);
                }
            }
        }
    }
    catch (const DeclareException& e) {
        cerr << e.what() << endl;
    }

    return root;
}
node* build_decls_tree(vector <PSK>& v) {
    node* root, * cur;
    root = cur = create_node("decls");
    while (v[i].first == "int" || v[i].first == "bool") {
        cur->firstchild = build_decl_tree(v);
        cur->nextsibling = create_node("decls");
        cur = cur->nextsibling;
        i++;
    }
    return root;
}