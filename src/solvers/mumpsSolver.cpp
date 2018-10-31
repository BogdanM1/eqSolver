#include "mumpsSolver.h"
#include "solverOptions.h" 

void MumpsSolver::solve(int rank, equationSystem& sistem, int seq)
{
	DMUMPS_STRUC_C dmumps;
	dmumps.comm_fortran = (MUMPS_INT) MPI_Comm_c2f(seq ? MPI_COMM_SELF:MPI_COMM_WORLD);
	dmumps.sym = solverOptions::get().isym;
	dmumps.par = 1;
	dmumps.job = -1;	
	dmumps_c(&dmumps);
	if(!solverOptions::get().mumps.imsgs)
	{
		dmumps.ICNTL(1)= -1; 
		dmumps.ICNTL(2)= -1;
		dmumps.ICNTL(3)= -1;
		dmumps.ICNTL(4)=  0;  
	}
	dmumps.ICNTL(14)= solverOptions::get().mumps.relaxparam;
	dmumps.ICNTL(7) = solverOptions::get().mumps.ordering; 		  
	if (!rank || seq) 
	{
		dmumps.n     = sistem.eq_count; 
		dmumps.nz    = sistem.nonzero_count; 
		dmumps.irn   = &sistem.rows[0]; 
		dmumps.jcn   = &sistem.cols[0];
		dmumps.a     = &sistem.vals[0]; 
	}
	dmumps.job = 1;
	dmumps_c(&dmumps);	
	dmumps.job = 2;
	dmumps_c(&dmumps); 	
	if (!rank || seq) dmumps.rhs = &sistem.rhs[0];  
	dmumps.job = 3;
	dmumps_c(&dmumps); 
	dmumps.job = -2;
	dmumps_c(&dmumps);		
}


