#include "IO_Manager.h"
#include "ThreadManager.h"
#include "Utilities.h"

using namespace std;

void write_network(ostream& ost, size_t index, const network_data& data) {
	for (unsigned i = 0; i < data.num_of_people; i++) {
		for (unsigned j = 0; j < data.num_of_people; j++) {
			if (data[i][j].num) {
				ost << data.people[i] << '\t' << data.people[j] << '\n';
				ost << data[i][j].num << '\t' << data[i][j].sum << '\n';
				for (unsigned k = 0; k < data[i][j].num; k++) {
					ost << data[i][j].data[k] << '\t';
				}
				ost << "\n\n";
			}
		}
	}
}

void write_clustering(ostream& ost, size_t i, const std::vector<clustering>& val) {
	ost << std::setprecision(9);
	for (unsigned j = 0; j < val.size(); j++) {
		ost << val[j].person << '\n';
		ost << val[j].cl_coeff << "\n\n";
	}
}


int main() {
	cout << std::setprecision(9);
	cout << "Reading file...\n" << endl;
	Timer<normal> time;
	std::vector<network_data> data{IO_Manager::read_file("Link prediction task.URL.txt")};
	cout << "Time used reading file:\n" << time.elapsed() << " seconds.\n" << endl;
	time.reset();
	IO_Manager::write_sorted_data(".txt", data, write_network);
	cout << "Time writing network data:\n" << time.elapsed() << " seconds.\n" << endl;
	time.reset();
	std::vector<std::vector<clustering>> val = Thread_Manager<network_data, std::vector<clustering>>::vector_thread(data, Algorithms::find_clustering_coeff);
	cout << "Time processing data:\n" << time.elapsed() << " seconds.\n" << endl;
	time.reset();
	IO_Manager::write_sorted_data<std::vector<clustering>>(".clust", val, write_clustering);
	cout << "Time writing clustering coefficient data:\n" << time.elapsed() << " seconds.\n" << endl;
	system("pause");
	return 0;
}
