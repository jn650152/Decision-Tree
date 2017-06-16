#include<fstream>
#include<sstream>
#include "functions.h"
#include "attribute_statistics.h"
using namespace std;
int main(){
	//read parameters
	string file_name;
	cout << "What is the name of the file containing your data?"<< endl;
	cin >> file_name;
	ifstream file;
	file.open(file_name);

	//check whether file exists
	if(file.good()==false){
		cout << "the file doesn't exist."<<endl;
		return 0;
	}
	//save attributes
	string line;
	vector<string> attributes;
	getline(file, line);
	stringstream ss;
	int attr_num =0;//count attribute number
	ss << line;
	string word;
	vector<int> attribute_nums;//save attributes in terms of number
	while(ss >> word){
		attributes.push_back(word);
		attribute_nums.push_back(attr_num++);
	}


	//load files
	vector<vector<string>> info;
	int record_num =0;
	vector<attribute_statistics> attribute_info;
	for(int i =0; i < attr_num; i++){
		attribute_statistics temp;
		attribute_info.push_back(temp);
	}
	while(getline(file,line)){
		vector<string> record;
		stringstream ss1;
		ss1 << line;
		string word1;
		int iterate_attribute =0;		
		while(ss1 >> word1){
			record.push_back(word1);
			//count the number of possible value for each attribute
			if(attribute_info[iterate_attribute].value_count.find(word1) == attribute_info[iterate_attribute].value_count.end()){//not found
				attribute_info[iterate_attribute].value_count[word1] = 1;
			}else{
				attribute_info[iterate_attribute].value_count[word1]++;
			}
			iterate_attribute++;
		}
		info.push_back(record);
		record_num++;
	}
	//get binary attributes	
	for(int i = 0; i < attr_num; i++){
		if(attribute_info[i].value_count.size()==2){
			attribute_info[i].binary = true;
		}else{
			attribute_info[i].binary = false;
		}
		
	}
	//choose target attribute
	cout << "Please choose an attribute (by number) as the prediction attribute: " << endl;
	map<int, int> target_map;
	int choice_num=0;
	vector<int> possible_choice_num;
	for(int i =0; i < attr_num;i++){
		if(attribute_info[i].binary ==true){
			choice_num++;
			cout << choice_num<<"."<< attributes[i]<<endl;
			possible_choice_num.push_back(choice_num);
			target_map[choice_num] = i;
		}
	}
	int target_choice_num;
	cout << "your choice:"<<endl;
	cin >> target_choice_num;

	//check whether choice is valid
	bool valid;
	valid = check_choice_valid(possible_choice_num,target_choice_num);
	if(valid==false){
		cout <<"choice numer is invalid."<<endl;
		return 0;
	}

	//get related parameters for DT 
	//the target related info from choice_num
	int target_num = target_map[target_choice_num];
	string target = attributes[target_num];
	//get the attribute list
	attribute_nums.erase(attribute_nums.begin() + target_num);
	//get possible target value
	vector<string> target_value;
	for(auto p:attribute_info[target_num].value_count){
		target_value.push_back(p.first);
	}

	//create a tree
	node root = DT(info,target_num,target, attribute_nums,attributes,target_value);		

	//draw the tree in a file
	ofstream output;
        output.open("Rules");
	int tap_num =0;
	draw_tree(output,root,target, target_value,tap_num);
	output.close();
       
	return 0;
}
