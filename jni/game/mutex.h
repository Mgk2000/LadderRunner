#ifndef MUTEX_H
#define MUTEX_H
#include <pthread.h>
class Mutex
{
public:
	Mutex();
	~Mutex();
	void lock();
	void unlock();
private:
	pthread_mutex_t _mutex;
};
#endif
