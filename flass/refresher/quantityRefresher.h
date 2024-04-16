#pragma once

#include "refresherInterface.h"

//获取sublist传过来的subNode**，队列重排，并且实现替换k个子图，再先刷新一下freq
//struct quantityRefresher :public Refresher {
//
//	quantityRefresher(MinimalStructuralInfo* structuralInfo, ofstream* out, istream* in) :Refresher(structuralInfo, out, in) {}
//
//
//	inline void refreshList(int* request_subid, size_t request_length, SubNode* subgraphs, size_t subgraphs_length) {
//		unordered_map<subgraph_index, SubNode*> locks;
//		//TODO【QPS影响】：这里的数据结构和算法的时间复杂度都很高，如果极度影响性能，后期需要讨论和优化！
//		for (size_t i = 0; i < request_length; i++) {
//
//			for (size_t j = subgraphs_length - 1; j >= 0; j--)//从后往前搜，因为后面的中靶率频率上更高
//			{
//				//中靶
//				if (request_subid[i] == subgraphs[j].subgraphID) {
//
//					subgraphs[j].freq[0]++;
//
//					pair<subgraph_index, SubNode*> temp_p;
//					temp_p.first = j;
//					temp_p.second = &subgraphs[j];
//					locks.insert(temp_p);
//
//					request_subid[i] = -1;
//
//					break;
//				}
//			}
//
//		}
//		//默认传进来的时候是排序好了的，所以接下来只需要更新子图
//		for (size_t i = 0; i < request_length; i++) {
//
//			if (request_subid[i] == -1)
//				continue;
//
//			for (size_t j = 0; j < subgraphs_length; j++) {
//				//确定要替换的子图
//				if (locks.find(j) != locks.end())
//					continue;
//
//				float* freq_temp = new float[1];
//				freq_temp[0] = 1;
//
//				getSubToMermory(request_subid[i], freq_temp, &subgraphs[j]);
//
//				pair<subgraph_index, SubNode*> temp_p;
//				temp_p.first = j;
//				temp_p.second = &subgraphs[j];
//				locks.insert(temp_p);
//				break;
//
//			}
//
//		}
//
//		//然后对数组进行排序
//		sort(subgraphs, subgraphs + subgraphs_length, freqCompare);
//
//	}
//
//};