//线程池
#include "smart_pthread_pool.h"
using namespace smart_lib;
//=======================================
//构造
smart_pthread_pool::smart_pthread_pool(unsigned int max,unsigned int leisure_min,unsigned int leisure_max){
	this->max=max;
	this->leisure_min=leisure_min;
	this->leisure_max=leisure_max;
	pthread_mutex_init(&leisure_list_mutex,NULL);
	pthread_cond_init( &leisure_cond,NULL);
	//初始化一定数量的线程
	for(int i=0;i<max;i++){if(add_smart_pthread()!=0) state=-1;}
}
//=======================================
//析构
smart_pthread_pool::~smart_pthread_pool(){}
//=======================================
//增加一个新的智能线程对象
int smart_pthread_pool::add_smart_pthread(){
	//线程数量超过上限制
	if(max<=smart_pthread_num) return -1;
	//创建一个新的线程对象
	shared_ptr<smart_pthread> new_pthread=shared_ptr<smart_pthread>(new smart_pthread(shared_ptr<smart_pthread_pool>(this)));
	//把新的线程对象压入空闲队列
	leisure_list.push(new_pthread);
	//线程池的线程总量+1
	smart_pthread_num++;
	//线程池当前数量=1,意味着在创建该线程之前已经没有空闲线程,所以在创建后,要唤醒leisure_cond,通知等待线程的任务
	if(leisure_list.size()==1) pthread_cond_signal(&leisure_cond);
	return 0;
}
//=======================================
//获取一个空闲的智能线程去执行任务
shared_ptr<smart_pthread> smart_pthread_pool::get_leisure_pthread(){
	//锁定空闲队列
	pthread_mutex_lock(&leisure_list_mutex);
	//没有空闲队列则等待
	while(leisure_list.size() == 0) pthread_cond_wait(&leisure_cond,&leisure_list_mutex);
	//取到第一个空闲的智能线程
	auto leisure_smart_pthread=leisure_list.front();
	//删除出空闲队列
	leisure_list.pop();
	//解锁空闲队列
	pthread_mutex_unlock(&leisure_list_mutex);
	//返回智能线程
	return leisure_smart_pthread;
}
//=======================================
//将一个完成任务的智能线程移动到空闲队列
void smart_pthread_pool::move_smart_pthread_to_leisure_list(shared_ptr<smart_pthread> busy_smart_pthread){
	//锁定空闲队列
	pthread_mutex_lock(&leisure_list_mutex);
	//加入空闲队列
	leisure_list.push(busy_smart_pthread);
	//通知等待线程的任务
	pthread_cond_signal(&leisure_cond);
	//解锁空闲队列
	pthread_mutex_unlock(&leisure_list_mutex);
}


