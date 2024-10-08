#pragma once
#include "Expression.h"

bool check_left(node* t, int k) {
    if (t->type == 5) {
        string s = t->token;
        if (symbol_table.find(s) == symbol_table.end()) {
            throwError(ErrorCode::ExecuteError, "执行语句左部" + s + "未声明");
            return false;
        }
        if (symbol_table[s] != k) {
            string op = k == 1 ? "=" : ":=";
            throwError(ErrorCode::ExecuteError, s + ":类型不匹配" + op + "运算符");
            return false;
        }
    } else {
        throwError(ErrorCode::ExecuteError, "缺少标识符");
    }
    initial_table[t->token] = 1;
    return true;
}
bool checkIO(node* T) {
    bool flag = false;
    if (T->firstchild->token == "read" || T->firstchild->token == "write") {
        if (T->nextsibling->type == 5) {
            node* t = T->nextsibling;
            if (t->type == 5) {
                string s = t->token;
                if (symbol_table.find(s) == symbol_table.end()) {
                    throwError(ErrorCode::ExecuteError, T->firstchild->token + "对象" + s + "未声明");
                    return false;
                }
                if (symbol_table[s] != 1) {
                    throwError(ErrorCode::ExecuteError, s + ":bool类型不可以I/O");
                    return false;
                }
            } else {
                throwError(ErrorCode::ExecuteError, "缺少标识符");
            }

        } else {
            throwError(ErrorCode::ExecuteError, T->firstchild->token + "后面不是标识符");
            return false;
        }
    } else {
        throwError(ErrorCode::ExecuteError, "IO关键字错误");
        return false;
    }
    return flag;
}
// 执行语句的语法树构建
node* build_exec_tree(vector <PSK>& v) {
    string str = v[i].first;
    int kind = v[i].second;
    node* root;
    try {
        if (kind != 5 && kind != 1) {
            throwError(ErrorCode::ExecuteError, "执行语句错误");
        }

        if (kind == 5) {
            node* left, * right;
            root = left = right = nullptr;
            vector <PSK> infix1, infix2;
            left = new node(kind, str);
            i++;
            if (v[i].first == "=" || v[i].first == ":=") {
                root = new node(v[i].second, v[i].first);
            } else {
                throwError(ErrorCode::ExecuteError, "赋值号错误");
            }
            try {
                check_left(left, v[i].first == "=" ? 1 : 2);
            }
            catch (const ExecuteException& e) {
                cerr << e.what() << endl;

            }
            if (v[i++].first == "=") {
                right = build_expr_tree(v);
            } else {
                right = build_bool_expr_tree(v);
            }

            root->firstchild = left;
            root->nextsibling = right;
        } else {
            if (str == "read" || str == "write") {
                root = new node(-1, "IO");
                root->firstchild = new node(kind, str);
                i++;
                root->nextsibling = new node(v[i].second, v[i].first);
                i++; //跳过标识符
                try {
                    checkIO(root);
                }
                catch (const ExecuteException& e) {
                    cerr << e.what() << endl;
                }

            } else {
                cout << str << " " << kind << endl;
                throwError(ErrorCode::ExecuteError, "I/O语句错误");
            }
        }
    }
    catch (const ExecuteException& e) {
        cerr << e.what() << endl;
    }
    return root;
}

vector<node*>getExecStack(node* root) {
    vector<node*> exp;
    post_vector(root, exp);
    return exp;
}
void execGen(node* root) {
    // 将表达式栈中的表达式生成四元式
    int temp_num = 1;
    vector<node*> exp = getExecStack(root);
    // if (exp.empty()) {
    //     // else分支可能为空
    //     cout << "执行语句为空" << endl;
    //     cout << root->token << endl;
    //     return;
    // }

    if (exp[0]->token == "read" || exp[0]->token == "write") {
        qua.emplace_back(exp[0]->token, "_", "_", exp[1]->token);
        return;
    }
    stack<node*> opnd;
    for (auto&& item : exp) {
        if (item->type == 2) { // 运算符
            auto op1 = opnd.top();
            opnd.pop();
            auto op2 = opnd.top();
            opnd.pop();
            string op = item->token;
            string res = "t" + to_string(temp_num++);
            qua.emplace_back(op, op2->token, op1->token, res);
            opnd.push(create_node(res));

        } else { // 操作数
            opnd.push(item);
        }
    }
    // 最后一个赋值操作
    auto& last = qua.back();
    last.result = last.arg1;
    last.arg1 = last.arg2;
    last.arg2 = "_";

}
void StmtsGen(node* root);
node* build_stmt_tree(vector<PSK>& v);
node* build_stmts_tree(vector<PSK>& v, int type = 0);

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
        cur->firstchild = build_stmt_tree(v);
        i++;
        if (v[i].first == "else") {
            i++;
            cur->nextsibling = build_stmt_tree(v);
        } else {
            i--;//回退
        }
    }
    catch (const IFException& e) {
        cerr << e.what() << endl;
    }
    return root;
}

