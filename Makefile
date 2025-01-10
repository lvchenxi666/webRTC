# ���������
CC = gcc

# �������ѡ��
CFLAGS = -Wall -g

# ������Ŀ����
PROJECT_NAME = noise_suppression_app

# ����Դ�ļ�
SRCS = main.c noise_suppression.c

# ����ͷ�ļ�
HDRS = noise_suppression.h dr_wav.h timing.h

# ����Ŀ���ļ�
OBJS = $(SRCS:.c=.o)

# Ĭ��Ŀ��
all: $(PROJECT_NAME)

# ����Ŀ���ļ����ɿ�ִ���ļ�
$(PROJECT_NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# ����Դ�ļ�����Ŀ���ļ�
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# �������ɵ��ļ�
clean:
	rm -f $(OBJS) $(PROJECT_NAME)

# αĿ�꣬������ʾ Makefile �еı���
print:
	@echo "CC: " $(CC)
	@echo "CFLAGS: " $(CFLAGS)
	@echo "PROJECT_NAME: " $(PROJECT_NAME)
	@echo "SRCS: " $(SRCS)
	@echo "HDRS: " $(HDRS)
	@echo "OBJS: " $(OBJS)