#ifndef NEW_ERROR_ELIMINATION_H
#define NEW_ERROR_ELIMINATION_H

#include "../data_structure/data_structure.h"

// 为了消除误差加入的新的结构
// 方案1：添加额外灯塔，完全消除两个非灯塔相邻的情况，完全消除误差
void add_extra_tower(int now_layer) {
    // 不允许任何相邻的非灯塔点
    if (error_elimination_scheme == "more_extra_tower") {
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
    // 不允许一个点周围全是非灯塔点
    if (error_elimination_scheme == "less_extra_tower") {
        // 用于标记是否要添加灯塔
        int flag;
        for (int x = 0; x <= max_node_id[now_layer]; x++) {
            // 如果当前顶点不是灯塔
            if (all_node[now_layer].all_node[x].tower == 0) {
                flag = 1;

                // 孤立点不选为灯塔
                if (adjacency_graph[now_layer].graph[x].size() == 0)
                    flag = 0;
                else // 遍历当前点的邻居点
                    for (int y = 0; y < adjacency_graph[now_layer].graph[x].size(); y++) {
                        // 如果当前点存在一个邻居是灯塔点，则该点不选取为灯塔点
                        if (all_node[now_layer].all_node[adjacency_graph[now_layer].graph[x][y]].tower == 1) {
                            flag = 0;
                            break;
                        }
                    }
                // 如果所有的邻居都为非灯塔点，则把该点选为灯塔点
                if (flag == 1) {
                    // 将当前点也设置为灯塔点
                    all_node[now_layer].all_node[x].tower = 1;
                    // 最大点id增加1
                    max_node_id[now_layer + 1]++;
                    // 创建下一层点对该点的映射关系
                    all_node[now_layer].all_node[x].next_id = max_node_id[now_layer + 1];
                }
            }
        }
    }
}

#endif //NEW_ERROR_ELIMINATION_H
