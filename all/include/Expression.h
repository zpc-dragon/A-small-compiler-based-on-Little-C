#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "func.h"
map<char, int> int_priority =
{
    {'+', 1},
    {'-', 1},
    {'*', 2},
    {'/', 2},
    {'(', 5},
    {')', 5},//括号优先级最高

};
map<string, int> bool_proirity = { // 逻辑运算符优先级(需要对照文法)
    {"||", -1},
    {"&&", 0},
    {"!", 1},
    {"==", 2},
    {"!=", 2},
    {"<", 2},
    {">", 2},
    {"<=", 2},
    {">=", 2},
};

set<string> arith_set = { "+", "-", "*", "/" };//算术运算符
set<string> bool_set = { "&&", "||", "!" };//逻辑运算符
set<string> compare_set = { "<", ">", "<=", ">=", "==", "!=" };//比较运算符

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
    } else if (t->type == 2) { //运算符
        if (compare_set.count(t->token)) {
            if (k != 2) {
                throwError(ErrorCode::ExprError, "比较运算符" + t->token + "不匹配布尔运算");
                return false;
            }
        } else if (bool_set.count(t->token)) {
            if (k != 2) {
                throwError(ErrorCode::ExprError, "逻辑运算符" + t->token + "不匹配布尔运算");
                return false;
            }
        } else if (arith_set.count(t->token)) {
            if (k != 1) {
                throwError(ErrorCode::ExprError, "算术运算符" + t->token + "不匹配数值运算");
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
void join(stack <node*>& s1, stack <node*>& s2) {

    if (s2.size() < 1 || s1.size() < 1) {
        cout << s2.size() << " " << s1.size() << endl;
        // cout << s2.top()->token << " " << s1.top()->token << endl;
        throwError(ErrorCode::ExprError, "操作数和运算符个数不匹配");
        return;
    }

    node* ch = s1.top();
    s1.pop();
    node* t = s2.top();
    s2.pop();
    if (ch->token == "!") { //单目运算符
        node* temp = new node(2, ch->token);
        temp->firstchild = t;
        s2.push(temp);
        try {
            checkID(t, 2);
        }
        catch (const ExprException& e) {
            std::cerr << e.what() << '\n';
        }
    } else {
        node* p = s2.top();
        s2.pop();

        node* temp = new node(2, ch->token);
        temp->firstchild = p;
        temp->nextsibling = t;
        s2.push(temp);
        int k;
        if (bool_set.count(ch->token)) {
            k = 2;
        } else if (compare_set.count(ch->token)) {
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

}
void cal(stack <node*>& s1, stack <node*>& s2) {
    if (s2.size() < 2 || s1.size() < 1) {
        cout << s2.size() << " " << s1.size() << endl;
        cout << s2.top()->token << " " << s1.top()->token << endl;
        throwError(ErrorCode::ExprError, "操作数和运算符个数不匹配");
        return;
    }

    node* ch = s1.top();
    s1.pop();
    node* t = s2.top();
    s2.pop();
    node* p = s2.top();
    s2.pop();
    try {
        if (ch->token == "(" || ch->token == ")") {
            throwError(ErrorCode::ExprError, "括号不能参与运算");
        }

    }
    catch (const ExprException& e) {
        std::cerr << e.what() << '\n';
    }

    node* temp = new node(2, ch->token);
    temp->firstchild = p;
    temp->nextsibling = t;
    s2.push(temp);
    if (arith_set.count(ch->token) == 0) {
        throwError(ErrorCode::ExprError, ch->token + "运算符不合法");
    }
    int k = 1;
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
                    break;
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
                        break;
                    }
                }
                // i++;
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
node* build_bool_expr_tree(vector<PSK>& v) {
    stack<node*> optr; //运算符栈
    stack<node*> opnd;  //操作数栈
    for (; i < v.size(); i++) {
        string s = v[i].first;
        int k = v[i].second;
        node* p = new node(k, s);
        if (k == 2) { //运算符
            if (s == "!") { //单目运算符，由于中缀表达式的算符在后面，这里处理不了，所以直接入栈
                optr.push(p);
                continue;
            }
            while (optr.size() && bool_proirity[optr.top()->token] >= bool_proirity[s]) {
                try {
                    join(optr, opnd);
                }
                catch (const ExprException& e) {
                    std::cerr << e.what() << '\n';
                }
            }
            optr.push(p);
        } else if (k == 3) { //分隔符
            if (s[0] == ';') {
                while (optr.size()) {
                    try {
                        join(optr, opnd);
                    }
                    catch (const ExprException& e) {
                        std::cerr << e.what() << '\n';
                        break;
                    }
                }
                // i++;
                break;
            } else {
                throwError(ErrorCode::ExprError, "布尔表达式中不应该有括号和逗号");
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