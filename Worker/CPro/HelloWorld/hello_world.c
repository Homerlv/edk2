#include "hello_world.h"
/**
 * @brief learn input and output in c.
 * 
 * @return void 
 *
 **/

 void hello_world(void){
    printf("hello, let's start to learn how to input or out in c.\n");
    
    // 定义变量
    char name[50];
    int age;
    
    // 正确的输入方式：先用 printf 提示，再用 scanf 读取
    printf("input your name: ");
    scanf("%s", name);
    printf("your name is %s\n", name);
    
    printf("input your age: ");
    scanf("%d", &age);
    printf("your age is %d\n", age);
    
    float math=30.123, english=80.456;
    printf("your grade is:\n Math: %f\t English: %f\n", math, english); 
    printf("your grade is:\n Math: %.2f\t English: %.2f\n", math, english);
    printf("your grade is:\n Math: %.2lf\t English: %.2lf\n", math, english);
    printf("your grade is:\n Math: %.2le\t English: %.2le\n", math, english);
    printf("your grade is:\n Math: %3.2le\t English: %3.2le\n", math, english);
    printf("let's end this test.\n");
 }

void verify_EOF(void) {
    int c;
    printf("input some characters, end with Ctrl+D\n");
    while ((c = getchar()) != EOF) {
        if (c == '\n') continue;
        printf("get char: %c\n", c);
    }
    printf("EOF received.\n");
}

char* read_multiline_string(void){
    int c;
    char * buffer = NULL;
    size_t capacity = 0;
    size_t length = 0;
    getchar();
    printf("input a long string, end with Ctrl+D:\n");
    while ((c = getchar()) != EOF) {
        if (length + 1 >= capacity) {
            capacity = (capacity == 0) ? 16 : capacity * 2;
            buffer = realloc(buffer, capacity);
        }
        buffer[length++] = (char)c;
    }
    printf("EOF received.\n");
    return buffer;
}

char * compress_spaces(char * str){
    // 快慢指针法原地压缩空格
    if (str == NULL) str = read_multiline_string();
    size_t length = strlen(str);
    size_t write_index = 0;
    bool pre_is_space = false;
    for (size_t read_index=0; read_index<length; read_index++){
        if (str[read_index] == ' ') {
            if (!pre_is_space) str[write_index++] = str[read_index];
            pre_is_space = true;
        }
        else {
            str[write_index++] = str[read_index];
            pre_is_space = false;
        }
    }
    str[write_index] = '\0';
    printf("compressed string: %s\n", str);
    printf("compressed string length: %zu\n", write_index);
    return str;
}

size_t calculate_escaped_length(const char *str) {
    size_t len = 0;
    for (size_t i = 0; str[i]; i++) {
        if (str[i] == '\t' || str[i] == '\n'  || str[i] == '\\' || str[i] == '\b') {
            len += 2; // "\\t" or "\\n"
        } else {
            len += 1;
        }
    }
    return len;
}

void for_while_test(void){
    printf("this test will do: \n"
        "1.input a long string, this test will output space count, tab count and line count.\n"
        "2. this test will replace consecutive spaces with one space.\n"
        "3. this test will replace tab with '\\t', line with '\\n'\n."
        );
    // 1.  count space, tab, line
    printf("input a long string, end with Ctrl+D:\n");
    int space_count = 0, tab_count = 0, line_count = 0;
    char * line = NULL;
    line = read_multiline_string();
    size_t length = strlen(line);
    char * output = malloc(calculate_escaped_length(line) + 1);
    size_t output_index = 0;
    for (int i = 0; i < length; i++){
        if (line[i] == ' ') {
            space_count++;
        }
        else if (line[i] == '\t') {
            tab_count++;
            output[output_index++] = '\\';
            output[output_index++] = 't';
        }
        else if (line[i] == '\n') {
            line_count++;
            output[output_index++] = '\\';
            output[output_index++] = 'n';
        }
        else if (line[i] == '\\') {
            output[output_index++] = '\\';
            output[output_index++] = '\\';
        }
        else if (line[i] == '\b') {
            output[output_index++] = '\\';
            output[output_index++] = 'b';
        }
        else {
            output[output_index++] = line[i];
        }
    }
    output[output_index] = '\0';
    printf("escaped string: %s\n", output);
    printf("escaped string length: %zu\n", output_index);
    compress_spaces(line);
    printf("space count: %d, tab count: %d, line count: %d\n", space_count, tab_count, line_count);
}