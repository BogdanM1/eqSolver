#include "MPI_Process.h"
#include "../log/Logs.h"
#include "../log/Timer.h" 

class MPI_Manager: public MPI_Process
{	   	
	public:
	int WhichData();	
	void WhichSolvers();
	void Accept_Client();
	void Accept_Data();
	void Respond();
	void exchange_systems(bool redistribute_systems);
	void exchange_solutions();
	void Solve_main();
	void Solve_subst();	
	void MPICleanup();
	void serverStart(int SERVER_PORT);
};
