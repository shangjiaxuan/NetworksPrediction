#include "ThreadManager.h"

unsigned Thread_Manager::workers = std::thread::hardware_concurrency() - 1;
unsigned Thread_Manager::thread_used = 0;


