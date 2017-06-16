#ifndef STATISTICS_H
#define STATISTICS_H
class statistics{
   public:
	int count;//the number of occurence for the value of the specified attribtue
	int count_target_T;//the number of True(target attribute) for records with the value 
	int count_target_F;//the number of False(target attribute) for records with the value
        void init(){
		count =0;
		count_target_T =0;
		count_target_F =0;
	}
};
#endif
