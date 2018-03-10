#ifndef DISCOUNTS_HPP_
#define DISCOUNTS_HPP__





class DiscountsBase{

public: 
int goods_num;
int discount_group_num;
map<string,int>  goods;
vector<vector<string> >  discount_group_list;
vector<string>  discount_group_names;

private:
int minimal_goods_num;
vector<int> current_path;
vector<int> current_best_path;
map<string,int>  current_remaining_goods;
vector<string>  minimal_remaining_goods;



}
 

Class Discounts 
