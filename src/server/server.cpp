#include "../parallel/MPI_Process.h"
#include "../parallel/MPI_Manager.h"
#include "../parallel/MPI_Worker.h"

#include "../conf/config_parser.h"

int Accept_Data(int rank, MPI_Process *mpi_proc)
{
	int ok = 1;
	if(rank==0)
	{
		try{
			static_cast<MPI_Manager*>(mpi_proc)->Accept_Data();
		}
		catch(...)
		{
			Logs::get().print("Error occured during acceptance of the data.");
			Logs::get().print_break();
			ok = 0; 
		}	
	}
	MPI_Bcast(&ok, 1, MPI_INT, 0, MPI_COMM_WORLD);
	return ok;
	
}

int Respond(int rank, MPI_Process *mpi_proc)
{
	int ok = 1;
	if(rank==0)
	{			
		try{
			static_cast<MPI_Manager*>(mpi_proc)->Respond();
		}
		catch(...)
		{
			Logs::get().print("Error occured during sending of the data.");
			Logs::get().print_break();
			ok = 0; 
		}		
	}
	MPI_Bcast(&ok, 1, MPI_INT, 0, MPI_COMM_WORLD);
	return ok;
}

int serverStart(int rank, MPI_Process *mpi_proc, int port)
{
	int ok = 1;
	if(rank==0)
	{			
		try{
			static_cast<MPI_Manager*>(mpi_proc)->serverStart(port);
		}
		catch(CommException& commEx)
		{
			cout << commEx.what() << "\n";	
			ok = 0; 
		}		
	}
	MPI_Bcast(&ok, 1, MPI_INT, 0, MPI_COMM_WORLD);
	return ok;	
}

int  main(int argc, char *args[]) 
{ 
	int rank, size;
	MPI_Init(&argc,&args);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);	
	MPI_Process * mpi_proc = (rank==0) ? (MPI_Process *)new MPI_Manager() : (MPI_Process *)new MPI_Worker();
	mpi_proc->Init(rank,size);	
	if(!serverStart(rank, mpi_proc, atoi(args[1]))) {MPI_Finalize(); exit(0);}
	while (1) 
	{
		if(rank==0) static_cast<MPI_Manager*>(mpi_proc)->Accept_Client(); 
		MPI_Barrier(MPI_COMM_WORLD);
		ConfParser::loadConf();
		ConfParser::loadSolverOptions();
		ConfParser::unloadConf();		
		while(1)
		{
			if(!mpi_proc->WhichData()) break; 
			mpi_proc->WhichSolvers();
			if(!Accept_Data(rank, mpi_proc)) break;
			mpi_proc->Solve();
			if(!Respond(rank, mpi_proc)) break;
		}
		mpi_proc->SystemsCleanup();	
	}
	 mpi_proc->MPICleanup();
	 return 0; 
}

