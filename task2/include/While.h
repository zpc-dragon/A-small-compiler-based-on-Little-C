#pragma once
#include "Block.h"
node* build_while_tree(vector <PSK>& v) {
    node* root, * cur;
    try {
        if (v[i].first != "while") {
            throwError(ErrorCode::WhileError, "缺少while关键字");
        }

        root = new node(1, "while");
        cur = root;
        i++;
        cur->firstchild = new node(v[i].second, v[i].first);// while后面的条件
        try {
            check_conditon(cur->firstchild);
        }
        catch (const ConditionException& e) {
            cerr << e.what() << endl;
        }
        i++;
        if (v[i].first != "do") {
            throwError(ErrorCode::WhileError, "缺少do关键字");
        }
        i++;
        if (v[i].first == "while") {
            cur->nextsibling = build_while_tree(v);
        } else if (v[i].first == "{") {
            cur->nextsibling = build_block_tree(v);
        } else {
            cur->nextsibling = build_exec_tree(v);
        }
    }
    catch (const WhileException& e) {
        cerr << e.what() << endl;
    }

    return root;
}
