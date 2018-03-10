/*
*Author: Tiantian
*Date: 2018-3-9
*This is interview task for ZhuJian Intelligence, only for interview, no other use
*
*            0         G0
*           / \
*          /   \
*         0     1      G1
*        / \    / \
*       0   1  0   1   G2
*      / \ / \/ \ / \
*     0  10  10 1 0 1  G3
*    ..................
*   ....................
*
* I don't construct this tree explicitly in my code, because I use path length to constrain
* the search, If a path length is as long as MAX_PATH, It means this path has reach
* tree bottom, I need to traceback this node and search other pathes
*
*
*/

#include<stdio.h>
#include<iostream>
#include<set>
#include<vector>
#include<string>
#include<map>

#define DISCOUNT_GROUP_NUM                7
#define MAX_PATH                          DISCOUNT_GROUP_NUM + 1
#define RELATIVE_DISTANCE                 2
#define GOODS_NUM                         12
#define RIGHT_CHILD                       0
#define LEFT_CHILD                        1
#define ROOT                              0
#define LENGTH_FOR_ONE_DISCOUNT_GROUP     2
#define OK                                1
#define ERR                               0

//if best path ever is found, update current_best_path and remaining_goods
//for code duplicate problem, I add a macro definition here
#define UpdatePathAndRemainingGoods()\
int ret = OK;\
remaining_goods_num = current_remaining_goods.size();\
if(remaining_goods_num  < minimal_goods_num ){\
minimal_goods_num = remaining_goods_num;\
ret = update_best_path(current_path,current_best_path);\
if(!ret){\
   ret =ERR;\
}\
ret = update_minimal_remaining_goods(current_remaining_goods,minimal_remaining_goods);\
if(!ret){\
   ret =ERR;\
}\
}

using namespace std;

// num of goods
int remaining_goods_num = GOODS_NUM;
//all goods a user buys
map<string,int>  goods;
//the discount_group_list
vector<vector<string> >  discount_group_list;
//name of discounts_group. it is 1-1 mapped to discount_group_list
vector<string>  discount_group_names;

//minimal remaining goods
int minimal_goods_num = GOODS_NUM;
//path is current search
vector<int> current_path;
//current best path
vector<int> current_best_path;
// the remaining goods in current path
map<string,int>  current_remaining_goods;
//current remaining_goods in current best path
vector<string>  minimal_remaining_goods;


//prepare data
int init();
// search bi-tree, prune when the node can not meet requirements
// a node with value 1 on ith layer means choose ith discount_group_list in this path
// a node with value 0 on ith layer means do not choose ith discount_group_list in this path
// recursively  search all pathes in this bi-tree, find the best one
int search_path(int path_value);

//check if this node can be put to current_path, if discounts_group are not included in goods in path,
//it need be pruned, can't put this node in, and paths after it do not need be searched
bool check_if_need_prune(const vector<string> &discount_group, const map<string,int> &current_remaining_goods);
//if current_path is best ever, than update this path to current_best_path
int update_best_path(const vector<int> &current_path, vector<int> &current_best_path);
//if current_path is best ever, than update remaining_goods in this path to remaining_goods
int update_minimal_remaining_goods(const map<string,int> &current_remaining_goods, vector<string> &minimal_remaining_goods);
//add current discounts_group to path
int add_node_to_path(const vector<string> &discount_group, map<string,int> &current_remaining_goods, vector<string> &temp_vec);
//if pathes behind a node all be searhed and checked, it need be trace back than search other pathes do not go through it
int trace_back_node(map<string,int> &current_remaining_goods, vector<string> &temp_vec, vector<int> &path);

int init() {
    string goods_list[GOODS_NUM] = {"A1","A2","A3","A4","A5","A6","A7","A10","A15","A20","A25","A30"};
    for(int i = 0; i < GOODS_NUM; i++) {
        if(goods.find(goods_list[i]) == goods.end()) {
            goods[goods_list[i]] = 1;
            current_remaining_goods[goods_list[i]] = 1;
        } else {
            goods[goods_list[i]] =  goods[goods_list[i]] + 1;
            current_remaining_goods[goods_list[i]] = current_remaining_goods[goods_list[i]] + 1;
        }
    }

    string discount_group_name_list[] = {"G1","G2","G3","G4","G5","G6","G7"};
    for(int i = 0; i < DISCOUNT_GROUP_NUM; i ++) {
        discount_group_names.push_back(discount_group_name_list[i]);
    }

    string g1[6] = {"A1","A2","A3","A4","A5","A6"};
    vector<string> g_1(g1,g1+6);
    discount_group_list.push_back(g_1);
    string g2[3] = {"A1","A2","A50"};
    vector<string> g_2(g2,g2 + 3);
    discount_group_list.push_back(g_2);
    string g3[4] = {"A1","A2","A3","A4"};
    vector<string> g_3(g3,g3 + 4);
    discount_group_list.push_back(g_3);
    string g4[4] = {"A5","A10","A20","A30"};
    vector<string> g_4(g4,g4 + 4);
    discount_group_list.push_back(g_4);
    string g5[4] = {"A5","A6","A10","A15"};
    vector<string> g_5(g5,g5 + 4);
    discount_group_list.push_back(g_5);
    string g6[2] = {"A20","A30"};
    vector<string> g_6(g6,g6 + 2);
    discount_group_list.push_back(g_6);
    string g7[3] = {"A20","A25","A30"};
    vector<string> g_7(g7,g7 + 3);
    discount_group_list.push_back(g_7);

    return OK;
}

