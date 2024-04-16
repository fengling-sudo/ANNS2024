#pragma once
#include "graphlib.h"
#include "space/L2.h"
#include "quant/SQ8_quant.h"


/*尚未完全实现*/

/*
结构体名称：距离计算器
结构体依赖：space.h距离计算方法，quant.h量化器策略
TODO：const void*是通用指针，导致维度的类型还是需要手动指定，没法解耦合
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
类名：配置器
用途：
	1. 用于配置quant，computer等策略,并且返回合适的quant，computer等
	2. 提供子图在内存的容器，在外存的文件
	3. 提供超图在内存的容器，在外存的文件
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
//	//开始量化数据,输入原始数据，获取量化数据，然后丢弃原始数据！
//	//TOTHINK：丢弃原始数据合理吗？
//	//TODO：后续我想把float**换成float*，前者浪费内存，影响性能！
//	char* getQuantData(float** raw_data, size_t data_num) {
//
//		char* (*encode)(float**, size_t, size_t) = getQuantizer(quantizerType);
//		char* codes = encode(raw_data, basicInformation->data_dim, data_num);
//		return codes;
//
//	}
//
//};