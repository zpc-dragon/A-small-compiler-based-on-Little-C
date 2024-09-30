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
void WhileGen(node* root) {
    int n = nextId;
    qua.emplace_back("JT", root->firstchild->token, "_", to_string(n + 2));//跳转到真分支
    qua.emplace_back("JF", "_", "_", "-1");//跳转到end   
    auto cur = root->nextsibling;
    if (cur->type == -1) {
        // printf("这是一个语句块\n");
        while (cur && cur->firstchild) {
            execGen(cur->firstchild);   //真分支
            cur = cur->nextsibling;
        }
    } else {
        execGen(cur);   //真分支
    }

    qua.emplace_back("JT", root->firstchild->token, "_", "-1");//跳转到while
    int end_id = nextId;
    qua[getQuaID("JT", n)].result = to_string(n);
    qua[getQuaID("JF", n)].result = to_string(end_id);
}