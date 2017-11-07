/*
执行shell的一些方法
g++ -g -gdwarf-2 -std=c++11 -shared -fpic -o libsmartshell.so smart_shell.cpp
*/
#ifndef SMART_SHELL_H
#define SMART_SHELL_H
#include <algorithm>
#include <string.h>
using namespace std;
//=======================================
//作用：执行一个shell命令
bool sh_shell(string command);
//=======================================
//作用：执行一个shell命令,并获取输出
bool sh_shell(string command,string &output);
//=======================================
#endif