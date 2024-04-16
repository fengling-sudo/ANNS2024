#pragma once

#include "../graphlib.h"

/*
space只做一件事，提供合适的距离计算策略
这个Space的class没有定义上的作用，只是要在装配flasslib里的computer时利用一个多态而已
*/

enum class SpaceStrategy { L2, IP };

//提供合适的距离计算方法
class Space {

public:
	//实现不带指令集的距离计算方法
	virtual dist_type dist_func(const char* data1, const char* data2, const size_t dim) = 0;

};

