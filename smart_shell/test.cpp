/*
g++ -g -gdwarf-2 -std=c++11 -o test test.cpp -I/smart_lib/smart_shell -L/smart_lib/lib -lsmart_shell
*/
#include <iostream>
#include <algorithm>
#include <string.h>
#include "smart_shell.h"
using namespace std;
//=======================================
int main(){
	string output;
	auto re=sh_shell("echo -e '1\n2\n3'",output);
	cout<<re<<endl;
	cout<<output<<endl;
	return 0;
}
