#pragma once
#include "graphlib.h"
#include "space/L2.h"
#include "quant/SQ8_quant.h"


/*��δ��ȫʵ��*/

/*
�ṹ�����ƣ����������
�ṹ��������space.h������㷽����quant.h����������
TODO��const void*��ͨ��ָ�룬����ά�ȵ����ͻ�����Ҫ�ֶ�ָ����û�������
*/
//typedef struct Computer_f {
//	size_t dim;
//	const void* query;
//
//	dist_type(*dist_func)(const void*, const void*, const size_t);
//
//	Computer_f(size_t dim, const void* query, SpaceStrategy space_strategy, QuantizerType quantizerType) {
//		this->dim = dim;
//		this->query = query;
//
//		if (quantizerType == QuantizerType::SQ8)
//			dist_func = get_dist_func<uint8_t>(space_strategy);
//		else if (quantizerType == QuantizerType::FP32)
//			dist_func = get_dist_func<float>(space_strategy);
//		else
//			dist_func = nullptr;
//	}
//
//
//
//};


/*
������������
��;��
	1. ��������quant��computer�Ȳ���,���ҷ��غ��ʵ�quant��computer��
	2. �ṩ��ͼ���ڴ���������������ļ�
	3. �ṩ��ͼ���ڴ���������������ļ�
*/

//struct Configurator
//{
//	SpaceStrategy spaceStrategy;
//	QuantizerType quantizerType;
//	RefreshPriority refreshPriority;
//
//	StructuralInfo* basicInformation;
//
//
//
//
//	Configurator(SpaceStrategy spaceStrategy, QuantizerType quantizerType, RefreshPriority refreshPriority, StructuralInfo* basicInformation) {
//		this->spaceStrategy = spaceStrategy;
//		this->quantizerType = quantizerType;
//		this->refreshPriority = refreshPriority;
//		this->basicInformation = basicInformation;
//	}
//
//	Computer getComputer(const void* query, const void* data) {
//		return Computer(basicInformation->data_dim, query, spaceStrategy, quantizerType);
//	}
//
//	//��ʼ��������,����ԭʼ���ݣ���ȡ�������ݣ�Ȼ����ԭʼ���ݣ�
//	//TOTHINK������ԭʼ���ݺ�����
//	//TODO�����������float**����float*��ǰ���˷��ڴ棬Ӱ�����ܣ�
//	char* getQuantData(float** raw_data, size_t data_num) {
//
//		char* (*encode)(float**, size_t, size_t) = getQuantizer(quantizerType);
//		char* codes = encode(raw_data, basicInformation->data_dim, data_num);
//		return codes;
//
//	}
//
//};