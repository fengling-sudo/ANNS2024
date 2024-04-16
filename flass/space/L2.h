#pragma once
#include "../graphlib.h"

/// <summary>
/// ����ģ����Ϊ�˲�ͬ����������
/// </summary>
/// <typeparam name="T">��������</typeparam>
/// TODO:��δʵ��Space���Եĵ���������ʹ�ú���ָ���ȡ����
template<typename T>
struct Computer :ComputerInit {
	size_t dim;
	char* query;



	Computer(const void* query, size_t data_dim_) {
		this->query = (char*)query;
		this->dim = data_dim_;
	}

	dist_type operator()(const void* data) const override {

		T* pVect1 = (T*)data;
		T* pVect2 = (T*)query;

		float res = 0;
		for (size_t i = 0; i < dim; i++) {
			float t = *pVect1 - *pVect2;
			pVect1++;
			pVect2++;
			res += t * t;
		}

		return (res);

	}

	//����query���������ã���Ϊ��������quant��ģ�����Ĭ��query�Ѿ�����
	void resetQuery(const void* query_) {
		this->query = (char*)query_;
	}


};