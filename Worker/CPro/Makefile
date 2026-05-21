# 定义编译器
CC = clang

# 定义编译选项
CFLAGS = -g -Wall

# 定义目标文件
TARGET = start

# 定义源文件（支持目录索引）
SOURCES = start.c
HELLO_WORLD_SOURCES = $(wildcard HelloWorld/*.c)

# 定义库文件名
LIBRARY = libhelloworld.a

# 定义目标文件（支持目录索引）
OBJECTS = $(HELLO_WORLD_SOURCES:.c=.o)

# 默认目标
all: $(TARGET)

# 构建静态库
$(LIBRARY): $(OBJECTS)
	$(CC) $(CFLAGS) -c $(HELLO_WORLD_SOURCES) -I HelloWorld/
	ar rcs $(LIBRARY) $(OBJECTS)

# 编译主程序并链接库
$(TARGET): $(SOURCES) $(LIBRARY)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) -L. -lhelloworld

# 清理生成的文件
clean:
	rm -f $(TARGET) $(LIBRARY) HelloWorld/*.o
