#pragma once
#include<iostream>
#include<string>
using namespace std;
/* 单词编码 */
enum TokenCode {
    /*未定义*/
    TK_UNDEF = 0,

    /* 关键字 */
    KW_VOID,	//void关键字
    KW_MAIN,	//main关键字
    KW_INT,		//int关键字
    KW_DOUBLE,	//double关键字
    KW_BOOL,	//bool关键字
    KW_FOR,		//for关键字
    KW_WHILE,	//while关键字
    KW_DO,		//do关键字

    KW_IF,		//if关键字
    KW_THEN,	//then关键字
    KW_ELSE,	//else关键字
    KW_RETURN,	//return关键字
    KW_READ,	//read关键字
    KW_WRITE,	//write关键字


    /* 运算符 */
    TK_PLUS,	//+加号
    TK_MINUS,	//-减号
    TK_STAR,	//*乘号
    TK_DIVIDE,	///除号
    TK_ASSIGN,	//=赋值运算符1
    TK_BASSIGN,	//:=赋值运算符2
    TK_EQ,		//==等于号
    TK_NEQ,		//!=不等于号
    TK_LT,		//<小于号
    TK_LEQ,		//<=小于等于号
    TK_GT,		//>大于号
    TK_GEQ,		//>=大于等于号
    TK_AND,        //&&与
    TK_OR,         //||或
    TK_NOT,        //!非

    /* 分隔符 */
    TK_OPENPA,	//(左圆括号
    TK_CLOSEPA,	//)右圆括号
    TK_OPENBR,	//[左中括号
    TK_CLOSEBR,	//]右中括号
    TK_BEGIN,	//{左大括号
    TK_END,		//}右大括号
    TK_COMMA,	//,逗号
    TK_SEMOCOLOM,	//;分号

    /* 常量 */
    TK_INT,		//整型常量
    TK_DOUBLE,	//浮点型常量
    TK_TRUE,    //true常量
    TK_FALSE,   //false常量

    /* 标识符 */
    TK_IDENT
};

/******************************************全局变量*****************************************************/
TokenCode code = TK_UNDEF;		//记录单词的种别码
const int MAX = 14;				//关键字数量
int row = 1;					//记录字符所在的行数
string token = "";				//用于存储单词
char  keyWord[][14] = {
    "void","main",
    "int","double","bool",
    "for","while",
    "if","then","do","else",
    "return","read","write"
};	//存储关键词

/**********************************************函数*****************************************************/

/********************************************
* 功能:打印词法分析的结果
* code:单词对应的种别码
* token:用于存储单词
* row:单词所在的行数
*********************************************/

void write(TokenCode code, FILE* fp, int row) {
    int kind = 0;
    switch (code) {
        /*未识别的符号*/
        case TK_UNDEF:
            break;
            /*关键字*/
        case KW_VOID:	//void关键字
        case KW_MAIN:	//main关键字
        case KW_INT:		//int关键字
        case KW_DOUBLE:	//double关键字
        case KW_BOOL:	//bool关键字
        case KW_FOR:		//for关键字
        case KW_WHILE:	//while关键字
        case KW_DO:		//do关键字
        case KW_IF:		//if关键字
        case KW_THEN:	//then关键字
        case KW_ELSE:	//else关键字
        case KW_RETURN:	//return关键字
        case KW_READ:	//read关键字
        case KW_WRITE:	//write关键字
            kind = 1;
            break;

            /* 运算符 */
        case TK_PLUS:	//+加号
        case TK_MINUS:	//-减号
        case TK_STAR:	//*乘号
        case TK_DIVIDE:	///除号
        case TK_ASSIGN:	//=赋值运算符
        case TK_BASSIGN:	//:=赋值运算符
        case TK_EQ:		//==等于号
        case TK_NEQ:		//!=不等于号
        case TK_LT:		//<小于号
        case TK_LEQ:	//<=小于等于号
        case TK_GT:		//>大于号
        case TK_GEQ:	//>=大于等于号
        case TK_AND:        //&&与
        case TK_OR:         //||或
        case TK_NOT:        //!非
            kind = 2;
            break;

            /* 分隔符 */
        case TK_OPENPA:	//(左圆括号
        case TK_CLOSEPA:	//)右圆括号
        case TK_OPENBR:	//[左中括号
        case TK_CLOSEBR:	//]右中括号
        case TK_BEGIN:	//{左大括号
        case TK_END:	//}右大括号
        case TK_COMMA:	//,逗号
        case TK_SEMOCOLOM:	//;分号
            kind = 3;
            break;

            /* 常量 */
        case TK_INT:	//整型常量
        case TK_DOUBLE:	//浮点型常量
        case TK_TRUE:    //true常量
        case TK_FALSE:   //false常量
            kind = 4;
            break;
            /* 标识符 */
        case TK_IDENT:
            kind = 5;
            break;
        default:
            break;
    }
    switch (kind) {
        case 0:
            fprintf(fp, "无法识别:%s, 第%d行\n", token.c_str(), row);
            break;
        case 1:
            fprintf(fp, "(%s, %d)\n", token.c_str(), code);
            break;
        case 2:
            fprintf(fp, "(%s, %d)\n", token.c_str(), code);
            break;
        case 3:
            fprintf(fp, "(%s, %d)\n", token.c_str(), code);
            break;
        case 4:
            if (token == "true") {
                fprintf(fp, "(true, %d)\n", code);
                break;
            }
            if (token == "false") {
                fprintf(fp, "(false, %d)\n", code);
                break;
            }

            if (token.find('.') == token.npos)
                fprintf(fp, "(%d, %d)\n", atoi(token.c_str()), code);					//单词为整型
            else
                fprintf(fp, "(%f, %d)\n", atof(token.c_str()), code);					//单词为浮点型

            break;
        case 5:
            fprintf(fp, "(%s, %d)\n", token.c_str(), code);
            break;
    }
}
/********************************************
* 功能:判断是否是关键字
* MAX:关键字数量
* token:用于存储单词
*********************************************/
bool isKey(string token) {
    for (int i = 0; i < MAX; i++) {
        if (token.compare(keyWord[i]) == 0)
            return true;
    }
    return false;
}

/********************************************
* 功能:返回关键字的内码值
* MAX:关键字数量
* token:用于存储单词
*********************************************/
int  getKeyID(string token) {
    for (int i = 0; i < MAX; i++) {
        if (token.compare(keyWord[i]) == 0)
            return i + 1;//关键字的内码值为keyWord数组中对应的下标加1
    }
    return -1;
}

/********************************************
* 功能:判断一个字符是否是字母
* letter:被判断的字符
*********************************************/
bool isLetter(char letter) {
    if ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z'))
        return true;
    return false;

}

/********************************************
* 功能:判断一个字符是否是数字
* digit:被判断的字符
*********************************************/
bool isDigit(char digit) {
    if (digit >= '0' && digit <= '9')
        return true;
    return false;
}

