//任务的虚基类
#include "smart_job.h"
using namespace smart_lib;
//=======================================
//给任务分配一个线程
smart_job::smart_job(shared_ptr<smart_pthread> my_pthread){this->my_pthread=my_pthread;}
//=======================================
//获取任务所分配的线程
shared_ptr<smart_pthread> smart_job::get_pthread(){return my_pthread;}