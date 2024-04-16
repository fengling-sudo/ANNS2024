#pragma once
#include "../graphlib.h"

/// <summary>
/// 设置模板是为了不同的量化类型
/// </summary>
/// <typeparam name="T">量化类型</typeparam>
/// TODO:尚未实现Space策略的调换，可以使用函数指针获取策略
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

	//重设query，反复利用，因为是内置在quant里的，所以默认query已经量化
	void resetQuery(const void* query_) {
		this->query = (char*)query_;
	}


};