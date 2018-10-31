#include "petscSolver.h"
#include "solverOptions.h"

void PetscSolver::createMatrixFomSystem(Mat& A, equationSystem& sistem)
{
	for(int i=0; i<sistem.nonzero_count; i++)
	{
		MatSetValue(A, sistem.rows[i]-1, sistem.cols[i]-1, sistem.vals[i], INSERT_VALUES); 
		if(solverOptions::get().isym && sistem.cols[i] != sistem.rows[i]) MatSetValue(A,sistem.cols[i]-1,sistem.rows[i]-1, sistem.vals[i], INSERT_VALUES);
	}	
}

void PetscSolver::createVecFromSystem(Vec& b, equationSystem& sistem)
{
	for(int i=0; i< sistem.eq_count; i++) 
		VecSetValue(b, i, sistem.rhs[i], INSERT_VALUES);	
}

int PetscSolver::solve(int rank, equationSystem&sistem, int seq)
{ 	
	Vec            x,b;      
	Mat            A;       
	KSP            ksp;       
	PetscInt       i;
	PetscInt prealokacija;    
	Vec vout;
	prealokacija=10*sistem.nonzero_count/sistem.eq_count;
	PC prec;
	const char* precond = (seq)?solverOptions::get().petsc.precondseq.c_str() : solverOptions::get().petsc.precond.c_str();
	if(seq)
		MatCreateSeqAIJ(PETSC_COMM_SELF,  sistem.eq_count,  sistem.eq_count, prealokacija,PETSC_NULL,&A);
	else 
	{
		MatCreate(PETSC_COMM_WORLD, &A); 
		MatSetSizes(A, PETSC_DECIDE, PETSC_DECIDE, sistem.eq_count, sistem.eq_count); 
		MatSetFromOptions(A); 
		MatMPIAIJSetPreallocation(A, prealokacija, PETSC_NULL, prealokacija, PETSC_NULL); 
		MatZeroEntries(A); 
	}		 		 
	MatSetOption(A, MAT_NEW_NONZERO_ALLOCATION_ERR, PETSC_FALSE) ; 
	if(!rank || seq)    createMatrixFomSystem(A, sistem);	
	MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY); 
	MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY); 
		 
	if(seq)
		VecCreateSeq(PETSC_COMM_SELF,  sistem.eq_count, &b);
	else 
	{
		VecCreate(PETSC_COMM_WORLD, &b); 
		VecSetSizes(b, PETSC_DECIDE, sistem.eq_count); 
		VecSetFromOptions(b); 
	}
	VecDuplicate(b, &x); 
	if(!rank || seq) createVecFromSystem(b, sistem);
	VecAssemblyBegin(b);
	VecAssemblyEnd(b);     
		 
	if(seq)
		KSPCreate(PETSC_COMM_SELF,&ksp);        
	else
		KSPCreate(PETSC_COMM_WORLD, &ksp); 
	KSPSetOperators(ksp, A, A); 
	KSPGetPC(ksp, &prec); 
	PCSetType(prec, precond); 
	KSPSetFromOptions(ksp); 
	KSPSolve(ksp, b, x); 	    

	if(seq)
		VecDuplicate(x, &vout); 
	else 
	{		
		VecCreateSeq( PETSC_COMM_SELF, (rank) ? 0:sistem.nonzero_count, &vout); 
		VecScatter    vecscatter;
		VecScatterCreateToZero(x, &vecscatter, &vout);  
		VecScatterBegin(vecscatter, x, vout, INSERT_VALUES,SCATTER_FORWARD); 
		VecScatterEnd(vecscatter, x, vout, INSERT_VALUES, SCATTER_FORWARD); 
		VecScatterDestroy(&vecscatter); 
	}
	if(!rank || seq)                                             
	{
		int index[sistem.eq_count];
		for(i=0; i < sistem.eq_count; i++) index[i]=i;
		VecGetValues(vout, sistem.eq_count, index, &sistem.rhs[0]);                      
	}	

	VecDestroy(&vout);  
	VecDestroy(&x); 
	VecDestroy(&b);   
	MatDestroy(&A); 
	KSPDestroy(&ksp); 	
	return 0;  
}

