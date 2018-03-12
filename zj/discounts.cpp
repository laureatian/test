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
#include"discounts.hpp"

#define PrintResults()\
    if(!goods.empty()){\
    if(best_discount_group.size() != 0) {\
        std::cout<<"best_discount_groups: ";\
        for(int i = 0; i < best_discount_group.size(); i ++) {\
            std::cout<<best_discount_group[i]<<"  ";\
        }\
    } else {\
        std::cout<<"best_discount_groups is null."<<std::endl;\
    }\
    std::cout << std::endl;\
    std::cout<<"remaining goods: ";\
    if(minimal_remaining_goods.size() != 0) {\
        for(int k = 0; k < minimal_remaining_goods.size(); k ++) {\
            std::cout<<minimal_remaining_goods[k]<<"  ";\
        }\
        std::cout<<std::endl;\
    } else {\
        std::cout<<"no goods left, all discounted." <<std::endl;\
    }\
}

using namespace std;

Discounts::Discounts(map<string, vector<string> > &discount_group_map) {
    if(discount_group_map.empty()) {
        discount_group_list.clear();
    } else {
        for(map<string, vector<string> >::iterator iter =  discount_group_map.begin(); \
                iter != discount_group_map.end(); iter ++) {
            discount_group_names.push_back(iter->first);
            discount_group_list.push_back(iter->second);
        }
    }
}

int Discounts::search_discount_groups(const map<string,int> &buyer_goods,vector<string>  &best_discount_group,\
                                      vector<string> &minimal_remaining_goods) {
    int ret = OK;
    vector<int> path;
    vector<int> best_path;
    map<string,int> current_remaining_goods;
    if(!buyer_goods.empty()) {
        current_remaining_goods.insert(buyer_goods.begin(),buyer_goods.end());
    } else {
        std::cout<<"nothing buys!"<<std::endl;
        return ERR;
    }
    ret =  search_node(-1,path,current_remaining_goods,best_path,minimal_remaining_goods);
    if (!ret) {
        return ret;
    }
    if(best_path.size() >=  RELATIVE_DISTANCE_PATH_DISCOUNT_NAME) {
        for(int i = 0; i < best_path.size(); i ++) {
            if(best_path[i] != -1) {
                best_discount_group.push_back(discount_group_names[best_path[i]]);

            }

        }
    }
    return ret;
}

int Discounts::set_discount_groups(map<string,vector<string> > &new_discount_group_map) {
    if(new_discount_group_map.empty()) {
        discount_group_list.clear();
        return OK;
    } else {
        for(map<string, vector<string> >::iterator iter =  new_discount_group_map.begin(); \
                iter != new_discount_group_map.end(); iter ++) {
            discount_group_names.push_back(iter->first);
            discount_group_list.push_back(iter->second);
        }
    }
    return OK;
}


Discounts::~Discounts() {

}

int Discounts::search_node(int node_value, vector<int> &current_path, map<string,int> &current_remaining_goods,\
                           vector<int> &best_path, vector<string> &minimal_remaining_goods) {
    int ret = OK;
    // if tree bottom is reached, current_path search ends
    /*   if (current_path.size() >= discount_group_list.size() + 1) {   //###1
           return ret;
       }*/
    bool need_prune = false;
    vector<string>  temp_vec;
    current_path.push_back(node_value);
    // if current node is not a dummy discount_group, add it or prune it
    if (node_value != -1) {          // ###2
        // take out corresponding discount_group
        vector<string>&  discount_group = discount_group_list[node_value];
        // check node status. prune or add to path
        need_prune = check_if_need_prune(discount_group,current_remaining_goods);
        if(need_prune) {
            current_path.pop_back();
            current_path.push_back(-1);

        }

        if(!need_prune) {// add_node_to_path
            ret = add_node_to_path(discount_group,current_remaining_goods,temp_vec); //###3
            if(!ret) {
                return ret;
            }
        }
        //###5 //prune and update_path_and_remaining_goods
        if(need_prune || current_path.size() == discount_group_list.size() + 1) {

            update_path_and_remaining_goods(current_path,current_remaining_goods,best_path,minimal_remaining_goods);
        }
    }
    if(!need_prune) { //  ###6(need_prune == false) ###4(need_prune == true)
        for(int i = 0; i < discount_group_list.size(); i ++) {
            ret =  search_node(i,current_path,current_remaining_goods,best_path,minimal_remaining_goods);
            if(!ret ) {
                return ret;
            }
        }


        /*    //###7
            ret = search_node(RIGHT_CHILD,current_path,current_remaining_goods,best_path,minimal_remaining_goods);
            if(!ret) {
                return ret;
            }   //###8
            ret = search_node(LEFT_CHILD,current_path,current_remaining_goods,best_path,minimal_remaining_goods);
            if(!ret) {
                return ret;
            }*/
    }
    // trace back a node
    ret = trace_back_node(current_remaining_goods,temp_vec,current_path); // ###9

    return  ret;
}

