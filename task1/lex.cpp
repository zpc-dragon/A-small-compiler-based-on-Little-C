/***********************************************
* 词法分析器
* 编译环境:Visual Studio 2017
***********************************************/

#include "lexical.h"
using namespace std;


int main() {
    // SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), \
        //     FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);	//字体恢复原来的颜色
    // setlocale(LC_ALL, "Chinese-simplified");
    batch_process(10);
    // single_process(10);

    return 0;
}
