#pragma once

#include "../solvers/equationSystem.h"
#include "SocketArrayTransfer.h"
#include "../comm/comm_phases.h"

#define BUF_SIZE 4096                    
#define QUEUE_SIZE 10  

class Communication
{
	public:
	void configure_servis(int SERVER_PORT);
	void accept_client();
	void accept_int(int& num);		
	void accept_data(int phase, equationSystem& main_sistem, vector<equationSystem> &sistemi, int& sys_num, vector<int>& sys_ind);
	void respond(int phase, equationSystem& main_sistem, vector<equationSystem> &sistemi,int sys_num, vector<int>& sys_ind);
	static Communication& get()
	{
		static Communication instance;
		return instance;		
	} 	
	struct sockaddr_in client_addr;  	

	private:
	int client_socket, server_socket;	
    Communication() {}    
    Communication(Communication const&);
    void operator=(Communication const&);	
	void accept_vals(equationSystem& sistem);	
	void accept_rhs(equationSystem& sistem);
	void accept_rows(equationSystem& sistem);
	void accept_cols(equationSystem& sistem);	
	void accept_system(equationSystem& sistem);
	void accept_systems(vector<equationSystem> &sistemi,int& sys_num, vector<int>& sys_ind);
	void accept_rhs_ind(vector<equationSystem> &sistemi, int& sys_num, vector<int>& sys_ind);
};


