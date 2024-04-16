#pragma once

#include "flasslib.h"
#include <unordered_set>

namespace flasslib {


	template<typename distance>
	class MyHNSW {

	public:

		MinimalStructuralInfo basicinfo;

		vector<SubGraphNode*> data_sub_memory_{ nullptr };
		MemoryManager* memoryManager;
		unordered_set<Idtype> visited_list_pool_;
		Quantizer* quantizer;

		/*
		TODO
		方法名称：构造函数
		*/
		MyHNSW(MinimalStructuralInfo& basicinfo) {


			this->basicinfo = basicinfo;

			basicinfo.cur_Subgraph_ = 0;

			offsetData_ = sizeof(neigh_num) + sizeof(labeltype) + basicinfo.subNeigh_max_ * sizeof(Idtype);
			this->basicinfo.size_sub_per_element_ = sizeof(neigh_num) + sizeof(labeltype) + basicinfo.subNeigh_max_ * sizeof(Idtype) + basicinfo.data_dim * sizeof(float);

			memoryManager = new MemoryManager(basicinfo);

		}
		

	protected:
		size_t offsetData_{ 0 };


		/*
		DONE
		传入一张子图，以及这张子图的某个元素的内部id，返回它在子图的邻居数和邻居id指针
		*/
		pair<neigh_num*, Idtype*> get_sub_ele_neigh_ByInternalId(Idtype internal_id, Idtype sub_id) const {
			pair<neigh_num*, Idtype*> res_ele_neigh;
			neigh_num* number = &(memoryManager->sub_memory[sub_id].elements[internal_id].neighNum);
			Idtype* neigh_id = memoryManager->sub_memory[sub_id].elements[internal_id].neighIDs;
			res_ele_neigh.first = number;
			res_ele_neigh.second = neigh_id;
			return res_ele_neigh;
		}

		/*
		TODO
		实现超图内的双向连接，只要传入两个需要连接的点即可
		*/
		inline void bi_directional_connections_in_hypergraph(Idtype subid1, Idtype eleid1, Idtype subid2, Idtype eleid2) {

			neigh_num& ele1_neigh_num = memoryManager->hyper_memory[subid1].elements[eleid1].neighNum;
			Idtype* ele1_neigh = memoryManager->hyper_memory[subid1].elements[eleid1].neighIDs;
			if (ele1_neigh_num < basicinfo.hyperNeigh_max_) {
				*(ele1_neigh + ele1_neigh_num * 2) = subid2;
				*(ele1_neigh + ele1_neigh_num * 2 + 1) = eleid2;
				memoryManager->hyper_memory[subid1].elements[eleid1].neighNum++;
			}
			else{

				Computer<float> com(memoryManager->hyper_memory[subid1].elements[eleid1].data, basicinfo.data_dim);

				dist_type farthest_dist = com(memoryManager->hyper_memory[subid2].elements[eleid2].data);

				int index = -1;

				for (size_t i = 0; i < ele1_neigh_num; i++)
				{
					Idtype neigh_subid = *(ele1_neigh + i * 2);
					Idtype neigh_eleid = *(ele1_neigh + i * 2 + 1);
					dist_type neigh_dist = com(memoryManager->hyper_memory[neigh_subid].elements[neigh_eleid].data);
					
					if (neigh_dist > farthest_dist) {
						farthest_dist = neigh_dist;
						index = i;

					}
				}
				if (index != -1) {
					*(ele1_neigh + index * 2) = subid2;
					*(ele1_neigh + index * 2 + 1) = eleid2;
				}

			}

			neigh_num& ele2_neigh_num = memoryManager->hyper_memory[subid2].elements[eleid2].neighNum;
			Idtype* ele2_neigh = memoryManager->hyper_memory[subid2].elements[eleid2].neighIDs;
			if (ele2_neigh_num < basicinfo.hyperNeigh_max_) {
				*(ele2_neigh + ele2_neigh_num * 2) = subid1;
				*(ele2_neigh + ele2_neigh_num * 2 + 1) = eleid1;
				memoryManager->hyper_memory[subid2].elements[eleid2].neighNum++;
			}
			else{
				Computer<float> com(memoryManager->hyper_memory[subid2].elements[eleid2].data, basicinfo.data_dim);

				dist_type farthest_dist = com(memoryManager->hyper_memory[subid1].elements[eleid1].data);

				int index = -1;

				for (size_t i = 0; i < ele2_neigh_num; i++)
				{
					Idtype neigh_subid = *(ele2_neigh + i * 2);
					Idtype neigh_eleid = *(ele2_neigh + i * 2 + 1);
					dist_type neigh_dist = com(memoryManager->hyper_memory[neigh_subid].elements[neigh_eleid].data);
					
					if (neigh_dist > farthest_dist) {
						farthest_dist = neigh_dist;
						index = i;
					}
				}
				if (index != -1) {
					*(ele2_neigh + index * 2) = subid1;
					*(ele2_neigh + index * 2 + 1) = eleid1;
				}

			}



		}

