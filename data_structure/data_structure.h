#ifndef NEW_DATA_STRUCTURE_H
#define NEW_DATA_STRUCTURE_H

// 优先队列所需头文件
#include <queue>
#include <unordered_map>
// 引入可设置参数
#include "set_parameter.h"
// 引入全局变量
#include "global_variable.h"

using namespace std;

// 边图类(原封不动的保存读取的数据)
class Edge_Graph {
public:
    // 类中唯一的变量，存储邻接表，不改变
    vector<vector<int>> graph;

    // 读入新的边（起始点id，终点id）
    void add_edge(int source, int dest) {
        graph.push_back({source, dest});
    }

    // 输出整个边图
    void print_graph() {
        for (auto &i: graph) {
            cout << i[0] << " " << i[1] << endl;
        }
    }
};

// 邻接表类
class Adjacency_Graph {
public:
    // 类中唯一的变量，存储邻接表，不改变
    vector<vector<int>> graph;
    // 边的数量统计
    int edge = 0;

    // 无参构造
    Adjacency_Graph() = default;

    // 传入顶点个数和边图，创建出邻接表
    void initial_adjacency_graph(const vector<vector<int>> &temp_edge_graph) {
        // 将整张边图加入到邻接表里面去
        for (const auto &i: temp_edge_graph) {
            add_edge(i[0], i[1]);
        }
    }

    // 添加边（起始点id，终点id）
    void add_edge(int source, int dest) {
        // 将graph的大小始终调整为装得下的大小
        if (source >= graph.size())
            graph.resize(source + 1);
        if (dest >= graph.size())
            graph.resize(dest + 1);
        graph[source].push_back(dest);
        // 如果是有向图，则不需要这一句双向添加
        graph[dest].push_back(source);
    }

    // 输出整个邻接表
    void print_graph() {
        for (int i = 0; i < graph.size(); i++) {
            std::cout << "Vertex " << i << " is connected to: ";
            for (int j: graph[i]) {
                std::cout << j << " ";
            }
            std::cout << std::endl;
        }
    }
};

// 单个点的类(存储每个单点的额外信息)
class Node {
public:
    // 点的id，初始化为0
    int id = 0;
    // 该点是否被照亮，初始化为亮（light=1被照亮，light=0黑暗）
    int light = 1;
    // 该点是否被选取为灯塔(tower=1为灯塔，tower=0为非灯塔)
    int tower = 0;
    // 该点在下一层的id为多少(-1为下一层不存在该点)
    int next_id = -1;

    //无参构造
    Node() = default;

    // 有参构造
    Node(int node_id, int node_light) {
        id = node_id;
        light = node_light;
    }
};

// 所有点的信息
class All_Node {
public:
    // 用于标记所有点的信息
    vector<Node> all_node;

    // 传入顶点个数和邻接表图，预先开辟好all_light_node的空间，并做好初始化
    void initial_all_light_node(int node_num, const vector<vector<int>> &temp_adjacency_graph) {
        // 将all_light_node的大小调整为node_num+1大小
        all_node.resize(node_num + 1);
        for (int x = 0; x <= node_num; x++) {
            // 初始化点编号
            all_node[x].id = x;
            // 初始的时候所有点都定义为明亮
            light(x);
        }
        // 根据邻接表图，来标记初始黑暗的点
        for (int x = 0; x < temp_adjacency_graph.size(); x++) {
            // 如果该点的邻居集不为空（顶点存在）
            if (!temp_adjacency_graph[x].empty()) {
                // 将该点设置为黑暗
                dark(x);
            }
        }
    }

    // 点亮该点
    void light(int node_id) {
        // 点亮（1为亮）
        all_node[node_id].light = 1;
    }

    // 使得该点为黑暗
    void dark(int node_id) {
        // 关闭（0为暗）
        all_node[node_id].light = 0;
    }

    // 将该点设置为灯塔
    void set_tower(int node_id) {
        // 点亮（1为亮）
        all_node[node_id].tower = 1;
    }

    // 输出点的所有信息
    void print_node() {
        for (auto &x: all_node) {
            cout << "id: " << x.id << " " << "light: " << x.light << " " << "tower: " << x.tower << " " << "next_id: "
                 << x.next_id << endl;
        }
    }
};

// 每一层的映射关系
//class Layer_mapping {
//public:
//    // 第一维：层数，原图为0层
//    // 第二维，映射关系，layer_mapping[0][0]=n，下一层的0号点在这一层是n号点
//    vector<vector<int>> layer_mapping;
//
//    // 输出所有层之间的映射关系
//    void print_mapping() {
//        for (int x = 0; x < layer_mapping.size(); x++) {
//            // 输出这是第几层
//            cout << "layer " << x << " :" << endl;
//            for (int y = 0; y < layer_mapping[x].size(); y++) {
//                cout << layer_mapping[x][y] << "->" << y << "  ";
//            }
//            cout << endl << endl;
//        }
//    }
//};

// 所有点的度数排序类
class Node_Degree {
public:
    vector<int> node_degree;

    // 进行点的度数排序初始化
    void initial_node_degree(const vector<vector<int>> &temp_adjacency_graph, int now_layer) {
        // 重置node_degree的大小和内容
        node_degree.resize(max_node_id[now_layer] + 1, 0);
        // 根据邻接表初始化node_degree的内容
        for (int x = 0; x < temp_adjacency_graph.size(); x++) {
            node_degree[x] = temp_adjacency_graph[x].size();
        }
    }

    // 输出所有点的度数信息
    void print_node_degree() {
        for (int x = 0; x < node_degree.size(); x++)
            cout << "node_id:" << x << " degree:" << node_degree[x] << endl;
    }
};

/// 实例化所有的类
// 实例化边图类
Edge_Graph edge_graph;
// 实例化邻接表
vector<Adjacency_Graph> adjacency_graph;
// 实例化所有点信息数组
vector<All_Node> all_node;
// 实例化所有点的度数储存数组
Node_Degree node_degree;

#endif //NEW_DATA_STRUCTURE_H
