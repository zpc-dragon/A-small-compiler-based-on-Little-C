#pragma once
#include"struct.h"
FILE* readfile(string filename) {
    // cout << "请输入源文件名:" << endl;
    // while (true) {
    //     cin >> filename;		//读取文件路径
    //     if ((fopen_s(&fp, filename.c_str(), "r")) == 0)		//打开文件
    //         break;
    //     else
    //         cout << "路径输入错误，请重新输入！" << endl;	//读取失败
    // }
    FILE* fp;				//文件指针
    fp = fopen(filename.c_str(), "r");
    if (fp == NULL) {
        cout << "文件打开失败！" << endl;
        exit(0);
    }

    return fp;
}
FILE* writefile(string filename) {

    FILE* fp1;				//文件指针
    fp1 = fopen(filename.c_str(), "w");

    if (fp1 == NULL) {
        cout << "文件打开失败！" << endl;
        exit(0);
    }
    return fp1;
}
/********************************************
* 功能:词法分析
* fp:文件指针
* code:单词对应的种别码
* token:用于存储单词
* row:单词所在的行数
*********************************************/
void lexicalAnalysis(FILE* fp, FILE* fp1) {
    char ch;			//用于存储从文件中获取的单个字符
    string preToken = "";	//用于存储上一个单词
    int negative_flag = 0;	//负号标记
    row = 1;		    //记录字符所在的行数

    while ((ch = fgetc(fp)) != EOF)	//未读取到文件尾，从文件中获取一个字符
    {
        token = ch;	//将获取的字符存入token中
        if (ch == ' ' || ch == '\t' || ch == '\n') {
            if (ch == '\n')							//遇到换行符，记录行数的row加1
                row++;
            continue;								//忽略空格、Tab和回车，继续执行循环
        } else if (isLetter(ch)) {		//以字母开头，可能是关键字或标识符
            token = "";					//token初始化
            while (isLetter(ch) || isDigit(ch))	//非字母或数字时退出，将单词存储在token中
            {
                token.push_back(ch);	//将读取的字符ch存入token中
                ch = fgetc(fp);			//获取下一个字符
            }

            fseek(fp, -1L, SEEK_CUR);//文件指针后退一个字节，即重新读取上述单词后的第一个字符
            if (isKey(token))	//关键字
                code = TokenCode(getKeyID(token));
            else if (token == "true")	//true常量
                code = TK_TRUE;
            else if (token == "false")	//false常量
                code = TK_FALSE;
            else	//标识符
                code = TK_IDENT;	//单词为标识符
        } else if (isDigit(ch)) {//无符号常数以数字开头
            int isdouble = 0;	//标记是否为浮点数
            token = negative_flag ? "-" : "";			//token初始化
            while (isDigit(ch))	//当前获取到的字符为数字
            {
                token.push_back(ch);		//读取数字，将其存入token中
                ch = fgetc(fp);				//从文件中获取下一个字符
                //该单词中第一次出现小数点
                if (ch == '.' && isdouble == 0) {
                    //小数点下一位是数字
                    if (isDigit(fgetc(fp))) {
                        isdouble = 1;		//标记该常数中已经出现过小数点
                        fseek(fp, -1L, SEEK_CUR);		//将超前读取的小数点后一位重新读取	
                        token.push_back(ch);			//将小数点入token中
                        ch = fgetc(fp);				//读取小数点后的下一位数字
                    }
                }
            }
            if (isdouble == 1)
                code = TK_DOUBLE;	//单词为浮点型
            else
                code = TK_INT;				//单词为整型
            //文件指针后退一个字节，即重新读取常数后的第一个字符
            fseek(fp, -1L, SEEK_CUR);
            negative_flag = 0;	//负号标记清零
        } else switch (ch) {
            /*运算符*/
            case '+':
                code = TK_PLUS;		//+加号			
                break;
            case '-':
                //判断是否是负号?
                //如果是负号，判断前一个字符是否是运算符或分隔符
                if (preToken == "" || preToken == "(" || preToken == "[" || preToken == "{" || preToken == "," || preToken == ";" || preToken == "=" || preToken == "==" || preToken == "<" || preToken == "<=" || preToken == ">" || preToken == ">=") {
                    negative_flag = 1;		//是负号
                    continue;                //忽略负号，继续执行循环
                } else
                    code = TK_MINUS;		//-减号
                break;
            case '*':
                code = TK_STAR;		//*乘号		
                break;
            case '/':
                // 去除注释
                ch = fgetc(fp);				//超前读取'/'后面的字符
                if (ch == '/') {			//单行注释
                    while (ch != '\n')	//读取到换行符为止
                        ch = fgetc(fp);
                    row++;				//行数加1
                    continue;			//忽略单行注释，继续执行循环
                } else if (ch == '*') {	//多行注释
                    while (true) {
                        ch = fgetc(fp);		//获取文件中的字符
                        if (ch == '\n')		//遇到换行符，行数加1
                            row++;
                        if (ch == '*') {	//遇到'*'，可能是多行注释结束符
                            ch = fgetc(fp);	//获取文件中的字符
                            if (ch == '/')	//多行注释结束符
                                break;		//退出循环
                        }
                    }
                    continue;			//忽略多行注释，继续执行循环
                } else {
                    code = TK_DIVIDE;		//除号
                    fseek(fp, -1L, SEEK_CUR);	//回退指针
                }
                break;
            case '=':
            {
                ch = fgetc(fp);				//超前读取'='后面的字符
                if (ch == '=') {           //==等于号
                    token.push_back(ch);	//将'='后面的'='存入token中
                    code = TK_EQ;			//单词为"=="
                } else {						//=赋值运算符
                    code = TK_ASSIGN;		//单词为"="
                    fseek(fp, -1L, SEEK_CUR);	//回退指针
                }
            }
            break;
            case '<':
            {
                ch = fgetc(fp);				//超前读取'<'后面的字符
                if (ch == '=')				//<=小于等于号
                {
                    token.push_back(ch);	//将'<'后面的'='存入token中
                    code = TK_LEQ;			//单词为"<="
                } else {						//<小于号
                    code = TK_LT;			//单词为"<"
                    fseek(fp, -1L, SEEK_CUR);	//回退指针
                }
            }
            break;
            case '>':
            {
                ch = fgetc(fp);				//超前读取'>'后面的字符
                if (ch == '=')				//>=大于等于号
                {
                    token.push_back(ch);	//将'>'后面的'='存入token中
                    code = TK_GEQ;			//单词为">="
                } else {						//>大于号
                    code = TK_GT;			//单词为">"
                    fseek(fp, -1L, SEEK_CUR);	//回退指针
                }
            }
            break;
            case ':':
            {
                ch = fgetc(fp);				//超前读取':'后面的字符
                if (ch == '=')				//:=赋值运算符
                {
                    token.push_back(ch);	//将':'后面的'='存入token中
                    code = TK_BASSIGN;		//单词为":="
                } else {
                    code = TK_UNDEF;		//未识别的符号
                    fseek(fp, -1L, SEEK_CUR);	//回退指针
                }
            }
            break;
            case '&':
            {
                ch = fgetc(fp);				//超前读取'&'后面的字符
                if (ch == '&')				//&&与
                {
                    token.push_back(ch);	//将'&'后面的'&'存入token中
                    code = TK_AND;			//单词为"&&"
                } else {
                    code = TK_UNDEF;		//未识别的符号
                    fseek(fp, -1L, SEEK_CUR);	//回退指针
                }
            }
            break;
            case '|':
            {
                ch = fgetc(fp);				//超前读取'|'后面的字符
                if (ch == '|')				//||或
                {
                    token.push_back(ch);	//将'|'后面的'|'存入token中
                    code = TK_OR;			//单词为"||"
                } else {
                    code = TK_UNDEF;		//未识别的符号
                    fseek(fp, -1L, SEEK_CUR);	//回退指针
                }
            }
            break;
            case '!':
            {
                ch = fgetc(fp);				//超前读取'!'后面的字符
                if (ch == '=')				//!=不等于号
                {
                    token.push_back(ch);	//将'!'后面的'='存入token中
                    code = TK_NEQ;			//单词为"!="
                } else {
                    code = TK_NOT;			//!非
                    fseek(fp, -1L, SEEK_CUR);	//回退指针
                }
            }
            break;
            /*分界符*/
            case '(': code = TK_OPENPA;		//(左圆括号
                break;
            case ')': code = TK_CLOSEPA;	//)右圆括号
                break;
            case '[': code = TK_OPENBR;		//[左中括号
                break;
            case ']': code = TK_CLOSEBR;	//]右中括号
                break;
            case '{': code = TK_BEGIN;		//{左大括号
                break;
            case '}': code = TK_END;		//}右大括号
                break;
            case ',': code = TK_COMMA;		//,逗号
                break;
            case ';': code = TK_SEMOCOLOM;	//;分号
                break;
                //未识别符号
            default: code = TK_UNDEF;
        }
        // print(code);				//打印词法分析结果
        write(code, fp1, row);			//将词法分析结果写入文件
        preToken = token;			//记录上一个单词

    }
}

void single_process(int index, string inpath, string outpath) {
    string input_file = inpath + to_string(index) + ".txt";
    string output_file = outpath + to_string(index) + ".txt";
    // cout << input_file << endl;
    // cout << output_file << endl;
    FILE* fp = readfile(input_file);		//读取文件
    FILE* fp1 = writefile(output_file);	//写入文件
    //词法分析
    lexicalAnalysis(fp, fp1);
    //关闭文件
    fclose(fp);
    fclose(fp1);
}
void batch_process(int n) {
    for (int i = 1; i <= n; i++) {
        single_process(i, "test/sourceProgram", "../task2/input/lex_out");
    }
    cout << "词法分析完成！" << endl;
}