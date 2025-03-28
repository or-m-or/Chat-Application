#pragma once
#include <thread>
#include <functional>

class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	void	Run(function<void(void)> callback);
	void	Join();

private:
	mutex			_lock;
	vector<thread>	_threads;
};

