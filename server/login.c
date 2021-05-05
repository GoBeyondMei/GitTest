/*******************************************************************************
* 服务器处理用户基本操作处理实现文件
* 2015-12-14 yrr实现
*
********************************************************************************/

#include "config.h"

/*声明全局变量 -- 在线用户链表*/
extern ListNode *userList;

/**************************************************
函数名：loginUser
功能：用户登陆函数实现
参数：msg--用户发送的登陆消息 sockfd--套接字描述符
返回值：成功登陆返回SUCCESS 否则返回异常类型
****************************************************/
static int callback(void *data, int argc, char **argv, char **azColName){
   printf("打印查询信息\n");

   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int loginUser(Message *msg , int sockfd) {
	int ret;
	/*声明用户信息*/
	User user;
	char buf[MAX_LINE];
	
	/*声明数据库变量*/
	sqlite3 *db;
	sqlite3_stmt *stmt;
	const char *tail;

	/*声明sql语句存储变量*/
	char sql[1000];

	/*存储操作结果消息*/
	Message message;

	/*接收用户信息*/
	recv(sockfd , buf , sizeof(user) , 0);
	memset(&user , 0 , sizeof(user));
	memcpy(&user , buf , sizeof(user));
	user.userAddr = (*msg).sendAddr;
	user.sockfd = sockfd;

	/*查看在线用户列表，该用户是否已在线*/
	if(isOnLine(userList , &user) == 1)
		return ALREADY_ONLINE;

	printf("进到数据库中\n");
	/*（1）打开数据库*/
	ret = sqlite3_open(DB_NAME, &db);
	if(ret != SQLITE_OK)
	{
		printf("unable open database.\n");
		return FAILED;
	}//if

	/*（2）检查登陆用户名和密码*/
	memset(sql , 0 , sizeof(sql));
	printf("接收到了参数userName=%s, password=%s\n", user.userName , user.password);
	sprintf(sql , "select userName,password from User where userName='%s' and password='%s';",user.userName , user.password);

	ret = sqlite3_prepare(db , sql , strlen(sql) , &stmt , &tail);	
	// char *zErrMsg = 0;
	// const char* data = "Callback function called";
	// ret = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

	if(ret != SQLITE_OK)
	{
		printf("44444444444\n");
		ret = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		printf("database select fail!\n");
		return FAILED;		
	}//if
	/*执行*/
	ret = sqlite3_step(stmt);
	//如果有数据则返回SQLITE_ROW，当到达末尾返回SQLITE_DONE
	while(ret == SQLITE_ROW)
	{
		printf("55555555555555\n");
		ret = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		ret = SUCCESS;
		/*如果登陆操作成功，添加到在线用户链表*/
		userList = insertNode(userList , &user);
		printf("77777777777777\n");
		return ret;
	}//while
	/*销毁句柄，关闭数据库*/
	sqlite3_finalize(stmt);
	sqlite3_close(db);

	return FAILED;
}





