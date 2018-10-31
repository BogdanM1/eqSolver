#pragma once 
#include <stdio.h>
#include <string>
using namespace std;

class UserMumpsOptions
{
	public:
	int ordering;
	int imsgs;
	int relaxparam;
};
class UserPetscOptions
{
	public:
	string precond;
	string precondseq;
};	
class UserCuSolverOptions
{
	public:
	int tolerance;
	int    reordering;
	string filltype;
};	

class solverOptions
{
	public:
	int isym;
	static solverOptions& get()
	{
		static solverOptions instance;
		return instance;		
	} 		
	UserMumpsOptions mumps;
	UserPetscOptions petsc;
	UserCuSolverOptions cusolver;

    private:
    solverOptions() {}
    solverOptions(solverOptions const&);
    void operator=(solverOptions const&);	 	
};
