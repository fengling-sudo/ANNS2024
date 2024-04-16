#pragma once
//��ͼ�ڴ���еĸ��²�����

#include "../graphlib.h"

enum class RefreshPriority { quantity, recency };


struct Refresher
{

public:
	MinimalStructuralInfo* structuralInfo;
	size_t offset_structuralInfo;
	ofstream* out;
	istream* in;

	Refresher(MinimalStructuralInfo* structuralInfo, ofstream* out, istream* in) {
		this->structuralInfo = structuralInfo;
		offset_structuralInfo = sizeof(structuralInfo);
		this->out = out;
		this->in = in;
	}

	//�����element_count_per_sub_ָ��id�ĵط�д��Ԫ����
	void writeSubCountToFile(size_t subid, size_t newNum) {
		//writeWithOffset(*out, newNum, offset_structuralInfo + subid * sizeof(size_t));
	}

	//�����ָ��λ��д��򸲸���ͼ
	/*void writeSubToFile(SubNode* subNode, Idtype subid) {
		size_t size_perSub = structuralInfo->max_element_count_per_sub_ * structuralInfo->size_sub_per_element_;
		writeWithOffset(
			*out,
			subNode->elements,
			offset_structuralInfo + structuralInfo->max_Subgraph_ * sizeof(size_t) + subid * size_perSub,
			size_perSub
		);
	}*/

	/*
	�������ƣ��������ȡһ����ͼ���ڴ棬������ͼ���ţ��滻����ͼ���ţ�
	*/
	inline void getSubToMermory(Idtype new_sub_id, float* freq, SubGraphNode* slot_sub) {
		size_t count;
		/*readWithOffset(
			*in,
			count,
			offset_structuralInfo + new_sub_id * sizeof(size_t)
		);
		size_t elements_size = structuralInfo->max_element_count_per_sub_ * structuralInfo->size_sub_per_element_;
		char* elements = (char*)malloc(elements_size);
		readWithOffset(
			*in,
			elements,
			offset_structuralInfo + structuralInfo->max_Subgraph_ * sizeof(size_t) + new_sub_id * elements_size,
			elements_size
		);*/

		//TODO:�����ϣ�������Ҫд����ɾ�Ĳ���֮�󣬶Ծ�ͼ��Ԫ�ظ�������ͼ�ĸ��£���������û��ʵ�֣���ʱֻ����ɲ�ѯ�͹����Ĺ���


		slot_sub->count = count;
		slot_sub->subgraphID = new_sub_id;
		slot_sub->freq = freq;
		//slot_sub->elements = elements;


	}

	/*bool freqCompare(SubNode* a, SubNode* b) const {
		return a->freq < b->freq;
	}*/



};
