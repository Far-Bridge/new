#ifndef NEW_CALCULATE_DISTANCE_H
#define NEW_CALCULATE_DISTANCE_H

#include <queue>
#include <vector>
// 最大整数的常量头文件
#include <climits>
// 取绝对值的头文件
#include <cmath>

#include "../data_structure/data_structure.h"

// 传入两个点的id，输出精确距离(跳数)
// 正数为两点之间距离，-1为不可达
int accurate_distance(int node_id1, int node_id2) {
    /// 两个用于BFS的队列
    // 创建BFS的队列
    queue<int> BFS_queue;
    // 创建BFS已经扩散过的标记队列，初始化大小为点的个数,并且设置所有点初始值为0 (0为未被扩散，1为已被扩散)
    vector<int> BFS_mark;
    BFS_mark.resize(max_node_id[0] + 1);
    fill(BFS_mark.begin(), BFS_mark.end(), 0);

    /// 初始化操作
    // 初始点进入队列
    BFS_queue.push(node_id1);
    // 加入-1进入队列代表这一轮扩散结束，用于最大扩散深度控制
    BFS_queue.push(-1);
    // 标记该点为已经扩散
    BFS_mark[node_id1] = 1;
    // 用于储存当前BFS的深度
    int now_BFS_depth = 0;
    // 用于储存当前正在操作的点的id
    int now_node_id;

    /// 正式的BFS遍历
    // 开始BFS遍历 如果BFS队列不为空
    while (!BFS_queue.empty()) {
        // 弹出队首点的id并保存
        now_node_id = BFS_queue.front();
        BFS_queue.pop();

        /// 退出循环的条件
        // 如果队首是-1，代表该轮深度的BFS扩散结束
        if (now_node_id == -1) {
            // 标记进入了下一轮扩散
            now_BFS_depth++;
            // 如果队列已经空了，这是最后一个-1，则结束BFS(未找到目标点)
            if (BFS_queue.empty())
                return -1;
            else {
                // 否则增加一个扩散深度的标记-1
                BFS_queue.push(-1);
                continue;
            }
        }

        /// 处理当前点
        // 如果找到了第二个点,返回他们之间的距离(跳数)
        if (now_node_id == node_id2)
            return now_BFS_depth;

        /// 处理当前点的邻居点
        // 将该点未被扩散的邻居加入BFS队列
        for (int x: adjacency_graph[0].graph[now_node_id]) {
            // 如果这个邻居已经被扩散过了，则跳过
            if (BFS_mark[x] == 1)
                continue;
            // 将这个没有被扩散过的邻居加入队列
            BFS_queue.push(x);
            // 标记该邻居为已经扩散
            BFS_mark[x] = 1;
        }
    }
    return -1;
}

