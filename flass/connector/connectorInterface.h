#pragma once
//连接器专门用来完成节点之间的连接，比如双向连接，单向连接，启发连接等等
//TODO：后续“启发式候选邻居”的策略也会定义在这里面！

//说明：元素在内存和外存的字段是一致的，因为设计时内外存已经足够简化，设计如下：
//******邻居数（size_t），label标签（size_t），邻居域（subNeigh_max_ * sizeof(Idtype)），量化/原始向量（data_dim_ * sizeof(...)）


#include "../flasslib.h"//此处被迫与flasslib里的computer循环依赖了

enum class ConnectionMethod { directedConnectors, undirectedConnectors };


template<typename T>
struct connector
{

	size_t size_per_element_in_sub;//一个元素的大小，可以从结构信息里传入
	size_t offset_to_data;//元素内偏移到data的字节量，减少依赖，不要在连接器里计算
	Computer* computer;

	connector(size_t size_per_element_in_sub, size_t offset_to_data, Computer* computer) {
		this->size_per_element_in_sub = size_per_element_in_sub;
		this->offset_to_data = offset_to_data;
		this->computer = computer;
	}

	/*
	DONE
	传入一张子图，以及这张子图的某个元素的内部id，返回原始向量or量化向量的地址
	*/
	inline char* getDataByInternalId(Idtype internal_id, SubNode* sub) {

		return sub->elements + internal_id * size_per_element_in_sub + offset_to_data;

	}

	/*
	DONE
	传入一张子图，以及这张子图的某个元素的内部id，返回它在子图的邻居数和邻居id指针
	*/
	pair<neigh_num*, Idtype*> get_sub_ele_neigh_ByInternalId(Idtype internal_id, SubNode* sub) {
		pair<neigh_num*, Idtype*> res_ele_neigh;
		neigh_num* number = (neigh_num*)(sub->elements + internal_id * size_per_element_in_sub);
		Idtype* neigh_id = (Idtype*)(sub->elements + internal_id * size_per_element_in_sub + sizeof(neigh_num) + sizeof(labeltype));
		res_ele_neigh.first = number;
		res_ele_neigh.second = neigh_id;
		return res_ele_neigh;
	}


};