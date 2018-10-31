#include "IO_Manager.h"
#include "Thread_Manager.h"

using namespace std;

struct numbered_data {
	network_data* data;
	int index;
};

int network_writer(const numbered_data& source) {
	ostringstream oss;
	oss << source.index;
	ofstream ofs;
	ofs.open(oss.str() + ".txt");
	if (!ofs) {
		cout << "Cannot open file for output!" << endl;
		return 1;
	}
	else {
		IO_Manager::write_sorted(ofs, *source.data);
	}
	ofs.close();
	return 0;
}

void write_network_data(const data_sets& source) {
	counted_array<numbered_data> pass;
	pass.num = source.num;
	pass.data = new numbered_data[source.num]();
	for (unsigned i = 0; i < source.num; i++) {
		pass.data[i].index = i;
		pass.data[i].data = &source.data[i];
	}
	counted_array<int> t = Thread_Manager<numbered_data, int>::work(pass, network_writer);
	delete[] t.data;
	delete[] pass.data;
}

struct numbered_clustering_array {
	counted_array<clustering>* data;
	int num;
};

int write_clustering_data(const numbered_clustering_array& source) {
	ostringstream oss;
	oss << source.num;
	ofstream ofs;
	ofs.open(oss.str() + ".clust");
	if (!ofs) {
		cout << "Cannot open file for output!" << endl;
		return 1;
	}
	else {
		ofs << std::setprecision(9);
		for (unsigned j = 0; j < source.data->num; j++) {
			ofs << source.data->data[j].person << '\n';
			ofs << source.data->data[j].cl_coeff << "\n\n";
		}
	}
	ofs.close();
	return 0;
}

void write_all_clustering_data(const counted_array<counted_array<clustering>>& source) {
	counted_array<numbered_clustering_array> pass;
	pass.num = source.num;
	pass.data = new numbered_clustering_array[source.num];
	for (unsigned i = 0; i < source.num; i++) {
		pass.data[i].num = i;
		pass.data[i].data = &source.data[i];
	}
	counted_array<int> val = Thread_Manager<numbered_clustering_array, int>::work(pass, write_clustering_data);
	delete[] pass.data;
	delete[] val.data;
}

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
	cout << "Time single threaded writing network data:\n" << dur.count() << " seconds.\n" << endl;
	start = std::chrono::steady_clock::now();
	write_network_data(data);
	dur = std::chrono::steady_clock::now() - start;
	cout << "Time used multi-threaded writing network data:\n" << dur.count() << " seconds.\n" << endl;
	start = std::chrono::steady_clock::now();
	counted_array<counted_array<clustering>> val=Thread_Manager<network_data, counted_array<clustering>>::work(data, Algorithms::find_clustering_coeff);
	dur = std::chrono::steady_clock::now() - start;
	cout << "Time processing data:\n" << dur.count() << " seconds.\n" << endl;
	start = std::chrono::steady_clock::now();
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
	dur = std::chrono::steady_clock::now() - start;
	cout << "Time single threaded writing clustering coefficient data:\n" << dur.count() << " seconds.\n" << endl;
	start = std::chrono::steady_clock::now();
	write_all_clustering_data(val);
	dur = std::chrono::steady_clock::now() - start;
	cout << "Time multi-threaded writing clustering coefficient data:\n" << dur.count() << " seconds.\n" << endl;
	system("pause");
	return 0;
}
