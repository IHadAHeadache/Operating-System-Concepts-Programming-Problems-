#include <cstdio>
#include <thread>
#include <mutex>
#include <condition_variable>
#define MAX_RESOURCES 5
std::mutex m;
std::condition_variable cond_var;
int available_resources = MAX_RESOURCES;
int decrease_count(int count){
	std::unique_lock<std::mutex> lock(m);
	if(available_resources < count){
		cond_var.wait(lock);
		return -1;
	}
	else {
		available_resources -= count;
		return 0;
	}
}
int increase_count(int count){
	std::unique_lock<std::mutex> lock(m);
	available_resources += count;
	cond_var.notify_one();
	return 0;
}
void test(void)
{
	while(1)
	{
		decrease_count(3);
		//if(available_resources < 0)
			printf("%d\n",available_resources);
		increase_count(3);
	}
}
int main(void)
{
	std::thread worker1(test);
	std::thread worker2(test);
	worker1.join();
	worker2.join();
	return 0;
}
