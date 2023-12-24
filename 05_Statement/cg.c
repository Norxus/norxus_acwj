#include "data.h"
#include "decl.h"


// 定义可用寄存器和它们的名字
// 状态1表示reg可用，状态0表示reg不可用
static int freereg[4];
static char *reglist[4]= {"%r8", "%r9", "%r10", "%r11"};

// 设置所有寄存器可用
void freeall_registers(void)
{
    freereg[0] = freereg[1] = freereg[2] = freereg[3] = 1;
}

// 分配一个释放的的寄存器，返回这个寄存器的下标，如果没有可用寄存器直接退出
static int alloc_register(void){
    for (int i = 0; i < 4; i++){
        // 找到一个可用的寄存器，并将其分配出去
        if (freereg[i]){
            freereg[i] = 0;
            return (i);
        }
    }
    fprintf(stderr, "out of registers!\n");
    exit(1);
}

// 释放掉指定寄存器，不允许重复释放
static void free_register(int reg){
    // 如果这个寄存器已经可用了，就不应该再进行释放，这里直接退出
    if (freereg[reg] != 0){
        fprintf(stderr, "Error trying to free register %d\n", reg);
        exit(1);
    }
    // 释放掉这个寄存器
    freereg[reg] = 1;
}


// 把前置命令写入文件
//主要实现了一个 printint 函数，用于打印整数。main 函数仅包含了函数的初始设置，并没有实际的执行内容。
void cgpreamble(){
    freeall_registers();
    fputs(
            // 只是后面是汇编代码
            "\t.text\n"
            // 定义一个局部符号，存储下面的字符串常量%d\n
            ".LC0:\n"
            // 定义字符串常量"%d\n"，用于后面的 printf 函数。
            "\t.string\t\"%d\\n\"\n"
            // 定义一个函数 printint，该函数接受一个整数参数，将其打印到屏幕上。
            "printint:\n"
            // 将基指针寄存器（%rbp）的值压入堆栈
            "\tpushq\t%rbp\n"
            // 将栈指针寄存器（%rsp）的值复制到基指针寄存器（%rbp）。
            "\tmovq\t%rsp, %rbp\n"
            // 将栈指针寄存器减去16，为函数局部变量分配空间。
            "\tsubq\t$16, %rsp\n"
            // 将传递给函数的整数参数（%edi）存储在栈上。
            "\tmovl\t%edi, -4(%rbp)\n"
            // 将栈上存储的整数参数复制到累加器寄存器（%eax）。
            "\tmovl\t-4(%rbp), %eax\n"
            // 将累加器寄存器的值复制到源索引寄存器（%esi）。
            "\tmovl\t%eax, %esi\n"
            // 将字符串标签.LC0的地址加载到目标寄存器（%rdi）。
            "\tleaq	.LC0(%rip), %rdi\n"
            // 将 0 移动到累加器寄存器。
            "\tmovl	$0, %eax\n"
            // 调用 printf 函数，将整数参数以"%d\n"的格式输出。
            "\tcall	printf@PLT\n"
            // 空指令，不执行任何操作。
            "\tnop\n"
            // 将基指针寄存器的值赋给栈指针寄存器，然后将栈上的值弹出到基指针寄存器。
            "\tleave\n"
            // 从函数返回。
            "\tret\n"
            "\n"
            // 声明 main 函数的全局符号。
            "\t.globl\tmain\n"
            // 指示 main 是一个函数。
            "\t.type\tmain, @function\n"
            // 定义 main 函数。
            "main:\n"
            // 将基指针寄存器的值压入堆栈。
            "\tpushq\t%rbp\n"
            // 将栈指针寄存器的值复制到基指针寄存器。
            "\tmovq	%rsp, %rbp\n",
            Outfile);
}

// 把后置命令写入文件
// 主要逻辑是恢复栈，然后返回
void cgpostamble(){
    fputs(
            "\tmovl	$0, %eax\n"
            "\tpopq	%rbp\n"
            "\tret\n"
            ,Outfile);
}

// cpu加载操作
int cgload(int value){
    // 获取一个可用的寄存器
    int r = alloc_register();

    // 打印code并初始化它
    // 将value传入寄存器reglist[r]中
    fprintf(Outfile, "\tmovq\t$%d, %s\n",value,reglist[r]);
    return (r);
}

// cpu加操作
int cgadd(int r1, int r2){
    // 将reglist[r1]中的数和reglist[r2]中的数据相加并最后将结果存储到reglist[r2]中
    fprintf(Outfile, "\taddq\t%s, %s\n", reglist[r1], reglist[r2]);
    // 释放r1寄存器
    free_register(r1);
    return(r2);
}


// cpu减操作
int cgsub(int r1, int r2) {
    // reglist[r2] - reglist[r1]然后将结果存入r1中
    fprintf(Outfile, "\tsubq\t%s, %s\n", reglist[r2], reglist[r1]);
    // 释放掉r2
    free_register(r2);
    return(r1);
}

// 乘法
int cgmul(int r1, int r2) {
    // 乘完之后存入r2中
    fprintf(Outfile, "\timulq\t%s, %s\n", reglist[r1], reglist[r2]);
    free_register(r1);
    return(r2);
}

// 带符号除法
int cgdiv(int r1, int r2) {
    // 将r1中的值复制到%rax寄存器中
    fprintf(Outfile, "\tmovq\t%s,%%rax\n", reglist[r1]);
    // 用于将`%rax`寄存器中的符号位扩展到`%rdx`寄存器。
    // 这是因为`idivq`指令期望待除数在`%rdx`（高32位）和`%rax`（低32位）寄存器中
    fprintf(Outfile, "\tcqo\n");
    // 进行带符号除法
    fprintf(Outfile, "\tidivq\t%s\n", reglist[r2]);
    // 将商存储入r1中
    fprintf(Outfile, "\tmovq\t%%rax,%s\n", reglist[r1]);
    free_register(r2);
    return(r1);
}


// 打印，使用上面已经定义好的函数
void cgprintint(int r) {
    fprintf(Outfile, "\tmovq\t%s, %%rdi\n", reglist[r]);
    fprintf(Outfile, "\tcall\tprintint\n");
    free_register(r);
}

