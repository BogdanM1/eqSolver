#include "Timer.h"

void Timer::init()
{ 
	f=fopen("time.txt","a");
	fprintf(f,"\n\tWaiting,\tReceiving data,\tSolving systems,\tSending data,\tTotal\n");		
	m_startG = MPI_Wtime();
	start();
}

void Timer::end(int total)
{
	m_end = MPI_Wtime();
	fprintf(f,"\t %lf,",m_end - m_start);
	if(total) fprintf(f, "\t %lf\n",m_end - m_startG);	
	fflush(f);
}

void Timer::start()
{
	m_start = MPI_Wtime(); 
}

void Timer::clear()
{
	fclose(f);
}
