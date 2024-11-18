#ifndef NEW_CREATE_MAP_H
#define NEW_CREATE_MAP_H

#include <vector>
#include <string>
// 随机数的头文件
#include <random>

#include "../data_structure/data_structure.h"

void write_adjacency_graph(int node_num, int min_node_degree, int max_node_degree, string dataset_name);

// 传入点的总数量、最大允许的度数和数据集名称，创建出数据集
void create_map(int node_num, int min_node_degree, int max_node_degree, string dataset_name = "default_customize_map") {
    // 设定随机数种子
    int rand_seed = 228;
    // 创建伪随机数生成器，并给定随机数种子
    mt19937 rander(rand_seed);
    // 创建了均匀分布的整数随机数生成器，定义要生成的随机数的范围(包含0和node_num-1)
    uniform_int_distribution<> dis(0, node_num - 1);
    // 用来统计每个点当前已经有的度数
    vector<int> all_degree;
    // 开辟空间
    adjacency_graph.emplace_back();
    adjacency_graph[0].graph.resize(node_num);
    all_degree.resize(node_num, 0);
    // 用来储存当前的最小度数信息
    int now_min_degree = 0;
    // 临时储存新的边的两个顶点
    int node1, node2;
    // 去除重复边的临时标记
    int flag = 0;
    // 如果当前的最小度数小于指定的最小度数，则继续创建新的边
    while (now_min_degree < min_node_degree) {
        // 生成介于0和max_node_id之间的随机数(包含0和max_node_id)
        node1 = dis(rander);
        // 刷一个合适的点出来
        while (all_degree[node1] >= max_node_degree) { node1 = dis(rander); }
        // 生成介于0和max_node_id之间的随机数(包含0和max_node_id)
        node2 = dis(rander);
        flag = 0;
        for (int x = 0; x < adjacency_graph[0].graph[node1].size(); x++) {
            if (adjacency_graph[0].graph[node1][x] == node2) {
                flag = 1;
                break;
            }
        }
        // 刷一个合适的点出来
        while (node1 == node2 || all_degree[node2] >= max_node_degree || flag == 1) {
            node2 = dis(rander);
            flag = 0;
            for (int x = 0; x < adjacency_graph[0].graph[node1].size(); x++) {
                if (adjacency_graph[0].graph[node1][x] == node2) {
                    flag = 1;
                    break;
                }
            }
        }
        // 度数统计
        all_degree[node1]++;
        all_degree[node2]++;
        // 添加新的连边
        adjacency_graph[0].graph[node1].emplace_back(node2);
        adjacency_graph[0].graph[node2].emplace_back(node1);
        adjacency_graph[0].edge++;
        // 计算最小度数
        for (int x = 0; x < all_degree.size(); x++) {
            now_min_degree = all_degree[x];
            if (now_min_degree < min_node_degree)
                break;
        }
    }
    // 以文件的形式输出邻接表
    write_adjacency_graph(node_num, min_node_degree, max_node_degree, dataset_name);
}

void write_adjacency_graph(int node_num, int min_node_degree, int max_node_degree, string dataset_name) {
    // 文件名上标注所有的信息
//    string write_adjacency_filename =
//            root_path + "data/" + dataset_name + to_string(node_num) + " " + to_string(min_node_degree) + "-" +
//            to_string(max_node_degree) + ".txt";
    // 文件名上只标注数据集的名称
    string write_adjacency_filename = root_path + "data/" + dataset_name + ".txt";
    // 定义文件输出流文件
    ofstream outfile(write_adjacency_filename);
    if (outfile.is_open()) {
        /// 输出层级划分邻接表(第0层邻接表是原图不输出，从第1层开始输出)
        for (int x = 0; x < adjacency_graph[0].graph.size(); x++) {
            // 文件输出顶点id
            outfile << x;
            // 如果这一行只有一个点，则输出完点的id之后换行
            if (adjacency_graph[0].graph[x].empty()) {
                outfile << endl;
                continue;
            } else {
                outfile << " ";
            }
            for (int y = 0; y < adjacency_graph[0].graph[x].size(); y++) {
                // 输出顶点id编号
                outfile << adjacency_graph[0].graph[x][y] << " ";
            }
            outfile << endl;
        }
        // 关闭文件输出流
        outfile.close();
        cout << endl;
    }
}

#endif //NEW_CREATE_MAP_H