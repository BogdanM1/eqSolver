#pragma once 
#include <stdlib.h>
#include <stdio.h>
#include <petscksp.h>
#include "equationSystem.h"


class PetscSolver
{
	private: 
	static void createMatrixFomSystem(Mat& A, equationSystem& sistem);
	static void createVecFromSystem(Vec& b, equationSystem& sistem);
	
	public: 
	static int solve(int rank, equationSystem& sistem, int seq = 0);	
};

