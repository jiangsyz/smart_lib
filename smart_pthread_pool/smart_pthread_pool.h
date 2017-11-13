/*
智能线程池
*/
#ifndef SMART_PTHREAD_POOL_H
#define SMART_PTHREAD_POOL_H
#include <memory>
#include <queue>
#include "smart_pthread.h"
using namespace std;
//=======================================
namespace smart_lib{
	class smart_pthread_pool{
		public:
			/*
			线程状态(0=正常)
			错误码:
			-1=初始化智能线程错误
			*/
			int state{0};
		private:
			//当前线程池中所允许并发存在的线程的最大数目
			unsigned int max{0};
			//维持空闲线程数量的下限
			unsigned int leisure_min{0};
			//维持空闲线程数量的上限
			unsigned int leisure_max{0};
			//线程池内所有线程数量
			unsigned int smart_pthread_num{0};
		private:
			//空闲队列
			queue<shared_ptr<smart_pthread>> leisure_list;
			//保护空闲队列leisure_list的互斥量
			pthread_mutex_t leisure_list_mutex;
			//有新空闲线程对象时被唤醒的条件变量
			pthread_cond_t leisure_cond;
		public:
			//构造
			smart_pthread_pool(unsigned int max,unsigned int leisure_min,unsigned int leisure_max);
			//析构
			~smart_pthread_pool();
		public:
			/*
			增加一个新的智能线程对象(0=成功)
			错误码:
			-1=线程数量超过上限制
			*/
			int add_smart_pthread();
			//获取一个空闲的智能线程去执行任务
			shared_ptr<smart_pthread> get_leisure_pthread();
			//将一个完成任务的智能线程移动到空闲队列
			void move_smart_pthread_to_leisure_list(shared_ptr<smart_pthread> busy_smart_pthread);
	};
}
//=======================================
#endif