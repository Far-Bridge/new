#ifndef NEW_ERROR_ELIMINATION_H
#define NEW_ERROR_ELIMINATION_H

#include "../data_structure/data_structure.h"

// 为了消除误差加入的新的结构
// 方案1：添加额外灯塔，完全消除两个非灯塔相邻的情况，完全消除误差
void add_extra_tower(int now_layer) {
    // 启用了这个误差消除方案
    if (error_elimination_scheme == "add_extra_tower")
        for (int x = 0; x <= max_node_id[now_layer]; x++) {
            // 如果当前顶点不是灯塔
            if (all_node[now_layer].all_node[x].tower == 0) {
                // 遍历当前点的邻居点
                for (int y = 0; y < adjacency_graph[now_layer].graph[x].size(); y++) {
                    // 如果当前点存在一个邻居也不是灯塔点
                    if (all_node[now_layer].all_node[adjacency_graph[now_layer].graph[x][y]].tower == 0) {
                        // 将当前点也设置为灯塔点
                        all_node[now_layer].all_node[x].tower = 1;
                        // 最大点id增加1
                        max_node_id[now_layer + 1]++;
                        // 创建下一层点对该点的映射关系
                        all_node[now_layer].all_node[x].next_id = max_node_id[now_layer + 1];
                        break;
                    }
                }
            }
        }
}

#endif //NEW_ERROR_ELIMINATION_H
