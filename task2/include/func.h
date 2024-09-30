#pragma once
#include <bits/stdc++.h>
#include "err.h"
using namespace std;
typedef pair<string, int> PSK;
size_t i = 0;  // 全局变量，用于扫描文本串的指针
unordered_map<string, int> symbol_table;// 符号表
unordered_map<string, int> initial_table;// 初始化符号表
class node {
    public:
    int type;
    string token;
    node* firstchild;
    node* nextsibling;
    node(int t, string s) : type(t), token(s), firstchild(NULL), nextsibling(NULL) {}

};

void err(string s) {
    cout << s << endl;
    exit(0);
}

int kindByEnum(int e) {
    if (!e)
        return 0;
    if (e < 15) {   // 0-14表示关键字
        return 1;
    } else if (e < 30) { // 15-29表示运算符
        return 2;
    } else if (e < 38) { // 30-37表示界符
        return 3;
    } else if (e < 42) { // 38-41表示常数
        return 4;
    } else {    // 42表示标识符
        return 5;
    }
}

void preorder(node* T) {
    if (T) {
        if (T->type != -1)
            cout << T->token << " ";
        if (T->firstchild) {
            preorder(T->firstchild);
        }
        if (T->nextsibling) {
            preorder(T->nextsibling);
        }
    }
}
void inorder(node* T) {
    if (T) {
        if (T->firstchild) {
            inorder(T->firstchild);
        }
        if (T->type != -1)
            cout << T->token << " ";
        if (T->nextsibling) {
            inorder(T->nextsibling);
        }
    }
}
void postorder(node* T) {
    if (T) {
        if (T->firstchild) {
            postorder(T->firstchild);
        }
        if (T->nextsibling) {
            postorder(T->nextsibling);
        }
        if (T->type != -1)
            cout << T->token << " ";
    }
}
node* create_node(string s) {
    node* p = new node(-1, s);
    return p;
}


vector<PSK> getVector(string file) {
    ifstream f(file);
    string s;
    vector <PSK> v;
    try {
        if (!f.is_open()) {
            throw FileException(file);
        }
    }
    catch (const FileException& e) {
        cerr << e.what() << endl;
        exit(0);
    }

    while (getline(f, s)) {
        int m = s.size();
        s = s.substr(1, m - 2);
        auto pos = s.find(',', 1);//从第二个字符开始找,因为可能会有分隔符
        string str = s.substr(0, pos);
        string str1 = s.substr(pos + 2);
        v.push_back(make_pair(str, kindByEnum(stoi(str1))));
    }
    // for (auto&& item : v) {
    //     cout << item.first << " " << item.second << endl;
    // }
    return v;

}
void out2terminal(vector <PSK> v, node* (*func)(vector <PSK>&)) {
    auto res = func(v);
    if (!res) {
        cout << "语法树为空" << endl;
        return;
    }
    cout << "前序遍历 \n";
    preorder(res);
    cout << endl;
    cout << "中序遍历 \n";
    inorder(res);
    cout << endl;
    cout << "后序遍历 \n";
    postorder(res);
    cout << endl;
}
void fpreorder(node* T, ofstream& f) {
    if (T) {
        if (T->type != -1)
            f << T->token << " ";
        if (T->firstchild) {
            fpreorder(T->firstchild, f);
        }
        if (T->nextsibling) {
            fpreorder(T->nextsibling, f);
        }
    }
}
void finorder(node* T, ofstream& f) {
    if (T) {
        if (T->firstchild) {
            finorder(T->firstchild, f);
        }
        if (T->type != -1)
            f << T->token << " ";
        if (T->nextsibling) {
            finorder(T->nextsibling, f);
        }
    }
}
void fpostorder(node* T, ofstream& f) {
    if (T) {
        if (T->firstchild) {
            fpostorder(T->firstchild, f);
        }
        if (T->nextsibling) {
            fpostorder(T->nextsibling, f);
        }
        if (T->type != -1)
            f << T->token << " ";
    }
}
void out2file(vector <PSK> v, node* (*func)(vector <PSK>&), string file) {
    auto res = func(v);
    // 以写的方式打开文件，没有就默认创建
    ofstream f(file);
    if (!f.is_open()) {
        cout << "文件打开失败" << endl;
        exit(0);
    }


    cout << file + "文件打开成功" << endl;
    f << "前序遍历 \n";
    fpreorder(res, f);
    f << endl;
    f << "中序遍历 \n";
    finorder(res, f);
    f << endl;
    f << "后序遍历 \n";
    fpostorder(res, f);
    f << endl;
    f.close();
}
bool check_conditon(node* t) {
    if (symbol_table.find(t->token) == symbol_table.end()) {
        throwError(ErrorCode::ConditonError, "条件中" + t->token + "未声明");
        return false;
    } else if (symbol_table[t->token] != 2) {
        throwError(ErrorCode::ConditonError, "条件中" + t->token + "不是bool类型");
        return false;
    } else if (initial_table.find(t->token) == initial_table.end()) {
        throwError(ErrorCode::ConditonError, "条件中" + t->token + "未初始化");
        return false;
    }

    return true;
}