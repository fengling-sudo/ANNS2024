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
���ܣ�ר���ض��塰ͼ���ı�������
Idtype��ԭ�ȵ�tableint,����ָ��id�����ͣ�����Ԫ������ͼ��id����ͼ��id
labeltype��lable���ͣ�label�����;���labeltype��
subgraph_index����ͼ���ڴ滺��������������ͼid�������ı���˳��index�����ڴ�Ĵ洢˳��
dist_type�����������
neigh_num������ʵ���ھ����ı���
*/
typedef unsigned int Idtype;
typedef size_t labeltype;
typedef size_t subgraph_index;
typedef float dist_type;
typedef size_t neigh_num;



/*��in�ļ���ȡT���ʹ�С�����ݵ�T����*/
template<typename T>
static void readBinaryPOD(istream& in, T& podRef)
{
	in.read((char*)&podRef, sizeof(T));
}


/*��out�ļ�д��T���ʹ�С�����ݵ�T����*/
template<typename T>
static void writeBinaryPOD(ofstream& out, T& podRef)
{
	out.write((char*)&podRef, sizeof(T));
}


/*
�������������HNSW�ṹ��Ϣ�ṹ�壬��Ҫ��flass.hʵ�֣��˴���ֹ����������ṹ��Ϣ������δ��ȫʵ�֣�
	max_Subgraph_����ʾȫ������������=��ͼ������������
	cur_Subgraph_����ʾ��ǰ���һ���е���ͼ����
	max_load_Subgraph_����ʾһ�οɼ��صľ��ࣨ��ͼ������
	subNeigh_max_��ÿ����ͼ�ĵ���ھ�����
	hyperNeigh_max_����ͼ�ĵ���ھ�����
	max_element_count_per_sub_����ͼ������ɶ��ٸ���
	max_element_cur_count_����ʾ��ǰһ���ж��ٸ���
	size_hyper_per_element_��ÿ�������ڳ�ͼ���ڴ��С
	basicinfo.size_sub_per_element_��ÿ����������ͼ���ڴ��С
	entryPoint_����ͼ��ڵ�
	ef_construction_����ѡ������
	data_dim��Ԫ�����ݵ�ά������ÿ��ԭʼ���ݵ�ÿ��ά��Ĭ����float�ֽڵģ�
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
	size_t res_k;//��ѯ�����Ķ��ٸ��ھ�

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

//���²�����ͼ����

/*
�ṹ�����ƣ���ͼ�ڵ���Ԫ�صĽṹ
*/
struct SubElementNode {
	//�ھ���
	neigh_num neighNum;
	//label
	labeltype label;
	//�ھ���
	Idtype* neighIDs;
	//������
	char* data;
	//������
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
�ṹ�����ƣ���ͼ�ڵ���Ԫ�صĽṹ
TODO:��һ����ƵıȽϲ��Ҫ���¹�˼
*/
struct HyperElementNode
{
	//��Ӧ��sid
	Idtype subid;
	//��hyper���Ӧ��id��TODO��������ʵ��һ��label��ѯ����һ��label�ܲ�ѯ��һ��subid��id��Ψһ��Ӧһ��Ԫ��
	Idtype eleid;
	//��sub���Ӧ��id
	Idtype elesubid;
	//Ψһ��ʶlabel
	labeltype label;
	//�ھ���
	neigh_num neighNum;
	//�ھ���һ���ھӵĴ洢��ʽ������subid+����eleid��һ���ھ�ռ����Idtype��С����TODO��Ӧ�û��и��õ��Ų���������ô������Ϊ��ͼ�Ұ�sub�����ˣ�������һ�����壬����һ�������ֻᵼ�²��ҷ�������������ڴ�׷��ʱ��
	Idtype* neighIDs;

	//�����򣨳�ͼ�ϲ�����,����float���ͣ�
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
�ṹ�����ƣ���ͼ���֤
����Ҫ����ͼ�ϵĵ㣬���������涨�壬֮���Գ�ͼҲ��Node��������sid���ֵģ���ʵ�����߼��ϣ���ͼ��һ����
*/
struct HyperGraphNode
{
	vector<HyperElementNode> elements;

	HyperGraphNode(){}
	HyperGraphNode(MinimalStructuralInfo& basicinfo) {
		//���ڳ�ͼ�ĵ��Ƕ�̬�ģ��޷�ȷ����ͼ���ж��ٵ㣬��˴˴���Ԥ��vector�ĳ���
	}

};



/*
�ṹ�����ƣ���ͼ���֤
ģ�����ͣ�����������������ڵ�
*/
struct SubGraphNode {
	Idtype subgraphID;
	size_t count;
	float* freq;
	vector<SubElementNode> elements;

	SubGraphNode(size_t elecount, Idtype sid, MinimalStructuralInfo& basicinfo) {
		this->count = elecount;
		this->subgraphID = sid;

		//Ԥ�����ڴ�
		elements.reserve(basicinfo.max_element_count_per_sub_);
		//ѭ��������Ԫ��(Ŀǰû��ʵ�ֶ�̬�����ڴ棬���¼ӵ��ʱ��Ƶ��Ų���ڴ�Ӱ������)
		for (size_t i = 0; i < basicinfo.max_element_count_per_sub_; i++) {
			elements.emplace_back(basicinfo);
		}

	}

	//��������
	void copy(SubGraphNode& oldNode) {
	
		if (this != &oldNode) {  // ��ֹ�Ը�ֵ
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
�ṹ�����ƣ��ڴ������
��Ź��ܣ�������ͼ�ͳ�ͼ;��ȡ֮��ķ���ȫ�������ﶨ��
*/
struct MemoryManager {

	

	vector<SubGraphNode> sub_memory;
	vector<HyperGraphNode> hyper_memory;

	MemoryManager(){}

	MemoryManager(MinimalStructuralInfo& basicinfo) {

		//cout << basicinfo.max_load_Subgraph_;

		//Ԥ������ͼ�ڴ�
		sub_memory.reserve(basicinfo.max_load_Subgraph_);
		//ѭ���������ڴ���ͼ
		for (size_t i = 0; i < basicinfo.max_load_Subgraph_; i++) {
			sub_memory.emplace_back(0, i, basicinfo);
		}
		//ѭ�������յĳ�ͼ�飬һ����ͼ��һ����Ӧ�ĳ�ͼ��
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




//���ȼ����бȽ���:��ͼ
struct CompareByFirst {
	constexpr bool
		operator()(std::pair<dist_type, Idtype> const& a,
			std::pair<dist_type, Idtype> const& b) const noexcept {
		return a.first < b.first;
	}
};

//���ȼ����бȽ���:��ͼ
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