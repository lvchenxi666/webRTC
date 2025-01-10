# 定义编译器
CC = gcc

# 定义编译选项
CFLAGS = -Wall -g

# 定义项目名称
PROJECT_NAME = noise_suppression_app

# 定义源文件
SRCS = main.c noise_suppression.c

# 定义头文件
HDRS = noise_suppression.h dr_wav.h timing.h

# 定义目标文件
OBJS = $(SRCS:.c=.o)

# 默认目标
all: $(PROJECT_NAME)

# 链接目标文件生成可执行文件
$(PROJECT_NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# 编译源文件生成目标文件
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm -f $(OBJS) $(PROJECT_NAME)

# 伪目标，用于显示 Makefile 中的变量
print:
	@echo "CC: " $(CC)
	@echo "CFLAGS: " $(CFLAGS)
	@echo "PROJECT_NAME: " $(PROJECT_NAME)
	@echo "SRCS: " $(SRCS)
	@echo "HDRS: " $(HDRS)
	@echo "OBJS: " $(OBJS)