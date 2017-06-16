#include "functions.h"
#include "statistics.h"
//check whether choice is valid
bool check_choice_valid(vector<int> possible_choice_num,int target_choice_num){
	for(auto i = possible_choice_num.begin(); i!=possible_choice_num.end();i++){
		if((*i)==target_choice_num){
			return true;
		}
	}
	return false;

}
//DT function
node DT(vector<vector<string>> database, int target_num,string target, vector<int> attribute_list,vector<string> attributes, vector<string> target_value){
	node *Node;
        Node = new node;
	string name;
	if(check_label(database,target_num,name)){//all records have the same label
		Node->name = name;
	}else if(attribute_list.size() == 0){//the attribute list is empty
		Node->name = get_majority_name(database,target_num);
	}else{//needs to determine the attribute test condition

		//get attribute which gets best split
		//print_attribute_list(attribute_list,attributes);
		int attribute_num = get_attribute_test_condition(database,target_num,attribute_list, target_value);
                Node->name = attributes[attribute_num];

		//split the database based on the determined attribute and name new branch.
		map<string, vector<int>> split_databases;//first represents the value of the attribute. second represents the index of records in original database with the value for the attribute
		split(database,split_databases,attribute_num,Node->children);
		//print_split_databases(database, split_databases,*Node);
		

		//remove the used attribute from attribute list and attributes
		int position = get_delete_position(attribute_num,attribute_list);
		attribute_list.erase(attribute_list.begin()+ position);

		//test stopping point for each splitted database
		for(auto p: split_databases){//for each splitted database
			//find the child_node
			auto f =Node->children.begin();
			for(auto q =Node->children.begin(); q != Node->children.end(); q++){
				if((*q).branch == p.first){
					f = q;
					break;		
				}
			}	
			//form new database using index
			vector<vector<string>> new_database = form_database_by_index(database, p.second);
			//print_database(new_database);
			node *child_Node;
			child_Node = new node;
		
			string name;	
			if(check_label(new_database,target_num,name)){//all records in the current splitted databases belong to the same label
				child_Node->name = name;
				(*f).child = child_Node;
			}else if(attribute_list.size()==0){//attribute list is empty
				child_Node->name = get_majority_name(new_database, target_num);
				(*f).child = child_Node;

			}else if(new_database.size()==0){//no sample
				child_Node->name = "no sample";
				(*f).child = child_Node;
			}else{//recursively call DT
				(*child_Node) = DT(new_database, target_num,target,attribute_list, attributes, target_value);
				(*f).child = child_Node;
			}
		}
	}
	
	return *Node;
}
//for testing attribute_list
void print_attribute_list(vector<int> attribute_list, vector<string> attributes){
	cout << "print attributes_list:"<<endl;
	for(auto i = attribute_list.begin(); i!= attribute_list.end(); i++){
		cout <<attributes[(*i)]<<endl;
	}
}

