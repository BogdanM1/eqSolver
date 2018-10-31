#include "MPI_Process.h"

void MPI_Process::Init( int process_rank, int process_number)
{	
	comm = MPI_COMM_WORLD;	
	solver = new Solver();	
	rank = process_rank;
	size = process_number;
	brzaht_preth = 0;
}

void MPI_Process::send_system(int i, equationSystem jedn)
{
    MPI_Send(&jedn.eq_count, 1,MPI_INT, i, TAG, comm);
    MPI_Send(&jedn.nonzero_count, 1,MPI_INT, i, TAG, comm);
    MPI_Send(&jedn.rows[0], jedn.nonzero_count, MPI_INT, i, TAG, comm);
    MPI_Send(&jedn.cols[0], jedn.nonzero_count, MPI_INT, i, TAG, comm);
    MPI_Send(&jedn.vals[0], jedn.nonzero_count, MPI_DOUBLE, i, TAG, comm);
    MPI_Send(&jedn.rhs[0],  jedn.eq_count,      MPI_DOUBLE, i, TAG, comm);
}

equationSystem MPI_Process::recv_system(int i)
{
    MPI_Status status;	equationSystem jedn;
    MPI_Recv(&jedn.eq_count,1,MPI_INT,i,TAG,comm,&status);
    MPI_Recv(&jedn.nonzero_count,1,MPI_INT,i,TAG,comm,&status);		
    jedn.reserve(); 
    MPI_Recv(&jedn.rows[0], jedn.nonzero_count, MPI_INT, i, TAG, comm, &status);
    MPI_Recv(&jedn.cols[0], jedn.nonzero_count, MPI_INT, i, TAG, comm, &status);
    MPI_Recv(&jedn.vals[0], jedn.nonzero_count, MPI_DOUBLE, i, TAG, comm, &status);
    MPI_Recv(&jedn.rhs[0],  jedn.eq_count, MPI_DOUBLE, i, TAG, comm, &status);
	return jedn;
}

void MPI_Process::WhichSolvers()
{
	if(phase == PHASE_MAIN_FULL)
	{		
		MPI_Bcast(&isolver_main, 1, MPI_INT, 0, comm);
		if(isolver_main == PETSC_SOLVER)	PetscInitialize(NULL,NULL,(char*)0,NULL); 
	}	
	if(phase == PHASE_SUBSTRUCT_FULL) 
	{
		MPI_Bcast(&isolver_sub, 1, MPI_INT, 0, comm);
		if(isolver_sub == PETSC_SOLVER)	PetscInitialize(NULL,NULL,(char*)0,NULL); 		
	}
}

void MPI_Process::SystemsCleanup()
{
	main_sistem.clear();
	int n = sistemi.size();
	for(int i=0; i < n; i++) sistemi[i].clear();
	sistemi.clear(); 
	brzaht_preth = 0;	
	if(isolver_main == PETSC_SOLVER|| isolver_sub == PETSC_SOLVER)	PetscFinalize(); 
}

void MPI_Process::MPICleanup()
{
	MPI_Finalize();		
}

int MPI_Process::WhichData()
{
	MPI_Bcast(&phase, 1, MPI_INT, 0, comm);		
	return (phase!=PHASE_DONE);
}

void MPI_Process::Set_main_system()
{
	MPI_Bcast(&main_sistem.eq_count, 1, MPI_INT, 0, comm);
	MPI_Bcast(&main_sistem.nonzero_count, 1, MPI_INT, 0, comm);
}

void MPI_Process::Set_subst_systems()
{
	MPI_Bcast(&broj_zahteva, 1, MPI_INT, 0, comm);		  			
	brzahteva_po_procesu = BLOCK_SIZE(rank,size,broj_zahteva);	
}

void MPI_Process::Solve_main()
{ 
	if(phase==PHASE_MAIN_FULL)  Set_main_system();			
	solver->solve_system(rank, isolver_main, main_sistem); 		 		
}

void MPI_Process::Solve_subst()
{	
	Set_subst_systems();		
	exchange_systems(broj_zahteva != brzaht_preth);  				
	solver->solve_systems(rank, isolver_sub, sistemi, brzahteva_po_procesu, sys_ind); 	
	exchange_solutions();
	brzaht_preth = broj_zahteva;		    	
}

void MPI_Process::Solve()
{	
	if(phase == PHASE_MAIN_FULL || phase == PHASE_MAIN_RHS || phase == PHASE_MAIN_VALS) 
		Solve_main();
	if (phase==PHASE_SUBSTRUCT_FULL || phase==PHASE_SUBSTRUCT_RHS) 
		Solve_subst();
}