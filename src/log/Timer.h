#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

class Timer
{
	public: 
	void init();
	void start();
	void end(int total = 0);
	void clear();
	static Timer& get()
	{
		static Timer instance;
		return instance;		
	} 			
    
	private:
    Timer() {}  
    Timer(Timer const&);
    void operator=(Timer const&);	
	double m_start, m_end, m_startG;	
	FILE *f;
};

