#pragma once

#include "refresherInterface.h"

//��ȡsublist��������subNode**���������ţ�����ʵ���滻k����ͼ������ˢ��һ��freq
//struct quantityRefresher :public Refresher {
//
//	quantityRefresher(MinimalStructuralInfo* structuralInfo, ofstream* out, istream* in) :Refresher(structuralInfo, out, in) {}
//
//
//	inline void refreshList(int* request_subid, size_t request_length, SubNode* subgraphs, size_t subgraphs_length) {
//		unordered_map<subgraph_index, SubNode*> locks;
//		//TODO��QPSӰ�졿����������ݽṹ���㷨��ʱ�临�Ӷȶ��ܸߣ��������Ӱ�����ܣ�������Ҫ���ۺ��Ż���
//		for (size_t i = 0; i < request_length; i++) {
//
//			for (size_t j = subgraphs_length - 1; j >= 0; j--)//�Ӻ���ǰ�ѣ���Ϊ������а���Ƶ���ϸ���
//			{
//				//�а�
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
//		//Ĭ�ϴ�������ʱ����������˵ģ����Խ�����ֻ��Ҫ������ͼ
//		for (size_t i = 0; i < request_length; i++) {
//
//			if (request_subid[i] == -1)
//				continue;
//
//			for (size_t j = 0; j < subgraphs_length; j++) {
//				//ȷ��Ҫ�滻����ͼ
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
//		//Ȼ��������������
//		sort(subgraphs, subgraphs + subgraphs_length, freqCompare);
//
//	}
//
//};