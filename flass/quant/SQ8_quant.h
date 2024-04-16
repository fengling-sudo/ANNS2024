#pragma once
/*
8bits量化器
内置computer计算器，用于计算量化结果
已实现
*/
#include "quantInterface.h"


struct SQ8Quantizer :public Quantizer
{
	//量化器里内置计算器:先恢复SQ8再计算
	struct SQ8_L2_Computer :ComputerInit
	{
		float* query;
		size_t dim;
		vector<float> mi;
		vector<float> dif;
		//默认query是未量化的，SQ8也不需要量化query
		SQ8_L2_Computer(){}

		SQ8_L2_Computer(const void* query_, SQ8Quantizer* quantizer) {
			query = (float*)query_;
			dim = quantizer->dim_;
			mi = quantizer->mi_;
			dif = quantizer->dif_;

		}
		void resetQuery(const void* query_) override { 
			delete query;
			query = (float*)query_;
		}
		//默认data是没有量化的，因为SQ8会恢复，所以不需要量化query
		dist_type operator()(const void* d) const {

			uint8_t* data = (uint8_t*)d;

			dist_type sum = 0.0;
			for (size_t i = 0; i < dim; ++i) {
				float yy = ((float)(*(data + i)) + 0.5f);
				yy = yy * dif[i] + mi[i] * 255.0f;
				float dif_ = query[i] * 255.0f - yy;
				sum += dif_ * dif_;
			}
			return sum;
		}
	};

	vector<float> mi_;
	vector<float> dif_;
	size_t dim_;
	char* codes_;
	SQ8_L2_Computer sq8_L2_computer;

	char* encode(float* raw_data, size_t dim, size_t data_num) override {
		this->dim_ = dim;
		char* codes = (char*)malloc(dim * data_num);
		vector<float> mx(dim, 0), mi(dim, numeric_limits<float>::max()), dif(dim, 0);
		for (size_t i = 0; i < data_num; i++)
		{
			for (size_t j = 0; j < dim; j++) {
				mx[j] = max(mx[j], raw_data[i * dim + j]);
				mi[j] = min(mi[j], raw_data[i * dim + j]);
			}
		}
		for (size_t i = 0; i < dim; i++) {
			dif[i] = mx[i] - mi[i];
		}
		mi_ = mi;
		dif_ = dif;
		for (size_t i = 0; i < data_num; i++) {
			for (size_t j = 0; j < dim; j++) {
				float temp = (raw_data[i * dim + j] - mi[j]) / dif[j];
				if (temp < 0) {
					temp = 0;
				}
				if (temp > 1) {
					temp = 1;
				}
				uint8_t res = temp * 255;
				memcpy(codes + i * dim + j, &res, sizeof(uint8_t));
			}
		}
		this->codes_ = codes;
		return codes;
	}


	QuantizerType getQuantizerType() override {
		return QuantizerType::SQ8;
	}

	char* getCodeByLabel(labeltype label) override {
		return codes_ + label * dim_ * sizeof(uint8_t);
	}

	//TODO：传入策略，传出计算器
	ComputerInit* getComputer(const void* query) override {
		return new SQ8_L2_Computer(query, this);
	}
};