#include <stdio.h>
#include "hello_world.h"

/**
 * @brief learn static variable in c.
 * 
 * @return void 
 *
 **/

void staticTest(void){
    int a = 0;
    static int b=0;
    a++;
    b++;
    printf("a = %d, b = %d\n", a, b);
}