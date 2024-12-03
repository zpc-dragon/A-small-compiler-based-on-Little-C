#include<bits/stdc++.h>
using namespace std;
enum Token {
    // 关键字
    tok_eof = -1, // 表示文件终止
    tok_def = -2, // 定义函数
    tok_extern = -3, // 外部定义
    // 标识符
    tok_identifier = -4,
    // 数字
    tok_number = -5
};
// 静态变量和存储区
static string IdentifierStr; // 找标识符    
static double NumVal;   // 找数值
static int gettok() {
    static int LastChar = ' ';
    // 跳过所有的空格
    while (isspace(LastChar)) {
        LastChar = getchar();
    }
    // 识别IdentifierStr
    if (isalpha(LastChar)) {
        IdentifierStr = LastChar;
        while (isalnum(LastChar = getchar())) {
            IdentifierStr += LastChar;
        }
        if (IdentifierStr == "def") {
            return tok_def;
        } else if (IdentifierStr == "extern") {
            return tok_extern;
        }
        return tok_identifier;

    } else if (isdigit(LastChar)) {
        string NumStr;
        int flag = 1;
        do {
            if (LastChar == '.') {
                if (flag) {
                    flag = 0;
                } else {
                    cout << "too many '.' in the num\n";
                    // 这里暂时性的忽略这个错误
                }
            }
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');
        NumVal = stod(NumStr);
        return tok_number;
    } else if (LastChar == '#') {
        do { // 读取注释
            LastChar = getchar();
        } while (LastChar != EOF && LastChar != '\n');
        if (LastChar != EOF) {
            return gettok(); // 继续执行
        }

    } else if (LastChar == EOF) {
        return tok_eof; // 终止符
    } else {
        int ThisChar = LastChar;
        LastChar = getchar();
        return ThisChar;
    }
    return -1;

}
