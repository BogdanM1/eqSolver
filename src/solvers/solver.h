#pragma once
#include "solverOptions.h"
#include "equationSystem.h"
#include "petscSolver.h"
#include "mumpsSolver.h"
#include "solverid.h"
#if defined HAVE_CUDA	
#include "cusolver.h"
#endif



class Solver
{
	public: 
	void solve_system(int rank, int ISOLVER, equationSystem& sistem, int seq = 0);
	void solve_systems(int rank, int ISOLVER, vector<equationSystem>& sistemi, int sys_num, vector<int>& sys_ind);
};












