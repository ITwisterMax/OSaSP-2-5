#pragma once

#include "CustomQueue.h"

class CustomHandler
{
public:
	CustomHandler(CustomQueue queue);
	void execute(int maxThreadCount);
private:
	CustomQueue customQueue;
};