MYNAME = makefile
CC = gcc

objects = client.o config.o interface.o register.o login.o chat.o
server: $(objects)
	cc -g -o client $(objects) -lsqlite3 -lpthread

client.o: client.c config.h
	gcc -c client.c 

register.o: register.c config.h
	cc -c register.c

interface.o: interface.c config.h
	cc -c interface.c

config.o: config.c config.h
	cc -c config.c
login.o: login.c config.h
	cc -c login.c
chat.o: chat.c config.h
	cc -c chat.c
#比较稳健的clean做法，表示clean是一个伪目标
.PHONY: clean

#前面-的意思是：也许某些文件出现问题，忽略，继续执行
clean:
	-rm client $(objects) 

