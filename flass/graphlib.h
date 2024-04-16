#pragma once
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unordered_map>
#include <queue>
#include <mutex>
#include <random>
#include <chrono>
#include <thread>

using namespace std;


/*
功能：专门重定义“图”的变量类型
Idtype：原先的tableint,用于指明id的类型，例如元素在子图的id，子图号id
labeltype：lable类型（label的类型就是labeltype）
subgraph_index：子图在内存缓冲区的索引（子图id是在外存的保存顺序，index是在内存的存储顺序）
dist_type：距离的类型
neigh_num：保存实际邻居数的变量
*/
typedef unsigned int Idtype;
typedef size_t labeltype;
typedef size_t subgraph_index;
typedef float dist_type;
typedef size_t neigh_num;



/*从in文件读取T类型大小的数据到T类型*/
template<typename T>
static void readBinaryPOD(istream& in, T& podRef)
{
	in.read((char*)&podRef, sizeof(T));
}


/*向out文件写入T类型大小的数据到T类型*/
template<typename T>
static void writeBinaryPOD(ofstream& out, T& podRef)
{
	out.write((char*)&podRef, sizeof(T));
}


/*
定义了最基本的HNSW结构信息结构体，需要被flass.h实现；此处禁止再添加其它结构信息！（尚未完全实现）
	max_Subgraph_：表示全部聚类数量（=子图数量）的上限
	cur_Subgraph_：表示当前外存一共有的子图个数
	max_load_Subgraph_：表示一次可加载的聚类（子图）上限
	subNeigh_max_：每张子图的点的邻居上限
	hyperNeigh_max_：超图的点的邻居上限
	max_element_count_per_sub_：子图最多容纳多少个点
	max_element_cur_count_：表示当前一共有多少个点
	size_hyper_per_element_：每个向量在超图的内存大小
	basicinfo.size_sub_per_element_：每个向量在子图的内存大小
	entryPoint_：超图入口点
	ef_construction_：候选集步长
	data_dim：元素数据的维度数（每个原始数据的每个维度默认是float字节的）
*/
struct MinimalStructuralInfo {
	size_t max_Subgraph_;
	size_t cur_Subgraph_{ 0 };
	size_t max_load_Subgraph_;
	size_t subNeigh_max_;
	size_t hyperNeigh_max_;
	size_t max_element_count_per_sub_;
	size_t max_element_cur_count_;
	size_t size_hyper_per_element_;
	size_t size_sub_per_element_;
	Idtype entryPoint_;
	size_t ef_construction_;
	size_t data_dim;
	size_t ef_hyper_;
	size_t res_k;//查询向量的多少个邻居

	MinimalStructuralInfo() {}

	MinimalStructuralInfo(
		size_t max_Subgraph_,
		size_t max_load_Subgraph_,
		size_t subNeigh_max_,
		size_t hyperNeigh_max_,
		size_t max_element_count_per_sub_,
		size_t ef_construction_,
		size_t data_dim
	) {
		this->max_Subgraph_ = max_Subgraph_;
		this->max_load_Subgraph_ = max_load_Subgraph_;
		this->subNeigh_max_ = subNeigh_max_;
		this->hyperNeigh_max_ = hyperNeigh_max_;
		this->max_element_count_per_sub_ = max_element_count_per_sub_;
		this->ef_construction_ = ef_construction_;
		this->data_dim = data_dim;
	}

};

//以下操作是图管理

/*
结构体名称：子图内单个元素的结构
*/
struct SubElementNode {
	//邻居数
	neigh_num neighNum;
	//label
	labeltype label;
	//邻居域
	Idtype* neighIDs;
	//数据域
	char* data;
	//量化域
	char* data_quant;


	SubElementNode(MinimalStructuralInfo& basicinfo, labeltype label = 0) {
		neighNum = 0;
		this->label = label;
		neighIDs = (Idtype*)malloc(basicinfo.subNeigh_max_ * sizeof(labeltype));
		memset(neighIDs, 0, basicinfo.subNeigh_max_ * sizeof(labeltype));
		data = (char*)malloc(basicinfo.data_dim * sizeof(float));
		memset(data, 0, basicinfo.data_dim * sizeof(float));
	}

};



/*
结构体名称：超图内单个元素的结构
TODO:这一段设计的比较差，需要重新构思
*/
struct HyperElementNode
{
	//对应的sid
	Idtype subid;
	//在hyper里对应的id；TODO：后续会实现一个label查询表，让一个label能查询到一对subid和id来唯一对应一个元素
	Idtype eleid;
	//在sub里对应的id
	Idtype elesubid;
	//唯一标识label
	labeltype label;
	//邻居数
	neigh_num neighNum;
	//邻居域（一个邻居的存储形式是它的subid+它的eleid，一个邻居占两个Idtype大小）；TODO：应该还有更好的排布，这里这么做是因为超图我按sub来分了，而不是一块整体，但是一块整体又会导致查找繁琐，因此舍弃内存追求时间
	Idtype* neighIDs;