		/*
		TODO：后期不要使用get_sub_ele_neigh_ByInternalId(id1, sub_id)
		实现子图内的双向连接，只要传入两个需要连接的点即可
		*/
		inline void bi_directional_connections_in_subgraph(Idtype id1, Idtype id2, Idtype sub_id) {
			
			pair<neigh_num*, Idtype*> pair1 = get_sub_ele_neigh_ByInternalId(id1, sub_id);

			

			if (*pair1.first < basicinfo.subNeigh_max_) {
				*(pair1.second + *pair1.first) = id2;
				(*pair1.first)++;

				

			}else{

				Computer<float> com(memoryManager->sub_memory[sub_id].elements[id1].data, basicinfo.data_dim);


				pair<dist_type, int> farthest;
				farthest.first = com(memoryManager->sub_memory[sub_id].elements[id2].data);
				
				farthest.second = -1;


				for (int i = 0; i < basicinfo.subNeigh_max_; i++) {
					Idtype neighID = *(pair1.second + i);
					dist_type neighDist = com(memoryManager->sub_memory[sub_id].elements[neighID].data);


				}

				if (farthest.second != -1) {
					*(pair1.second + farthest.second) = id2;
				}

			}			

			pair<neigh_num*, Idtype*> pair2 = get_sub_ele_neigh_ByInternalId(id2, sub_id);

			if (*pair2.first < basicinfo.subNeigh_max_) {
				*(pair2.second + *pair2.first) = id1;
				(*pair2.first)++;
			}
			else
			{
				Computer<float> com(memoryManager->sub_memory[sub_id].elements[id2].data, basicinfo.data_dim);
				
				pair<dist_type, int> farthest;
				farthest.first = com(memoryManager->sub_memory[sub_id].elements[id2].data);
				farthest.second = -1;

				for (int i = 0; i < basicinfo.subNeigh_max_; i++) {
					Idtype neighID = *(pair2.second + i);
					dist_type neighDist = com(memoryManager->sub_memory[sub_id].elements[neighID].data);







					if (neighDist > farthest.first) {
						farthest.first = neighDist;
						farthest.second = i;

					}
				}
				if (farthest.second != -1) {
					*(pair2.second + farthest.second) = id1;
				}


			}

		}



	public:

		

