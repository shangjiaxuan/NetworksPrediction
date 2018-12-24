#include "IO_Manager.h"
#include "ThreadManager.h"
#include "Utilities.h"

#include <future>

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
	ost << std::setprecision(15);
	size_t size = val.size();
	for (unsigned j = 0; j < size; j++) {
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
	size_t size = source.size();
	for (size_t i = 0; i < size; i++) {
		for (int j = 0; j < 12; j++) {
			rtn[j] += source[i][j];
		}
	}
	return rtn;
}

std::vector<double> add_percentages(std::vector<std::vector<unsigned>>& data) {
	size_t overall_size = data.size();
	size_t rtn_size = 0;
	for (size_t i = 0; i < overall_size; i++) {
		rtn_size = std::max(rtn_size, data[i].size());
	}
	rtn_size /= 2;
	unsigned* same_friends = new unsigned[rtn_size]();
	unsigned* is_friend = new unsigned[rtn_size]();
	for (size_t i = 0; i < overall_size; i++) {
		size_t cur_size = ( data[i].size() >> 0x01 );
		for (size_t j = 0; j < cur_size; j++) {
			same_friends[j] += data[i][j << 0x01];
			is_friend[j] += data[i][( j << 0x01 ) + 1];
		}
	}
	std::vector<double> rtn(rtn_size);
	for (size_t i = 0; i < rtn_size; i++) {
		rtn[i] = double(is_friend[i]) / same_friends[i];
	}
	delete[] same_friends;
	delete[] is_friend;
	return rtn;
}

void write_fdist(std::ostream& ost, std::vector<double>& data) {
	size_t size = data.size();
	ost << std::setprecision(15);
	for (size_t i = 0; i < data.size(); i++) {
		ost << i << ":\t" << data[i]<<'\n';
	}
	ost << std::endl;
}


std::vector<item> sort_all_item(const std::vector<std::vector<item>>& data) {
	size_t size = data.size();
	std::vector<item> rtn;
	for (int i = 0; i < size; i++) {
		std::vector<item> temp;
		std::merge(rtn.begin(), rtn.end(), data[i].begin(), data[i].end(), std::back_inserter(temp), Algorithms::comp_weight);
		rtn = temp;
	}
	return rtn;
}


int main() {
	{
	//inputs and initializations
		cout << std::setprecision(15);
		cout << "Reading file...\n" << endl;
		Timer<normal> time;
		std::vector<network_data> data{ IO_Manager::read_file("Link prediction task.URL.txt") };
		size_t data_sets = data.size();
		cout << "Time used reading data:\t\t" << time.elapsed() << endl;
		//process data
		time.reset();
		std::vector<std::vector<clustering>> val = Thread_Manager<network_data, std::vector<clustering>>::vector_thread(data, Algorithms::find_clustering_coeff);
		std::vector<std::array<int, 12>> cdist_v = Thread_Manager<std::vector<clustering>, std::array<int, 12>>::vector_thread(val, Algorithms::find_clust_distrib);
		std::vector<std::vector<unsigned>> fdist0_v = Thread_Manager<network_data, std::vector<unsigned>>::vector_thread(data, Algorithms::count_friends_and_trios);
		std::vector<std::vector<item>> ans0_v = Thread_Manager<network_data, std::vector<item>>::vector_thread(data, Algorithms::find_using_pure_same_friends);
		std::vector<double> fdist0_all = add_percentages(fdist0_v);
		std::array<int, 12> all_cdist = add_up(cdist_v);
		std::vector<item> ans0 = sort_all_item(ans0_v);
		cout << "Time used processing data:\t" << time.elapsed() << endl;
		//outputs
		{
			time.reset();
			auto t1 = std::async(IO_Manager::write_sorted_data<network_data>, ".txt", data, write_network);
			auto t2 = std::async(IO_Manager::write_sorted_data<std::vector<clustering>>, ".clust", val, write_clustering);
			auto t3 = std::async(IO_Manager::write_sorted_data<std::array<int, 12>>, ".cdist", cdist_v, write_clustering_dist);
			ofstream ofs;
			ofs.open("networks_num.txt");
			for (size_t i = 0; i < data_sets; i++) {
				ofs << data[i].num_of_people << '\n';
			}
			ofs.close();
			ofs.open("all.cdist");
			write_clustering_dist(ofs, 0, all_cdist);
			ofs.close();
			ofs.open("fdist_0.txt");
			write_fdist(ofs, fdist0_all);
			ofs.close();
			ofs.open("ans0_s.txt");
			size_t num_of_edge = ans0.size();
			for (size_t i = 0; i < num_of_edge; i++) {
				ofs << ans0[i].d_person << '\t' << ans0[i].s_person << '\n';
//				for (unsigned j = 0; j < ans0[i].contact_list->num; j++) {
//					ofs << ans0[i].contact_list->data[j] << '\t';
//				}
				ofs << ans0[i].weight << "\n\n";
			}
			ofs.close();
			ofs.open("ans0.txt");
			for (size_t i = 0; i < 10000; i++) {
				ofs << ans0[i].d_person << ' ' << ans0[i].s_person << '\n';
			}
			ofs.close();
			t1.get();
			t2.get();
			t3.get();
			cout << "Time used writing output:\t" << time.elapsed() << endl;
		}
	}
	system("pause");
	return 0;
}
