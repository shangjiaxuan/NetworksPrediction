#include "IO_Manager.h"
#include "ThreadManager.h"

#ifdef DEBUG
#include "Utilities.h"
#endif

#include <future>

using namespace std;

//functions are called from heavy task to light task
constexpr std::array<vector<item>(*)( network_data& ), 7> calc_funcs{
	Algorithms::find_multiply_four_sums,
	Algorithms::find_multiply_four_nums,
	Algorithms::find_multiply_two_sums,
	Algorithms::find_multiply_two_nums,
	Algorithms::find_add_four_sums,
	Algorithms::find_add_four_nums,
	Algorithms::find_count_same_friends
};

void write_clustering(ostream& ost, size_t i, const std::vector<clustering>& val) {
	ost << std::setprecision(17);
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
	ost << std::setprecision(17);
	for (size_t i = 0; i < data.size(); i++) {
		ost << i << ":\t" << data[i]<<'\n';
	}
	ost << std::endl;
}


std::vector<item> sort_all_item(std::vector<std::vector<item>>& data) {
	size_t size = data.size();
	std::vector<item> rtn;
	for (size_t i = 0; i < size; i++) {
		std::vector<item> temp;
		temp.reserve(rtn.size() + data[i].size());
		std::merge(rtn.begin(), rtn.end(), data[i].begin(), data[i].end(), std::back_inserter(temp), Algorithms::comp_weight);
		rtn = std::move(temp);
	}
	return rtn;
}

bool comp_persons(const item& it1, const item& it2) {
	if (it1.s_person < it2.s_person) return true;
	if (it1.s_person > it2.s_person) return false;
	if (it1.d_person < it2.d_person) return true;
	if (it1.d_person > it2.d_person) return false;
	return it1.weight < it2.weight;
}


std::vector<item> merge_different(std::vector<item> v1, std::vector<item> v2) {
	std::vector<item> rtn;
	rtn.reserve(v1.size() + v2.size());
	size_t i = 0, j = 0;
	size_t m_i = v1.size();
	size_t m_j = v2.size();
	std::sort(v1.begin(), v1.end(), comp_persons);
	std::sort(v2.begin(), v2.end(), comp_persons);
	while (i < m_i && j < m_j) {
		if (v1[i].s_person == v2[j].s_person) {
			if (v1[i].d_person == v2[j].d_person) {
				rtn.emplace_back(item{ v1[i].s_person,v1[i].d_person,0,nullptr });
				i++; j++;
			} else if (v1[i].d_person < v2[j].d_person) {
				rtn.emplace_back(item{ v1[i].s_person,v1[i].d_person,0,nullptr });
				i++;
			} else {
				rtn.emplace_back(item{ v2[j].s_person,v2[j].d_person,0,nullptr });
				j++;
			}
		}
		else if (v1[i].s_person < v2[j].s_person) {
			rtn.emplace_back(item{ v1[i].s_person,v1[i].d_person,0,nullptr });
			i++;
		}
		else {
			rtn.emplace_back(item{ v2[j].s_person,v2[j].d_person,0,nullptr });
			j++;
		}
	}
	while (i < m_i) {
		rtn.emplace_back(item{ v1[i].s_person,v1[i].d_person,0,nullptr });
		i++;
	}
	while (j < m_j) {
		rtn.emplace_back(item{ v2[j].s_person,v2[j].d_person,0,nullptr });
		j++;
	}
	rtn.shrink_to_fit();
	return rtn;
}

bool insert_item(std::vector<item>& list, item inserted) {
	for (size_t i = 0; i < list.size(); i++) {
		if (list[i].s_person == inserted.s_person) {
			if (list[i].d_person == inserted.d_person) {
				return false;
			}
			if (list[i].d_person > inserted.d_person) {
				list.insert(list.begin() + i, { inserted.s_person,inserted.d_person,0, nullptr });
				return true;
			}
		} else if (list[i].s_person > inserted.s_person) {
			list.insert(list.begin() + i, { inserted.s_person,inserted.d_person,0, nullptr });
			return true;
		}
	}
	list.emplace_back(item{ inserted.s_person,inserted.d_person,0, nullptr });
	return true;
}

void write_ans(const std::string filename, const std::vector<item>& data) {
	std::ofstream ofs;
	ofs.open(filename);
	for (size_t i = 0; i < data.size(); i++) {
		ofs << data[i].s_person << '\t' << data[i].d_person << '\n';
	}
	ofs.close();
}

void write_item_vector(const std::string filename, const std::vector<item>& data) {
	std::ofstream ofs;
	ofs.open(filename);
	ofs << setprecision(17);
	for (size_t i = 0; i < data.size(); i++) {
		ofs << data[i].s_person << '\t' << data[i].d_person << '\n' << data[i].weight << '\n\n';
	}
	ofs.close();
}

