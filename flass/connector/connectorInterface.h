#pragma once
//������ר��������ɽڵ�֮������ӣ�����˫�����ӣ��������ӣ��������ӵȵ�
//TODO������������ʽ��ѡ�ھӡ��Ĳ���Ҳ�ᶨ���������棡

//˵����Ԫ�����ڴ�������ֶ���һ�µģ���Ϊ���ʱ������Ѿ��㹻�򻯣�������£�
//******�ھ�����size_t����label��ǩ��size_t�����ھ���subNeigh_max_ * sizeof(Idtype)��������/ԭʼ������data_dim_ * sizeof(...)��


#include "../flasslib.h"//�˴�������flasslib���computerѭ��������

enum class ConnectionMethod { directedConnectors, undirectedConnectors };


template<typename T>
struct connector
{

	size_t size_per_element_in_sub;//һ��Ԫ�صĴ�С�����Դӽṹ��Ϣ�ﴫ��
	size_t offset_to_data;//Ԫ����ƫ�Ƶ�data���ֽ�����������������Ҫ�������������
	Computer* computer;

	connector(size_t size_per_element_in_sub, size_t offset_to_data, Computer* computer) {
		this->size_per_element_in_sub = size_per_element_in_sub;
		this->offset_to_data = offset_to_data;
		this->computer = computer;
	}

	/*
	DONE
	����һ����ͼ���Լ�������ͼ��ĳ��Ԫ�ص��ڲ�id������ԭʼ����or���������ĵ�ַ
	*/
	inline char* getDataByInternalId(Idtype internal_id, SubNode* sub) {

		return sub->elements + internal_id * size_per_element_in_sub + offset_to_data;

	}

	/*
	DONE
	����һ����ͼ���Լ�������ͼ��ĳ��Ԫ�ص��ڲ�id������������ͼ���ھ������ھ�idָ��
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