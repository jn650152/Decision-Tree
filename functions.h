#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "node.h"
#include "child_node.h"
#include <iostream>
#include <fstream>
#include<map>
#include<math.h>
using namespace std;
bool check_choice_valid(vector<int> possible_choice_num,int target_choice_num);
node DT(vector<vector<string>> database, int target_num, string target, vector<int> attribute_list,vector<string> attributes, vector<string> target_value);
bool check_label(vector<vector<string>> database,int target_num, string &name);
string get_majority_name(vector<vector<string>> database, int target_num);
int get_attribute_test_condition(vector<vector<string>> database,int target_num, vector<int> attribute_list,vector<string> target_value);
//float count_info_gain(vector<vector<string>> database, int attribute_num, int target_num,vector<string> target_value);
float count_before_entropy(vector<vector<string>> database, int target_num);
float count_after_entropy(vector<vector<string>> database, int attribute_num, int target_num,vector<string> target_value);
void split(vector<vector<string>> database,map<string, vector<int>> &split_databases, int attribute_num,vector<child_node> &children);
void print_split_databases(vector<vector<string>> database,map<string, vector<int>> split_databases,node Node);
vector<vector<string>> form_database_by_index(vector<vector<string>> database, vector<int> index_vector);
void print_database(vector<vector<string>> new_database);
void draw_tree(ofstream & output, node root, string target, vector<string>  target_value,int &tap_num);
int get_delete_position(int attribute_num,vector<int> attribute_list);
void print_attribute_list(vector<int> attribute_list, vector<string> attributes);

#endif