		/*
		DOING
		方法名称：用点集初次构建一个子图（点集是二维的）
		方法特征：传入一群聚类的点，一次性把每个点都加载好，并且按顺序生成它的子图号；先全部保存到外存，然后缓冲区不满就放入缓冲区（默认此子图是第一层init，外存和内存是没有它的）
		传入的参数：原始数据*data_point_with_label，与原始数据一一对应的label，数据的个数
		可能的优化：一个HNSW理论上在构建的时候就已经指定computer了，computer的策略可以改变，另外computer应该被HNSW依赖，glass之所以依赖于HNSW，还是因为优化得不够彻底和大胆；本方法的另外一个弊端是，只能一次性读取全部的聚类点集然后再构建，能不能一部分一部分地构建呢？？？
		隐藏的风险：会不会存在同一个聚类被两次构建了呢？只能完全依赖聚类之后的输入数据完全不会出错吗？？？
		*/
		void buildSubgraph_init(float* data_points, size_t* data_numbers, size_t sub_num) {
			
			auto start = std::chrono::high_resolution_clock::now();;
			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

			labeltype enter_element_label = 0;
			size_t totalNum = 0;
			for (size_t t = 0; t < sub_num; t++) {
				totalNum += data_numbers[t];
			}

			quantizer = new SQ8Quantizer();
			quantizer->encode(data_points, basicinfo.data_dim, totalNum);

			for (size_t k = 0; k < sub_num; k++) {

				SubGraphNode newSub(data_numbers[k], k, basicinfo);

				memoryManager->updateSubByindex(k, newSub);

				bool flag = false;
				size_t hypercount = 0;


				for (size_t new_ele_id = 0; new_ele_id < data_numbers[k]; new_ele_id++)
				{

					if (enter_element_label % 1000 == 0) {
						end = std::chrono::high_resolution_clock::now();
						cout << "已经插入点数：" << enter_element_label;
						duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
						std::cout << "，Duration: " << duration.count() << " milliseconds." << std::endl;
						start = std::chrono::high_resolution_clock::now();
					}

					float* data_point = data_points + enter_element_label * basicinfo.data_dim;

					memcpy(memoryManager->sub_memory[k].elements[new_ele_id].data, data_point, basicinfo.data_dim * sizeof(float));

					memoryManager->sub_memory[k].elements[new_ele_id].label = enter_element_label;
					
					memoryManager->sub_memory[k].elements[new_ele_id].data_quant = quantizer->getCodeByLabel(enter_element_label);
					
					
					


					if (new_ele_id == 0)
					{						
						flag = true;
					}
					else {
						priority_queue< pair<dist_type, Idtype>, vector<std::pair<dist_type, Idtype>  >, CompareByFirst  > top_candidates = sub_search(new_ele_id - 1, k, data_point);


						while (top_candidates.size() > basicinfo.subNeigh_max_)
							top_candidates.pop();
						size_t cand_size = top_candidates.size();


						for (size_t i = 0; i < cand_size; i++) {
							Idtype neigh_id = top_candidates.top().second;

							bi_directional_connections_in_subgraph(new_ele_id, neigh_id,k);


							top_candidates.pop();
						}




						std::random_device rd;
						std::mt19937 gen(rd());
						std::uniform_int_distribution<> dis(0, 100);
						int randomNumber = dis(gen);
						if (randomNumber <= 1) {

							flag = true;

						}
					}

					if (flag == true) {

						if (enter_element_label > 499) {
							int a = 1;
						}
						HyperElementNode newHyperelement(basicinfo, k, hypercount, new_ele_id, enter_element_label);
						newHyperelement.data = (char*)data_point;
						memoryManager->hyper_memory[k].elements.push_back(newHyperelement);

						if (enter_element_label > 499) {
							int a = 1;
						}

						priority_queue< pair<dist_type, pair<Idtype, Idtype>>, vector<std::pair<dist_type, pair<Idtype, Idtype>>  >, CompareByFirst_hyper  >
							cand = hyper_search(0, 0, data_point, basicinfo.ef_hyper_);


						size_t cand_size = cand.size();


						if (enter_element_label > 499) {
							int a = 1;
						}

						for (size_t j = 0; j < cand_size; j++)
						{
							bi_directional_connections_in_hypergraph(k, hypercount, cand.top().second.first, cand.top().second.second);
							cand.pop();
						}

						flag = false;
						hypercount++;
						//cout << hypercount << endl;

						if (enter_element_label > 499) {
							int a = 1;
						}

					}
					enter_element_label++;
				}
				basicinfo.cur_Subgraph_++;
			}
		}
		
		
		

