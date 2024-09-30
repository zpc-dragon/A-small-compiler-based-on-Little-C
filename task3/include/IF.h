#pragma once
#include "Execute.h"

node* build_cond_tree(vector <PSK>& v) {
    node* root, * cur;
    try {
        if (v[i].first != "if") {
            throwError(ErrorCode::IFError, "缺少if关键字");
        }

        cur = root = new node(1, "if");
        i++;
        cur->firstchild = new node(v[i].second, v[i].first);// if后面的条件
        try {
            check_conditon(cur->firstchild);
        }
        catch (const ConditionException& e) {
            cerr << e.what() << endl;
        }
        i++;
        if (v[i].first != "then") {
            throwError(ErrorCode::IFError, "缺少then关键字");
        }
        cur->nextsibling = create_node("stmts");
        cur = cur->nextsibling;
        i++;
        //忽略{}，只允许一个语句
        if (v[i].first == "if") { // 嵌套if
            cur->firstchild = build_cond_tree(v);
        } else {
            cur->firstchild = build_exec_tree(v);
        }

        i++;
        if (v[i].first == "else") {
            i++;
            cur->nextsibling = build_exec_tree(v);
        } else {
            i--;//回退
        }
    }
    catch (const IFException& e) {
        cerr << e.what() << endl;
    }
    return root;
}
vector<node*> getIFVector(node* root) {
    vector<node*>v;
    v = in_vector(root, v);
    return v;
}
void IFGen(node* root) {
    int n = nextId;
    qua.emplace_back("JT", root->firstchild->token, "_", to_string(n + 2));//跳转到真分支
    qua.emplace_back("JF", "_", "_", "-1");//跳转到else    
    auto cur = root->nextsibling;
    if (cur->firstchild->token == "if") {
        IFGen(cur->firstchild);   //递归
    } else {
        execGen(cur->firstchild);   //真分支
    }

    qua.emplace_back("JE", "_", "_", "-1");//跳转到end
    int else_id = nextId;
    if (cur->nextsibling) { //有else
        execGen(cur->nextsibling);  //假分支
    }
    int end_id = nextId;
    qua[getQuaID("JF", n)].result = to_string(else_id);
    qua[getQuaID("JE", n)].result = to_string(end_id);

}