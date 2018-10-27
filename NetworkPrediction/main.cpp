#include "Header.h"
#include "FileReader.h"

using namespace std;

int main() {
	string line;
	getline(cin, line);
	const auto start = std::chrono::steady_clock::now();
	FileReader::read_file(line);
	auto dur = std::chrono::steady_clock::now() - start;
	cout << dur.count() << endl;
	system("pause");
	return 0;
}
