#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#include "dmumps_c.h"
#include "equationSystem.h"

#define ICNTL(I) icntl[(I)-1] 

class MumpsSolver
{
	public: 
	static void solve(int rank, equationSystem& sistem, int seq = 0);
};
