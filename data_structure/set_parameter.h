#ifndef NEW_SET_PARAMETER_H
#define NEW_SET_PARAMETER_H

#include <string>
// 测算运行时间的头文件
#include <chrono>

using namespace std;

/// 设置读写图的名称
const string graph_name = "github";
//const string graph_name = "cora";
//const string graph_name = "facebook";
//const string graph_name = "twitch";
//const string graph_name = "dbpedia";

/// 测试
//const string graph_name = "mini";
//const string graph_name = "amazon";
//const string graph_name = "DBLP";

/// 设置算法的名称 (各种算法不允许同时运行，仅允许单独运行)
// 分层距离估计算法 Hierarchical Distance Estimation(HDE)
const string algorithm_name = "HDE";

/// 设置运行的模式
const string running_mode = "create";
//const string running_mode = "read";

/// 策略汇总
// 选择灯塔的策略
//const string tower_strategy = "BFS_choose"; // 均匀分布
const string tower_strategy="degree_first"; // 度数优先
// 误差消除的策略
//const string error_elimination_scheme = "more_extra_tower";
const string error_elimination_scheme = "less_extra_tower";

/// 数据参数
// 小范围扩散构建下一层的边，最大允许距离(max_BFS_depth=2代表距离为2会连成一条新的边)
const int max_BFS_depth = 2;

/// 文件根路径参数
const string root_path = "E:/essay_start/new/";

/// 以下无需调整
// 存储读写数据的文件名称
// 读入边图的数据
string graph_filename;
// 读入图的形式（边图edge_graph/邻接表adjacency_graph）
string graph_mode;
// 从文件中读入坐标数据
string read_layer_filename;
// 写入坐标信息
string write_layer_filename;
// 运行时间测量
chrono::duration<double> accurate_duration;
chrono::duration<double> layer_duration;

// 设置读写文件名称参数
void set_filename() {
    if (graph_name == "github") {
        // 设置github图的数据集的名称
        graph_filename = root_path + "data/github_graph.txt";
        // 设置图的模式为边图
        graph_mode = "edge_graph";
    } else if (graph_name == "DBLP") {
        // 设置DBLP图的数据集的名称
        graph_filename = root_path + "data/DBLP_graph.txt";
        // 设置图的模式设置为邻接表图，需要去除边的id
        graph_mode = "adjacency_graph edge_id";
    } else if (graph_name == "cora") {
        // 设置cora图的数据集的名称
        graph_filename = root_path + "data/cora_graph_reflect.txt";
        // 设置图的模式为边图
        graph_mode = "edge_graph";
    } else if (graph_name == "facebook") {
        // 设置facebook图的数据集的名称
        graph_filename = root_path + "data/facebook_graph.txt";
        // 设置图的模式为边图
        graph_mode = "edge_graph";
    } else if (graph_name == "dbpedia") {
        // 设置dbpedia图的数据集的名称
        graph_filename = root_path + "data/dbpedia_graph.txt";
        // 设置图的模式为边图
        graph_mode = "edge_graph";
    } else if (graph_name == "twitch") {
        // 设置facebook图的数据集的名称
        graph_filename = root_path + "data/twitch_graph.txt";
        // 设置图的模式为邻接表图
        graph_mode = "adjacency_graph";
    } else if (graph_name == "amazon") {
        // 设置amazon图的数据集的名称
        graph_filename = root_path + "data/amazon_graph.txt";
        // 设置图的模式为邻接表图
        graph_mode = "edge_graph";
    } else if (graph_name == "mini") {
        // 读入自定义小图的数据
        graph_filename = root_path + "data/my_mini_graph.txt";
        // 设置图的模式为边图
        graph_mode = "edge_graph";
    }
    // 仅HDE算法需要设置读取坐标文件名称
    if (algorithm_name == "HDE") {
        //从文件中读入图坐标数据
        read_layer_filename = root_path + "output_file/" + graph_name + ".txt";
        //写入图坐标信息
        write_layer_filename = root_path + "output_file/" + graph_name + ".txt";
    }
}

#endif //NEW_SET_PARAMETER_H
