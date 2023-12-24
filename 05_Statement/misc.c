#include "data.h"
#include "decl.h"

// 确认当前Token中保存的是t，如果是，那么扫描下一个字符，如果不是，那么直接报错
void match(int t, char *what){
    if (Token.token == t){
        scan(&Token);
    }else{
        printf("%s expected on line %d\n", what, Line);
        exit(1);
    }
}

// 匹配一个;并获取下一个token
void semi(void){
    match(T_SEMI,";");
}
