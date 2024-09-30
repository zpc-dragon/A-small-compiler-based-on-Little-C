#pragma once
#include "IF.h"
node* build_block_tree(vector <PSK>& v) {
    node* root, * cur;
    try {
        if (v[i].first != "{") {
            throwError(ErrorCode::ExecuteError, "block缺少{");
        }
        root = new node(-1, "stmts");
        cur = root;
        i++;
        for (; i < v.size(); i++) {
            string s = v[i].first;
            int k = v[i].second;
            node* p = new node(k, s);
            if (s == "}") {
                break;
            } else if (s == "if") {//只允许有if,不允许有其他的嵌套
                p = build_cond_tree(v);//
            } else {
                if (cur->firstchild) {
                    cur->nextsibling = create_node("stmt");
                    cur = cur->nextsibling;
                    i--;
                } else {
                    cur->firstchild = build_exec_tree(v);
                }
            }
        }
    }
    catch (const ExecuteException& e) {
        cerr << e.what() << endl;
    }
    return root;
}
