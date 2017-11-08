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
	db_connection_conf conf=db_connection_conf("1","1","1","1","1");
	return 0;
}
