// 改进1
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <map>
using namespace std;

void get_postfix(vector<string>& postfix) {
    postfix.clear();
    string line, op_op;
    getline(cin, line);
    istringstream sin(line);
    while (sin >> op_op) {
        postfix.push_back(op_op);
    }
}

void init_operators(map<string, int>& optors) {
    optors.clear();
    optors["+"] = 100;
    optors["-"] = 100;
    optors["*"] = 200;
    optors["/"] = 200;
}

bool is_operator(const map<string, int>& optors, const string& str) {
    auto cit = optors.find(str);
    if (cit != optors.end()) {
        return true;
    } else {
        return false;
    }
}

void post_to_in(const vector<string>& postfix, string& infix, map<string, int>& optors) {
    infix.clear();
    if (postfix.empty()) {
        return;
    }
    vector<string> post_optors; // 按照后缀表达式操作符的顺序，记录表达式中的操作符
    for (auto i = 0; i != postfix.size(); ++i) {
        if (is_operator(optors, postfix[i])) {
            post_optors.push_back(postfix[i]);
        }
    }
    auto pos = 0; // 记录当前操作符在post_optors中的位置

    // 表达式栈
    stack<string> exp_stk;
    string a, b, c;

    for (auto i = 0; i != postfix.size(); ++i) {
        if (!is_operator(optors, postfix[i])) {
            exp_stk.push(postfix[i]);
        } else {
            switch (postfix[i][0]) {
            case '+':
            case '-':
            case '*':
            case '/':
                b = exp_stk.top();
                exp_stk.pop();
                a = exp_stk.top();
                exp_stk.pop();

                // 加括号 || 不加括号
                ++pos;
                if (pos < post_optors.size() && optors[post_optors[pos]] > optors[postfix[i]]) {
                    //上述条件判断了当前操作符的优先级是否小于下一个操作符的优先级，如果小于，则需要加括号
                    c = "( " + a + " " + postfix[i] + " " + b + " )";
                } else {
                    c = a + " " + postfix[i] + " " + b;
                }

                exp_stk.push(c);
                break;

            default:
                break;
            }
        }
    }
    if (exp_stk.size() == 1) {
        infix = exp_stk.top();
    } else {
        infix = "后缀表达式非法，转换失败！";
    }
}

void display(const vector<string>& strs) {
    for (auto cit = strs.begin(); cit != strs.end(); ++cit) {
        cout << *cit << ' ';
    }
    cout << endl;
}

int main() {
    vector<string> postfix;
    string infix;

    map<string, int> optors;
    init_operators(optors);

    while (1) {
        printf("请输入后缀表达式：\n");
        get_postfix(postfix);
        post_to_in(postfix, infix, optors);
        printf("中缀表达式：\n");
        display(postfix);
        cout << infix << endl;
        cout << endl;
    }

    return 0;
}