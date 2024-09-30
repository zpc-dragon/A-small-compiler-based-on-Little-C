#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "func.h"
map<char, int> int_priority =
{
    // {'=', 0},
    {'+', 1},
    {'-', 1},
    {'*', 2},
    {'/', 2},
    {'(', 5},
    {')', 5},//括号优先级最高

};
map<string, int> alog_map = { //算术运算符
    {"+", 1},
    {"-", 1},
    {"*", 2},
    {"/", 2},
};
map<string, int> bool_map = {   //逻辑运算符
    {"&&", 1},
    {"||", 1},
    {"!", 2},

};
map<string, int> compare_map = {    //比较运算符
    {"<", 1},
    {">", 1},
    {"<=", 1},
    {">=", 1},
    {"==", 1},
    {"!=", 1},
};
bool checkID(node* t, int k) {
    if (t->type == 5) { //标识符
        string s = t->token;
        if (symbol_table.find(s) == symbol_table.end()) {
            throwError(ErrorCode::ExprError, "赋值表达式中" + s + "未声明");
            return false;
        }
        if (symbol_table[s] != k) {
            string op = k == 1 ? "int" : "bool";
            throwError(ErrorCode::ExprError, s + ":不匹配" + op + "运算符");
            return false;
        }
    } else  if (t->type == 4) { //常数
        if (t->token == "true" || t->token == "false") {
            if (k != 2) {
                throwError(ErrorCode::ExprError, "常数不匹配布尔运算");
                return false;
            }
        } else {
            if (k != 1) {
                throwError(ErrorCode::ExprError, "常数不匹配数值运算");
                return false;
            }
        }
    } else if (t->type == 2) {
        if (compare_map.find(t->token) != compare_map.end()) {
            if (k != 2) {
                throwError(ErrorCode::ExprError, "比较结果不匹配布尔运算");
                return false;
            }
        } else if (bool_map.find(t->token) != bool_map.end()) {
            if (k != 2) {
                throwError(ErrorCode::ExprError, "逻辑运算符不匹配布尔运算");
                return false;
            }
        } else if (alog_map.find(t->token) != alog_map.end()) {
            if (k != 1) {
                throwError(ErrorCode::ExprError, "算术运算符不匹配数值运算");
                return false;
            }
        } else {
            throwError(ErrorCode::ExprError, t->token + "运算符不合法");
            return false;
        }
    } else {
        throwError(ErrorCode::ExprError, "表达式不完整");
        return false;
    }
    return true;

}
void cal(stack <node*>& s1, stack <node*>& s2) {
    if (s2.size() < 2 || s1.size() < 1) {
        throwError(ErrorCode::ExprError, "操作数和运算符个数不匹配");
        return;
    }

    node* ch = s1.top();
    s1.pop();
    node* t = s2.top();
    s2.pop();
    node* p = s2.top();
    s2.pop();

    node* temp = new node(2, ch->token);
    temp->firstchild = p;
    temp->nextsibling = t;
    s2.push(temp);
    int k;
    if (bool_map.find(ch->token) != bool_map.end()) {
        k = 2;
    } else if (alog_map.find(ch->token) != alog_map.end() || compare_map.find(ch->token) != compare_map.end()) {
        k = 1;
    } else {
        throwError(ErrorCode::ExprError, ch->token + "运算符不合法");
    }
    try {
        checkID(p, k);
    }
    catch (const ExprException& e) {
        std::cerr << e.what() << '\n';
    }
    try {
        checkID(t, k);
    }
    catch (const ExprException& e) {
        std::cerr << e.what() << '\n';
    }

}
bool static_expr_check(node* T, int k = 0) {
    if (T) {
        if (T->type == 2) {
            if (T->token[0] == '+' || T->token[0] == '-' || T->token[0] == '*' || T->token[0] == '/') {
                k = 1;
            } else if (T->token == "&&" || T->token == "||" || T->token == "==" || T->token == "!=" || T->token == "<" || T->token == ">" || T->token == "<=" || T->token == ">=") {
                k = 2;
            } else if (T->token == "!") { // 单目运算符，只能是bool
                if (T->firstchild->type == 5) {
                    try {
                        checkID(T->firstchild, 2);
                    }
                    catch (const ExprException& e) {
                        cerr << e.what() << endl;
                        return false;
                    }
                } else if (T->firstchild->type == 2) {
                    try {
                        static_expr_check(T->firstchild, 2);
                    }
                    catch (const ExprException& e) {
                        cerr << e.what() << endl;
                        return false;
                    }
                } else {
                    throwError(ErrorCode::ExprError, "操作数不合法");
                    return false;
                }
            } else {
                throwError(ErrorCode::ExprError, "运算符不合法");
                return false;
            }
            bool flag = false;
            if (T->firstchild) {
                try {
                    if (T->firstchild->type == 5) {
                        try {
                            flag = checkID(T->firstchild, k);
                        }
                        catch (const ExprException& e) {
                            std::cerr << e.what() << '\n';
                        }

                    } else if (T->firstchild->type == 2) { //递归检查子树
                        try {
                            flag = static_expr_check(T->firstchild);
                        }
                        catch (const ExprException& e) {
                            std::cerr << e.what() << '\n';
                        }
                    } else if (T->firstchild->type == 4) { //常数
                        flag = true;
                    } else {
                        throwError(ErrorCode::ExprError, "表达式不合法");
                    }
                }
                catch (const ExprException& e) {
                    cerr << e.what() << endl;
                    return false;
                }
            } else {
                throwError(ErrorCode::ExprError, "运算符前缺少操作数");
                return false;
            }
            bool flag1 = false;
            if (T->nextsibling) {
                try {
                    if (T->nextsibling->type == 5) {
                        try {
                            flag1 = checkID(T->nextsibling, k);
                        }
                        catch (const ExprException& e) {
                            std::cerr << e.what() << '\n';
                        }
                    } else if (T->nextsibling->type == 2) {
                        try {
                            flag1 = static_expr_check(T->nextsibling);
                        }
                        catch (const ExprException& e) {
                            std::cerr << e.what() << '\n';
                        }
                    } else if (T->nextsibling->type == 4) {
                        flag1 = true;
                    } else {
                        throwError(ErrorCode::ExprError, "表达式不合法");
                    }

                }
                catch (const ExprException& e) {
                    cerr << e.what() << endl;
                    return false;
                }
            } else {
                throwError(ErrorCode::ExprError, "运算符后缺少操作数");
                return false;
            }
            return flag && flag1;
        } else if (T->type == 5 || T->type == 4) {
            return true;
        } else {
            throwError(ErrorCode::ExprError, "非法根节点");// 调用者去处理
            return false;
        }

    }
    return true;
}

