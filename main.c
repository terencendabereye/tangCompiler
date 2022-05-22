#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *ptr ;
    int val = strtol("0110",&ptr,2);
    printf("%d\n", val);
}