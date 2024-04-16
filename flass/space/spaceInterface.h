#pragma once

#include "../graphlib.h"

/*
spaceֻ��һ���£��ṩ���ʵľ���������
���Space��classû�ж����ϵ����ã�ֻ��Ҫ��װ��flasslib���computerʱ����һ����̬����
*/

enum class SpaceStrategy { L2, IP };

//�ṩ���ʵľ�����㷽��
class Space {

public:
	//ʵ�ֲ���ָ��ľ�����㷽��
	virtual dist_type dist_func(const char* data1, const char* data2, const size_t dim) = 0;

};

