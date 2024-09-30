// 后缀表达式转换为中缀表达式
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <set>
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

void init_operators(set<string>& optors) {
    optors.clear();
    optors.insert("+");
    optors.insert("-");
    optors.insert("*");
    optors.insert("/");
}

bool is_operator(const set<string>& optors, const string& str) {
    auto cit = optors.find(str);
    if (cit != optors.end()) {
        return true;
    } else {
        return false;
    }
}

void post_to_in(const vector<string>& postfix, string& infix, const set<string>& optors) {
    infix.clear();
    if (postfix.empty()) {
        return;
    }
    // 表达式栈
    stack<string> exp_stk;
    string a, b, c;
    for (size_t i = 0; i != postfix.size(); ++i) {
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
                c = "( " + a + " " + postfix[i] + " " + b + " )";
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

    set<string> optors;
    init_operators(optors);

    while (1) {

        printf("please input postfix expression: \n");
        get_postfix(postfix);

        post_to_in(postfix, infix, optors);
        // display(postfix);
        cout << infix << endl;
        cout << endl;
    }

    return 0;
}