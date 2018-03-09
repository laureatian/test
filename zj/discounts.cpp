/*
*Author: Tiantian
*Date: 2018-3-9
*
*
*           0  G0
*           / \
*          /   \
*         0     1   G1
*        / \    / \
*       0   1  0   1  G2
*      / \ / \/ \ / \      
*     0  10  10 1 0 1  G3
*    ..................
*   ....................
*
*
*
*
*/

#include<stdio.h>
#include<iostream>
#include<set>
#include<vector>
#include<string>

#define DISCOUNT_GROUP_NUM     7
#define MAX_PATH              DISCOUNT_GROUP_NUM + 1
#define RELATIVE_DISTANCE      2
#define GOODS_NUM              12
#define RIGHT_CHILD            0 
#define LEFT_CHILD             1
#define ROOT                   0
#define LENGTH_FOR_ONE_DISCOUNT_GROUP 2 
#define OK                     1
#define ERR                    0

#define UpdatePathAndGoods()\
remaining_goods_num = goods_in_path.size();\
if(remaining_goods_num  < minimal_goods ){\
minimal_goods = remaining_goods_num;\
update_path(path,returned_path);\
update_remaining_goods(goods_in_path,remaining_goods);\
}


using namespace std;

// num of goods
int remaining_goods_num = GOODS_NUM;
// final remaining goods
int minimal_goods = GOODS_NUM;
set<string>  goods;
// the remaining goods in current path
set<string>  goods_in_path;
vector<vector<string> >  discount_group;
vector<int> path;
vector<int> returned_path;
vector<string>  remaining_goods;
vector<string>  discount_group_name;

bool update_path(const vector<int> &current_path, vector<int> &new_path);
bool update_remaining_goods(const set<string> &goods_in_path, vector<string> &remaining_goods);
bool check_if_need_prune(const vector<string> &discounts_group_ele, const set<string> &goods_in_path);
bool add_node_to_path(const vector<string> &discounts_group_ele, set<string> &goods_in_path, vector<string> &temp_vec);
bool roll_back_node(set<string> &goods_in_path, vector<string> &temp_vec, vector<int> &path);

// prepare the data
int init(){
    string goods_list[GOODS_NUM] = {"A1","A2","A3","A4","A5","A6","A7","A10","A15","A20","A25","A30"};
    for(int i = 0; i < GOODS_NUM; i++){
        goods.insert(goods_list[i]);
        goods_in_path.insert(goods_list[i]);
    }    
    
    string discount_group_name_list[] = {"G1","G2","G3","G4","G5","G6","G7"};
    for(int i = 0; i < DISCOUNT_GROUP_NUM; i ++){
        discount_group_name.push_back(discount_group_name_list[i]);

    }
 
    string g1[6] = {"A1","A2","A3","A4","A5","A6"};
    vector<string> g_1(g1,g1+6);
    discount_group.push_back(g_1);
    string g2[3] = {"A1","A2","A50"};
    vector<string> g_2(g2,g2 + 3);
    discount_group.push_back(g_2);
    string g3[4] = {"A1","A2","A3","A4"};
    vector<string> g_3(g3,g3 + 4);
    discount_group.push_back(g_3);
    string g4[4] = {"A5","A10","A20","A30"};
    vector<string> g_4(g4,g4 + 4);
    discount_group.push_back(g_4);
    string g5[4] = {"A5","A6","A10","A15"};
    vector<string> g_5(g5,g5 + 4);
    discount_group.push_back(g_5);
    string g6[2] = {"A20","A30"};
    vector<string> g_6(g6,g6 + 2);
    discount_group.push_back(g_6);
    string g7[3] = {"A20","A25","A30"};
    vector<string> g_7(g7,g7 + 3);
    discount_group.push_back(g_7);
    return 0;
}

