#include<stdio.h>
#include"libsysy.h"
/* Input & output functions */
__int64_t getint() {
    __int64_t t;
    scanf("%ld", &t);
    return t;
}

__int64_t getch() {   
    char c; 
    scanf("%c",&c);
    return (__int64_t)c;
}

__int64_t getarray(__int64_t a) {
    __int64_t* p = (__int64_t*)a;
    __int64_t n;
    scanf("%ld",&n);
    for(int i=0;i<n;i++)scanf("%ld",&p[i]);
    return n;

}

void putint(__int64_t a) {
    printf("%ld",a);
}

void putch(__int64_t a) {
    char c = (char) a;
    printf("%c",c); 
}

void putarray(__int64_t n, __int64_t a) {
    __int64_t* p = (__int64_t*)a;
    printf("%ld:",n);
    for(int i=0;i<n;i++)printf(" %ld",p[i]);
    printf("\n");
}


