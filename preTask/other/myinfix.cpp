#include<bits/stdc++.h>
using namespace std;

int main() {

    // 输入样例和输出样例
    vector<string> input{ "ab*cd++", "ab*c+d+", "ab*cd+*e+" };
    vector<string> output{ "a*b+(c+d)", "a*b+c+d", "a*b*(c+d)+e" };
    // 运算符优先级
    map<char, int> optors;
    optors['+'] = 100;
    optors['-'] = 100;
    optors['*'] = 200;
    optors['/'] = 200;

    vector<string> optors_stack(input.size()); // 操作符栈

    for (size_t i = 0; i < input.size(); i++) {
        cout << input[i] << endl;
        // 拿到操作符栈
        for (auto&& item : input[i]) {
            if (item == '+' || item == '-' || item == '*' || item == '/') {
                optors_stack[i].push_back(item);
            }
        }
    }

    cout << "-------------------" << endl;
    int pos = 0;
    for (size_t i = 0; i < input.size(); i++) {
        stack<string> val; // 操作数栈，由于操作数会因为因为连接而变长，所以用string
        string postfix = input[i];
        for (auto&& item : postfix) {
            if (item == '+' || item == '-' || item == '*' || item == '/') {
                string a = val.top();
                val.pop();
                string b = val.top();
                val.pop();

                if ((pos < 3 && optors[optors_stack[i][pos + 1]] > optors[item]) || (pos > 0 && optors[optors_stack[i][pos - 1]] > optors[item])) {
                    val.push("(" + b + item + a + ")");
                } else {
                    val.push(b + item + a);
                }
                pos++;

            } else {
                val.push(string(1, item));
            }
        }
        string infix = val.top();
        val.pop();
        cout << infix << endl;
    }
    cout << "-------------------" << endl;
    for (size_t i = 0; i < output.size(); i++) {
        cout << output[i] << endl;
    }

    return 0;
}