node* build_expr_tree(vector <PSK>& v) {
    stack<node*> optr; //运算符栈
    stack<node*> opnd;  //操作数栈
    for (;i < v.size(); i++) {
        string s = v[i].first;
        int k = v[i].second;
        node* p = new node(k, s);
        if (k == 2) { //运算符
            // p->op = s[0];
            while (optr.size() && int_priority[optr.top()->token[0]] >= int_priority[s[0]]) {
                if (optr.top()->token[0] == '(') {
                    break;//遇到左括号停止,不然会一直循环
                }
                try {
                    cal(optr, opnd);
                }
                catch (const ExprException& e) {
                    std::cerr << e.what() << '\n';
                }
            }
            optr.push(p);
        } else if (k == 3) { //分隔符
            if (s[0] == '(') {
                optr.push(p);
            } else if (s[0] == ')') {
                while (optr.top()->token[0] != '(') {
                    try {
                        cal(optr, opnd);
                    }
                    catch (const ExprException& e) {
                        std::cerr << e.what() << '\n';
                    }
                }

                optr.pop();
            } else if (s[0] == ';') {
                while (optr.size()) {
                    try {
                        cal(optr, opnd);
                    }
                    catch (const ExprException& e) {
                        std::cerr << e.what() << '\n';
                    }
                }
                break;
            }
        } else {    //标识符和常量
            opnd.push(p);
        }
    }


    try {
        if (opnd.size() != 1) {
            throwError(ErrorCode::ExprError, "操作数栈长度不为1");
        }
    }
    catch (const ExprException& e) {
        std::cerr << e.what() << '\n';
    }


    return opnd.top();
}

vector<node*>getExprStack(node* root) {
    vector<node*> exp;
    post_vector(root, exp);
    return exp;
}
void exprGen(node* root) {
    // 将表达式栈中的表达式生成四元式
    int temp_num = 1;
    vector<node*> exp = getExprStack(root);
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
}
#endif