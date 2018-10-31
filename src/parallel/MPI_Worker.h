#include "MPI_Process.h"


class MPI_Worker:public MPI_Process
{	
	public:
	void exchange_systems(bool redistribute_systems);
	void exchange_solutions();	
	void Set_main_system();
	void Set_subst_systems();	
};