int main() {
	//inputs and initializations
#ifdef DEBUG
	cout << std::setprecision(15);
	cout << "Reading file...\n" << endl;
	Timer<normal> time;
#endif
	std::vector<network_data> data{ IO_Manager::read_file("Link prediction task.URL.txt") };
	size_t data_sets = data.size();
#ifdef DEBUG
	cout << "Time used reading data:\t\t" << time.elapsed() << endl;
	cout << "Processing data, please wait:" << endl;
	//process data
	time.reset();
#endif
	std::array<std::vector<std::future<std::vector<item>>>, calc_funcs.size()> all_ans_vecs;
	for (size_t i = 0; i < calc_funcs.size(); i++) {
		all_ans_vecs[i] = 
			Thread_Manager::vector_async<network_data, std::vector<item>>
			(data, calc_funcs[i]);
	}
	std::vector<std::future<std::vector<clustering>>> val_f = Thread_Manager::vector_async<network_data, std::vector<clustering>>(data, Algorithms::find_clustering_coeff);
	std::vector<std::future<std::vector<unsigned>>> fdist_v_f = Thread_Manager::vector_async<network_data, std::vector<unsigned>>(data, Algorithms::count_friends_and_trios);
	std::array<std::vector<std::vector<item>>, calc_funcs.size()> ans_v_a;
	for (size_t i = 0; i < calc_funcs.size(); i++) {
		ans_v_a[i] =
			Thread_Manager::get_future_vector_reverse<std::vector<item>>(all_ans_vecs[i]);
	}
	std::vector<std::vector<clustering>> val = Thread_Manager::get_future_vector_reverse<std::vector<clustering>>(val_f);
	std::vector<std::vector<unsigned>> fdist_v = Thread_Manager::get_future_vector_reverse<std::vector<unsigned>>(fdist_v_f);
	std::vector<std::future<std::array<int, 12>>> cdist_v_f = Thread_Manager::vector_async<std::vector<clustering>, std::array<int, 12>>(val, Algorithms::find_clust_distrib);
	std::array<std::future<std::vector<item>>, calc_funcs.size()> ans_f_a = 
		Thread_Manager::array_async<
			std::vector<std::vector<item>>,
			std::vector<item>,
			calc_funcs.size()>
		(ans_v_a, sort_all_item);
	std::vector<double> fdist_all = add_percentages(fdist_v);
	std::vector<std::array<int, 12>> cdist_v = Thread_Manager::get_future_vector<std::array<int, 12>>(cdist_v_f);
	std::array<int, 12> all_cdist = add_up(cdist_v);
	std::array<std::vector<item>, calc_funcs.size()> ans_v=
		Thread_Manager::get_future_array_reverse<std::vector<item>,calc_funcs.size()>(ans_f_a);
	std::vector<item> ans_all;
	ans_all.reserve(10000 + calc_funcs.size() - 2);
	ans_all = merge_different(ans_v[0], ans_v[1]);
	for (size_t i = 0; ans_all.size() < 10000;  i++) {
		for (size_t j = 2; j < calc_funcs.size(); j++) {
			if (i < ans_v[j].size()) {
				insert_item(ans_all, ans_v[j][i]);
			}
		}
	}
#ifdef DEBUG
	cout << "Time used processing data:\t" << time.elapsed() << endl;
#endif
	//outputs
	{
#ifdef DEBUG
		time.reset();
#endif
		std::vector<std::future<void>> write_tasks;
		write_tasks.reserve(3 + 2 * calc_funcs.size() + 1);
		write_tasks.emplace_back(std::async(std::launch::async | std::launch::deferred, IO_Manager::write_sorted_data<network_data>, ".txt", data, IO_Manager::write_network));
		write_tasks.emplace_back(std::async(std::launch::async | std::launch::deferred, IO_Manager::write_sorted_data<std::vector<clustering>>, ".clust", val, write_clustering));
		write_tasks.emplace_back(std::async(std::launch::async | std::launch::deferred, IO_Manager::write_sorted_data<std::array<int, 12>>, ".cdist", cdist_v, write_clustering_dist));
		ofstream ofs1;
		ofs1.open("all.cdist");
		write_tasks.emplace_back(std::async(std::launch::async | std::launch::deferred, write_clustering_dist, std::ref(ofs1), 0, all_cdist));
		ofstream ofs2;
		ofs2.open("fdist_all.txt");
		write_tasks.emplace_back(std::async(std::launch::async | std::launch::deferred, write_fdist, std::ref(ofs2), std::ref(fdist_all)));
		for (size_t i = 0; i < calc_funcs.size(); i++) {
			ostringstream oss("ans");
			oss << i << ".txt";
			const string s1 = oss.str();
			write_tasks.emplace_back(std::async(std::launch::async | std::launch::deferred, write_ans, s1, std::ref(ans_v[i])));
			oss.str("ans");
			oss << i << "_s.txt";
			const string s2 = oss.str();
			write_tasks.emplace_back(std::async(std::launch::async | std::launch::deferred, write_item_vector, s2, std::ref(ans_v[i])));
		}
		write_tasks.emplace_back(std::async(std::launch::async | std::launch::deferred, write_ans, "all_ans.txt", ans_all));
		for (size_t i = 0; i < write_tasks.size(); i++) {
			write_tasks[i].wait();
		}
		ofs1.close();
		ofs2.close();
#ifdef DEBUG
		cout << "Time used writing output:\t" << time.elapsed() << endl;
#endif
	}
#ifdef DEBUG
	system("pause");
#endif
	return 0;
}
