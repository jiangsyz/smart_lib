#include "smart_mysql.h"
using namespace smart_lib;
//=======================================
//设置错误
void query_result::set_error(int state,string error){
	this->state=state;
	this->error=error;
}
//=======================================
//初始化一个错误的结果集
query_result::query_result(int state,string error){this->set_error(state,error);}
query_result::query_result(){}
//=======================================
//打印结果集
void query_result::display(){for(auto e:results) cout<<e<<endl;}
//=======================================
//初始化连接配置
db_connection_conf::db_connection_conf(string h,string u,string p,string d,string c){
	host=h;
	user=u;
	pass=p;
	dbname=d;
	charset=c;
}
//=======================================
//配置比较,用于在map中作为key查找
bool db_connection_conf::operator<(db_connection_conf conf)const{
	if(host<conf.host) return true;
	if(user<conf.user) return true;
	if(pass<conf.pass) return true;
	if(dbname<conf.dbname) return true;
	return false;
}
//=======================================
//初始化连接池
map<db_connection_conf,shared_ptr<smart_mysql>> smart_mysql::connect_pool=map<db_connection_conf,shared_ptr<smart_mysql>>();
//=======================================
//连接数据库,并设置编码
smart_mysql::smart_mysql(db_connection_conf conf){
	//初始化执行sql的互斥量
	pthread_mutex_init(&sql_mutex,NULL);
	//初始化句柄
	connection=mysql_init(NULL);
	//连接
	if(!mysql_real_connect(connection,conf.host.c_str(),conf.user.c_str(),conf.pass.c_str(),conf.dbname.c_str(),0,NULL,0)) return;
	//更改状态为连接成功
	state=1;
	//设置编码,成功更改状态为设置编码成功
	if(!mysql_query(connection,("SET NAMES "+conf.charset).c_str())) state=2;
}
//=======================================
//释放数据库句柄
smart_mysql::~smart_mysql(){if(state>0) mysql_close(connection);}
//=======================================
//通过配置,获取相应的smart_mysql实例的智能指针,同一份配置单例
shared_ptr<smart_mysql> smart_mysql::get_instance(db_connection_conf conf){
	//查找,找到直接返回
	auto itor=connect_pool.find(conf); if(itor!=connect_pool.end()) return itor->second;
	//找不到,创建并加入连接池
	connect_pool[conf]=shared_ptr<smart_mysql>(new smart_mysql(conf));
	//返回
	return connect_pool[conf];
}
//=======================================
//执行sql语句
query_result smart_mysql::query(string sql){
	//smart_mysql不可用,返回带有错误信息的query_result
	if(state==0) return query_result(-1,"smart_mysql connection error");
	if(state==1) return query_result(-1,"smart_mysql set charset error");
	if(state!=2) return query_result(-1,"smart_mysql state error");
	//加锁
	pthread_mutex_lock(&sql_mutex);
	//执行sql
	if(mysql_query(connection,sql.c_str())){
		//解锁
		pthread_mutex_unlock(&sql_mutex);
		//返回带有错误信息的query_result
		return query_result(-1,mysql_error(connection));
	}
	//初始化正确的query_result
	query_result result=query_result();
	//获取mysql结果集
	auto res=mysql_store_result(connection);
	//填充结果集数据
	if(res){
		//获取结果集行数
		result.rows=mysql_num_rows(res);
		//获取每一行的字段数
		result.cols=mysql_num_fields(res);
		//读取结果填充query_result
		MYSQL_ROW row;
		while((row=mysql_fetch_row(res))!=NULL) for(int i=0;i<result.cols;i++) result.results.push_back(string(row[i]));
		//释放mysql结果集
		mysql_free_result(res);
	}
	//解锁
	pthread_mutex_unlock(&sql_mutex);
	//返回正确的query_result
	return result;
}