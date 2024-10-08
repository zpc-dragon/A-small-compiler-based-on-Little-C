#pragma once
#include <bits/stdc++.h>
using namespace std;
typedef pair<string, int> PSK;
size_t i = 0;  // 全局变量，用于扫描文本串的指针
static int nextId = 1; // 静态成员变量，用于跟踪下一个可用的id
unordered_map<string, int> symbol_table;// 符号表
unordered_map<string, int> initial_table;// 初始化表
class node {
    public:
    int type;
    string token;
    node* firstchild;
    node* nextsibling;
    node(int t, string s) : type(t), token(s), firstchild(NULL), nextsibling(NULL) {}

};
class quaternion {
    public:
    int id; // 顺序编号
    string optr;
    string arg1;
    string arg2;
    string result;
    // 构造函数初始化
    quaternion(string o, string a1, string a2, string r) : optr(o), arg1(a1), arg2(a2), result(r) {
        id = nextId++; // 这里更新id和nextId的值
    }

    void print() {
        cout << id << ": (" << optr << "," << arg1 << "," << arg2 << "," << result << ")" << endl;
    }




};
// optr类型：+ - * / = < > <= >= == != :=  j jnz je jrop 
// arg1,arg2,result类型：标识符，常量，临时变量

vector<quaternion> qua;
int getQuaID(string optr, int index) {
    for (int i = index; i < qua.size(); i++) {
        if (qua[i].optr == optr && qua[i].result == "-1") {
            return i;
        }
    }
    return -1;

}

