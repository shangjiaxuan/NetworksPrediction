#include "Header.h"
#include "IO_Manager.h"
#include "Algorithms.h"

using namespace std;

int main() {
	cout << std::setprecision(9);
	cout << "Reading file...\n" << endl;
	chrono::time_point<chrono::steady_clock> start = std::chrono::steady_clock::now();
	network_data data = IO_Manager::read_file("Link prediction task.URL.txt");
	chrono::duration<double> dur = std::chrono::steady_clock::now() - start;
	cout << "Time used reading file:\n" << dur.count() << " seconds.\n" << endl;
	start = std::chrono::steady_clock::now();
	IO_Manager::write_sorted("out.txt", data);
	dur = std::chrono::steady_clock::now() - start;
	cout << "Time used writing file:\n" << dur.count() << " seconds.\n" << endl;
	start = std::chrono::steady_clock::now();
	data_sets sets = Algorithms::separate_sets(data);
	dur = std::chrono::steady_clock::now() - start;
	cout << "Time used separating data:\n" << dur.count() << " seconds.\n" << endl;
	start = std::chrono::steady_clock::now();
	for (int i = 0; i < sets.num; i++) {
		ostringstream oss;
		oss << i;
		IO_Manager::write_sorted(oss.str()+".txt", sets.pdata[i]);
	}
	dur = std::chrono::steady_clock::now() - start;
	cout << "Time writing separated data:\n" << dur.count() << " seconds.\n" << endl;
	system("pause");
	return 0;
}
