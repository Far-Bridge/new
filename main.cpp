#include "function_module/read_write.h"
#include "function_module/distinguish_layer.h"
#include "function_module/calculate_distance.h"

// 测试模块
#include "test_module/test_module.h"

using namespace std;

// 数据初始化处理函数
void HDE_initial_process();

// 分层距离估计算法 Hierarchical Distance Estimation(HDE)
void run_HDE();

// 主文件中的测试函数
void main_test();

int main() {
    // 运行HDE算法
    run_HDE();
    // 测试输出函数
    main_test();
}

void main_test() {
    // 输出所有点的各种信息
    if (graph_name == "mini" && algorithm_name == "HDE") {
        test_information("all");
        test_distance("all");
    }
    test_distance(100);
}

// 数据初始化处理函数(读图并初始化)
void HDE_initial_process() {
    // 使用默认构造函数将第0层所需要的数据结构创建出来
    all_node.emplace_back();
    adjacency_graph.emplace_back();
    // 设置读写文件名称参数
    set_filename();
    // 读取图信息
    read_graph();
    // 进行该层所有点的初始化
    all_node[max_layer_id].initial_all_light_node(max_node_id[0], adjacency_graph[0].graph);
}

void run_HDE() {
    /// 数据初始化处理函数(读图并初始化)
    HDE_initial_process();
    /// 输出基础的信息
    basic_information_print();
    /// 首次运行该图，构建并输出文件
    if (running_mode == "create") {
        // 将原图分成不同的层
        distinguish_layer();
        // 将分层信息用文件保存
        // TODO 层次划分中，如果某一个点的邻接表信息是空的，即附近2跳没有其他灯塔，能否正确读取
        write_layer_and_node();
    }
    /// 直接读入坐标文件
    if (running_mode == "read") {
        // 读取分层邻接表和点的映射关系，以及每层最大的id编号
        if (read_layer_and_node() == -1)
            // 文件打开失败，终止后续操作
            return;
    }
}