#include <stdio.h>
#include "HelloWorld/hello_world.h"

int main(void){
    // learn start entry
    printf("start run learn test func\n");
    int choice;
    printf("support test choice:\n"
        "1. hello_world\n"
        "2. verify_EOF\n"
        "3. compress_spaces\n"
        "4. for_while_test\n"
    );
    printf("pls input test choice: ");
    scanf("%d", &choice);
    switch (choice){
        case 1: 
            hello_world();
            break;
        case 2: 
            verify_EOF();
            break;
        case 3: 
            compress_spaces(NULL);
            break;
        case 4: 
            for_while_test();
            break;
        default:
            printf("invalid test choice: %d.\n", choice);
            break;
    }
    return 0;
}