void IFGen(node* root) {
    int n = nextId;
    qua.emplace_back("JT", root->firstchild->token, "_", to_string(n + 2));//跳转到真分支
    qua.emplace_back("JF", "_", "_", "-1");//跳转到else    
    auto cur = root->nextsibling;
    StmtsGen(cur->firstchild);   //真分支

    int else_id = nextId;
    if (cur->nextsibling) { //有else
        qua.emplace_back("JN", "_", "_", "-1");//跳转到end
        else_id = nextId;// 更新else_id
        StmtsGen(cur->nextsibling);
    }
    int end_id = nextId;
    qua[getQuaID("JF", n)].result = to_string(else_id);
    if (cur->nextsibling) {//有else
        qua[getQuaID("JN", n)].result = to_string(end_id);
    }

}

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
        cur->nextsibling = build_stmt_tree(v);
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
    StmtsGen(cur);   //真分支

    qua.emplace_back("JT", root->firstchild->token, "_", "-1");//跳转到while
    int end_id = nextId;
    qua[getQuaID("JT", n)].result = to_string(n);
    qua[getQuaID("JF", n)].result = to_string(end_id);
}
node* build_stmt_tree(vector<PSK>& v) {
    node* root, * p = nullptr;
    try {
        string s = v[i].first;
        int k = v[i].second;
        if (s == "if") {//if语句
            p = build_cond_tree(v);
            // IFGen(p);
        } else if (s == "while") { // while语句
            p = build_while_tree(v);
            // WhileGen(p);
        } else if (s == "{") { // stmts
            i++;
            p = build_stmts_tree(v);
            // StmtsGen(p);
        } else if (s == "read" || s == "write" || k == 5) { // 执行语句
            p = build_exec_tree(v);
            // execGen(p);
        } else {
            throwError(ErrorCode::ProgramError, "非法语句");
        }
        root = p;

    }
    catch (const BaseException& e) {
        cerr << e.what() << endl;
    }
    return root;
}
void CodeGen(node* root) {
    string s = root->token;
    int k = root->type;
    if (s == "if") {//if语句
        IFGen(root);
    } else if (s == "while") { // while语句
        WhileGen(root);
    } else if (s == "IO" || k == 2) { // 执行语句
        execGen(root);
    } else {
        throwError(ErrorCode::ProgramError, "非法语句");
    }
}
node* build_stmts_tree(vector<PSK>& v, int type) {
    node* root, * cur;
    root = cur = create_node("stmts");

    while (i < v.size() - 1) {
        string tk = v[i].first;
        int k = v[i].second;
        if (tk == "}") {
            break;
        }
        cur->firstchild = build_stmt_tree(v);
        // 应该在这里生成四元式
        if (type) { // type用于检测是否是program中的Stmts而不是if或while中的Stmts
            CodeGen(cur->firstchild);
        }
        cur->nextsibling = create_node("stmts");
        cur = cur->nextsibling;
        i++;
    }
    return root;
}
void StmtsGen(node* root) {
    // 生成stmts的四元式
    if (root == nullptr) {
        return;
    }
    string s = root->token;
    if (s == "stmts") {
        StmtsGen(root->firstchild);
        StmtsGen(root->nextsibling);
    } else {
        if (s == "if") {
            IFGen(root);
        } else if (s == "while") {
            WhileGen(root);
        } else {
            execGen(root);
        }
    }
}