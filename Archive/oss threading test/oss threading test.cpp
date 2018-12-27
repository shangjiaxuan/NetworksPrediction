// oss threading test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <future>
#include <sstream>
#include <fstream>

using namespace std;

const std::vector<int> val{1, 2, 3, 4};
const std::vector<std::vector<int>> vals{ val, val, val };

void test_write(const std::vector<int>& source, const std::string& filename) {
	ofstream ofs;
	ofs.open(filename);
	for (int i = 0; i < source.size(); i++) {
		ofs << source[i] << '\t';
	}
	ofs << endl;
}

int main() {
	std::vector<std::future<void>> write_tasks;
	write_tasks.reserve(vals.size());
	for (int i = 0; i < vals.size(); i++) {
		ostringstream oss("test",ios::app);
		oss << i << ".txt";
		const string s1 = oss.str();
		cout << s1;
		write_tasks.emplace_back(std::async(std::launch::async | std::launch::deferred, test_write, std::ref(vals[i]), s1));
		oss.str("test");
		oss << i << "_s.txt";
		const string s2 = oss.str();
		cout << s2;
		write_tasks.emplace_back(std::async(std::launch::async | std::launch::deferred, test_write, std::ref(vals[i]), s2));
	}
	for (int i = 0; i < vals.size(); i++) {
		write_tasks[i].wait();
	}
    std::cout << "Done!\n"; 
	system("pause");
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
