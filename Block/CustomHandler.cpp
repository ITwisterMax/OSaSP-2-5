#include <thread>
#include "../Api/CustomHandler.h"

using namespace std;

CustomHandler::CustomHandler(CustomQueue customQueue)
{
	CustomHandler::customQueue = customQueue;
}

void threadFunction(CustomQueue customQueue, int threadsCount)
{
	vector<thread> threads;

	// Get task and add it to thread
	while (threadsCount)
	{
		TTask task = customQueue.pop();

		if (task)
		{
			thread thr(task);
			threads.push_back(move(thr));

			threadsCount--;
		}
	}

	// Get result
	for (size_t i = 0; i < threads.size(); i++)
	{
		threads[i].join();
	}
}

void CustomHandler::execute(int threadsCount)
{
	// Create new thread with task
	thread newThread(threadFunction, customQueue, threadsCount);

	newThread.join();
}