//get the position of the element attribute_num in a vector
int get_delete_position(int attribute_num,vector<int> attribute_list){
	int position = -1;
	for(auto i = attribute_list.begin(); i!=attribute_list.end();i++){
		position++;
		if((*i)==attribute_num){
			return position;
		}
	}
	cout <<"failed to find the position of the attribute to be deleted"<<endl;
	return -1;
}
//form database using index
vector<vector<string>> form_database_by_index(vector<vector<string>> database, vector<int> index_vector){
	vector<vector<string>> new_database;
	for(auto i = index_vector.begin(); i != index_vector.end(); i++){
		auto j = database.begin()+(*i);
		vector<string> record;
		for(auto m=(*j).begin(); m != (*j).end(); m++){
			record.push_back(*m);
		}
		new_database.push_back(record);
	}
	return new_database;
}
//split database based on the determined attribute and store the newly splitted database in the form of index of the original database and name branch.
void split(vector<vector<string>> database,map<string, vector<int>> &split_databases, int attribute_num, vector<child_node> &children){
	int index= 0;
	for(auto i = database.begin(); i != database.end(); i++){//scan original database
		if(split_databases.find((*i)[attribute_num])==split_databases.end()){//the value of the attribute has not been found yet
			child_node child_Node_ref;
			child_Node_ref.branch = (*i)[attribute_num];//name the branch
			children.push_back(child_Node_ref);
		}
		split_databases[(*i)[attribute_num]].push_back(index);//save records to corresponding splitted database
		index++;
	}
}
//for testing the splitted databases
void print_split_databases(vector<vector<string>> database,map<string, vector<int>> split_databases, node Node){
	for(auto p: split_databases){
		cout << "database "<< p.first<<":"<< endl;
		for(auto i = p.second.begin(); i != p.second.end(); i++){
			for(int j =0; j< 5;j++){
				cout << database[*i][j]<< " ";
			}
			cout << endl;
		}
	}
}
//for testing database
void print_database(vector<vector<string>> new_database){
	cout << "print database:"<<endl;
	for(auto i = new_database.begin(); i != new_database.end(); i++){
		for(auto j = (*i).begin(); j != (*i).end(); j++){
			cout << (*j)<<" ";
		}
		cout << endl;
	}
}
//using database, check whether all records in the database belongs to one catergory or not
bool check_label(vector<vector<string>> database,int target_num, string &name){
	for(auto i = database.begin(); i != database.end();i++){
		if(i == database.begin()){
			name = (*i)[target_num];
		}
		string temp = (*i)[target_num];
		if(name!=temp){
			return false;
		}
	}
	return true;
}
//get the name of the majority
string get_majority_name(vector<vector<string>> database, int target_num){
	//records the occurence of different attribute's values
	map<string,int> values;
	for(auto i = database.begin(); i != database.end();i++){
		if(values.find((*i)[target_num])==values.end()){//the value is not found
			values[(*i)[target_num]] = 1;
		}else{
			values[(*i)[target_num]]++;
		}
	}
	//get the value which occurs most frequently
	int count =0 ;
	string majority_name;
	for(auto p:values){
		if(p.second> count){
			count = p.second;
			majority_name = p.first;
		}
	}
	return majority_name;
}
//get attribute which gets best split
int get_attribute_test_condition(vector<vector<string>> database,int target_num, vector<int> attribute_list, vector<string> target_value){
	map<int, float> info_gain;//store information gain for all attributes of attribute list
	float before_entropy = count_before_entropy(database,target_num);
	float after_entropy = 0;
	for(auto i = attribute_list.begin(); i != attribute_list.end(); i++){
		//count and save information gain for each of the attribute in attribute list
		after_entropy = count_after_entropy(database, (*i), target_num, target_value);
		info_gain[(*i)] = before_entropy-after_entropy;
	}
	//find the best split attribute by the highest information gain
	int best_split_attribute_num=-1;
	float max_info_gain =0;
	for(auto p: info_gain){
		if(p.second>=max_info_gain){
			max_info_gain = p.second;
			best_split_attribute_num=p.first;
		}
	}
	if(best_split_attribute_num==-1){
		cout << "not found best split"<<endl;
		if(attribute_list.size()==0){
			cout << "because attribute_list is empty"<<endl;
		}
	}
	return  best_split_attribute_num;
}
//calculate entropy before splitting
float count_before_entropy(vector<vector<string>> database, int target_num){

	float before_info = 0.0;
	map<string, int> before;//first:value for target,second:count its occurence
	int count=0;//store the number of records in the database
	for(auto i = database.begin(); i != database.end(); i++){//calculate the occurence of possible value for target attribute
		if(before.find((*i)[target_num])==before.end()){
			before[(*i)[target_num]] =1;
		}else{
			before[(*i)[target_num]]++;
		}
		count++;
	}
	//calculate the probability of all possible value of target attribute
	for(auto p: before){
		float probability = float(p.second)/float(count);
		if(probability == 0){
			;
		}else{
			before_info += -(probability)*log2(probability);
		}
	}
	return before_info;
}
float count_after_entropy(vector<vector<string>> database, int attribute_num, int target_num, vector<string> target_value){
	//get statistics on the information for different values of the specified attribute
	int count = 0;//the number of records in the current database
	map<string,statistics> stat;//first:the value of the specified attribute,second: statistics on the value of the attribute 
	for(auto i = database.begin(); i != database.end(); i++){
		if(stat.find((*i)[attribute_num])==stat.end()){//not found yet
			stat[(*i)[attribute_num]].count = 1;
			if((*i)[target_num]==target_value[0]){
				stat[(*i)[attribute_num]].count_target_T =1; 
			}else{
				stat[(*i)[attribute_num]].count_target_F = 1;
			}
		}else{
			stat[(*i)[attribute_num]].count++;
			if((*i)[target_num]==target_value[0]){
                                stat[(*i)[attribute_num]].count_target_T++;
                        }else{
                                stat[(*i)[attribute_num]].count_target_F++;
                        }
		}
			count++;
	}

	//calculate the entropy based on the collected statistics
	float entropy =0;
	for(auto p:stat){
		float probability_of_attribute_value =float(p.second.count)/float(count);
		float probability_of_target_T_for_attribute_value = float(p.second.count_target_T)/float(p.second.count);
		float probability_of_target_F_for_attribute_value = float(p.second.count_target_F)/float(p.second.count);
		if(probability_of_target_T_for_attribute_value ==0 ||probability_of_target_F_for_attribute_value == 0){
			entropy = 0;
		}else{
			entropy +=(-probability_of_attribute_value)*(probability_of_target_T_for_attribute_value*log2(probability_of_target_T_for_attribute_value)+probability_of_target_F_for_attribute_value*log2(probability_of_target_F_for_attribute_value)); 
		}
	}
	return entropy;
}
//draw the tree in a file.
void draw_tree(ofstream & output, node root, string target, vector<string>  target_value, int &tap_num){
	if(root.children.size()==0){//end node
		output<< target << " is "<< root.name<<"."<<endl;
	}else{	
		output << endl;
		for(auto a = root.children.begin(); a != root.children.end(); a++){
			int temp = tap_num;
			int keep =tap_num;
			while(temp){
				output << "	";
				temp--;
			}
			output << "if "<< root.name<< " is "<< (*a).branch<<", then"<<" ";
			tap_num++;
			draw_tree(output, *((*a).child),target, target_value,tap_num);
			tap_num = keep;
		}
	}
}
