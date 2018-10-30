#include "IO_Manager.h"

using namespace std;

int main() {
	cout << std::setprecision(9);
	cout << "Reading file...\n" << endl;
	chrono::time_point<chrono::steady_clock> start = std::chrono::steady_clock::now();
	const data_sets data{IO_Manager::read_file("Link prediction task.URL.txt")};
	chrono::duration<double> dur = std::chrono::steady_clock::now() - start;
	cout << "Time used reading file:\n" << dur.count() << " seconds.\n" << endl;
	start = std::chrono::steady_clock::now();
	for(unsigned i = 0; i < data.num; i++) {
		ostringstream oss;
		oss << i;
		IO_Manager::write_sorted(oss.str() + ".txt", data.pdata[i]);
	}
	dur = std::chrono::steady_clock::now() - start;
	cout << "Time writing data:\n" << dur.count() << " seconds.\n" << endl;
	system("pause");
	return 0;
}