		/*
		DOING：
		方法名称：子图内查询
		传入参数：入口点id（这张子图范围内），子图id，查询点原始向量
		TODO：
		1.因为超图尚未构建，目前手动传入入口点，超图构建完毕后需要重写代码
		2.不具备搜索策略，不具备对删除点的操作
		3.sub_search和sub_search_quant是同一个方法的两种形式，后期需要合并它（使用搜索器合并）
		*/
		priority_queue< pair<dist_type, Idtype>, vector<std::pair<dist_type, Idtype>  >, CompareByFirst  >
			sub_search(Idtype ep_id, Idtype sub_id, const void* query_data, bool clearVisitPool = true) {


			


			std::priority_queue<
				std::pair<dist_type, Idtype>,
				std::vector<std::pair<dist_type, Idtype>>,
				CompareByFirst>
				top_candidates;

			std::priority_queue<
				std::pair<dist_type, Idtype>,
				std::vector<std::pair<dist_type, Idtype>>,
				CompareByFirst>
				candidate_set;

			dist_type lowerBound;

			ComputerInit* computer = new Computer<float>(query_data, basicinfo.data_dim);

			float* data_temp = (float*)memoryManager->sub_memory[sub_id].elements[ep_id].data;
			labeltype label_cur = memoryManager->sub_memory[sub_id].elements[ep_id].label;


			

			dist_type dist_temp = (*computer)(data_temp);
			lowerBound = dist_temp;

			if (visited_list_pool_.find(label_cur) != visited_list_pool_.end()) {
				return top_candidates;
			}

			top_candidates.emplace(dist_temp, ep_id);
			candidate_set.emplace(-dist_temp, ep_id);
			visited_list_pool_.insert(label_cur);

			while (!candidate_set.empty())
			{
				pair<dist_type, Idtype> curr_el_pair = candidate_set.top();

				if ((-curr_el_pair.first) > lowerBound && top_candidates.size() == basicinfo.ef_construction_) { 
					break;
				}

				candidate_set.pop();
				Idtype curr_id = curr_el_pair.second;
				
				pair<neigh_num*, Idtype*> curr_neigh_pair = get_sub_ele_neigh_ByInternalId(curr_id, sub_id);
				neigh_num* neighNumber = &(memoryManager->sub_memory[sub_id].elements[curr_id].neighNum);
				Idtype* neigh_id_point = (memoryManager->sub_memory[sub_id].elements[curr_id].neighIDs);

				for (size_t i = 0; i < *neighNumber; i++) {

					Idtype neigh_id = *(neigh_id_point + i);
					label_cur = memoryManager->sub_memory[sub_id].elements[neigh_id].label;

					if (visited_list_pool_.find(label_cur) != visited_list_pool_.end()) {continue; }//这个元素访问过了就不访问了,否则就把它的子图内id插入已访问节点池
					visited_list_pool_.insert(label_cur);

					

					dist_temp = (*computer)(memoryManager->sub_memory[sub_id].elements[neigh_id].data);

					


					if (top_candidates.size() < basicinfo.ef_construction_ || lowerBound > dist_temp) {

						if (dist_temp == 0) {
							cout << "距离可能出错，出错的子图是：" << sub_id << "，出错的label是" << label_cur << endl;
							cout << (visited_list_pool_.find(label_cur) != visited_list_pool_.end() ? "本来就有它" : "它是第一次") << endl;
						}

						candidate_set.emplace(-dist_temp, neigh_id);
						top_candidates.emplace(dist_temp, neigh_id);
						if (top_candidates.size() > basicinfo.ef_construction_) {

							top_candidates.pop();
						}
							
						if (!top_candidates.empty())
							lowerBound = top_candidates.top().first;
					}
				}
			}

			if(clearVisitPool)
				visited_list_pool_.clear();


			return top_candidates;

		}
		
