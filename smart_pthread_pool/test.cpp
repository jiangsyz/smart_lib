/*
g++ -g -gdwarf-2 -std=c++11 -o test test.cpp -L/smart_lib/lib -lsmart_pthread_pool
*/
#include <unistd.h>
#include "smart_pthread_pool.h"
using namespace smart_lib;
int main(){
	smart_pthread_pool pool=smart_pthread_pool(10,1,2);
	while(1){
		sleep(1000);
	}
}