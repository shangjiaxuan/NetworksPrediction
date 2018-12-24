#include"ThreadManager.h"

using namespace std;

int add_one(const int& a) {
	return a+1;
}

int main() {
	bool lock=false;
	vector<int> in{ 1 };
	vector<int> out2 = Thread_Manager<int,int>::vector_async(in, add_one);
	system("pause");
	return 0;
}


