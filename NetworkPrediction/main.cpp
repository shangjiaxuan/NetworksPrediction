#include "Header.h"
#include "FileReader.h"

using namespace std;

int main() {
	cout << "Reading file...\n" << endl;
	const chrono::time_point<chrono::steady_clock> start = std::chrono::steady_clock::now();
	FileReader::read_file("Link prediction task.URL.txt");
	chrono::duration<double> dur = std::chrono::steady_clock::now() - start;
	cout << std::setprecision(9);
	cout << "Time used reading file:\n" << dur.count() << " seconds.\n" << endl;
	system("pause");
	return 0;
}
