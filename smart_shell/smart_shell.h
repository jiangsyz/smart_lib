/*
执行shell的一些方法
*/
#ifndef SMART_SHELL_H
#define SMART_SHELL_H
using namespace std;
//=======================================
struct shell_output{
	string standard_output;
	string error_output;
};
//=======================================
//作用：执行一个shell命令
bool sh_shell(string command);
//=======================================
//作用：执行一个shell命令,并获取输出
bool sh_shell(string command,struct shell_output &output);
//=======================================
#endif