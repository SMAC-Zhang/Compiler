#ifndef __SYLIB_H_
#define __SYLIB_H_

#include<stdio.h>
#include<stdarg.h>
#include<sys/time.h>
/* Input & output functions */
__int64_t getint();
__int64_t getch();
__int64_t getarray(__int64_t a);
void putint(__int64_t a);
void putch(__int64_t a);
void putarray(__int64_t n, __int64_t a);
#endif
