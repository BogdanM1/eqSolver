#include "cusolver.h"
#include "solverOptions.h"

void CuSolver::solve(equationSystem& sistem)
{  

	int*d_irows,*d_jcols;
	double*d_vals,*d_vekt;
    int sing=0;

	cusolverSpHandle_t cusolverH;
	csrqrInfo_t info;             
	cusparseMatDescr_t descrA;
	cusparseHandle_t handle;		
	
	cudaMalloc((void**)&d_irows,sistem.nonzero_count*sizeof(int));
    cudaMalloc((void**)&d_jcols,sistem.nonzero_count*sizeof(int));
    cudaMalloc((void**)&d_vals,sistem.nonzero_count*sizeof(double));
    cudaMalloc((void**)&d_vekt,sistem.eq_count*sizeof(double));

	cudaMemcpy(d_irows, &sistem.rows[0], (size_t)(sistem.nonzero_count*sizeof(int)), cudaMemcpyHostToDevice);
	cudaMemcpy(d_jcols, &sistem.cols[0], (size_t)(sistem.nonzero_count*sizeof(int)), cudaMemcpyHostToDevice);
	cudaMemcpy(d_vals,  &sistem.vals[0], (size_t)(sistem.nonzero_count*sizeof(double)), cudaMemcpyHostToDevice);
	cudaMemcpy(d_vekt,  &sistem.rhs[0], (size_t)(sistem.eq_count*sizeof(double)), cudaMemcpyHostToDevice);

	
	cusolverH=NULL; info=NULL; descrA=NULL; handle=NULL;
	cusparseCreate(&handle);
	cusolverSpCreate(&cusolverH);
	cusparseCreateMatDescr(&descrA);
	cusparseSetMatType(descrA, CUSPARSE_MATRIX_TYPE_GENERAL);
	if(solverOptions::get().isym==1)
	{
		cusparseSetMatType(descrA, CUSPARSE_MATRIX_TYPE_SYMMETRIC);
		if(solverOptions::get().cusolver.filltype == "lower")
			cusparseSetMatFillMode(descrA, CUSPARSE_FILL_MODE_LOWER);
		else 
			cusparseSetMatFillMode(descrA, CUSPARSE_FILL_MODE_UPPER);
	}
	cusparseSetMatIndexBase(descrA, CUSPARSE_INDEX_BASE_ONE);
	cusolverSpCreateCsrqrInfo(&info);
	cusparseXcoo2csr(handle,d_irows,sistem.nonzero_count,sistem.eq_count, d_irows,CUSPARSE_INDEX_BASE_ONE);
 			
	cusolverSpDcsrlsvchol(cusolverH, sistem.eq_count, sistem.nonzero_count, descrA, 
	                     d_vals, d_irows, d_jcols, d_vekt, 
						 solverOptions::get().cusolver.tolerance, solverOptions::get().cusolver.reordering, d_vekt, &sing);
  
	cudaMemcpy(&sistem.rhs[0], d_vekt, sizeof(double)*sistem.eq_count ,cudaMemcpyDeviceToHost);
		
	cudaFree(d_irows);     
    cudaFree(d_jcols);
    cudaFree(d_vals);
    cudaFree(d_vekt); 
	cusparseDestroy(handle);
	cusparseDestroyMatDescr(descrA);
	cusolverSpDestroyCsrqrInfo(info);
	cusolverSpDestroy(cusolverH);		
	
}