		priority_queue< pair<dist_type, Idtype>, vector<std::pair<dist_type, Idtype>  >, CompareByFirst  >
			sub_search_quant(Idtype ep_id, Idtype sub_id, const void* query_data, bool clearVisitPool = false, ComputerInit* com = nullptr) {

			std::priority_queue<
				std::pair<dist_type, Idtype>,
				std::vector<std::pair<dist_type, Idtype>>,
				CompareByFirst>
				top_candidates;

			std::priority_queue<
				std::pair<dist_type, Idtype>,
				std::vector<std::pair<dist_type, Idtype>>,
				CompareByFirst>
				candidate_set;

			dist_type lowerBound;

			ComputerInit* computer;

			if (com != nullptr)
				computer = com;
			else
				computer = new Computer<float>(query_data, basicinfo.data_dim);

			char* data_temp = (char*)memoryManager->sub_memory[sub_id].elements[ep_id].data_quant;
			labeltype label_cur = memoryManager->sub_memory[sub_id].elements[ep_id].label;

			

			dist_type dist_temp = (*computer)(data_temp);
			lowerBound = dist_temp;

			if (visited_list_pool_.find(label_cur) != visited_list_pool_.end()) {
				return top_candidates;
			}

			top_candidates.emplace(dist_temp, ep_id);
			candidate_set.emplace(-dist_temp, ep_id);
			visited_list_pool_.insert(label_cur);

			while (!candidate_set.empty())
			{
				pair<dist_type, Idtype> curr_el_pair = candidate_set.top();

				if ((-curr_el_pair.first) > lowerBound && top_candidates.size() == basicinfo.ef_construction_) {
					break;
				}
				candidate_set.pop();
				Idtype curr_id = curr_el_pair.second;

				pair<neigh_num*, Idtype*> curr_neigh_pair = get_sub_ele_neigh_ByInternalId(curr_id, sub_id);
				neigh_num* neighNumber = curr_neigh_pair.first;
				Idtype* neigh_id_point = curr_neigh_pair.second;

				for (size_t i = 0; i < *neighNumber; i++) {

					Idtype neigh_id = *(neigh_id_point + i);
					label_cur = memoryManager->sub_memory[sub_id].elements[neigh_id].label;
					if (visited_list_pool_.find(label_cur) != visited_list_pool_.end()) {continue; }
					visited_list_pool_.insert(label_cur);



					dist_temp = (*computer)(memoryManager->sub_memory[sub_id].elements[neigh_id].data_quant);
					if (top_candidates.size() < basicinfo.ef_construction_ || lowerBound > dist_temp) {
						candidate_set.emplace(-dist_temp, neigh_id);
						top_candidates.emplace(dist_temp, neigh_id);
						if (top_candidates.size() > basicinfo.ef_construction_)
							top_candidates.pop();
						if (!top_candidates.empty())
							lowerBound = top_candidates.top().first;
					}
				}
			}

			if (clearVisitPool)
				visited_list_pool_.clear();

			return top_candidates;

		}


		/*
		超图内搜索，返回m个点的入口
		TODO：搞清楚超图的入口点是什么，目前选定的是第一张子图的第一个超点(因为0，0点一定在超图内)
		TODO：之后要移植到搜索器中去
		*/
		priority_queue< pair<dist_type, pair<Idtype, Idtype>>, vector<std::pair<dist_type, pair<Idtype, Idtype>>  >, CompareByFirst_hyper  >
			hyper_search(Idtype enterSub_id, Idtype enterEle_id, const void* query_data, size_t cand_ef) const {


			unordered_set<labeltype> visited_list_pool_;


			std::priority_queue<
				std::pair<dist_type, pair<Idtype, Idtype>>,
				std::vector<std::pair<dist_type, pair<Idtype, Idtype>>>,
				CompareByFirst_hyper>
				top_candidates;

			std::priority_queue<
				std::pair<dist_type, pair<Idtype, Idtype>>,
				std::vector<std::pair<dist_type, pair<Idtype, Idtype>>>,
				CompareByFirst_hyper>
				candidate_set;


			dist_type lowerBound;

			Computer<float> computer(query_data, basicinfo.data_dim);

			float* data_temp = (float*)(memoryManager->hyper_memory[enterSub_id].elements[enterEle_id].data);

			dist_type dist_temp = computer(data_temp);
			lowerBound = dist_temp;

			top_candidates.emplace(dist_temp, make_pair(enterSub_id, enterEle_id));
			candidate_set.emplace(-dist_temp, make_pair(enterSub_id, enterEle_id));
			visited_list_pool_.insert(memoryManager->hyper_memory[enterSub_id].elements[enterEle_id].label);

			while (!candidate_set.empty())
			{
				pair<dist_type, pair<Idtype, Idtype>> curr_el_pair = candidate_set.top();
				if ((-curr_el_pair.first) > lowerBound && top_candidates.size() == cand_ef) { break; }
				candidate_set.pop();

				pair<Idtype, Idtype> curr_subid_eleid = curr_el_pair.second;

				neigh_num& neighNumber = memoryManager->hyper_memory[curr_subid_eleid.first].elements[curr_subid_eleid.second].neighNum;
				Idtype* neighIDs = memoryManager->hyper_memory[curr_subid_eleid.first].elements[curr_subid_eleid.second].neighIDs;

				for (size_t i = 0; i < neighNumber; i++) {

					Idtype neigh_subid = *(neighIDs + i * 2);
					Idtype neigh_eleid = *(neighIDs + i * 2 + 1);
					
					if (visited_list_pool_.find(memoryManager->hyper_memory[neigh_subid].elements[neigh_eleid].label) != visited_list_pool_.end()) { continue; }
					visited_list_pool_.insert(memoryManager->hyper_memory[neigh_subid].elements[neigh_eleid].label);
					dist_temp = computer(memoryManager->hyper_memory[neigh_subid].elements[neigh_eleid].data);

					if (top_candidates.size() < cand_ef || lowerBound > dist_temp) {
						candidate_set.emplace(-dist_temp, make_pair(neigh_subid, neigh_eleid));
						top_candidates.emplace(dist_temp, make_pair(neigh_subid, neigh_eleid));
						if (top_candidates.size() > basicinfo.ef_construction_)
							top_candidates.pop();
						if (!top_candidates.empty())
							lowerBound = top_candidates.top().first;
					}
				}
			}
			visited_list_pool_.clear();

			return top_candidates;
		}
	








