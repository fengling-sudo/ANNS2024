#pragma once
//有向连接器

#include "connectorInterface.h"

template<typename T>
struct directedConnector : connector
{

	directedConnector(size_t size_per_element_in_sub, size_t offset_to_data, Computer* computer) :connector(size_per_element_in_sub, offset_to_data, computer) {}

	//TODO:目前这个方法只能给【初次构建服务】
	inline void establishConnection(Idtype point_id, Idtype neigh_id, SubNode* sub, size_t subNeigh_max_) {

		pair<neigh_num*, Idtype*> pair_ = get_sub_ele_neigh_ByInternalId(point_id, sub);
		if (*(pair_.first) < subNeigh_max_) {
			*(pair_.second + *pair_.first) = neigh_id;
			(*pair_.first)++;
		}
		else
		{
			computer->setQuantifiedQuery(getDataByInternalId(point_id, sub));

			pair<dist_type, int> farthest;
			farthest.first = (*computer)(getDataByInternalId(neigh_id, sub));
			farthest.second = -1;//-1表示id2最大;

			for (int i = 0; i < subNeigh_max_; i++) {
				Idtype old_neighID = *(pair_.second + 1);
				dist_type old_neighDist = (*computer)(getDataByInternalId(old_neighID, sub));
				if (old_neighDist > farthest.first) {
					farthest.first = old_neighDist;
					farthest.second = i;
				}
			}

			if (farthest.second != -1) {
				*(pair_.second + farthest.second) = neigh_id;
			}




		}

	}

};