// search bi-tree, prune when the node can not meet requirements
// a node with value 1 on ith layer means choose ith discount_group in this path 
// a node with value 0 on ith layer means do not choose ith discount_group in this path
// recursively  search all pathes in this bi-tree, find the best one
vector<int> min_remaining(int path_value){

    if ( path.size() < MAX_PATH ){
        vector<string>  temp_vec;
        path.push_back(path_value);
        bool need_prune = false; 
        if (path.size() >= LENGTH_FOR_ONE_DISCOUNT_GROUP &&  path_value == LEFT_CHILD){
            vector<string>  discount_group_ele = discount_group[path.size() - RELATIVE_DISTANCE];
            need_prune = check_if_need_prune(discount_group_ele,goods_in_path);
             
            if(need_prune){//check if need update
                  UpdatePathAndGoods();
            } else {
                add_node_to_path(discount_group_ele,goods_in_path,temp_vec);
               if(path.size() == DISCOUNT_GROUP_NUM + 1){
                   UpdatePathAndGoods();
               }
           }
      }
      if(!need_prune){ 
          min_remaining(LEFT_CHILD);
          min_remaining(RIGHT_CHILD);
      }
      roll_back_node(goods_in_path,temp_vec,path);
    }    
    return  returned_path;
}

bool roll_back_node(set<string> &goods_in_path, vector<string> &temp_vec, vector<int> &path){
    if(path.empty()){
        return false;

    }
    path.pop_back();
    if(temp_vec.empty()){
    return true;
    }
   
    for(int i = 0; i < temp_vec.size(); i++){
       goods_in_path.insert(temp_vec[i]);
    }
    temp_vec.clear(); 
    return true;

}

bool add_node_to_path(const vector<string> &discounts_group_ele, set<string> &goods_in_path, vector<string> &temp_vec){
    temp_vec.clear();
    if(discounts_group_ele.empty()){
       return true;
    }
    for(int i = 0; i < discounts_group_ele.size(); i ++){
        set<string>::iterator iter = goods_in_path.find(discounts_group_ele[i]);
        if(iter ==  goods_in_path.end()){
            std::cout<<" wrong" << std::endl;
        } else {
            goods_in_path.erase(discounts_group_ele[i]);
            temp_vec.push_back(discounts_group_ele[i]);
        } 

    }
     
    return true;

}

bool check_if_need_prune(const vector<string> &discounts_group_ele, const set<string> &goods_in_path){
    if(discounts_group_ele.empty()){
        return false;
    }
   
    if(goods_in_path.empty()){
        return true;
    }
    for(int i = 0; i < discounts_group_ele.size(); i ++){
        if(goods_in_path.find(discounts_group_ele[i]) == goods_in_path.end()){
           return true;
        }
    } 
    return false;
}


bool update_path(const vector<int> &current_path, vector<int> &new_path){
    new_path.clear();
    if(current_path.empty()){
   
        std::cout<<"current_path is empty!"<<std::endl;
        return true;
    }
   for(int i = 0; i < current_path.size(); i ++){
      new_path.push_back(current_path[i]);
   }

   return true;

}

bool update_remaining_goods(const set<string> &goods_in_path, vector<string> &remain_goods){
    remain_goods.clear();  
    if(goods_in_path.empty()){
        return true;
    }
    for(set<string>::iterator iter = goods_in_path.begin(); iter != goods_in_path.end(); iter ++){
       remain_goods.push_back(*iter);
    }
    return true;
}

// print discounts and the remaining goods
int main(){
  
    init();
    min_remaining(ROOT);
    std::cout<<"discounts groups: "<<std::endl;
    if(returned_path.size() != 0){
       for(int i = 0; i < returned_path.size(); i ++){
          //std::cout<<"path value："<< returned_path[i];
           if(returned_path[i] != 0){
               std::cout<<discount_group_name[i - 1]<<"  ";
           }   
       }
    }
    std::cout << std::endl;
    std::cout<<"remaining goods: "<<std::endl;
    if(remaining_goods.size() != 0){
        for(int k = 0; k < remaining_goods.size(); k ++){
            std::cout<<remaining_goods[k]<<"  "<< std::endl;

    }


    }

    return 0;

}
