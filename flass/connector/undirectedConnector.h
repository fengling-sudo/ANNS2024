#pragma once
//TODO：双向连接器，也叫无向连接器；一开始插入的点都会变成无向的，随着点数的增多，会变得越来越有向

/*
TODO
实现子图内的双向连接，只要传入两个需要连接的点即可
*/

/*
inline void bi_directional_connections_in_subgraph(Idtype id1, Idtype id2, Idtype sub_id) {

	//TOTHINK:在建构的时候，邻居域里的邻居id会不会有重复呢？
	//TOTHINK:此处受原HNSW的mutuallyConnectNewElement()方法启发而部分改进和删减的简化版，但是部分功能没有实现：更新机制，删除点机制，启发式机制......

	//1.首先是让id1加上id2：
	//1.1.如果id1的邻居域没有满——id1的邻居域直接添加id2
	//1.2.如果id1的邻居域满了  ——计算id2和id1的邻居节点，选择最优的（如果id1有邻居节点被删掉了，那么id1与原先邻居节点之间的无向边就会衰退为有向边），注意：原生的HNSW是有向图，利弊仍然需要权衡清楚;无向图带来的劣势尚且不清楚，但是HNSW官方为什么不用呢？？？
	pair<neigh_num*, Idtype*> pair1 = get_sub_ele_neigh_ByInternalId(id1, sub_id);

	if (*pair1.first < subNeigh_max_) {
		*(pair1.second + *pair1.first) = id2;
		(*pair1.first)++;
	}
	else
	{
		//找出最大的那个id替换掉就行
		ComputerInit com(getDataByInternalId(id1, sub_id), data_dim_);

		pair<dist_type, int> farthest;//存储距离和节点在邻居域的index
		farthest.first = com(getDataByInternalId(id2, sub_id));
		farthest.second = -1;//-1表示id2最大;


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
	//2.然后是让id2加上id1：
	pair<neigh_num*, Idtype*> pair2 = get_sub_ele_neigh_ByInternalId(id2, sub_id);

	if (*pair2.first < subNeigh_max_) {
		*(pair2.second + *pair2.first) = id1;
		(*pair2.first)++;
	}
	else
	{
		ComputerInit com(getDataByInternalId(id2, sub_id), data_dim_);//找出最大的那个id替换掉就行
		pair<dist_type, int> farthest;//存储距离和节点在邻居域的index
		dist_type temp = com(getDataByInternalId(id1, sub_id));
		farthest.first = com(getDataByInternalId(id1, sub_id));
		farthest.second = -1;//-1表示id2最大;

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