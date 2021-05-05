/*******************************************************************************
* 服务器处理用户基本操作处理实现文件
* 2015-12-10 yrr实现
*
********************************************************************************/

#include "config.h"

/*********************************************
函数名：registerUser
功能：用户注册函数实现
参数：msg--用户发送的注册消息 sockfd--套接字描述符
返回值：成功登陆返回SUCCESS 否则返回异常类型
**********************************************/
int registerUser(Message *msg , int sockfd)
{
	int ret;
	/*声明用户需要的注册信息*/
	User user;
	char buf[MAX_LINE];
	
	/*声明数据库变量*/
	sqlite3 *db = NULL;
	sqlite3_stmt *stmt = NULL;
	const char *tail;
	char * errorMsg = 0;
	/*声明sql语句存储变量*/
	char sql[1000];

	/*当前系统时间*/
	time_t timeNow;

	/*存储操作结果消息*/
	Message message;

	/*接收用户注册信息 会一直阻塞 知道在client中register.c输入用户名和秘密 send()之后才会进到下面来 */
	recv(sockfd , buf , sizeof(user) , 0);
	memset(&user , 0 , sizeof(user));
	memcpy(&user , buf , sizeof(user));
	user.userAddr = (*msg).sendAddr;
	user.sockfd = sockfd;
	
	printf("接收到的用户名：%s, 密码：%s\n", user.userName, user.password);
	if(strlen(user.userName) > 20)
	{	
		return INVALID;
	}//if

	/*（1）打开数据库*/
	ret = sqlite3_open(DB_NAME, &db);
	if(ret != SQLITE_OK)
	{
		printf("unable open database.\n");
		return FAILED;
	}//if
	printf("Opened database successfully.\n");


	//下面出现问题
	
	printf("999\n");
	
	/*（2）检查要注册用户名是否已存在？*/
	memset(sql , 0 , sizeof(sql));
	sprintf(sql , "select * from User where userName='%s';",(user.userName));
	printf("数据库sql的长度：%d\n", strlen(sql));

	ret = sqlite3_prepare(db , sql , strlen(sql) , &stmt , &tail);
	
	if(ret != SQLITE_OK)
	{
		printf("88888888888\n");		
		ret = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		printf("database select fail!\n");
		return FAILED;
	}//if
	/*执行*/
	
	ret = sqlite3_step(stmt);
	//如果有数据则返回SQLITE_ROW，当到达末尾返回SQLITE_DONE
	 while (ret == SQLITE_ROW)
	 {
	     ret = sqlite3_step(stmt);
		 sqlite3_finalize(stmt);
		 sqlite3_close(db);
		 return FAILED;
	 }
	/*销毁句柄，关闭数据库*/
	sqlite3_finalize(stmt);
	printf("777777777\n");	
	/*执行插入操作*/
	memset(sql , 0 , sizeof(sql));
	time(&timeNow);
	sprintf(sql , "insert into User(userName , password , userAddr , sockfd , speak , registerTime) values('%s','%s','%s',%d, %d , '%s');",user.userName , user.password , inet_ntoa(user.userAddr.sin_addr),user.sockfd , YES, asctime(gmtime(&timeNow)));
	printf("数据库sql的长度：%d\n", strlen(sql));
	
	ret = sqlite3_prepare(db , sql , strlen(sql) , &stmt , &tail);
	// ret = sqlite3_exec(db, sql, NULL, 0, &errorMsg);	
	if(ret != SQLITE_OK)
	{
		printf("44444\n");	
		ret = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return FAILED;
	}//if
	printf("55555555\n");
	/*顺利注册*/
	ret = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	printf("6666666\n");
	/*注册成功*/	
	return SUCCESS;
}
