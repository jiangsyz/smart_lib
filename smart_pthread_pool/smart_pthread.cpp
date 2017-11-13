//智能线程
#include "smart_pthread.h"
#include "smart_pthread_pool.h"
using namespace smart_lib;
//=======================================
//构造
smart_pthread::smart_pthread(shared_ptr<smart_pthread_pool> my_pthread_pool){
	//确认线程池
	this->my_pthread_pool=my_pthread_pool;
	//初始化互斥量和条件变量
	pthread_mutex_init(&smart_pthread_mutex,NULL);
	pthread_cond_init(&smart_pthread_cond,NULL);
	//创建linux线程成功
	if(pthread_create(&thread_id,NULL,thread_function,(void *)this)==0) return;
	//创建失败修改状态
	state=-1;
}
//=======================================
//析构
smart_pthread::~smart_pthread(){}
//=======================================
//智能线程启动
void smart_pthread::start(){
	//使该智能线程拥有的linux线程可以被取消
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	//等待分配任务或等待被线程池通知销毁
	while(1){
		wait();
		//没有任务意味着被通知销毁了直接退出循环结束该linux线程
		if(!my_job) return;
		//执行任务
		do_job();
		//完成任务
		finish_job();
	}
}
//=======================================
//等待分配任务或等待被线程池通知销毁
void smart_pthread::wait(){
	pthread_mutex_lock(&smart_pthread_mutex);
	//等待任务或等待被线程池通知销毁
	while(!my_job && !stop) pthread_cond_wait(&smart_pthread_cond,&smart_pthread_mutex);
	pthread_mutex_unlock(&smart_pthread_mutex);
}
//=======================================
//智能线程执行任务
void smart_pthread::do_job(){my_job->run();}
//=======================================
//智能线程完成任务
void smart_pthread::finish_job(){
	pthread_mutex_lock(&smart_pthread_mutex);
	//清空任务
	my_job=NULL;
	//将智能线程移回线程池空闲队列
	my_pthread_pool->move_smart_pthread_to_leisure_list(shared_ptr<smart_pthread>(this));
	pthread_mutex_unlock(&smart_pthread_mutex);
}
//=======================================
//创建linux线程后的线程入口函数
void* smart_lib::thread_function(void* arg){((smart_pthread *)arg)->start();}