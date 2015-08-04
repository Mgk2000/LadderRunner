#include "mutex.h"
Mutex::Mutex()
{
	pthread_mutex_init(&_mutex, 0);
}
Mutex::~Mutex()
{
	pthread_mutex_destroy(&_mutex);
}
void Mutex::lock()
{
	pthread_mutex_lock(&_mutex);
}
void Mutex::unlock()
{
	pthread_mutex_unlock(&_mutex);
}
