#ifndef DISCOUNTS_HPP__
#define DISCOUNTS_HPP__


/*
*Author: Tiantian
*Date: 2018-3-11
*This is an interview task for ZhuJian Intelligence, only for interview, no other use.
*/
#include<map>
#include<vector>

#define ROOT                              0
#define RIGHT_CHILD                       1
#define LEFT_CHILD                        0
#define LENGTH_FOR_ONE_DISCOUNT_GROUP     2
#define OK                                1
#define ERR                              OK - 1
#define RELATIVE_DISTANCE                 2

using namespace std;

class Discounts {
public:
    /**
     * keys of @discount_group_map are assigned to discount_group_names
     * values of @discount_group_map are assigned to discount_group_list
     */
    Discounts(map<string,vector<string> > &discount_group_map);
    /**
     * @buyer_goods is the goods to be computed, @best_discount_group will store the result of selected discount_group names
     * @minimal_remaining_goods will store the remaining_goods that are not discounted
     */
    int search_discount_groups(const map<string,int> &buyer_goods,vector<string> &best_discount_group,\
                               vector<string> &minimal_remaining_goods);
    /**
     * set new values for private member
     */
    int set_discount_groups(map<string,vector<string> > &new_discount_group_map);
    ~Discounts();
private:
    /**
     * store discount_group_names like  G1 G2 G3 in a vector
     */
    vector<string>  discount_group_names;
    /**
     * vector of discounts groups. like{[A1,A2,A3], [A4,A5,A6]...}
     * it is one-one mapped to discount_group_names
     */
    vector<vector<string> >  discount_group_list;


    /**
     * search bi-tree for @best_path, prune when the node can not meet requirements
     */
    int search_node(int node_value,vector<int> &current_path, map<string,int> &current_remaining_goods,\
                    vector<int> &best_path, vector<string>  &minimal_remaining_goods);
    /**
     * check if this node can be put to current_path, if discount_group are not included in goods in current_path,
     * it need be pruned, can't put this node in, and paths after it don't need be searched
     */
    bool check_if_need_prune(const vector<string> &discount_group, const map<string,int> &current_remaining_goods) const;
    /**
     * update best_path and minimal_remaining_goods if this path is best ever
     */
    int update_path_and_remaining_goods(const vector<int> &current_path, const map<string,int>  &current_remaining_goods,\
                                        vector<int> &best_path, vector<string> &minimal_remaining_goods);
    /**
     *if current_path is best ever, than update this path to best_path
     */
    int update_best_path(const vector<int> &current_path, vector<int> &best_path);
    /**
     *if current_path is best ever, than update remaining_goods in this path to remaining_goods
     */
    int update_minimal_remaining_goods(const map<string,int> &current_remaining_goods, vector<string> &minimal_remaining_goods);
    /**
     *add current discount_group to path
     */
    int add_node_to_path(const vector<string> &discount_group, map<string,int> &current_remaining_goods, vector<string> &temp_vec);
    /**
     *if pathes behind a node all be searhed and checked, it need be trace back than search other pathes do not go through it
     */
    int trace_back_node(map<string,int> &current_remaining_goods, vector<string> &temp_vec, vector<int> &path);

};

#endif //DISCOUNTS_HPP__
