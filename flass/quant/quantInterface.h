#pragma once
/*
quant只干一件事：传入到quant的数据都就行量化
它只传出量化数据
*/
#include "../graphlib.h"
#include "../space/L2.h"

enum class QuantizerType { FP32, SQ8, SQ4 };


struct Quantizer
{

public:
	virtual char* encode(float* raw_data, size_t dim, size_t data_num) = 0;
	virtual QuantizerType getQuantizerType() = 0;
	virtual char* getCodeByLabel(labeltype label) = 0;
	virtual ComputerInit* getComputer(const void* query) = 0;
};