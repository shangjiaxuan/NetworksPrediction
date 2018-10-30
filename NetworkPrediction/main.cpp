#include "IO_Manager.h"
#include "Thread_Manager.h"

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
		ofstream ofs;
		ofs.open(oss.str() + ".txt");
		if(!ofs) {
			cout << "Cannot open file for output!" << endl;
		}
		else {
			IO_Manager::write_sorted(ofs, data.data[i]);
		}
	}
	dur = std::chrono::steady_clock::now() - start;
	cout << "Time writing data:\n" << dur.count() << " seconds.\n" << endl;
	start = std::chrono::steady_clock::now();
	Thread_Manager<network_data, sorted_items>::work(data, Algorithms::func1);
	dur = std::chrono::steady_clock::now() - start;
	cout << "Time processing data:\n" << dur.count() << " seconds.\n" << endl;
	system("pause");
	return 0;
}
