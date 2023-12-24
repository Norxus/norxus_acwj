#include "data.h"
#include "decl.h"


// 返回字符c在字符串s中的位置，如果没有发现则返回-1
static int chrpos(char *s, int c){
    char *p;

    // 接受字符的整数值，比如'a'字符就会传入65，返回该字符在字符串中首次出现的位置的指针
    // 找不到则返回NULL
    p = strchr(s, c);

    // 如果p不为NULL，那么就返回c在字符串中的位置
    // 因为s是字符串的首地址，并且字符串是连续分配的，所以p - s就能得到字符c在字符串s中的索引。
    return (p ? p - s : -1);
}

// 从输入文件中返回下一个字符串
static int next(void){
    int c;

    // 如果Putback有值了，就直接用Putback的值，同时将Putback置0
    if (Putback){
        c = Putback;
        Putback = 0;
        return c;
    }

    c = fgetc(Infile);
    // 如果c == '\n'说明这一行读完了，行数+1
    if ('\n' == c)
        Line++;
    return c;
}

static void putback(int c){
    Putback = c;
}

// 跳过一些干扰字符
static int skip(void) {
    int c;

    // next会移动文件指针
    c = next();
    // 干扰项直接跳过
    while (' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c){
        c = next();
    }
    return (c);
}

// 扫描出int类型的数据
static int scanint(int c){
    int k, val = 0;

    while(((k = chrpos("0123456789", c)) >= 0)){
        val = val * 10 + k;
        c = next();
    }

    // 如果不是整数字符串，将它放回去
    // 之所以需要放回去是因为，下次同感fgetc获取的时候，就会获取到下一个字符。
    // 但这个字符因为不是整数就没有被处理，所以需要保存一下，用于后续的处理。
    putback(c);
    return val;
}

// 从输入文件中扫描出一个标识符，然后将其存在字符数组buf中，最后返回标识符的长度
// lim变量表示标识符限制的长度
static int scanident(int c, char *buf, int lim){
    int i = 0;

    // 标识符允许为字母、数字、下划线
    while (isalpha(c) || isdigit(c) || '_' == c){
        // 超过标识符的长度限制就会报错，如果正常那么直接添加到字符数组buf中
        if (i == lim - 1){
            printf("identifier too long on line %d\n", Line);
            exit(1);
        } else if (i < lim - 1){
            buf[i++] = c;
        }
        c = next();
    }

    // 如果遇到一个无效字符，说明标识符就到此位置了, 那么就直接退出循环
    // 并返回标识符的长度。
    putback(c);
    // 这个很重要，对于c语言中的字符串，需要一个结尾字符
    buf[i] = '\0';
    return (i);
}

// 获取关键字的类型，这里做了优化，首先判断字符串的首字符是否正确，如果不正确那么久不用再调用
// strcmp函数了
static int keyword(char *s){
    switch (*s) {
        case 'p':
            if (!strcmp(s, "print"))
                return (T_PRINT);
            break;
    }
    return (0);
}

// 这是读取下一个字符串，并将其转换为token形式
// 读取当前字符指针指向的字符串，然后将其类型或值填入token中，如果有效则返回1，否则退出
// 如果token有效则返回1，如果没有token剩余则返回0
int scan(struct token *t){
    int c, tokentype;

    // 跳过空格以及一些干扰字符
    // 移动到下一个有效字符
    c = skip();

    // 根据输入字符进行判断
    switch (c) {
        // 读完了直接返回
        case EOF:
            t->token = T_EOF;
            return (0);
        case '+':
            t->token = T_PLUS;
            break;
        case '-':
            t->token = T_MINUS;
            break;
        case '*':
            t->token=T_STAR;
            break;
        case '/':
            t->token = T_SLASH;
            break;
        case ';':
            t->token = T_SEMI;
            break;
        default:
            // 如果是整数，则获取整数值。
            if (isdigit(c)){
                t->intvalue = scanint(c);
                t->token=T_INTLIT;
                break;
            }else if(isalpha(c) || '_' == c){
                // 读取一个标识符
                scanident(c, Text, TEXTLEN);

                // 如果这个标识符是一个内置的关键字，返回这个token
                if((tokentype = keyword(Text))){
                    t->token = tokentype;
                    break;
                }

                //如果不是内置关键字，那么报错
                printf("Unrecognised symbol %s on line %d\n", Text, Line);
                exit(1);
            }
            printf("Unrecognised character %c on line %d\n",c,Line);
            exit(1);
    }

    // 发现了一个token
    return (1);

}