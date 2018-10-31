#pragma once
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "../comm/comm_phases.h"
#include "../solvers/solver.h"
#include "../solvers/equationSystem.h"
#include "../comm/Communication.h"     

#define TAG 10
#define BLOCK_LOW(id, p, n) ((id) * (n) / (p))
#define BLOCK_HIGH(id, p, n) (BLOCK_LOW((id)+1, p, n) - 1)
#define BLOCK_SIZE(id, p, n) (BLOCK_HIGH(id, p, n) - BLOCK_LOW(id, p, n) + 1)
#define BLOCK_OWNER(index, p, n) (((p) * ((index) + 1) - 1) / (n))


class MPI_Process
{	
	protected:
	MPI_Comm comm;
	int broj_zahteva, brzaht_preth, brzahteva_po_procesu;
	int rank, size, isolver_main, isolver_sub, phase;
	vector<int> sys_ind;	  
	vector<equationSystem> sistemi;   
	equationSystem main_sistem; 
	Solver* solver;	 	
	void send_system(int i, equationSystem jedn);
	equationSystem recv_system(int i);
	virtual void exchange_systems(bool redistribute_systems) = 0;
	virtual void exchange_solutions() = 0;	
	
	public:
	virtual void Init(int process_rank, int process_number);
	virtual void Set_main_system();
	virtual void Set_subst_systems();	
	virtual void Solve_main();
	virtual void Solve_subst();	
	virtual void Solve();
	virtual void WhichSolvers();
	virtual void SystemsCleanup();
	virtual void MPICleanup();
	virtual int  WhichData();		
};
