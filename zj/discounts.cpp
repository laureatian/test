/*
*Author: Tiantian
*Date: 2018-3-11
*This is an interview task for ZhuJian Intelligence, only for interview, no other use.
*
* Below is my thinking about this problem.
*
* The searching space is built form a bi-tree.
* Root G0 is a (imaginary)dummy discount_group that means no discounts.
* 0(all left child) also means a dummy discount_group, 1(all right child) in ith layer means corresponding ith real discount_group.
* Path is constuted of zeros and ones in a vector of int.
*
* I search the following bi-tree heuristicly for the best path, during the search, if a discount_group
* in path fails to be included in remaining_goods, search for sub-path will be prohibited.
*
*
*            0         G0   dummy discount_group
*           / \
*          /   \
*         0     1      G1   real discount_group
*        / \    / \
*       0   1  0   1   G2   real discount_group
*      / \ / \/ \ / \
*     0  10  10 1 0 1  G3   real discount_group
*    .....................
*   .......................
*
* 1, init data
* 2, search root
*        if path.length >= MAX_PATH  ###1
*            return
*        if node is 1                ###2
*            check node, add it to path  ###3  or prune this node and all sub-path  ###4
*            update best path and minimal remaining_goods if needed ###5
*        if node is not pruned (node = 0 included)       ###6
*            search left child;      ###7
*            search right child;     ###8
*        trace back this node        ###9
*        return
*
* I don't construct this tree explicitly in my code, because I use path length to constrain
* the search, If a path length is as long as MAX_PATH, It means this path has reach
* tree bottom, I need to traceback this node and search other pathes
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
#define RIGHT_CHILD                       1
#define LEFT_CHILD                        0
#define ROOT                              0
#define LENGTH_FOR_ONE_DISCOUNT_GROUP     2
#define OK                                1
#define ERR                              OK - 1

//if best path ever is found, update current_best_path and remaining_goods
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
//name of discount_group. it is 1-1 mapped to discount_group_list
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
// a node with value 1 on ith layer means choose ith discount_group in this path
// a node with value 0 on ith layer means do not choose ith discount_group in this path
// recursively  search all pathes in this bi-tree, find the best one
int search_node(int node_value);

//check if this node can be put to current_path, if discount_group are not included in goods in path,
//it need be pruned, can't put this node in, and paths after it do not need be searched
bool check_if_need_prune(const vector<string> &discount_group, const map<string,int> &current_remaining_goods);
//if current_path is best ever, than update this path to current_best_path
int update_best_path(const vector<int> &current_path, vector<int> &current_best_path);
//if current_path is best ever, than update remaining_goods in this path to remaining_goods
int update_minimal_remaining_goods(const map<string,int> &current_remaining_goods, vector<string> &minimal_remaining_goods);
//add current discount_group to path
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

int search_node(int node_value) {
    int ret = OK;
    // if tree bottom is reached, current_path search ends
    if (current_path.size() >= MAX_PATH) {   //###1
        return ret;
    }
    bool need_prune = false;
    vector<string>  temp_vec;
    current_path.push_back(node_value);
    // if current node is not a dummy discount_group, add it or prune it
    if (node_value == RIGHT_CHILD) {          // ###2
        // take out corresponding discount_group
        vector<string>  discount_group = discount_group_list[current_path.size() - RELATIVE_DISTANCE];
        // check node status. prune or add to path
        need_prune = check_if_need_prune(discount_group,current_remaining_goods);

        if(!need_prune) {// add_node_to_path
            ret = add_node_to_path(discount_group,current_remaining_goods,temp_vec); //###3
            if(!ret) {
                return ret;
            }
        }
        if(need_prune || current_path.size() == MAX_PATH) {            //###5 //prune and UpdatePathAndRemainingGoods

            UpdatePathAndRemainingGoods();
        }
    }
    if(!need_prune) { //  ###6(need_prune == false) ###4(need_prune == true)
        ret = search_node(LEFT_CHILD);                                 // ###7
        if(!ret) {
            return ret;
        }
        ret = search_node(RIGHT_CHILD);                                //###8
        if(!ret) {
            return ret;
        }
    }
    // trace back a node
    ret = trace_back_node(current_remaining_goods,temp_vec,current_path); // ###9

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
    if(current_path.empty()) {
        return ERR;
    }
    current_best_path.clear();
    for(int i = 0; i < current_path.size(); i ++) {
        current_best_path.push_back(current_path[i]);
    }
    return OK;
}

int update_minimal_remaining_goods(const map<string,int> &current_remaining_goods, vector<string> &minimal_remaining_goods) {
    if(current_remaining_goods.empty()) {
        return OK;
    }
    minimal_remaining_goods.clear();
    for(map<string,int>::const_iterator iter = current_remaining_goods.begin(); iter != current_remaining_goods.end(); iter ++) {
        for(int k = 0; k < iter->second; k ++) {
            minimal_remaining_goods.push_back(iter->first);
        }
    }
    return OK;
}

int add_node_to_path(const vector<string> &discount_group, map<string,int> &current_remaining_goods, vector<string> &temp_vec) {
    if(discount_group.empty()) {
        return OK;
    }
    temp_vec.clear();
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
    ret = search_node(ROOT);
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
    return OK;
}
