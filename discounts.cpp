#include<stdio.h>
#include<iostream>
#include<set>
#include<vector>
#include<string>
using namespace std;

int remaining_goods_num;
set<string>  goods;
vector<vector<string> >  discount_group;

int init(){
    string goods_list[12] = {"A1","A2","A3","A4","A5","A6","A7","A10","A15","A20","A25","A30"};
    for(int i = 0; i < 12; i++){
    goods.insert(goods_list[i]);
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

int max_discount(){

    int height = 0;
    int remaining_goods_num =  goods.size();
    vector<int> path;
    path.push_back(0);
   /*  
    for(;;){
   
      ;

    }*/


}

int min_remaining(){




}


int main(){

std::cout<<"compile end!"<<std::endl;
return 0;

}
