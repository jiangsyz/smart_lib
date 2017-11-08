#include "smart_shell.h"
//=======================================
bool smart_lib::sh_shell(string command){
	//打开管道,执行命令
	auto popen_fd=popen(command.c_str(),"r"); 
	if(!popen_fd) return false;
	//关闭管道
	pclose(popen_fd); 
	return true;
}
//=======================================
bool smart_lib::sh_shell(string command,string &output){
	//初始化
	output.assign("");
	//打开管道
	auto popen_fd=popen(command.c_str(),"r");
	if(!popen_fd) return false;
	//读取
	char buf[sizeof(char)*1024]={'\0'};
	while(fread(buf,sizeof(char),1023,popen_fd)) output+=buf;
	//关闭管道
	pclose(popen_fd);
	//去除最后一个回车符
	output.replace(output.length()-sizeof(char),sizeof(char),"");
	return true;
}