		/*
		搜索函数
		TODO：以后要内置到搜索器里去
		*/
		std::priority_queue<
			std::pair<dist_type, labeltype>,
			std::vector<std::pair<dist_type, labeltype>>,
			CompareByFirst> search(float* query) {

			
			std::priority_queue<
				std::pair<dist_type, labeltype>,
				std::vector<std::pair<dist_type, labeltype>>,
				CompareByFirst>
				res_candidates;

			priority_queue< pair<dist_type, pair<Idtype, Idtype>>, vector<std::pair<dist_type, pair<Idtype, Idtype>>  >, CompareByFirst_hyper  >
				hyper_cand = hyper_search(basicinfo.cur_Subgraph_-1, 0, query, basicinfo.ef_hyper_);

			size_t hyperindex = hyper_cand.size();

			ComputerInit* quantComputer = quantizer->getComputer(query);
			

			for (size_t i = 0; i < hyperindex; i++) {				

				std::priority_queue<
					std::pair<dist_type, Idtype>,
					std::vector<std::pair<dist_type, Idtype>>,
					CompareByFirst>
					top_candidates;

				top_candidates = sub_search_quant(hyper_cand.top().second.second, hyper_cand.top().second.first, query, false, quantComputer);


				size_t top_size = top_candidates.size();

				for (size_t j = 0; j < top_size; j++) {

					

					dist_type dist_temp = top_candidates.top().first;

					labeltype label_temp = memoryManager->sub_memory[hyper_cand.top().second.first].elements[top_candidates.top().second].label;

					res_candidates.emplace(make_pair(dist_temp, label_temp));

					if (res_candidates.size() > basicinfo.res_k)
						res_candidates.pop();

					top_candidates.pop();

				}

				hyper_cand.pop();

			}


			visited_list_pool_.clear();

			return res_candidates;

		}

		
		void coutSub(Idtype sid) {

			size_t count = memoryManager->sub_memory[sid].count;

			for (int i = 0; i < 200; i++) {


				for (int j = 0; j < 128; j++) {
					

					if (
						*((float*)(memoryManager->sub_memory[0].elements[i].data) + j) == *((float*)(memoryManager->sub_memory[2].elements[i].data) + j)
						)

						cout << "维度是一个,他们的地址是："<< memoryManager->sub_memory[0].elements[i].data<<"和"<< memoryManager->sub_memory[1].elements[i].data << endl;


				}

				cout << endl;

			}

		}

};


}