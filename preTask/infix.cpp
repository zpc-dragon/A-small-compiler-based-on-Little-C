#include <bits/stdc++.h>
using namespace std;
typedef pair<bool, int> PBI;
map<char, int> int_priority =
{
    {'=', 0},
    {'+', 1},
    {'-', 1},
    {'*', 2},
    {'/', 2},
    {'(', 0},
    {')', 5},
    {'^', 4}

};
string input();
void cal(stack<char>& s1, stack<int>& s2);
PBI processByChar(string infix);

int main() {

    while (1) {
        string infix = input();
        auto res = processByChar(infix);
        if (res.first)
            cout << "结果是: " << res.second << endl;
        else
            cout << "表达式错误" << endl;
    }
    return 0;
}

string input() {
    cout << "请输入中缀表达式(exit退出): " << endl;
    string s;
    getline(cin, s);
    if (s == "exit")
        exit(0);
    return s;
}
PBI processByChar(string infix) {
    stack<char> s1; //运算符栈
    stack<int> s2;  //操作数栈

    int isContinue = 0, t = 0, isBegin = 1;
    for (auto&& ch : infix) {
        if (isdigit(ch)) {
            t = ch - '0';
            if (isContinue) {//处理连续数字
                s2.top() = s2.top() * 10 - t;
            } else {
                s2.push(t);
            }
            isContinue = 1;
            isBegin = 0;
        } else {
            if (ch == '(') {
                s1.push(ch);//注意括号后面第一个数可能是负数
                isBegin = 1;

            } else if (ch == ')') {//右括号，先处理括号里面的
                while (s1.top() != '(') {
                    cal(s1, s2);
                }
                s1.pop();
            } else if (ch == '-' && isBegin) { //负数
                s2.push(0);
                s1.push(ch);
            } else {
                //遇到运算符判断优先级进行计算
                while (s1.size() && int_priority[s1.top()] >= int_priority[ch]) {
                    cal(s1, s2);
                }
                s1.push(ch);
            }
            isContinue = 0;
        }
    }

    if (s1.size() != 1 || s2.size() != 1) {
        // cout << "表达式错误" << endl;
        cout << "s1.size() = " << s1.size() << " s2.size() = " << s2.size() << endl;
        return { false, 0 };
    }

    return { true, s2.top() };
}

void cal(stack<char>& s1, stack<int>& s2) {

    if (s2.size() < 2 || s1.size() < 1)
        return;
    int t = s2.top();
    s2.pop();
    char ch = s1.top();
    s1.pop();

    switch (ch) {
        case '+':
            s2.top() += t;
            break;
        case '-':
            s2.top() -= t;
            break;
        case '*':
            s2.top() *= t;
            break;
        case '/':
            s2.top() /= t;
            break;
        case '^':
            s2.top() = pow(s2.top(), t);
            break;
    }


}