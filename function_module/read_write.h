#ifndef NEW_READ_WRITE_H
#define NEW_READ_WRITE_H

#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include "../data_structure/data_structure.h"
#include "../test_module/test_module.h"

// 去指定的文件中读取数据(读取边图)
void read_edge_graph() {
    ifstream inputFile(graph_filename);
    // 边的起始点和终点
    int source, dest;
    // 读取文件 如果文件中仍然有数据未读取完，则继续读取到图中
    while (inputFile >> source >> dest) {
        // 加入边图
        edge_graph.add_edge(source, dest);
        // 边的数量统计
        adjacency_graph[0].edge++;
    }
    inputFile.close();
}

// 去指定的文件中读取邻接表图(按照指定的方式读取)
void read_adjacency_graph(int read_mode) {
    ifstream inputFile(graph_filename);
    if (inputFile.is_open()) {
        string line;
        adjacency_graph[0].graph.resize(1);
        // 不断的以字符串形式一行一行的读取数据
        while (getline(inputFile, line)) {
            // 将字符串数字化
            istringstream num_line(line);
            // 读入顶点id
            int node_id;
            num_line >> node_id;
            // 纯邻接表，不包含边的id
            if (read_mode == 1) {
                // 读入这个顶点所有的邻居
                int neighbor_id;
                while (num_line >> neighbor_id) {
                    // 预先开辟邻接表的空间
                    if (neighbor_id >= adjacency_graph[0].graph.size())
                        adjacency_graph[0].graph.resize(neighbor_id + 1);
                    // 插入这个邻居
                    adjacency_graph[0].graph[node_id].push_back(neighbor_id);
                    // 边的数量统计
                    adjacency_graph[0].edge++;
                }
            } else if (read_mode == 2) {
                // 需要去除边的id
                int neighbor_id, edge_id;
                while (num_line >> neighbor_id >> edge_id) {
                    // 预先开辟邻接表的空间
                    if (neighbor_id >= adjacency_graph[0].graph.size())
                        adjacency_graph[0].graph.resize(neighbor_id + 1);
                    // 插入这个邻居
                    adjacency_graph[0].graph[node_id].push_back(neighbor_id);
                    // 边的数量统计
                    adjacency_graph[0].edge++;
                }
            }
        }
        inputFile.close();
        // 读了双向的重复边，要除2
        adjacency_graph[0].edge = adjacency_graph[0].edge / 2;
    }
}

// 读取图总函数，根据图的不同类型来读取不同的图
void read_graph() {
    if (graph_mode == "edge_graph") {
        // 读取数据进入边图
        read_edge_graph();
        // 利用边图创建出邻接表
        adjacency_graph[0].initial_adjacency_graph(edge_graph.graph);
    } else if (graph_mode == "adjacency_graph") {
        // 直接读取邻接表
        read_adjacency_graph(1);
    } else if (graph_mode == "adjacency_graph edge_id") {
        // 读取邻接表 去除边的id
        read_adjacency_graph(2);
    }
    // 保存点的最大id值
    max_node_id.push_back(adjacency_graph[0].graph.size() - 1);
}

// 将层次划分和映射关系写入到文件中
void write_layer_and_node() {
    // 定义文件输出流文件
    ofstream outfile(write_layer_filename);
    if (outfile.is_open()) {
        /// 输出层级划分邻接表(第0层邻接表是原图不输出，从第1层开始输出)
        for (int x = 1; x < adjacency_graph.size(); x++) {
            // 输出层次的特殊标记(-1 层数 当前层的最大id编号)
            outfile << "-1 " << x << " " << max_node_id[x] << endl;
            for (int y = 0; y < adjacency_graph[x].graph.size(); y++) {
                // 输出顶点id编号
                outfile << y << " ";
                // 如果这一行只有一个点，则输出完点的id之后换行
                if (adjacency_graph[x].graph[y].empty())
                    outfile << endl;
                else
                    for (int z = 0; z < adjacency_graph[x].graph[y].size(); z++) {
                        outfile << adjacency_graph[x].graph[y][z];
                        // 每行结尾空格和回车控制
                        if (z != adjacency_graph[x].graph[y].size() - 1)
                            outfile << " ";
                        else outfile << endl;
                    }
            }
        }
        // 层次信息输出完毕的特殊标记
        outfile << "-1 " << "-1" << endl << endl;
        /// 输出所有的结点映射信息
        for (int x = 0; x < all_node.size(); x++) {
            // 输出层次的特殊标记
            outfile << "-1 " << x << endl;
            for (int y = 0; y < all_node[x].all_node.size(); y++) {
                // 输出其中的node信息(tower next_id)
                outfile << all_node[x].all_node[y].tower << " " << all_node[x].all_node[y].next_id << endl;
            }
        }
        // 结点映射信息输出完毕的特殊标记
        outfile << "-1 " << "-1" << endl;
        // 关闭文件输出流
        outfile.close();
        cout << endl;
    }
}

