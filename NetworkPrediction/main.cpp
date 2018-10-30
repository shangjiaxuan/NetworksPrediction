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
		ofs.close();
	}
	dur = std::chrono::steady_clock::now() - start;
	cout << "Time writing data:\n" << dur.count() << " seconds.\n" << endl;
	start = std::chrono::steady_clock::now();
	counted_array<counted_array<clustering>> val=Thread_Manager<network_data, counted_array<clustering>>::work(data, Algorithms::find_clustering_coeff);
	dur = std::chrono::steady_clock::now() - start;
	cout << "Time processing data:\n" << dur.count() << " seconds.\n" << endl;
	for (unsigned i = 0; i < val.num; i++) {
		ostringstream oss;
		oss << i;
		ofstream ofs;
		ofs.open(oss.str() + ".clust");
		if (!ofs) {
			cout << "Cannot open file for output!" << endl;
		}
		else {
			ofs << std::setprecision(9);
			for (unsigned j = 0; j < val.data[i].num; j++) {
				ofs << val.data[i].data[j].person << '\n';
				ofs << val.data[i].data[j].cl_coeff << "\n\n";
			}
		}
		ofs.close();
	}
	system("pause");
	return 0;
}
