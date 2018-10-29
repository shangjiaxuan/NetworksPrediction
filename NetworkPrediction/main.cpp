#include "Header.h"
#include "IO_Manager.h"

using namespace std;

int main() {
	cout << "Reading file...\n" << endl;
	chrono::time_point<chrono::steady_clock> start = std::chrono::steady_clock::now();
	network_data data = IO_Manager::read_file("Link prediction task.URL.txt");
	chrono::duration<double> dur = std::chrono::steady_clock::now() - start;
	cout << std::setprecision(9);
	cout << "Time used reading file:\n" << dur.count() << " seconds.\n" << endl;
	start = std::chrono::steady_clock::now();
	IO_Manager::write_sorted("out.txt", data);
	dur = std::chrono::steady_clock::now() - start;
	cout << std::setprecision(9);
	cout << "Time used writing file:\n" << dur.count() << " seconds.\n" << endl;
	start = std::chrono::steady_clock::now();
	network_data dest = data;
	dur = std::chrono::steady_clock::now() - start;
	cout << std::setprecision(9);
	cout << "Time used copying data:\n" << dur.count() << " seconds.\n" << endl;
	system("pause");
	return 0;
}
