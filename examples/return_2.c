// This is a comment.

#include "return_2.h"

#define DEBUG

#ifdef DEBUG
c = 2;
#endif

#ifndef DEBUG
c = 3;
#endif

int add(int a, int b){
    return a + b;
}

int main() {
    int f0 = 1;
    int f1 = 1;
    int f2 = 0;
    for (int i = 0; i <= (123-1); i++) {
        f2 =f0 + f1;
        f0 = f1;
        f1 = f2;
    }

    char hi[10] = "hi";

    char* b = "hello";

    int hello[5] = {1, 2, 3, 4, 5};

    if (f1 == 2) {
        f2 = 0;
    }
    else if (f2 == 0) {
        f1 = 2;
    }
    else {
        f0 = 0;
    }

    return 0;
}