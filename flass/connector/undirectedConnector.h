#pragma once
//TODO��˫����������Ҳ��������������һ��ʼ����ĵ㶼��������ģ����ŵ��������࣬����Խ��Խ����

/*
TODO
ʵ����ͼ�ڵ�˫�����ӣ�ֻҪ����������Ҫ���ӵĵ㼴��
*/

/*
inline void bi_directional_connections_in_subgraph(Idtype id1, Idtype id2, Idtype sub_id) {

	//TOTHINK:�ڽ�����ʱ���ھ�������ھ�id�᲻�����ظ��أ�
	//TOTHINK:�˴���ԭHNSW��mutuallyConnectNewElement()�������������ָĽ���ɾ���ļ򻯰棬���ǲ��ֹ���û��ʵ�֣����»��ƣ�ɾ������ƣ�����ʽ����......

	//1.��������id1����id2��
	//1.1.���id1���ھ���û��������id1���ھ���ֱ�����id2
	//1.2.���id1���ھ�������  ��������id2��id1���ھӽڵ㣬ѡ�����ŵģ����id1���ھӽڵ㱻ɾ���ˣ���ôid1��ԭ���ھӽڵ�֮�������߾ͻ�˥��Ϊ����ߣ���ע�⣺ԭ����HNSW������ͼ��������Ȼ��ҪȨ�����;����ͼ�������������Ҳ����������HNSW�ٷ�Ϊʲô�����أ�����
	pair<neigh_num*, Idtype*> pair1 = get_sub_ele_neigh_ByInternalId(id1, sub_id);

	if (*pair1.first < subNeigh_max_) {
		*(pair1.second + *pair1.first) = id2;
		(*pair1.first)++;
	}
	else
	{
		//�ҳ������Ǹ�id�滻������
		ComputerInit com(getDataByInternalId(id1, sub_id), data_dim_);

		pair<dist_type, int> farthest;//�洢����ͽڵ����ھ����index
		farthest.first = com(getDataByInternalId(id2, sub_id));
		farthest.second = -1;//-1��ʾid2���;


		for (int i = 0; i < subNeigh_max_; i++) {
			Idtype neighID = *(pair1.second + i);
			dist_type neighDist = com(getDataByInternalId(neighID, sub_id));
			if (neighDist > farthest.first) {
				farthest.first = neighDist;
				farthest.second = i;
			}
		}

		if (farthest.second != -1) {
			*(pair1.second + farthest.second) = id2;
		}

	}
	//2.Ȼ������id2����id1��
	pair<neigh_num*, Idtype*> pair2 = get_sub_ele_neigh_ByInternalId(id2, sub_id);

	if (*pair2.first < subNeigh_max_) {
		*(pair2.second + *pair2.first) = id1;
		(*pair2.first)++;
	}
	else
	{
		ComputerInit com(getDataByInternalId(id2, sub_id), data_dim_);//�ҳ������Ǹ�id�滻������
		pair<dist_type, int> farthest;//�洢����ͽڵ����ھ����index
		dist_type temp = com(getDataByInternalId(id1, sub_id));
		farthest.first = com(getDataByInternalId(id1, sub_id));
		farthest.second = -1;//-1��ʾid2���;

		for (int i = 0; i < subNeigh_max_; i++) {
			Idtype neighID = *(pair2.second + i);
			dist_type neighDist = com(getDataByInternalId(neighID, sub_id));

			if (neighDist > farthest.first) {
				farthest.first = neighDist;
				farthest.second = i;
			}
		}
		if (farthest.second != -1) {
			*(pair2.second + farthest.second) = id1;
		}
	}
}
*/