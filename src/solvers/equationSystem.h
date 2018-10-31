#pragma once
#include "stdlib.h"
#include "stdio.h"
#include <vector>
#include <memory>

using namespace std; 

class equationSystem
{
	public:
	int  eq_count, nonzero_count;            
	vector<int>     rows;    
	vector<int> 	cols;    
	vector<double>  vals;
	vector<double>	rhs;   
	void    reserve();
	void    clear();
	void 	print(FILE*out);
	equationSystem& operator=(equationSystem& r);
}; 

