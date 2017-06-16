#ifndef ATTRIBUTE_STATISTICS_H
#define ATTRIBUTE_STATISTICS_H
#include <string>
#include <map>
class attribute_statistics{
	public:
		bool binary;
		map<string, int> value_count;
		void init(){
			binary = false;
		}
};
#endif