// 使用分层结构估算距离(跳数)
// 传入两个点的id，层数，非灯塔转移最多允许一次的标记（no_tower=1则已经进行过一次非灯塔转移）
int layer_distance_scheme1(int node_id1, int node_id2, int now_layer, int no_tower1 = 0, int no_tower2 = 0) {
    // 测试
//    cout << node_id1 << " " << node_id2 << " " << now_layer << endl;
    // 存储层次计算中的最小距离
    int layer_min_distance = INT_MAX;
    // 用于暂存计算结果
    int temp;
    /// 递归结束条件
    // 相同点返回0距离
    if (node_id1 == node_id2)
        return 0;
    // 另外一个点没有邻居，距离设置为不可达
    if (adjacency_graph[now_layer].graph[node_id1].empty() || adjacency_graph[now_layer].graph[node_id2].empty()) {
        if (now_layer == 0)
            return -1;
        else
            return INT_MAX;
    }
    // 邻居点直接返回层次距离
    for (int x = 0; x < adjacency_graph[now_layer].graph[node_id1].size(); x++) {
        // 如果他们是邻居
        if (adjacency_graph[now_layer].graph[node_id1][x] == node_id2)
            // 返回层次距离
            return (1 << now_layer);
    }
    /// 进入下一层递归条件
    // 情况1：两个点在这一层都是灯塔
    if (all_node[now_layer].all_node[node_id1].tower == 1 && all_node[now_layer].all_node[node_id2].tower == 1) {
        // 无需排除点，直接转换下一层id，进入下一层(转移符号重置为0)
        temp = layer_distance_scheme1(all_node[now_layer].all_node[node_id1].next_id,
                                      all_node[now_layer].all_node[node_id2].next_id, now_layer + 1, 0, 0);
        if (layer_min_distance > temp) {
            layer_min_distance = temp;
        }
        for (int x = 0; x < adjacency_graph[now_layer].graph[node_id1].size(); x++) {
            // TODO 已经转移过了就不允许再转移了
            if (all_node[now_layer].all_node[adjacency_graph[now_layer].graph[node_id1][x]].tower == 1 &&
                no_tower1 == 1)
                continue;
            // TODO 邻居为灯塔，允许转移一次
            if (all_node[now_layer].all_node[adjacency_graph[now_layer].graph[node_id1][x]].tower == 1) {
                temp = layer_distance_scheme1(adjacency_graph[now_layer].graph[node_id1][x], node_id2, now_layer, 1,
                                              no_tower2);

                if (temp != INT_MAX && layer_min_distance > (1 << now_layer) + temp) {
                    layer_min_distance = (1 << now_layer) + temp;
                }
            }
        }
        for (int x = 0; x < adjacency_graph[now_layer].graph[node_id2].size(); x++) {
            // TODO 已经转移过了就不允许再转移了
            if (all_node[now_layer].all_node[adjacency_graph[now_layer].graph[node_id2][x]].tower == 1 &&
                no_tower2 == 1)
                continue;
            // TODO 邻居为灯塔，允许转移一次
            if (all_node[now_layer].all_node[adjacency_graph[now_layer].graph[node_id2][x]].tower == 1) {
                temp = layer_distance_scheme1(node_id1, adjacency_graph[now_layer].graph[node_id2][x], now_layer,
                                              no_tower1, 1);

                if (temp != INT_MAX && layer_min_distance > (1 << now_layer) + temp) {
                    layer_min_distance = (1 << now_layer) + temp;
                }
            }
        }
        if (layer_min_distance == INT_MAX) {
            if (now_layer == 0)
                return -1;
            else
                return INT_MAX;
        }
        return layer_min_distance;
    } else if (all_node[now_layer].all_node[node_id1].tower == 0) {
        // 情况2：node1不是灯塔
        for (int x = 0; x < adjacency_graph[now_layer].graph[node_id1].size(); x++) {
            // 找到node1邻居灯塔，停留在这一层
            temp = layer_distance_scheme1(adjacency_graph[now_layer].graph[node_id1][x], node_id2,
                                          now_layer, no_tower1, no_tower2);
            if (temp != INT_MAX && layer_min_distance > (1 << now_layer) + temp) {
                layer_min_distance = (1 << now_layer) + temp;
            }
        }
        // 返回得出的最小距离
        if (layer_min_distance == INT_MAX) {
            if (now_layer == 0)
                return -1;
            else
                return INT_MAX;
        }
        return layer_min_distance;
    } else if (all_node[now_layer].all_node[node_id2].tower == 0) {
        // 情况3：node2不是灯塔
        for (int x = 0; x < adjacency_graph[now_layer].graph[node_id2].size(); x++) {
            // 找到node2邻居灯塔，停留在这一层
            temp = layer_distance_scheme1(node_id1, adjacency_graph[now_layer].graph[node_id2][x],
                                          now_layer, no_tower1, no_tower2);
            if (temp != INT_MAX && layer_min_distance > (1 << now_layer) + temp) {
                layer_min_distance = (1 << now_layer) + temp;
            }
        }
        // 返回得出的最小距离
        if (layer_min_distance == INT_MAX) {
            if (now_layer == 0)
                return -1;
            else
                return INT_MAX;
        }
        return layer_min_distance;
    }
}

