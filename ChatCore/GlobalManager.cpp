#include "pch.h"
#include "GlobalManager.h"
#include "ThreadManager.h"

ThreadManager* GThreadManager = nullptr;

GlobalManager::GlobalManager()
{
	GThreadManager = new ThreadManager();
}

GlobalManager::~GlobalManager()
{
	delete GThreadManager;
}

