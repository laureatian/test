#ifndef DISCOUNTS_HPP__
#define DISCOUNTS_HPP__


/*
*Author: Tiantian
*Date: 2018-3-11
*This is an interview task for ZhuJian Intelligence, only for interview, no other use.
*/
#include<map>
#include<vector>

#define RIGHT_CHILD                       1
#define LEFT_CHILD                        0
#define ROOT                              0
#define LENGTH_FOR_ONE_DISCOUNT_GROUP     2
#define OK                                1
#define ERR                              OK - 1

using namespace std;

class Discounts {
public:
    Discounts();
    // search bi-tree, prune when the node can not meet requirements
    // a node with value 1 on ith layer means choose ith discount_group in this path
    // a node with value 0 on ith layer means do not choose ith discount_group in this path
    // recursively  search all pathes in this bi-tree, find the best one
    int search_discount_groups(const map<string,int> &buyer_goods);
    int set_goods(map<string,int> &buyer_goods);
//    int set_discount_group_list(vector<vector<string> > &new_discount_group_list);
private:
    map<string,int>  goods;
    vector<vector<string> >  discount_group_list;
    vector<string>  discount_group_names;

    //int minimal_goods_num;
    vector<int> best_path;
    vector<string>  minimal_remaining_goods;

    int search_node(int node_value,vector<int> &current_path, map<string,int> &current_remaining_goods);
    int print_result() const;
    int init();
    //check if this node can be put to current_path, if discount_group are not included in goods in path,
    //it need be pruned, can't put this node in, and paths after it do not need be searched
    bool check_if_need_prune(const vector<string> &discount_group, const map<string,int> &current_remaining_goods) const;
    int update_path_and_remaining_goods(const vector<int> &current_path, const map<string,int>  &current_remaining_goods);
    //if current_path is best ever, than update this path to best_path
    int update_best_path(const vector<int> &current_path);
    //if current_path is best ever, than update remaining_goods in this path to remaining_goods
    int update_minimal_remaining_goods(const map<string,int> &current_remaining_goods);
    //add current discount_group to path
    int add_node_to_path(const vector<string> &discount_group, map<string,int> &current_remaining_goods, vector<string> &temp_vec);
    //if pathes behind a node all be searhed and checked, it need be trace back than search other pathes do not go through it
    int trace_back_node(map<string,int> &current_remaining_goods, vector<string> &temp_vec, vector<int> &path);

};

#endif //DISCOUNTS_HPP__
