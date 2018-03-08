#include<stdio.h>
#include<iostream>
#include<set>
#include<vector>
#include<string>

#define DISCOUNT_GROUP_NUM 7
#define GOODS_NUM          12

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

//    std::cout<<"path size"<<path.size()<<std::endl; 
    if ( path.size() < DISCOUNT_GROUP_NUM + 1 ){
        vector<string>  temp_vec;
        temp_vec.clear();
        path.push_back(path_value);
  //      std::cout<<"path_value"<<path_value<<std::endl;
        if (path.size() > 1 &&  path_value == 1){
            vector<string>  discount_group_ele = discount_group[path.size() - 2];
            for(int j = 0; j < discount_group_ele.size(); j ++){
                set<string>::iterator iter1 = goods.find(discount_group_ele[j]);
                set<string>::iterator iter2 = goods_in_path.find(discount_group_ele[j]);
                // update best path if needed
                if( iter2 == goods_in_path.end()){
    //                std::cout<<"prune"<<std::endl; 
                    remaining_goods_num = goods_in_path.size(); 
                     if(remaining_goods_num  < minimal_goods ){
      //                   std::cout<<"update  path" << std::endl;
                         minimal_goods = remaining_goods_num;
                         returned_path.clear(); 
                         for(int k = 0; k < path.size(); k++){
                             returned_path.push_back(path[k]); 
//                             std::cout<<" "<<path[k];                 
                         }
                        // update remaining_goods
                         remaining_goods.clear();
                         if(goods_in_path.size() != 0 ){
                             set<string>::iterator setiter;
                             for(setiter =  goods_in_path.begin(); setiter !=  goods_in_path.end(); setiter++){
                                 remaining_goods.push_back(*setiter);
        //                         std::cout<<"setiter"<<*setiter<<std::endl;

                             }
                         }
                     }
 
                     goto label1;      // prune this branch if parent node fails  
                } 

            }
      
            temp_vec.clear();
            for(int j = 0; j < discount_group_ele.size(); j ++){
                set<string>::iterator iter1 = goods.find(discount_group_ele[j]);
                set<string>::iterator iter2 = goods_in_path.find(discount_group_ele[j]);
                //if( iter1 != goods.end()  && iter2 != goods_in_path.end()){
                if(iter2 != goods_in_path.end()){
                    goods_in_path.erase(discount_group_ele[j]);   
                    temp_vec.push_back(discount_group_ele[j]);
                } else {
          ;//          std::cout<<"  print  wrong" <<std::endl;
                }  

            }
            // if the last layer meets requirement, check if it is best path
            // update best path if needed 
            if(path.size() == DISCOUNT_GROUP_NUM + 1){
                 remaining_goods_num = goods_in_path.size();
                 if(remaining_goods_num < minimal_goods){
                      minimal_goods =  remaining_goods_num;
                      returned_path.clear();
             //         std::cout<<"update  path" << std::endl;
                      for(int k = 0; k < path.size(); k++){
                         returned_path.push_back(path[k]);
//                         std::cout<<" "<<path[k];                 
                      } 
                      remaining_goods.clear();
                      if(goods_in_path.size() !=0){
                          set<string>::iterator iter3 = goods_in_path.begin();
                          for(iter3; iter3 != goods_in_path.end(); iter3++){
                               remaining_goods.push_back(*iter3);
                          }
                      }
               }  

            } 
      
        }
        if(path.size() != 8){ 
            min_remaining(1);
            min_remaining(0);
        }
label1: path.pop_back(); 
        // when rollback, the deleted goods needed to put back too
        if(temp_vec.size() != 0){
            for(int k = 0; k <temp_vec.size(); k++){
                goods_in_path.insert(temp_vec[k]);
            }
            temp_vec.clear();
        }
     
    }    
    return  returned_path;
}

// print discounts and the remaining goods
int main(){
  
    init();
    min_remaining(0);
    std::cout<<"discounts groups: "<<std::endl;
    if(returned_path.size() != 0){
       for(int i = 0; i < returned_path.size(); i ++){
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
