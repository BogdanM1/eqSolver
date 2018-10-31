#include "MPI_Manager.h"

void MPI_Manager::serverStart(int SERVER_PORT)
{
	Communication::get().configure_servis(SERVER_PORT);	
	Logs::get().init();	
	Timer::get().init();	
}	

void MPI_Manager::MPICleanup()
{
	Logs::get().clear(); 
	Timer::get().clear();
	MPI_Process::MPICleanup();	
}

void MPI_Manager::Accept_Client()
{	
	Communication::get().accept_client();
	Logs::get().print_ip(Communication::get().client_addr); 		
}

void MPI_Manager::WhichSolvers()
{	
	if(phase==PHASE_MAIN_FULL)      Communication::get().accept_int(isolver_main);  
	if(phase==PHASE_SUBSTRUCT_FULL) Communication::get().accept_int(isolver_sub);    	
	MPI_Process::WhichSolvers();
}

int MPI_Manager::WhichData()
{
	Timer::get().start();	
	Communication::get().accept_int(phase); 			
	Logs::get().start(phase);	
	if(phase == PHASE_DONE) Logs::get().print_break();	
	else 	Timer::get().end();
	return MPI_Process::WhichData();
}

void MPI_Manager::Accept_Data()
{			
	Timer::get().start();			
	Communication::get().accept_data(phase, main_sistem, sistemi, broj_zahteva, sys_ind); 			
	Timer::get().end();
}

void MPI_Manager::Respond()
{	
	Timer::get().start();  								   
	Communication::get().respond(phase,main_sistem,sistemi,broj_zahteva,sys_ind); 
	Logs::get().end();
	Timer::get().end(1);
}

void MPI_Manager::exchange_solutions()
{				 
    int i, j,jhigh,jlow;
    MPI_Status status;
    for(i=1;i<size;i++)
    {    
        jlow=BLOCK_LOW(i,size, broj_zahteva);
        jhigh=BLOCK_HIGH(i,size, broj_zahteva);
        for(j=jlow;j<=jhigh;j++)
             MPI_Recv(&sistemi[sys_ind[j]].rhs[0], sistemi[sys_ind[j]].eq_count,MPI_DOUBLE,i,TAG,comm,&status);     
    }
}

void MPI_Manager::exchange_systems(bool redistribute_systems)
{				 
    int i, j,jhigh,jlow;
    MPI_Status status;
    for(i=1;i<size;i++)
    {    
        jlow=BLOCK_LOW(i,size, broj_zahteva);
        jhigh=BLOCK_HIGH(i,size, broj_zahteva);
        for(j=jlow;j<=jhigh;j++)
		{
			if (redistribute_systems) 
				send_system(i, sistemi[sys_ind[j]]); 
			else 
				MPI_Send(&sistemi[sys_ind[j]].rhs[0], sistemi[sys_ind[j]].eq_count,MPI_DOUBLE,i,TAG,comm);
		}
    }
}

void MPI_Manager::Solve_main()
{ 	
	Timer::get().start(); 	
	MPI_Process::Solve_main();		
	Timer::get().end(); 		
}

void MPI_Manager::Solve_subst()
{
	Timer::get().start(); 	
	MPI_Process::Solve_subst();	
	Timer::get().end();		    	
}

