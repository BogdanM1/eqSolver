#include "solver.h"

void Solver::solve_system(int rank,  int ISOLVER, equationSystem& sistem, int seq)
{
	if ( ISOLVER == PETSC_SOLVER )                           
		PetscSolver::solve(rank, sistem, seq);     
		 
	if( ISOLVER == MUMPS_SOLVER )                         
		MumpsSolver::solve(rank, sistem, seq);	
		 
	#if defined HAVE_CUDA		
	if( ISOLVER == CUSOLVER)                           
		CuSolver::solve(sistem);
	#endif			 
}

void Solver::solve_systems(int rank, int ISOLVER, vector<equationSystem>& sistemi, int sys_num, vector<int>& sys_ind)
{ 	
	for(int i=0;i<sys_num;i++) solve_system(rank, ISOLVER, sistemi[sys_ind[i]], 1);
}


