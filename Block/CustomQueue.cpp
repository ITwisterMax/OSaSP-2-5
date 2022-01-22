#include "../Api/CustomQueue.h"
#include <mutex>

using namespace std;

mutex locker;

void CustomQueue::push(TTask task)
{
	// Atomic push new task
	locker.lock();
	tasksQueue->push(task);
	locker.unlock();
}

TTask CustomQueue::pop()
{
	TTask task;

	// Atomic pop new task
	locker.lock();

	if (tasksQueue->empty())
	{
		task = NULL;
	}
	else
	{
		task = tasksQueue->front();
		tasksQueue->pop();
	}

	locker.unlock();

	return task;
}