int search_path(int path_value) {
    int ret = OK;
    if (current_path.size() >= MAX_PATH) { // if tree bottom is reached, current_path search ends
        return ret;
    }
    bool need_prune = false;
    vector<string>  temp_vec;
    current_path.push_back(path_value);
    if (current_path.size() >= LENGTH_FOR_ONE_DISCOUNT_GROUP &&  path_value == LEFT_CHILD) {
        // take out corresponding discount_group
        vector<string>  discount_group = discount_group_list[current_path.size() - RELATIVE_DISTANCE];
        // check node status. prune or add to path
        need_prune = check_if_need_prune(discount_group,current_remaining_goods);

        if(need_prune) { //check if path and remain_goods need update
            UpdatePathAndRemainingGoods();
        } else {// no prune, add to path
            ret = add_node_to_path(discount_group,current_remaining_goods,temp_vec);
            if(!ret) {
                return ret;
            }
            // if it is last node, check if path and corresponding remaining_goods need update
            if(current_path.size() == MAX_PATH) {
                UpdatePathAndRemainingGoods();
            }
        }
    }
    if(!need_prune) { // search downward if (not pruned) && (not last node)
        ret = search_path(LEFT_CHILD);
        if(!ret) {
            return ret;
        }
        ret = search_path(RIGHT_CHILD);
        if(!ret) {
            return ret;
        }
    }
    // trace back a node
    ret = trace_back_node(current_remaining_goods,temp_vec,current_path);

    return  ret;
}

bool check_if_need_prune(const vector<string> &discount_group, const map<string,int> &current_remaining_goods) {
    if(discount_group.empty()) {
        return false;
    }
    if(current_remaining_goods.empty()) {
        return true;
    }
    for(int i = 0; i < discount_group.size(); i ++) {
        if(current_remaining_goods.find(discount_group[i]) == current_remaining_goods.end()) {
            return true;
        }
    }
    return false;
}

int update_best_path(const vector<int> &current_path, vector<int> &current_best_path) {
    current_best_path.clear();
    if(current_path.empty()) {

        std::cout<<"current_path is empty!"<<std::endl;
        return ERR;
    }
    for(int i = 0; i < current_path.size(); i ++) {
        current_best_path.push_back(current_path[i]);
    }
    return OK;
}

int update_minimal_remaining_goods(const map<string,int> &current_remaining_goods, vector<string> &minimal_remaining_goods) {
    minimal_remaining_goods.clear();
    if(current_remaining_goods.empty()) {
        return OK;
    }
    for(map<string,int>::const_iterator iter = current_remaining_goods.begin(); iter != current_remaining_goods.end(); iter ++) {
        for(int k = 0; k < iter->second; k ++) {
            minimal_remaining_goods.push_back(iter->first);
        }
    }
    return OK;
}

int add_node_to_path(const vector<string> &discount_group, map<string,int> &current_remaining_goods, vector<string> &temp_vec) {
    temp_vec.clear();
    if(discount_group.empty()) {
        return OK;
    }
    for(int i = 0; i < discount_group.size(); i ++) {
        map<string,int>::iterator iter = current_remaining_goods.find(discount_group[i]);
        if(iter ==  current_remaining_goods.end()) {
            return ERR;
        } else {
            if(current_remaining_goods[discount_group[i]] > 1) {
                current_remaining_goods[discount_group[i]] =  current_remaining_goods[discount_group[i]] - 1;
            } else {
                current_remaining_goods.erase(discount_group[i]);
            }
            temp_vec.push_back(discount_group[i]);
        }
    }
    return OK;
}

int trace_back_node(map<string,int> &current_remaining_goods, vector<string> &temp_vec, vector<int> &path) {
    if(path.empty()) {
        return ERR;
    }
    path.pop_back();
    if(temp_vec.empty()) {
        return OK;
    }

    for(int i = 0; i < temp_vec.size(); i++) {
        if(current_remaining_goods.find(temp_vec[i]) != current_remaining_goods.end()) {
            current_remaining_goods[temp_vec[i]] =  current_remaining_goods[temp_vec[i]] + 1;
        } else {
            current_remaining_goods[temp_vec[i]] = 1;
        }
    }
    temp_vec.clear();
    return OK;
}


// print discounts and the remaining goods
int main() {
    int ret = OK;
    init();
    ret = search_path(ROOT);
    if(!ret) {
        return ret;
    }
    std::cout<<"discounts groups: "<<std::endl;
    if(current_best_path.size() != 0) {
        for(int i = 0; i < current_best_path.size(); i ++) {
            if(current_best_path[i] != 0) {
                std::cout<<discount_group_names[i - 1]<<"  ";
            }
        }
    }
    std::cout << std::endl;
    std::cout<<"remaining goods: "<<std::endl;
    if(minimal_remaining_goods.size() != 0) {
        for(int k = 0; k < minimal_remaining_goods.size(); k ++) {
            std::cout<<minimal_remaining_goods[k]<<"  "<< std::endl;
        }
    }
    return 0;
}