// 直接从文件中读取层次划分和映射关系
int read_layer_and_node() {
    ifstream file(read_layer_filename);
    // 用来标记当前正在读取的层数信息
    int now_layer = 0;
    // 保存当前正在读入的点的id
    int now_node_id = 0;
    // 读入是否是灯塔
    int tower = -1;
    // 读入与下一层顶点的映射关系
    int next_id = -1;
    if (file.is_open()) {
        string line;
        /// 不断的以字符串形式一行一行的读取数据 (先读取分层邻接表)
        while (getline(file, line)) {
            // 读取到空行则再继续读取下一行
            if (line.empty())
                continue;
            // 将字符串数字化
            istringstream num_line(line);
            num_line >> now_node_id;
            // 如果读取到特殊标记(-1代表新的一层或者读取结束)
            if (now_node_id == -1) {
                // 读取层数信息
                num_line >> now_layer;
                // 如果层数信息也是特殊标记，那么读取已经结束，退出
                if (now_layer == -1)
                    break;
                else {
                    // 创建出新的一层邻接表所需的所有数据结构
                    adjacency_graph.emplace_back(); // 邻接表
                    max_node_id.emplace_back(-1); // 最大点的id
                    // 读取当前层最大的点的id编号
                    num_line >> max_node_id[now_layer];
                    // 设置总的最大层数
                    max_layer_id = now_layer;
                    // 预先开辟邻接表的空间
                    adjacency_graph[now_layer].graph.resize(max_node_id[now_layer] + 1);
                }
                // 这一行是特殊信息，读取完毕之后进入下一行
                continue;
            }
            // 读入这个顶点所有的邻居
            int neighbor_id;
            while (num_line >> neighbor_id) {
                // 插入这个邻居
                adjacency_graph[now_layer].graph[now_node_id].push_back(neighbor_id);
                // 边的数量统计
                adjacency_graph[now_layer].edge++;
            }
            // 中途输出测试信息
            test_halfway(now_node_id, max_node_id[now_layer] + 1, "layer" + to_string(now_layer) + "_reading", 1000);
        }
        // 输出记录到的最大层数
        cout << "\n\nmax_layer_id is: " << max_layer_id << endl;
        // 读了双向的重复边，要除2
        for (int x = 1; x <= max_layer_id; x++){
            adjacency_graph[x].edge /= 2;
        }
        /// 不断的以字符串形式一行一行的读取数据 (再读取点的映射关系)
        while (getline(file, line)) {
            // 读取到空行则再继续读取下一行
            if (line.empty())
                continue;
            // 将字符串数字化
            istringstream num_line(line);
            // 读入灯塔信息
            num_line >> tower;
            // 如果读取到特殊标记(-1代表新的一层或者读取结束)
            if (tower == -1) {
                // 读取层数信息
                num_line >> now_layer;
                // 如果层数信息也是特殊标记，那么读取已经结束，退出
                if (now_layer == -1)
                    break;
                // 创建出新的一层映射关系所需的所有数据结构
                all_node.emplace_back();
                // 预先开辟点的映射关系所需要的空间
                all_node[now_layer].all_node.resize(max_node_id[now_layer] + 1);
                // 重置 用于记录当前的id
                now_node_id = 0;
                // 这一行是特殊信息，读取完毕之后进入下一行
                continue;
            } else {
                // 中途输出测试信息
                test_halfway(now_node_id, max_node_id[now_layer] + 1, "node_reading layer:" + to_string(now_layer),
                             1000);
                // 读入映射关系
                num_line >> next_id;
                // 将读取到的tower和next_id赋值进去
                all_node[now_layer].all_node[now_node_id].tower = tower;
                all_node[now_layer].all_node[now_node_id].next_id = next_id;
                // 用于记录当前的id+1
                now_node_id++;
            }
        }
        cout << "\n\n";
        // 读取完毕，关闭文件
        file.close();
        // 正常完成的返回值
        return 0;
    } else {
        cout << "file open failed!" << endl;
        // 文件打开失败，终止后续操作
        return -1;
    }
}

#endif //NEW_READ_WRITE_H
