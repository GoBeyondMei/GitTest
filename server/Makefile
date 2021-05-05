MYNAME = makefile
CC = gcc -Wall

objects = server.o config.o register.o list.o login.o chat.o

server: $(objects)
	$(CC) -g -o server $(objects) -lsqlite3 -lpthread

server.o: server.c config.h
	$(CC) -g -c server.c 

register.o: register.c config.h
	$(CC) -g -c register.c

login.o: login.c config.h
	$(CC) -g -c login.c

list.o: list.c config.h
	$(CC) -g -c list.c

config.o: config.c config.h
	$(CC) -g -c config.c

chat.o: chat.c config.h
	cc -c chat.c
#比较稳健的clean做法，表示clean是一个伪目标
.PHONY: clean

#前面-的意思是：也许某些文件出现问题，忽略，继续执行
clean:
	-rm server $(objects) 

