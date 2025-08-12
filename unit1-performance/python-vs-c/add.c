// add.c
#include <stdio.h>

int add_numbers(int a, int b) {
    return a + b;
}

int main() {
    int result = add_numbers(5, 3);
    printf("Result: %d\n", result);
    return 0;
}
