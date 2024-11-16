#ifndef NEW_DISTINGUISH_LAYER_H
#define NEW_DISTINGUISH_LAYER_H

#include "../data_structure/data_structure.h"
#include "../test_module/test_module.h"
#include "error_elimination.h"
#include "node_BFS.h"

void choose_tower(int);

void create_next_layer();

// 将原图分成不同的层
void distinguish_layer() {
    // 输出当前最大分层
    cout << "\rmax_layer is: \t\t\t" << max_layer_id + 1;
    // 判断是否需要继续分层
    // TODO 暂定为当前图为完全图时候停止分层
    while (adjacency_graph[max_layer_id].edge != (max_node_id[max_layer_id] * (max_node_id[max_layer_id] + 1)) / 2) {
        // 创建出新的一层所需的所有数据结构
        all_node.emplace_back(); // 点的信息
        adjacency_graph.emplace_back(); // 邻接表
        max_node_id.emplace_back(-1); // 最大点的id
        // 在最高层选出灯塔，建立映射关系，记录信息
        choose_tower(max_layer_id);
        // 消除误差的方案1
        add_extra_tower(max_layer_id);
        // 根据已经有的信息，创建新的一层
        create_next_layer();
        // 到这里，新的一层创建完毕，进行统计
        max_layer_id++;
        // 输出当前最大分层
        cout << "\rmax_layer is: \t\t\t" << max_layer_id + 1;
    }
    cout << endl;
}

// 传入当前已经存在的最大层数，选出所有灯塔点，为构建下一层做准备
void choose_tower(int now_layer) {
    /// 如果选择灯塔的策略是度数优先
    if (tower_strategy == "degree_first") {
        // 暂时存储最大度数点的id和最大的度数
        int max_degree_node = 1, max_degree = 0;
        // 临时变量
        int temp;
        // 初始化度数数组
        node_degree.initial_node_degree(adjacency_graph[now_layer].graph, now_layer);
        // 不断选出灯塔
        while (max_degree_node != 0) {
            max_degree = 0;
            /// 选出最大剩余度数的点
            for (int x = 0; x < node_degree.node_degree.size(); x++) {
                // 保存最大剩余度数的点
                if (node_degree.node_degree[x] > max_degree) {
                    max_degree_node = x;
                    max_degree = node_degree.node_degree[x];
                }
            }
            // 如果所有点中最大度数等于1，则不要继续选择灯塔了
            if (node_degree.node_degree[max_degree_node] == 1)
                break;
            /// 选择其为灯塔
            all_node[now_layer].all_node[max_degree_node].tower = 1;
            // 最大点id增加1
            max_node_id[now_layer + 1]++;
            // 创建下一层点对该点的映射关系
            all_node[now_layer].all_node[max_degree_node].next_id = max_node_id[now_layer + 1];
            // 已经被选择为灯塔的点，不需要再次被选择，设置其剩余度数为0
            node_degree.node_degree[max_degree_node] = 0;
            /// 遍历选出点的邻居，将他们剩余度数-1
            for (int x = 0; x < adjacency_graph[now_layer].graph[max_degree_node].size(); x++) {
                temp = adjacency_graph[now_layer].graph[max_degree_node][x];
                if (node_degree.node_degree[temp] > 0)
                    node_degree.node_degree[temp] -= 1;
            }
        }

    }
        /// 如果选择灯塔的策略是均匀分布
    else if (tower_strategy == "BFS_choose") {
        // 对所有的黑暗点进行遍历，直到没有黑暗点
        for (int x = 0; x <= max_node_id[now_layer]; x++) {
            // 如果这一层的某个点是黑暗的(以id为序)
            if (all_node[now_layer].all_node[x].light == 0) {
                // 以这个点为起始点向周围进行扩散
                light_BFS(x, now_layer);
            }
        }
    }
}

// 传入当前的层数，根据当前层以及信息，构建完整的下一层
void create_next_layer() {
    /// 创建下一层的邻接表
    // 为邻接表开辟新的空间
    adjacency_graph[max_layer_id + 1].graph.resize(max_node_id[max_layer_id + 1] + 1);
    // 对当前层的每一个灯塔点做小范围BFS扩散，来确定下一层的边(距离为2跳则构建一条边)
    for (int x = 0; x <= max_node_id[max_layer_id]; x++) {
        // 中途输出测试信息
        test_halfway(x, max_node_id[max_layer_id], "create_edge layer:" + to_string(max_layer_id + 1));
        // 如果该点在上一层是灯塔点
        if (all_node[max_layer_id].all_node[x].tower == 1) {
            // 进行小范围的BFS扩散，来确定新的边
            create_edge_BFS(x, max_layer_id);
        }
    }
    // 边的数量由于统计了两次，这里除以2
    adjacency_graph[max_layer_id + 1].edge /= 2;
    // 使用下一层最大点的数量和邻接表来创建新的一层
    all_node[max_layer_id + 1].initial_all_light_node(max_node_id[max_layer_id + 1],
                                                      adjacency_graph[max_layer_id + 1].graph);
}

#endif //NEW_DISTINGUISH_LAYER_H
