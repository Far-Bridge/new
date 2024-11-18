#ifndef NEW_TEST_MODULE_H
#define NEW_TEST_MODULE_H

// 随机数的头文件
#include <random>

#include "../function_module/calculate_distance.h"

using namespace std;

// 输出分隔线，并打印注释
void separate_line(const string &comment);

// 测试输出所有信息的函数
void test_information(const string &information = "all") {
    // 输出边图
    // 先输出分割线和注释
    if (information == "edge_graph" || information == "all") {
        separate_line("edge_graph");
        edge_graph.print_graph();
    }
    // 输出邻接表
    // 先输出分割线和注释
    if (information == "adjacency_graph" || information == "all") {
        separate_line("adjacency_graph");
        adjacency_graph[1].print_graph();
    }
    // 输出点的所有信息
    // 先输出分割线和注释
    if (information == "all_node" || information == "all") {
        separate_line("all_node");
        all_node[1].print_node();
    }
    // 输出所有点的剩余度数
    // 先输出分割线和注释
    if (information == "node_degree" || information == "all") {
        separate_line("node_degree");
        node_degree.print_node_degree();
    }
}

// 输出分隔线，并打印注释
void separate_line(const string &comment) {
    cout << endl;
    cout << "******************************************************************" << endl;
    cout << "*********************    " << comment << "    ********************" << endl;
}

// 输出基础的信息
void basic_information_print() {
    system("cls");
    separate_line("basic_information");
    // 输出点的最大id值
    cout << "max_node_id is: \t\t" << max_node_id[0] << endl;
    // 输出总共的边的数量
    cout << "total_edge_num is: \t\t" << adjacency_graph[0].edge << endl;
    // 输出当前处理的数据集名称
    cout << "graph_name is: \t\t\t" << graph_name << endl;
    // 输出运行模式
    cout << "running_mode is: \t\t" << running_mode << endl;
}

// 传入已经扩散的灯塔数量，进行中途的输出
inline void test_halfway(int complete_node, int total_node, const string &comment = "", int per_node = 100) {
    // 第一次先输出分割线
    if (complete_node == 0)
        separate_line(comment);
    // 每次扩散完成per_light_tower个灯塔之后中途输出一次
    if (complete_node % per_node == 0) {
        cout << "\r" << comment << ": complete_node: " << complete_node << "\t";
        printf("complete: %.2f", (float(complete_node) / float(total_node)) * 100);
        cout << "%";
    }
}

// 测试输出两个点之间的精确距离和坐标计算距离，如果准确返回0，不准确返回误差值
int test_distance(int node1_id, int node2_id) {
    // 储存精确距离
    int accurate_hop = 0;
    // 存储HDE返回结果
    int layer_hop = -1;
    // 储存SPT返回结果
    int SPT_distance_num = 0;
    // 储存CDA返回结果
    int CDA_distance_num = 0;
    // 计算精确距离并统计运行时间
    auto accurate_start = std::chrono::high_resolution_clock::now();
    accurate_hop = accurate_distance(node1_id, node2_id);
    auto accurate_end = std::chrono::high_resolution_clock::now();
    // LCA距离估计 返回三个值{估算距离，相减的绝对值的最大值，相加的最小值}
    auto layer_start = std::chrono::high_resolution_clock::now();
    if (algorithm_name == "HDE") {
        if (error_elimination_strategy == "more_extra_tower")
            layer_hop = layer_distance_scheme1(node1_id, node2_id, 0);
        else if (error_elimination_strategy == "less_extra_tower")
            layer_hop = layer_distance_scheme2(node1_id, node2_id, 0);
    }

    auto layer_end = std::chrono::high_resolution_clock::now();
    accurate_duration += accurate_end - accurate_start;
    layer_duration += layer_end - layer_start;
    cout << "node1: " << node1_id << "\t";
    cout << "node2: " << node2_id << "\t";
    /// 输出精确距离
    cout << "accurate: " << accurate_hop << "\t";
    /// 输出坐标计算的距离
    // LCA算法
    if (algorithm_name == "HDE") {
        // 输出使用坐标计算得出的距离
        cout << "layer distance: " << layer_hop << "\t";
        // 返回这次坐标计算是否精准
        if (accurate_hop != layer_hop) {
            cout << "not accurate" << endl << endl;
        } else cout << endl << endl;
        // 返回精确跳数-坐标计算跳数
        return accurate_hop - layer_hop;
    }
    return 0;
}

