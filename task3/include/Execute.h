#pragma once
#include "Expression.h"
// 执行语句的语法树
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
bool static_check_exec(node* T) {
    if (T) {
        bool lf, rf;
        if (T->type == 2) {
            int k;
            if (T->token == "=") {
                k = 1;
            } else if (T->token == ":=") {
                k = 2;
            } else {
                throwError(ErrorCode::ExecuteError, "赋值号错误");
                return false;
            }
            if (T->firstchild->type == 5) {
                try {
                    lf = check_left(T->firstchild, k);
                }
                catch (const ExecuteException& e) {
                    cerr << e.what() << endl;
                    return false;
                }
            } else {
                throwError(ErrorCode::ExecuteError, "赋值号左边不是标识符");
                return false;
            }
            if (T->nextsibling) {
                try {
                    // 其实右侧不需要进行expr的静态检查，因为在构造expr树的时候已经进行了检查
                    rf = static_expr_check(T->nextsibling);
                }
                catch (const ExprException& e) {
                    cerr << e.what() << endl;
                    return false;
                }
            } else {
                throwError(ErrorCode::ExecuteError, "赋值号右边为空");
                return false;
            }


            return lf && rf;
        } else  if (T->type == -1) {
            try {
                return checkIO(T);
            }
            catch (const ExecuteException& e) {
                cerr << e.what() << endl;
                return false;
            }

        } else {
            throwError(ErrorCode::ExecuteError, "执行语句非法");
            return false;
        }
    }
    return true;
}
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
            i++;
            right = build_expr_tree(v);
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
    // try {
    //     if (!static_check_exec(root)) {
    //         throw ExecuteException("执行语句静态检查失败");
    //         // root = nullptr;
    //     }
    // }
    // catch (const ExecuteException& e) {
    //     std::cerr << e.what() << '\n';
    // }

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