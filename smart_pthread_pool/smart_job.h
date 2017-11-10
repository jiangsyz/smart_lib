//任务的虚基类
#ifndef SMART_JOB_H
#define SMART_JOB_H
#include <memory>
using namespace std;
//=======================================
namespace smart_lib{
	//智能线程
	class smart_pthread;
	//智能任务
	class smart_job{
		public:
			//给任务分配一个线程
			smart_job(shared_ptr<smart_pthread> my_pthread);
		private:
			//任务被分配的线程对象
			shared_ptr<smart_pthread> my_pthread{NULL};
		public:
			//获取任务所分配的线程
			shared_ptr<smart_pthread> get_pthread();
			//执行任务，子类只要自己实现它，通过多态就能做不一样的事情了
			virtual void *run(void *data)=0;
	};
}
//=======================================
#endif