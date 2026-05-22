#include <stdio.h>
#include "HelloWorld/hello_world.h"

int main(void){
    // learn start entry
    printf("start run learn test func\n");
    int choice;
    while(true){
        printf("\nsupport test choice:\n"
                "1. hello_world\n"
                "2. verify_EOF\n"
                "3. compress_spaces\n"
                "4. for_while_test\n"
                "5. static_test\n"
                "0. exit test\n"
            );
        printf("pls input test choice: \n");
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
            case 5: 
                staticTest();
                break;
            case 0:
                printf("exit learn test func\n");
                return 0;
            default:
                printf("invalid test choice: %d.\n", choice);
                break;
        }
    }
    
    return 0;
}