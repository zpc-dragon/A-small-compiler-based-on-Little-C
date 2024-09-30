// 改进2
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

    // 添加位置信息
    struct op_pos {
        string op;    // 操作符
        int    pos;   // 在后缀表达式中的位置
        int    pol;   // 几目运算符
    };
    vector<op_pos> post_optors; // 按照后缀表达式操作符的顺序，记录表达式中的操作符
    op_pos tmp;
    for (size_t i = 0; i != postfix.size(); ++i) {
        if (is_operator(optors, postfix[i])) {
            tmp.op = postfix[i];
            tmp.pos = i;
            tmp.pol = 2; // 可以在optors中添加几目信息，并在这里通过optors进行赋值
            post_optors.push_back(tmp);
        }
    }
    auto pos = 0; // 记录当前操作符在post_optors中的位置

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

                // 加括号 || 不加括号
                ++pos;
                if (pos < int(post_optors.size()) && optors[post_optors[pos].op] > optors[postfix[i]]
                    && post_optors[pos].pos - post_optors[pos - 1].pos < post_optors[pos].pol + 1) {/* 这里需要加1，因为记录的是两个操作符的位置 */
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
        printf("please input postfix expression: \n");
        get_postfix(postfix);
        post_to_in(postfix, infix, optors);
        printf("infix expression: \n");
        display(postfix);
        cout << infix << endl;
        cout << endl;
    }

    return 0;
}