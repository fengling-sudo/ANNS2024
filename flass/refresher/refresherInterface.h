#pragma once
//子图内存队列的更新策略器

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

	//在外存element_count_per_sub_指定id的地方写入元素数
	void writeSubCountToFile(size_t subid, size_t newNum) {
		//writeWithOffset(*out, newNum, offset_structuralInfo + subid * sizeof(size_t));
	}

	//在外存指定位置写入或覆盖子图
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
	方法名称：从外存拿取一张子图到内存，用新子图（号）替换旧子图（号）
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

		//TODO:理论上，这里需要写入增删改操作之后，对旧图的元素个数和子图的更新，但是这里没有实现，暂时只是完成查询和构建的过程


		slot_sub->count = count;
		slot_sub->subgraphID = new_sub_id;
		slot_sub->freq = freq;
		//slot_sub->elements = elements;


	}

	/*bool freqCompare(SubNode* a, SubNode* b) const {
		return a->freq < b->freq;
	}*/



};
