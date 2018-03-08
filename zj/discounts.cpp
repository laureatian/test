#include<stdio.h>
#include<iostream>
#include<set>
#include<vector>
#include<string>
using namespace std;

int remaining_goods_num = 12;
int minimal_goods = 12;
set<string>  goods;
set<string>  goods_in_path;
vector<vector<string> >  discount_group;
vector<int> path;
vector<int> returned_path;
vector<string>  remaining_goods;
int init(){
    string goods_list[12] = {"A1","A2","A3","A4","A5","A6","A7","A10","A15","A20","A25","A30"};
    for(int i = 0; i < 12; i++){
    goods.insert(goods_list[i]);
    goods_in_path.insert(goods_list[i]);
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

/*int max_discount(){

    int height = 0;
    int remaining_goods_num =  goods.size();
    vector<int> path;
    path.push_back(0);
     
    for(;;){
   
      ;

    }


}
i*/
vector<int> min_remaining(int path_value){

    std::cout<<"pathsie"<<path.size()<<std::endl; 
    if ( path.size() < 8 ){
        vector<string>  temp_vec;
        temp_vec.clear();
        path.push_back(path_value);
        if (path.size() > 1 &&  path_value == 1){
        vector<string>  discount_group_ele = discount_group[path.size() - 2];
        for(int j = 0; j < discount_group_ele.size(); j ++){
            set<string>::iterator iter1 = goods.find(discount_group_ele[j]);
            set<string>::iterator iter2 = goods_in_path.find(discount_group_ele[j]);
             //if(iter1 == goods.end()||(iter1 != goods.end() && iter2 == goods_in_path.end())){
             if( iter2 == goods_in_path.end()){

             // can't put this group in,compute this remaining_goods prune
          
             remaining_goods_num = goods_in_path.size(); 
             if(remaining_goods_num  < minimal_goods ){
                 minimal_goods = remaining_goods_num;
                 returned_path.clear(); 
                 for(int k = 0; k < path.size(); k++){
                     returned_path.push_back(path[k]);                  
                 }
                 remaining_goods.clear();
                 if(goods_in_path.size() != 0 ){
                     set<string>::iterator setiter;
                     for(setiter =  goods_in_path.begin(); setiter !=  goods_in_path.end(); setiter++){
                        remaining_goods.push_back(*setiter);
                        std::cout<<"setiter"<<*setiter<<std::endl;

                     }
                 }
             }
 
             goto label1;        
            } 

        }
      
        for(int j = 0; j < discount_group_ele.size(); j ++){
            set<string>::iterator iter1 = goods.find(discount_group_ele[j]);
            set<string>::iterator iter2 = goods_in_path.find(discount_group_ele[j]);
            //if( iter1 != goods.end()  && iter2 != goods_in_path.end()){
            if(iter2 != goods_in_path.end()){
                goods_in_path.erase(discount_group_ele[j]);   
                temp_vec.push_back(discount_group_ele[j]);
            } else {
                std::cout<<"  print  wrong" <<std::endl;
            }

        } 
        if( path.size() > 8){
           std::cout<<"path bigger than eight"<< std::endl;
           int min_value = goods.size();
     
        } 
        }  
        min_remaining(0);
        min_remaining(1);
       
label1: path.pop_back();
        if(temp_vec.size()!=0){
            for(int k = 0; k <temp_vec.size(); k++){
                goods_in_path.insert(temp_vec[k]);
            }
        }
    }    
    return  returned_path;
}


int main(){

    std::cout<<"compile end!"<<std::endl;
    init();
    min_remaining(0);
    std::cout<<"print path "<<std::endl;
    if(returned_path.size() != 0){
       for(int i = 0; i < returned_path.size(); i ++){
   
           std::cout<< returned_path[i] <<"  "<<std::endl;
       }
    }
   
    std::cout<<"print remaining_goods "<<std::endl;
    if(remaining_goods.size() != 0){
        for(int k = 0; k < remaining_goods.size(); k ++){
        std::cout<<remaining_goods[k]<<"  "<< std::endl;

    }


    }

    return 0;

}