bool Discounts::check_if_need_prune(const vector<string> &discount_group, const map<string,int> &current_remaining_goods) const {
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

int Discounts::update_path_and_remaining_goods(const vector<int> &current_path, const map<string,int> &current_remaining_goods,\
        vector<int> &best_path, vector<string> &minimal_remaining_goods) {
    int ret = OK;
    int remaining_goods_num = 0;
    if(!current_remaining_goods.empty()) {
        for(map<string, int>::const_iterator iter = current_remaining_goods.begin(); iter != current_remaining_goods.end(); iter ++) {
            remaining_goods_num += iter->second;
        }
    }

    if(remaining_goods_num  < minimal_remaining_goods.size() ) {
        //  std::cout<<"update path and  remaining_goods: "<<remaining_goods_num<<std::endl;
        ret = update_best_path(current_path,best_path);
        if(!ret) {
            ret =ERR;
        }
        ret = update_minimal_remaining_goods(current_remaining_goods,minimal_remaining_goods);
        if(!ret) {
            ret =ERR;
        }
    }
    return ret;
}

int Discounts::update_best_path(const vector<int> &current_path, vector<int> &best_path) {
    if(current_path.empty()) {
        return ERR;
    }
    best_path.clear();
    // std::cout<<"update path"<<std::endl;
    for(int i = 0; i < current_path.size(); i ++) {
        //   std::cout<<" "<<current_path[i];
        best_path.push_back(current_path[i]);
    }
    //std::cout<<std::endl;
    return OK;
}

int Discounts::update_minimal_remaining_goods(const map<string,int> &current_remaining_goods, vector<string> &minimal_remaining_goods) {
    minimal_remaining_goods.clear();
    if(current_remaining_goods.empty()) {
        return OK;
    }
    //std::cout<<"update remaining_goods"<<std::endl;
    for(map<string,int>::const_iterator iter = current_remaining_goods.begin(); iter != current_remaining_goods.end(); iter ++) {
        for(int k = 0; k < iter->second; k ++) {
            minimal_remaining_goods.push_back(iter->first);
            //std::cout<<" "<<iter->first;
        }
        //std::cout<<std::endl;
    }
    return OK;
}

int Discounts::add_node_to_path(const vector<string> &discount_group, map<string,int> &current_remaining_goods, vector<string> &temp_vec) {
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

int Discounts::trace_back_node(map<string,int> &current_remaining_goods, vector<string> &temp_vec, vector<int> &path) {
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


//test 1, same test data with the problem
int test_1(Discounts &dis) {
    std::cout<<std::endl;
    std::cout<<"test 1, same test data with the problem."<<std::endl;
    int ret = OK;
    map<string,int> goods;
    vector<string>  minimal_remaining_goods;
    vector<string>  best_discount_group;

    string goods_list[12] = {"A1","A2","A3","A4","A5","A6","A7","A10","A15","A20","A25","A30"};
    for(int i = 0; i < 12; i++) {
        if(goods.find(goods_list[i]) == goods.end()) {
            goods[goods_list[i]] = 1;
        } else {
            goods[goods_list[i]] =  goods[goods_list[i]] + 1;
        }
        minimal_remaining_goods.push_back(goods_list[i]);
    }
    ret = dis.search_discount_groups(goods,best_discount_group,minimal_remaining_goods);
    if(!ret) {
        return ret;
    }
    PrintResults();

    return ret;
}

//test 2, buyed goods is exactly the same with one discount_group
int test_2(Discounts &dis) {
    std::cout<<std::endl;
    std::cout<<"test 2, buyed goods exactly the same with one discount_group, for example(G5)"<<std::endl;
    int ret = OK;
    map<string,int> goods;
    vector<string>  minimal_remaining_goods;
    vector<string>  best_discount_group;
    goods.clear();
    string goods_list[4] = {"A5","A6","A10","A15"};
    for(int i = 0; i < 4; i++) {
        if(goods.find(goods_list[i]) == goods.end()) {
            goods[goods_list[i]] = 1;
        } else {
            goods[goods_list[i]] =  goods[goods_list[i]] + 1;
        }
        minimal_remaining_goods.push_back(goods_list[i]);
    }
    ret = dis.search_discount_groups(goods,best_discount_group,minimal_remaining_goods);
    if(!ret) {
        return ret;
    }
    PrintResults();

    return ret;
}

//test 3, two of the buyed goods are same
int test_3(Discounts &dis) {
    std::cout<<std::endl;
    std::cout<<"test 3, two of the buyed goods are same."<<std::endl;
    int ret = OK;
    map<string,int> goods;
    vector<string>  minimal_remaining_goods;
    vector<string>  best_discount_group;
    goods.clear();
    string goods_list[5] = {"A20","A30","A20","A25","A30"};
    for(int i = 0; i < 5; i++) {
        if(goods.find(goods_list[i]) == goods.end()) {
            goods[goods_list[i]] = 1;
        } else {
            goods[goods_list[i]] =  goods[goods_list[i]] + 1;
        }
        minimal_remaining_goods.push_back(goods_list[i]);
    }
    ret = dis.search_discount_groups(goods,best_discount_group,minimal_remaining_goods);
    if(!ret) {
        return ret;
    }
    PrintResults();

    return ret;
}

//test 4, buys nothing
int test_4(Discounts &dis) {
    std::cout<<std::endl;
    std::cout<<"test 4, buys nothing."<<std::endl;
    int ret = OK;
    map<string,int> goods;
    vector<string>  minimal_remaining_goods;
    vector<string>  best_discount_group;
    goods.clear();
    ret = dis.search_discount_groups(goods,best_discount_group,minimal_remaining_goods);
    if(!ret) {
        return ret;
    }
    PrintResults();

    return ret;
}

//test 5, buys but can't discounts
int test_5(Discounts &dis) {
    std::cout<<std::endl;
    std::cout<<"test 5, buys but can't discounts."<<std::endl;
    int ret = OK;
    map<string,int> goods;
    vector<string>  minimal_remaining_goods;
    vector<string>  best_discount_group;
    goods.clear();
    string goods_list[5] = {"A1","A3","A5","A7","A9"};
    for(int i = 0; i < 5; i++) {
        if(goods.find(goods_list[i]) == goods.end()) {
            goods[goods_list[i]] = 1;
        } else {
            goods[goods_list[i]] =  goods[goods_list[i]] + 1;
        }
        minimal_remaining_goods.push_back(goods_list[i]);
    }
    ret = dis.search_discount_groups(goods,best_discount_group,minimal_remaining_goods);
    if(!ret) {
        return ret;
    }
    PrintResults();

    return ret;
}

// test_6, add a discount_group
int test_6(Discounts &dis) {
    std::cout<<std::endl;
    std::cout<<"test 6, add a discount_group."<<std::endl;
    int ret = OK;
    map<string,int> goods;
    vector<string>  minimal_remaining_goods;
    vector<string>  best_discount_group;
    goods.clear();
    string goods_list[13] = {"A33","A1","A2","A3","A4","A5","A6","A7","A10","A15","A20","A25","A30"};
    for(int i = 0; i < 13; i++) {
        if(goods.find(goods_list[i]) == goods.end()) {
            goods[goods_list[i]] = 1;
        } else {
            goods[goods_list[i]] =  goods[goods_list[i]] + 1;
        }
        minimal_remaining_goods.push_back(goods_list[i]);
    }
    ret = dis.search_discount_groups(goods,best_discount_group,minimal_remaining_goods);
    if(!ret) {
        return ret;
    }
    PrintResults();

    return ret;
}

// test_7, a discount_group appears 3 times/ twice
int test_7(Discounts &dis) {
    std::cout<<std::endl;
    std::cout<<"test 7, a discount_group appears twice."<<std::endl;
    int ret = OK;
    map<string,int> goods;
    vector<string>  minimal_remaining_goods;
    vector<string>  best_discount_group;
    goods.clear();
    //string goods_list[16] = {"A1","A2","A3","A4","A5","A6","A7","A10","A15","A20","A25","A30","A1","A2","A3","A4"};
    string goods_list[20] = {"A1","A2","A3","A4","A5","A6","A7","A10","A15","A20","A25","A30","A1","A2","A3","A4","A1","A2","A3","A4"};
    for(int i = 0; i < 20; i++) {
        if(goods.find(goods_list[i]) == goods.end()) {
            goods[goods_list[i]] = 1;
        } else {
            goods[goods_list[i]] =  goods[goods_list[i]] + 1;
        }
        minimal_remaining_goods.push_back(goods_list[i]);
    }
    ret = dis.search_discount_groups(goods,best_discount_group,minimal_remaining_goods);
    if(!ret) {
        return ret;
    }
    PrintResults();

    return ret;
}

int main() {
    int ret = OK;
    map<string,int> goods;
    vector<string>  minimal_remaining_goods;
    vector<string>  best_discount_group;

    map<string,vector<string> > discount_group_map;
    string discount_group_name_list[] = {"G1","G2","G3","G4","G5","G6","G7","G8"};

    string g1[6] = {"A1","A2","A3","A4","A5","A6"};
    vector<string> g_1(g1,g1+6);
    discount_group_map[discount_group_name_list[0]] = g_1;
    string g2[3] = {"A1","A2","A50"};
    vector<string> g_2(g2,g2 + 3);
    discount_group_map[discount_group_name_list[1]] = g_2;
    string g3[4] = {"A1","A2","A3","A4"};
    vector<string> g_3(g3,g3 + 4);
    discount_group_map[discount_group_name_list[2]] = g_3;
    string g4[4] = {"A5","A10","A20","A30"};
    vector<string> g_4(g4,g4 + 4);
    discount_group_map[discount_group_name_list[3]] = g_4;
    string g5[4] = {"A5","A6","A10","A15"};
    vector<string> g_5(g5,g5 + 4);
    discount_group_map[discount_group_name_list[4]] = g_5;
    string g6[2] = {"A20","A30"};
    vector<string> g_6(g6,g6 + 2);
    discount_group_map[discount_group_name_list[5]] = g_6;
    string g7[3] = {"A20","A25","A30"};
    vector<string> g_7(g7,g7 + 3);
    discount_group_map[discount_group_name_list[6]] = g_7;

    Discounts dis =  Discounts(discount_group_map);

    test_1(dis);
    test_2(dis);
    test_3(dis);
    test_4(dis);
    test_5(dis);

    string g8[2] = {"A7","A33"};
    vector<string> g_8(g8,g8 + 2);

    discount_group_map[discount_group_name_list[7]] = g_8;
    dis.set_discount_groups(discount_group_map);

    test_6(dis);
    test_7(dis);
    return OK;
}


