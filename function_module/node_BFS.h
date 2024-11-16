#ifndef NEW_NODE_BFS_H
#define NEW_NODE_BFS_H

#include <queue>

#include "../data_structure/data_structure.h"

// 传入扩散起始点id，传入当前处理的层数，对全图扩散一遍并选择灯塔点
void light_BFS(int node_id, int now_layer) {
    /// 两个用于BFS的队列
    // 创建BFS的队列
    queue<int> BFS_queue;
    // 创建BFS已经扩散过的标记队列，初始化大小为点的个数,并且设置所有点初始值为0 (0为未被扩散，1为已被扩散)
    vector<int> BFS_mark;
    BFS_mark.resize(max_node_id[now_layer] + 1);
    fill(BFS_mark.begin(), BFS_mark.end(), 0);
    /// 初始化操作
    // 初始点进入队列
    BFS_queue.push(node_id);
    // 加入-1进入队列代表这一轮扩散结束，用于最大扩散深度控制
    BFS_queue.push(-1);
    // 标记该点为已经扩散
    BFS_mark[node_id] = 1;
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
            // 如果队列已经空了，这是最后一个-1，则结束BFS
            if (BFS_queue.empty())
                break;
            else {
                // 否则增加一个扩散深度的标记-1
                BFS_queue.push(-1);
                continue;
            }
        }

        /// 处理当前点
        // 如果处理的当前点为黑暗(黑暗则选为灯塔，并加入下一层)
        if (all_node[now_layer].all_node[now_node_id].light == 0) {
            // 将该点设置为灯塔点并设置为明亮
            all_node[now_layer].all_node[now_node_id].tower = 1;
            all_node[now_layer].all_node[now_node_id].light = 1;
            // 最大点id增加1
            max_node_id[now_layer + 1]++;
            // 创建下一层点对该点的映射关系
            all_node[now_layer].all_node[now_node_id].next_id = max_node_id[now_layer + 1];
            // 照亮新的灯塔点附近的所有1跳邻居
            for (int x: adjacency_graph[now_layer].graph[now_node_id]) {
                all_node[now_layer].all_node[x].light = 1;
            }
        }

        /// 处理当前点的邻居点
        // 将该点未被扩散的邻居加入BFS队列
        for (int x: adjacency_graph[now_layer].graph[now_node_id]) {
            // 如果这个邻居已经被扩散过了，则跳过
            if (BFS_mark[x] == 1)
                continue;
            // 将这个没有被扩散过的邻居加入队列
            BFS_queue.push(x);
            // 标记该邻居为已经扩散
            BFS_mark[x] = 1;
        }
    }
}

// 灯塔点小范围扩散，确定下一层的边的关系
void create_edge_BFS(int node_id, int now_layer) {
    /// 两个用于BFS的队列
    // 创建BFS的队列
    queue<int> BFS_queue;
    // 创建BFS已经扩散过的标记队列，初始化大小为点的个数,并且设置所有点初始值为0 (0为未被扩散，1为已被扩散)
    vector<int> BFS_mark(max_node_id[now_layer] + 1,0);
    /// 初始化操作
    // 初始点进入队列
    BFS_queue.push(node_id);
    // 加入-1进入队列代表这一轮扩散结束，用于最大扩散深度控制
    BFS_queue.push(-1);
    // 标记该点为已经扩散
    BFS_mark[node_id] = 1;
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
            // 如果超出了扩散深度的限制，则结束扩散
            if (now_BFS_depth > max_BFS_depth)
                break;
            // 如果队列已经空了，这是最后一个-1，则结束BFS
            if (BFS_queue.empty())
                break;
            else {
                // 否则增加一个扩散深度的标记-1
                BFS_queue.push(-1);
                continue;
            }
        }

        /// 处理当前点
        // 仅将距离恰好的点连成边，过近或者过远都无视
        if (now_BFS_depth == max_BFS_depth) {
            // 如果该点为灯塔点，则构建一条新的边
            if (all_node[now_layer].all_node[now_node_id].tower == 1) {
                // 在下一层插入他们互相为邻居
                adjacency_graph[now_layer + 1].graph[all_node[now_layer].all_node[node_id].next_id].push_back(
                        all_node[now_layer].all_node[now_node_id].next_id);
                // 统计边的数量
                adjacency_graph[now_layer + 1].edge++;
            }
        }

        /// 处理当前点的邻居点
        // 将该点未被扩散的邻居加入BFS队列
        for (int x: adjacency_graph[now_layer].graph[now_node_id]) {
            // 如果这个邻居已经被扩散过了，则跳过
            if (BFS_mark[x] == 1)
                continue;
            // 将这个没有被扩散过的邻居加入队列
            BFS_queue.push(x);
            // 标记该邻居为已经扩散
            BFS_mark[x] = 1;
        }
    }
}

#endif //NEW_NODE_BFS_H
