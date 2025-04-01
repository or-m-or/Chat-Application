#include "pch.h"
#include "ThreadManager.h"

ThreadManager::ThreadManager()
{

}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Run(function<void()> callback)
{
	lock_guard<mutex> guard(_lock);

	_threads.push_back(thread([=]() // ���� ĸ�� - �� ����
		{
			callback();
		}));
}

void ThreadManager::Join()
{
	for (thread& tread : _threads)
	{
		if (tread.joinable())
			tread.join();
	}
	_threads.clear();
}
