// 定义一个名为extern_的宏，ifndef用于编译期避免extern_被重复定义
#ifndef extern_
#define extern_ extern
#endif

#include <stdio.h>

extern_ int     Line;
extern_ int	Putback;
extern_ FILE	*Infile;