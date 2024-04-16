#pragma once

/*
�ļ����ƣ�subgraphMermory.h
�����޸����ڣ�2024/4/9
�ļ����ݸ�����������ͼ�ġ����֤����ʵ�ֲ�ͬ����ͼfreq���²��ԣ�ʵ����ͼ��������ȡ����
�ļ���ϵ����flasslib.h������������grahplib�ļ�
*/

//#include "graphlib.h"
//
//namespace flasslib {
//
//	enum class RefreshPriority { quantity, recency };
//
//	//FREQ�������ڴ洢��ͬ������ԵĻ�����Ϣ
//	typedef float* FREQARR;
//
//
//
//	/*
//	�������ƣ��������Ȳ���
//	����������ÿһ�α����ã�����ͼ��freq+1
//	*/
//	void quantityPriority(FREQARR freq) { freq[0]++; }
//
//	/*
//	TODO:��δʵ��
//	�������ƣ��������Ȳ���
//	����������û�б����ã���enterTotal++����������enterTotal++��hitTotal++���Դ�Ϊ����freq������:freq = hit/total;
//	*/
//	void recencyPriority(FREQARR ifhit_total_hitnum) { cout << "TODO" << endl; }
//
//
//
//
//
//	//��ͼ���֤�ڵ�
//	typedef struct SubNode {
//		Idtype subgraphID;
//		size_t count;
//		char* elements;
//		FREQARR freq;
//		void(*refresh_func)(FREQARR);
//
//		//ˢ�²���װ����SubNode���������װ�䵽SubgraphsMemoryInitializer�����
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
//				cout << "û���������ԣ�" << endl;//TODO:��ôд�����棡����
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
//	��������ͼ�ڴ����г�ʼ����
//	�๦�ܣ�������ͼ���ڴ��еĻ
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
//		unordered_map<Idtype, size_t> subid_lookup_index;//�ڴ�����ͼid����ͼindex�������ұ�
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
//		//�����element_count_per_sub_ָ��id�ĵط�д��Ԫ����
//		void writeSubCountToFile(size_t subid, size_t newNum) {
//			writeWithOffset(*out, newNum, offset_structuralInfo + subid * sizeof(size_t));
//		}
//
//		//�����ָ��λ��д��򸲸���ͼ
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
//		�������ƣ���������һ����ͼ������ڴ�û��Ҳ�ӵ��ڴ���ȥ
//		*/
//		void addSubToFile(SubNode* newSub, int subid = -1) {
//
//			if (subid == -1) {
//
//				if (structuralInfo->cur_Subgraph_ < structuralInfo->max_load_Subgraph_) {//����ڴ�û���ʹ浽�ڴ���ȥ
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
//		�������ƣ��������ȡһ����ͼ���ڴ棬������ͼ���ţ��滻����ͼ���ţ�
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
//			//TODO:�����ϣ�������Ҫд����ɾ�Ĳ���֮�󣬶Ծ�ͼ��Ԫ�ظ�������ͼ�ĸ��£���������û��ʵ��
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