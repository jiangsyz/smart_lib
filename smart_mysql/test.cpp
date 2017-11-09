/*
g++ -g -gdwarf-2 -std=c++11 -o test test.cpp -I/smart_lib/smart_mysql -I/usr/include/mysql/ -L/smart_lib/lib -lsmart_mysql
*/
#include <iostream>
#include <algorithm>
#include <string.h>
#include "smart_mysql.h"
using namespace smart_lib;
//=======================================
int main(){
	db_connection_conf conf=db_connection_conf("host","user","pass","db","charset");
	shared_ptr<smart_mysql> my_mysql=smart_mysql::get_instance(conf);
	auto result=my_mysql->query("SELECT * FROM `customer` ORDER BY `id` DESC  LIMIT 0,50;");
	cout<<result.flag<<endl;
	cout<<result.error<<endl;
	cout<<result.rows<<endl;
	cout<<result.cols<<endl;
	return 0;
}
