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

void write_clustering_dist(ostream& ost, size_t i, const std::array<int, 12>& data) {
	ost << "Num of '0':\t" << data[0]
		<< "\n0.0<x<=0.1:\t" << data[1]
		<< "\n0.1<x<=0.2:\t" << data[2]
		<< "\n0.2<x<=0.3:\t" << data[3]
		<< "\n0.3<x<=0.4:\t" << data[4]
		<< "\n0.4<x<=0.5:\t" << data[5]
		<< "\n0.5<x<=0.6:\t" << data[6]
		<< "\n0.6<x<=0.7:\t" << data[7]
		<< "\n0.7<x<=0.8:\t" << data[8]
		<< "\n0.8<x<=0.9:\t" << data[9]
		<< "\n0.9<x<=1.0:\t" << data[10]
		<< "\nNum of NAN:\t" << data[11] << '\n' << endl;
}

std::array<int, 12> add_up(const std::vector<std::array<int, 12>>& source) {
	std::array<int, 12> rtn{};
	for (int i = 0; i < source.size(); i++) {
		for (int j = 0; j < 12; j++) {
			rtn[j] += source[i][j];
		}
	}
	return rtn;
}

std::vector<int> find_number(std::vector<network_data>& source) {
	std::vector<int> rtn(source.size());
	for (int i = 0; i < source.size(); i++) {
		rtn[i] = source[i].num_of_people;
	}
	return rtn;
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
	std::vector<int> num_vec = find_number(data);
	ofstream ofs;
	ofs.open("networks_num.txt");
	for (int i = 0; i < num_vec.size(); i++) {
		ofs << num_vec[i] << '\n';
	}
	ofs.close();
	time.reset();
	std::vector<std::vector<clustering>> val = Thread_Manager<network_data, std::vector<clustering>>::vector_thread(data, Algorithms::find_clustering_coeff);
	cout << "Time processing data:\n" << time.elapsed() << " seconds.\n" << endl;
	time.reset();
	IO_Manager::write_sorted_data<std::vector<clustering>>(".clust", val, write_clustering);
	cout << "Time writing clustering coefficient data:\n" << time.elapsed() << " seconds.\n" << endl;
	time.reset();
	std::vector<std::array<int, 12>> dist_v = Thread_Manager<std::vector<clustering>, std::array<int, 12>>::vector_thread(val, Algorithms::find_clust_distrib);
	IO_Manager::write_sorted_data<std::array<int, 12>> ( ".cdist", dist_v, write_clustering_dist );
	std::array<int, 12> all_dist = add_up(dist_v);
	ofs.open("all.cdist");
	write_clustering_dist(ofs, 0, all_dist);
	ofs.close();
	system("pause");
	return 0;
}