// 使用分层结构估算距离(跳数)
// 传入两个点的id，层数，非灯塔转移最多允许一次的标记（no_tower=1则已经进行过一次非灯塔转移）
int layer_distance_scheme2(int node_id1, int node_id2, int now_layer, int no_tower1 = 0, int no_tower2 = 0) {
    // 测试
//    cout << node_id1 << " " << node_id2 << " " << now_layer << endl;
    // 存储层次计算中的最小距离
    int layer_min_distance = INT_MAX;
    // 用于暂存计算结果
    int temp;
    /// 递归结束条件
    // 相同点返回0距离
    if (node_id1 == node_id2)
        return 0;
    // 另外一个点没有邻居，距离设置为不可达
    if (adjacency_graph[now_layer].graph[node_id1].empty() || adjacency_graph[now_layer].graph[node_id2].empty()) {
        if (now_layer == 0)
            return -1;
        else
            return INT_MAX;
    }
    // 邻居点直接返回层次距离
    for (int x = 0; x < adjacency_graph[now_layer].graph[node_id1].size(); x++) {
        // 如果他们是邻居
        if (adjacency_graph[now_layer].graph[node_id1][x] == node_id2)
            // 返回层次距离
            return (1 << now_layer);
    }
    /// 进入下一层递归条件
    // 情况1：两个点在这一层都是灯塔
    if (all_node[now_layer].all_node[node_id1].tower == 1 && all_node[now_layer].all_node[node_id2].tower == 1) {
        // 无需排除点，直接转换下一层id，进入下一层(转移符号重置为0)
        temp = layer_distance_scheme2(all_node[now_layer].all_node[node_id1].next_id,
                                      all_node[now_layer].all_node[node_id2].next_id, now_layer + 1, 0, 0);
        if (layer_min_distance > temp) {
            layer_min_distance = temp;
        }
        for (int x = 0; x < adjacency_graph[now_layer].graph[node_id1].size(); x++) {
            // TODO 已经转移过了就不允许再转移了
            if (all_node[now_layer].all_node[adjacency_graph[now_layer].graph[node_id1][x]].tower == 1 &&
                no_tower1 == 1)
                continue;
            // TODO 邻居为灯塔，允许转移一次
            if (all_node[now_layer].all_node[adjacency_graph[now_layer].graph[node_id1][x]].tower == 1) {
                temp = layer_distance_scheme2(adjacency_graph[now_layer].graph[node_id1][x], node_id2, now_layer, 1,
                                              no_tower2);

                if (temp != INT_MAX && layer_min_distance > (1 << now_layer) + temp) {
                    layer_min_distance = (1 << now_layer) + temp;
                }
            }
        }
        for (int x = 0; x < adjacency_graph[now_layer].graph[node_id2].size(); x++) {
            // TODO 已经转移过了就不允许再转移了
            if (all_node[now_layer].all_node[adjacency_graph[now_layer].graph[node_id2][x]].tower == 1 &&
                no_tower2 == 1)
                continue;
            // TODO 邻居为灯塔，允许转移一次
            if (all_node[now_layer].all_node[adjacency_graph[now_layer].graph[node_id2][x]].tower == 1) {
                temp = layer_distance_scheme2(node_id1, adjacency_graph[now_layer].graph[node_id2][x], now_layer,
                                              no_tower1, 1);

                if (temp != INT_MAX && layer_min_distance > (1 << now_layer) + temp) {
                    layer_min_distance = (1 << now_layer) + temp;
                }
            }
        }
        if (layer_min_distance == INT_MAX) {
            if (now_layer == 0)
                return -1;
            else
                return INT_MAX;
        }
        return layer_min_distance;
    } else if (all_node[now_layer].all_node[node_id1].tower == 0) {
        // 情况2：node1不是灯塔
        for (int x = 0; x < adjacency_graph[now_layer].graph[node_id1].size(); x++) {
            // TODO 已经转移过了就不允许再转移了
            if (all_node[now_layer].all_node[adjacency_graph[now_layer].graph[node_id1][x]].tower == 1 &&
                no_tower1 == 1)
                continue;
            // TODO 邻居为灯塔，允许转移一次
            if (all_node[now_layer].all_node[adjacency_graph[now_layer].graph[node_id1][x]].tower == 1) {
                temp = layer_distance_scheme2(adjacency_graph[now_layer].graph[node_id1][x], node_id2, now_layer, 1,
                                              no_tower2);

                if (temp != INT_MAX && layer_min_distance > (1 << now_layer) + temp) {
                    layer_min_distance = (1 << now_layer) + temp;
                }
            }
        }
        // 返回得出的最小距离
        if (layer_min_distance == INT_MAX) {
            if (now_layer == 0)
                return -1;
            else
                return INT_MAX;
        }
        return layer_min_distance;
    } else if (all_node[now_layer].all_node[node_id2].tower == 0) {
        // 情况3：node2不是灯塔
        for (int x = 0; x < adjacency_graph[now_layer].graph[node_id2].size(); x++) {
            // TODO 已经转移过了就不允许再转移了
            if (all_node[now_layer].all_node[adjacency_graph[now_layer].graph[node_id2][x]].tower == 1 &&
                no_tower2 == 1)
                continue;
            // TODO 邻居为灯塔，允许转移一次
            if (all_node[now_layer].all_node[adjacency_graph[now_layer].graph[node_id2][x]].tower == 1) {
                temp = layer_distance_scheme2(node_id1, adjacency_graph[now_layer].graph[node_id2][x], now_layer,
                                              no_tower1, 1);

                if (temp != INT_MAX && layer_min_distance > (1 << now_layer) + temp) {
                    layer_min_distance = (1 << now_layer) + temp;
                }
            }
        }
        // 返回得出的最小距离
        if (layer_min_distance == INT_MAX) {
            if (now_layer == 0)
                return -1;
            else
                return INT_MAX;
        }
        return layer_min_distance;
    }
}

#endif //NEW_CALCULATE_DISTANCE_H