// 重载 测试输出任意两个点之间的精确距离和坐标计算距离
void test_distance(const string &mode) {
    // 用于统计精确的点对数量
    double accuracy_num = 0;
    // 用于统计总计测试的点对数量
    double total_test_num = 0;
    // 统计各种误差的点对有多少对
    vector<int> error_hop_num;
    // 当前的误差
    int err_hop;
    // 测试输出所有的任意两个点之间的距离
    if (mode == "all") {
        for (int x = 0; x <= max_node_id[0]; x++)
            for (int y = 0; y <= max_node_id[0]; y++) {
                // 总的测试数量增加
                total_test_num++;
                // 计算误差
                err_hop = test_distance(x, y);
                // 如果没有误差
                if (err_hop == 0)
                    accuracy_num += 1;
                    // 如果存在误差
                else {
                    // 取误差的绝对值
                    err_hop = abs(err_hop);
                    // 如果统计误差对数量尺寸够用，则对应位置+1
                    if (error_hop_num.size() >= err_hop) {
                        error_hop_num[err_hop - 1] += 1;
                    } else {
                        // 如果统计误差对数量尺寸不够用，则先开辟到那么大的空间，再对应位置+1
                        error_hop_num.resize(err_hop);
                        error_hop_num[err_hop - 1] = 1;
                    }
                }
            }
    }
    // 输出总测试点对数量
    cout << "total_test_num: " << total_test_num << "\t";
    // 输出计算精确的点对数量
    cout << "accuracy_num: " << accuracy_num << "\t";
    // 输出准确率
    cout << "accuracy_percent: " << (accuracy_num / total_test_num) * 100 << "%" << endl << endl;
    // 输出各种误差所占的百分率
    for (int x = 0; x < error_hop_num.size(); x++) {
        if (error_hop_num[x] != 0) {
            cout << "error_hop " << x + 1 << ": " << (error_hop_num[x] / total_test_num) * 100 << "%" << "\t";
        }
    }
    // 输出平均运行时间
    cout << endl << endl << "accurate_duration: " << accurate_duration.count() / (max_node_id[0] * max_node_id[0])
         << "second" << "\t";
    cout << "layer_duration:" << layer_duration.count() / (max_node_id[0] * max_node_id[0]) << "second";
    cout << endl;
}

// 重载 测试输出num个随机点对之间的精确距离和坐标计算距离
void test_distance(int num) {
    // 如果不需要测试则直接退出测试函数
    if (num <= 0)
        return;
    // 先输出分隔线
    separate_line("test_distance");
    // 用于统计精确的点对数量
    double accuracy_num = 0;
    // 用于统计总计测试的点对数量
    double total_test_num = 0;
    // 统计各种误差的点对有多少对
    vector<int> error_hop_num;
    // 当前的误差
    int err_hop;
    // 用于保存随机数
    int random_node1_id;
    int random_node2_id;
    // 创建伪随机数生成器，并给定随机数种子
    mt19937 rander(random_seed);
    // 创建了均匀分布的整数随机数生成器，定义要生成的随机数的范围(包含0和max_node_id)
    uniform_int_distribution<> dis(0, max_node_id[0]);

    for (int x = 0; x < num; x++) {
        // 生成介于0和max_node_id之间的随机数(包含0和max_node_id)
        random_node1_id = dis(rander);
        random_node2_id = dis(rander);
        // 计算误差
        err_hop = test_distance(random_node1_id, random_node2_id);
        if (err_hop == 0)
            // 如果没有误差
            accuracy_num += 1;
        else {
            // 取误差的绝对值
            err_hop = abs(err_hop);
            // 如果统计误差对数量尺寸够用，则对应位置+1
            if (error_hop_num.size() >= err_hop) {
                error_hop_num[err_hop - 1] += 1;
            } else {
                // 如果统计误差对数量尺寸不够用，则先开辟到那么大的空间，再对应位置+1
                error_hop_num.resize(err_hop);
                error_hop_num[err_hop - 1] = 1;
            }
        }
        // 总的测试数量增加
        total_test_num++;
    }
    // 输出准确率
    cout << "accuracy_percent: " << (accuracy_num / total_test_num) * 100 << "%" << endl << endl;
    // 输出总测试点对数量
    cout << "total_test_num: " << total_test_num << "\t";
    // 输出计算精确的点对数量
    cout << "accuracy_num: " << accuracy_num << "\t";
    // 输出准确率
    cout << "accuracy_percent: " << (accuracy_num / total_test_num) * 100 << "%" << endl << endl;
    // 输出各种误差所占的百分率
    for (int x = 0; x < error_hop_num.size(); x++) {
        if (error_hop_num[x] != 0) {
            cout << "error_hop " << x + 1 << ": " << (error_hop_num[x] / total_test_num) * 100 << "%" << "\t";
        }
    }
    // 输出平均运行时间
    cout << endl << endl << "accurate_duration: " << accurate_duration.count() / num
         << "second" << "\t";
    cout << "layer_duration:" << layer_duration.count() / num << "second";
    cout << endl;
}

#endif //NEW_TEST_MODULE_H
