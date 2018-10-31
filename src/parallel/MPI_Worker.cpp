#include "MPI_Worker.h"

void MPI_Worker::exchange_solutions()
{				 
    for(int i=0;i < brzahteva_po_procesu; i++)
        MPI_Send(&sistemi[i].rhs[0], sistemi[i].eq_count, MPI_DOUBLE, 0, TAG, comm);
}

void MPI_Worker::exchange_systems(bool redistribute_systems)
{	
	if(redistribute_systems) 
	{
		sistemi.clear();
		for(int i=0; i<brzahteva_po_procesu; i++) 
		{
			equationSystem sistem = recv_system(0);
			sistemi.push_back(sistem);
			sistemi[i] = sistem;			
		}
	}	
	else 	
	{
		for(int i=0; i<brzahteva_po_procesu; i++)
			MPI_Recv(&sistemi[i].rhs[0], sistemi[i].eq_count, MPI_DOUBLE, 0, TAG, comm, NULL);
	}
}

void MPI_Worker::Set_main_system()
{
	MPI_Process::Set_main_system();
	main_sistem.reserve();	
}

void MPI_Worker::Set_subst_systems()
{
	MPI_Process::Set_subst_systems();
	if(phase==PHASE_SUBSTRUCT_FULL) 
	{	
		sys_ind.reserve( brzahteva_po_procesu);
		for(int i=0; i < brzahteva_po_procesu; i++) sys_ind[i] = i;
	}	
}