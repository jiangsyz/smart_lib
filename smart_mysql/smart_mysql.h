/*
g++ -g -gdwarf-2 -std=c++11 -shared -fpic -o libsmart_mysql.so smart_mysql.cpp -I/usr/include/mysql/ -L/usr/lib64/mysql/ -lmysqlclient
*/
#ifndef SMART_MYSQL_H
#define SMART_MYSQL_H
#include <mysql.h>
#include <pthread.h>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <memory>
using namespace std;
//=======================================
namespace smart_lib{
	//结果集
	struct query_result
	{
		//结果集是否可用
		bool flag{false};
		//错误信息
		string error{""};
		//结果集的行数和列数
		int rows{0};
		int cols{0};
		//存放结果集的容器
		vector<string> results;
		//初始化一个正确的结果集
		query_result();
		//初始化一个错误的结果集
		query_result(string err_msg);
		//打印结果集
		void display();
	};
	//=======================================
	//连接配置
	struct db_connection_conf{
		string host{""};
		string user{""};
		string pass{""};
		string dbname{""};
		string charset{""};
		//初始化连接配置
		db_connection_conf(string h,string u,string p,string d,string c);
		//配置比较,用于在map中作为key查找
		bool operator<(db_connection_conf conf)const;
	};
	//=======================================
	class smart_mysql{
		private:
			//smart_mysql实例是否可用,true=可用,false=异常
			bool flag{false};
			//数据库句柄
			MYSQL *connection{nullptr};
		private:
			//执行sql的互斥量
			pthread_mutex_t sql_mutex;
			//连接数据库,并设置编码
			smart_mysql(db_connection_conf conf);
		public:
			//连接池,一组相同的连接配置只允许一个smart_mysql实例
			static map<db_connection_conf,shared_ptr<smart_mysql>> connect_pool;
			//通过配置,获取相应的smart_mysql实例的智能指针,同一份配置单例
			static shared_ptr<smart_mysql> get_instance(db_connection_conf conf);
		public:
			//释放数据库句柄
			~smart_mysql();
			//执行sql语句
			query_result query(string sql);
	};
}
#endif