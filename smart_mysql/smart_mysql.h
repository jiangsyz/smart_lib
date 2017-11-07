#ifndef SMART_MYSQL_H
#define SMART_MYSQL_H
#include <mysql.h>
#include <pthread.h>
#include <vector>
using namespace smart_mysql;
//=======================================
//结果集
struct query_result
{
	//结果集是否可用
	bool flag;
	//错误信息
	std::string error;
	//结果集的行数和列数
	int rows;
	int cols;
	//存放结果集的容器
	vector<std::string> results;
	//打印结果集
	void display(){for(auto e:results) cout<<e<<endl;}
};
//=======================================
//连接配置
struct db_connection_conf{
	std::string host;
	std::string user;
	std::string pass;
	std::string dbname;
	std::string charset;
};
//=======================================
class smart_mysql{
	private:
		//连接池,一组相同的连接配置只允许一个smart_mysql实例
		static map<db_connection_conf,smart_mysql>connect_pool;
		//执行sql的互斥量
		pthread_mutex_t sql_mutex;
		//连接数据库,并设置编码
		smart_mysql(db_connection_conf conf){}
	public:
		static shared_ptr<smart_mysql> get_instance(db_connection_conf conf){}

};
//=======================================
#endif