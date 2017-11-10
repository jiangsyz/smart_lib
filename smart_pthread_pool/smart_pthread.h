//智能线程
#ifndef SMART_PTHREAD_H
#define SMART_PTHREAD_H
#include <memory>
#include <pthread.h>
#include <string.h>
#include "smart_job.h"
using namespace std;
//=======================================
namespace smart_lib{
	//智能线程池
	class smart_pthread_pool;
	//智能线程
	class smart_pthread{
		public:
			//线程号
			pthread_t thread_id{0};
			//线程状态(-1=开辟错误,1=正常)
			int state{1};
		private:
			//是否接到退出命令(false=没接到/true=接到),接到该命令意味着线程池想销毁该智能线程
			bool stop{false};
		private:
			//操纵智能线程对象的互斥量(比如分配任务，销毁线程对象等)
			pthread_mutex_t smart_pthread_mutex;
			//操纵智能线程对象的条件变量(比如分配任务，销毁线程对象等)
			pthread_cond_t smart_pthread_cond;
		private:
			//智能线程当前处理的任务
			shared_ptr<smart_job> my_job{NULL};
			//智能线程所在线程池
			shared_ptr<smart_pthread_pool> my_pthread_pool{NULL};
		public:
			//构造
			smart_pthread(shared_ptr<smart_pthread_pool> my_pthread_pool);
			//析构
			~smart_pthread();
		public:
			//智能线程启动
			void start();
			//智能线程等待分配任务
			void wait();
			//智能线程执行任务
			void do_job(){}
			//智能线程完成任务
			void finish_job(){}
	};
	//创建linux线程后的线程入口函数
	void* thread_function(void* arg);
}
//=======================================
#endif