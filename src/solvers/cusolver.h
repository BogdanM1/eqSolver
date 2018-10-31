#include <stdio.h>
#include <stdlib.h>  
#include <cuda.h>
#include <cusolverSp.h>  
#include <cusparse.h>
#include "equationSystem.h"

class CuSolver
{
	public: 
	static void solve(equationSystem& sistem);
};
