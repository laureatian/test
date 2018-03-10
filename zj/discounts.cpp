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

#define UpdatePathAndRemainingGoods()\
int ret = OK;\
remaining_goods_num = goods_in_path.size();\
if(remaining_goods_num  < minimal_goods ){\
minimal_goods = remaining_goods_num;\
ret = update_path(path,returned_path);\
if(!ret){\
   ret =ERR;\
}\
ret = update_remaining_goods(goods_in_path,remaining_goods);\
if(!ret){\
   ret =ERR;\
}\
}


using namespace std;

// num of goods
int remaining_goods_num = GOODS_NUM;
// final remaining goods
int minimal_goods = GOODS_NUM;
map<string,int>  goods;
// the remaining goods in current path
map<string,int>  goods_in_path;
vector<vector<string> >  discount_group;
vector<int> path;
vector<int> returned_path;
vector<string>  remaining_goods;
vector<string>  discount_group_name;

int update_path(const vector<int> &current_path, vector<int> &new_path);
int update_remaining_goods(const map<string,int> &goods_in_path, vector<string> &remaining_goods);
bool check_if_need_prune(const vector<string> &discounts_group_ele, const map<string,int> &goods_in_path);
int add_node_to_path(const vector<string> &discounts_group_ele, map<string,int> &goods_in_path, vector<string> &temp_vec);
int roll_back_node(map<string,int> &goods_in_path, vector<string> &temp_vec, vector<int> &path);

// prepare the data
int init(){
    string goods_list[GOODS_NUM] = {"A1","A2","A3","A4","A5","A6","A7","A10","A15","A20","A25","A30"};
    for(int i = 0; i < GOODS_NUM; i++){
        if(goods.find(goods_list[i]) == goods.end()){
           goods[goods_list[i]] = 1;
           goods_in_path[goods_list[i]] = 1;
        } else {
          goods[goods_list[i]] =  goods[goods_list[i]] + 1;
          goods_in_path[goods_list[i]] = goods_in_path[goods_list[i]] + 1;
        }
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
int min_remaining(int path_value) {
    int ret = OK;
    if ( path.size() < MAX_PATH ){
        vector<string>  temp_vec;
        path.push_back(path_value);
        bool need_prune = false; 
        if (path.size() >= LENGTH_FOR_ONE_DISCOUNT_GROUP &&  path_value == LEFT_CHILD){
            vector<string>  discount_group_ele = discount_group[path.size() - RELATIVE_DISTANCE];
            need_prune = check_if_need_prune(discount_group_ele,goods_in_path);
             
            if(need_prune){//check if need update
                  UpdatePathAndRemainingGoods();
            } else {
               ret = add_node_to_path(discount_group_ele,goods_in_path,temp_vec);
               if(!ret){
                   return ret;
               }
               if(path.size() == MAX_PATH){
                   UpdatePathAndRemainingGoods();
               }
           }
      }
      if(!need_prune){ 
          ret = min_remaining(LEFT_CHILD);
          if(!ret){
              return ret;
          }
          
          ret = min_remaining(RIGHT_CHILD);
          if(!ret){
              return ret;
          }
      }
      ret = roll_back_node(goods_in_path,temp_vec,path);
      if(!ret){
          return ret;
      }
    }    
    return  ret;
}

int roll_back_node(map<string,int> &goods_in_path, vector<string> &temp_vec, vector<int> &path){
    if(path.empty()){
        return ERR;

    }
    path.pop_back();
    if(temp_vec.empty()){
        return OK;
    }
   
    for(int i = 0; i < temp_vec.size(); i++){
       if(goods_in_path.find(temp_vec[i]) != goods_in_path.end()){ 
           goods_in_path[temp_vec[i]] =  goods_in_path[temp_vec[i]] + 1;
       } else {
           goods_in_path[temp_vec[i]] = 1;
       }
    }
    temp_vec.clear(); 
    return OK;

}

int add_node_to_path(const vector<string> &discounts_group_ele, map<string,int> &goods_in_path, vector<string> &temp_vec){
    temp_vec.clear();
    if(discounts_group_ele.empty()){
       return OK;
    }
    for(int i = 0; i < discounts_group_ele.size(); i ++){
        map<string,int>::iterator iter = goods_in_path.find(discounts_group_ele[i]);
        if(iter ==  goods_in_path.end()){
             return ERR;
        } else {
            if(goods_in_path[discounts_group_ele[i]] > 1){
                goods_in_path[discounts_group_ele[i]] =  goods_in_path[discounts_group_ele[i]] - 1;
            } else {
                goods_in_path.erase(discounts_group_ele[i]);
            }
            temp_vec.push_back(discounts_group_ele[i]);
        } 

    }
     
    return OK;

}

bool check_if_need_prune(const vector<string> &discounts_group_ele, const map<string,int> &goods_in_path){
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


int update_path(const vector<int> &current_path, vector<int> &new_path){
    new_path.clear();
    if(current_path.empty()){
   
        std::cout<<"current_path is empty!"<<std::endl;
        return ERR;
    }
   for(int i = 0; i < current_path.size(); i ++){
      new_path.push_back(current_path[i]);
   }

   return OK;

}

int update_remaining_goods(const map<string,int> &goods_in_path, vector<string> &remain_goods){
    remain_goods.clear();  
    if(goods_in_path.empty()){
        return OK;
    }
    for(map<string,int>::const_iterator iter = goods_in_path.begin(); iter != goods_in_path.end(); iter ++){
       for(int k = 0; k < iter->second; k ++){
           remain_goods.push_back(iter->first);
       }
    }
    return OK;
}

// print discounts and the remaining goods
int main(){
    int ret = OK;  
    init();
    ret = min_remaining(ROOT);
    if(!ret){
       return ret;
    }
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