	//数据域（超图上不量化,保留float类型）
	char* data;

	HyperElementNode(MinimalStructuralInfo& basicinfo,Idtype subid, Idtype eleid_in_hyper,Idtype eleid_in_sub,labeltype label) {
		this->subid = subid;
		this->eleid = eleid_in_hyper;
		this->elesubid = eleid_in_sub;
		this->label = label;
		neighNum = 0;
		this->neighIDs = (Idtype*)malloc(basicinfo.hyperNeigh_max_ * sizeof(labeltype) * 2);
		memset(neighIDs, 0, basicinfo.hyperNeigh_max_ * sizeof(labeltype) * 2);
		data = (char*)malloc(basicinfo.data_dim * sizeof(float));
		memset(data, 0, basicinfo.data_dim * sizeof(float));
	}


};




/*
结构体名称：超图身份证
后续要管理超图上的点，就在这里面定义，之所以超图也是Node，是依据sid来分的，其实计算逻辑上，超图是一整块
*/
struct HyperGraphNode
{
	vector<HyperElementNode> elements;

	HyperGraphNode(){}
	HyperGraphNode(MinimalStructuralInfo& basicinfo) {
		//由于超图的点是动态的，无法确定超图会有多少点，因此此处不预设vector的长度
	}

};



/*
结构体名称：子图身份证
模板类型，方便后续更换量化节点
*/
struct SubGraphNode {
	Idtype subgraphID;
	size_t count;
	float* freq;
	vector<SubElementNode> elements;

	SubGraphNode(size_t elecount, Idtype sid, MinimalStructuralInfo& basicinfo) {
		this->count = elecount;
		this->subgraphID = sid;

		//预分配内存
		elements.reserve(basicinfo.max_element_count_per_sub_);
		//循环创建空元素(目前没有实现动态分配内存，我怕加点的时候频繁挪动内存影响性能)
		for (size_t i = 0; i < basicinfo.max_element_count_per_sub_; i++) {
			elements.emplace_back(basicinfo);
		}

	}

	//拷贝方法
	void copy(SubGraphNode& oldNode) {
	
		if (this != &oldNode) {  // 防止自赋值
			this->subgraphID = oldNode.subgraphID;
			this->count = oldNode.count;

			for (size_t i = 0; i < oldNode.elements.size();i++) {
				this->elements[i] = oldNode.elements[i];
			}

			this->freq = oldNode.freq;

		}
	}

};





/*
结构体名称：内存管理器
大概功能：管理子图和超图;存取之类的方法全部在这里定义
*/
struct MemoryManager {

	

	vector<SubGraphNode> sub_memory;
	vector<HyperGraphNode> hyper_memory;

	MemoryManager(){}

	MemoryManager(MinimalStructuralInfo& basicinfo) {

		//cout << basicinfo.max_load_Subgraph_;

		//预分配子图内存
		sub_memory.reserve(basicinfo.max_load_Subgraph_);
		//循环创建空内存子图
		for (size_t i = 0; i < basicinfo.max_load_Subgraph_; i++) {
			sub_memory.emplace_back(0, i, basicinfo);
		}
		//循环创建空的超图组，一个子图有一个对应的超图组
		for (size_t i = 0; i < basicinfo.max_Subgraph_; i++)
		{
			HyperGraphNode newNode(basicinfo);
			hyper_memory.push_back(newNode);
		}


	}

	void updateSubByindex(size_t index, SubGraphNode& newSub) {
		sub_memory[index].copy(newSub);
	}

};




//优先级队列比较器:子图
struct CompareByFirst {
	constexpr bool
		operator()(std::pair<dist_type, Idtype> const& a,
			std::pair<dist_type, Idtype> const& b) const noexcept {
		return a.first < b.first;
	}
};

//优先级队列比较器:超图
struct CompareByFirst_hyper {
	constexpr bool
		operator()(std::pair<dist_type, pair<Idtype, Idtype>> const& a,
			std::pair<dist_type, pair<Idtype, Idtype>> const& b) const noexcept {
		return a.first < b.first;
	}
};

struct ComputerInit
{

	virtual dist_type operator()(const void* data) const = 0;

	virtual void resetQuery(const void* query_) = 0;

};