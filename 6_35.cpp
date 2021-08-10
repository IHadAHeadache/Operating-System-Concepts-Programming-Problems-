#include <cstdio>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>
#define N 10
std::mutex locker;
int barrier;
int init(int n)
{
	barrier = n;
	return 0;
}
int barrier_point(void)
{
	locker.lock();
	barrier--;
	locker.unlock();
	while(barrier != 0)
		;
	return 0;
}
int sleep(int t)
{
	std::chrono::seconds timespan(t);
	std::this_thread::sleep_for(timespan);
	printf("Finished %d\n",t);
	barrier_point();
	printf("Finished\n");
	return 0;
}
int main(void)
{
	std::vector<std::thread> threads;
	init(N);
	for(int i = 0; i < N;++i)
		threads.push_back(std::thread(sleep,i%5));
	for(auto& th : threads) th.join();
	return 0;
}
