#pragma once

/*
文件名称：subgraphMermory.h
最新修改日期：2024/4/9
文件内容概述：定义子图的“身份证”，实现不同的子图freq更新策略，实现子图的内外存调取策略
文件关系：被flasslib.h依赖，依赖于grahplib文件
*/

//#include "graphlib.h"
//
//namespace flasslib {
//
//	enum class RefreshPriority { quantity, recency };
//
//	//FREQ数组用于存储不同计算策略的基础信息
//	typedef float* FREQARR;
//
//
//
//	/*
//	方法名称：总数优先策略
//	方法概述：每一次被调用，该子图的freq+1
//	*/
//	void quantityPriority(FREQARR freq) { freq[0]++; }
//
//	/*
//	TODO:尚未实现
//	方法名称：近期优先策略
//	方法概述：没有被调用，则enterTotal++，被调用则enterTotal++，hitTotal++，以此为更新freq的依据:freq = hit/total;
//	*/
//	void recencyPriority(FREQARR ifhit_total_hitnum) { cout << "TODO" << endl; }
//
//
//
//
//
//	//子图身份证节点
//	typedef struct SubNode {
//		Idtype subgraphID;
//		size_t count;
//		char* elements;
//		FREQARR freq;
//		void(*refresh_func)(FREQARR);
//
//		//刷新策略装配在SubNode里合理，还是装配到SubgraphsMemoryInitializer里合理？
//		SubNode(RefreshPriority refreshpriority, Idtype subgraphID, size_t count) {
//			switch (refreshpriority)
//			{
//			case flasslib::RefreshPriority::quantity:
//			{
//				float temp[1] = { 1 };
//				freq = temp;
//				refresh_func = quantityPriority;
//				break;
//			}
//			case flasslib::RefreshPriority::recency:
//			{
//				refresh_func = recencyPriority;
//				break;
//			}
//			default:
//			{
//				cout << "没有这条策略！" << endl;//TODO:这么写不正规！！！
//				break;
//			}
//			}
//
//			this->subgraphID = subgraphID;
//			this->count = count;
//
//		}
//
//
//
//
//
//
//	}SubList;
//
//
//
//
//
//
//
//
//	/*
//	类名：子图内存序列初始化器
//	类功能：管理子图在内存中的活动
//	*/
//	class SubgraphsMemoryInitializer {
//
//	public:
//		SubList** sublist;
//		ofstream* out;
//		istream* in;
//		MinimalStructuralInfo* structuralInfo;
//		size_t offset_structuralInfo;
//
//		unordered_map<Idtype, size_t> subid_lookup_index;//内存内子图id和子图index索引查找表
//
//		SubgraphsMemoryInitializer(MinimalStructuralInfo* info, ofstream* out, istream* in) {
//			sublist = new SubList * [info->max_load_Subgraph_];
//			structuralInfo = info;
//			offset_structuralInfo = sizeof(structuralInfo);
//			this->out = out;
//			this->in = in;
//		}
//
//
//
//	protected:
//		//在外存element_count_per_sub_指定id的地方写入元素数
//		void writeSubCountToFile(size_t subid, size_t newNum) {
//			writeWithOffset(*out, newNum, offset_structuralInfo + subid * sizeof(size_t));
//		}
//
//		//在外存指定位置写入或覆盖子图
//		void writeSubToFile(SubNode* subNode, Idtype subid) {
//			size_t size_perSub = structuralInfo->max_element_count_per_sub_ * structuralInfo->size_sub_per_element_;
//			writeWithOffset(
//				*out,
//				subNode->elements,
//				offset_structuralInfo + structuralInfo->max_Subgraph_ * sizeof(size_t) + subid * size_perSub,
//				size_perSub
//			);
//		}
//
//
//
//	public:
//		/*
//		方法名称：向外存加入一张子图，如果内存没满也加到内存里去
//		*/
//		void addSubToFile(SubNode* newSub, int subid = -1) {
//
//			if (subid == -1) {
//
//				if (structuralInfo->cur_Subgraph_ < structuralInfo->max_load_Subgraph_) {//如果内存没满就存到内存里去
//					sublist[structuralInfo->cur_Subgraph_] = newSub;
//					pair<size_t, Idtype> p;
//					p.first = structuralInfo->cur_Subgraph_;
//					p.second = structuralInfo->cur_Subgraph_;
//					subid_lookup_index.insert(p);
//				}
//
//				writeSubCountToFile(structuralInfo->cur_Subgraph_, newSub->count);
//				writeSubToFile(newSub, structuralInfo->cur_Subgraph_);
//				structuralInfo->cur_Subgraph_++;
//
//			}
//			else
//			{
//				writeSubCountToFile(subid, newSub->count);
//				writeSubToFile(newSub, subid);
//			}
//
//		}
//
//		/*
//		方法名称：从外存拿取一张子图到内存，用新子图（号）替换旧子图（号）
//		*/
//		void getSubToMermory(Idtype new_sub_id, Idtype old_sub_id) {
//			size_t count;
//			readWithOffset(
//				*in,
//				count,
//				offset_structuralInfo + new_sub_id * sizeof(size_t)
//			);
//			size_t elements_size = structuralInfo->max_element_count_per_sub_ * structuralInfo->size_sub_per_element_;
//			char* elements = (char*)malloc(elements_size);
//			readWithOffset(
//				*in,
//				elements,
//				offset_structuralInfo + structuralInfo->max_Subgraph_ * sizeof(size_t) + new_sub_id * elements_size,
//				elements_size
//			);
//
//			//TODO:理论上，这里需要写入增删改操作之后，对旧图的元素个数和子图的更新，但是这里没有实现
//
//			//subindex_subid_lookup[subindex] = subid;
//
//			size_t subindex = subid_lookup_index[old_sub_id];
//
//			SubNode* newSub = new SubNode(flasslib::RefreshPriority::quantity, new_sub_id, count);
//			newSub->elements = elements;
//			SubNode* tempNode = sublist[subindex];
//			sublist[subindex] = newSub;
//			delete tempNode;
//
//			subid_lookup_index.erase(old_sub_id);
//			pair<size_t, Idtype> p;
//			p.first = new_sub_id;
//			p.second = subindex;
//			subid_lookup_index.insert(p);
//
//
//		}
//
//
//
//	};
//
//
//
//
//
//}