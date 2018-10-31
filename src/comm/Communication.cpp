#include "Communication.h"

void Communication::configure_servis(int SERVER_PORT)
{       
	int on = 1; 
    struct sockaddr_in channel;                      
	memset(&channel, 0, sizeof(channel));             
	channel.sin_family = AF_INET; 
	channel.sin_addr.s_addr = htonl(INADDR_ANY); 
	channel.sin_port = htons(SERVER_PORT); 	 												 
	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);   
	if (server_socket < 0)  
		throw CommException("Socket failed."); 
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char*) &on, sizeof(on)); 
	if ( bind(server_socket, (struct sockaddr *) &channel, sizeof(channel)) < 0)  
		throw CommException("Bind failed.");                       
	if (listen(server_socket, QUEUE_SIZE) < 0)  
		throw CommException("Listen failed."); 
}

void Communication::accept_client()
{
	socklen_t client_len = (sizeof(client_addr));
	client_socket = accept(server_socket,(struct sockaddr *)&client_addr,&client_len);        
}

void Communication::accept_int(int& num)
{
	recvArray(client_socket, &num, sizeof(int), BUF_SIZE);          
}

void Communication::accept_rhs(equationSystem& sistem)
{
	recvArray(client_socket, &sistem.rhs[0], sistem.eq_count*sizeof(double),BUF_SIZE);	
}

void Communication::accept_vals(equationSystem& sistem)
{   
	recvArray(client_socket, &sistem.vals[0], sistem.nonzero_count*sizeof(double), BUF_SIZE);		
}

void Communication::accept_rows(equationSystem& sistem)
{   
	recvArray(client_socket, &sistem.rows[0], sistem.nonzero_count*sizeof(int), BUF_SIZE);	
}

void Communication::accept_cols(equationSystem& sistem)
{   
	recvArray(client_socket, &sistem.cols[0], sistem.nonzero_count*sizeof(int), BUF_SIZE);	
}

void Communication::accept_system(equationSystem& sistem)
{	
	accept_int(sistem.eq_count);  
	accept_int(sistem.nonzero_count);	
	sistem.reserve();	
	accept_rows(sistem);
	accept_cols(sistem);
	accept_vals(sistem);
	accept_rhs(sistem);
}

void Communication::accept_systems(vector<equationSystem> &sistemi, int& sys_num, vector<int>& sys_ind)
{
	equationSystem sistem;
	accept_int(sys_num);
	sistemi.reserve(sys_num);
	sys_ind.reserve(sys_num);	
	for(int i=0;i<sys_num;i++)
	{
		accept_system(sistem);	
		sistemi.push_back(sistem);
		sistemi[i] = sistem;
		sys_ind[i] = i;
	}
}
 
void Communication::accept_rhs_ind(vector<equationSystem> &sistemi,int& sys_num, vector<int>& sys_ind)
{
    sys_num=0;
	int index;
	while(1)                           
	{   
		accept_int(index);
		if(index==-1) break;
        accept_rhs(sistemi[index]);
		sys_ind[sys_num] = index;
		sys_num++;
	}	
}

void Communication::accept_data(int phase, equationSystem& main_sistem, vector<equationSystem> &sistemi, int& sys_num, vector<int>& sys_ind)
{	 
	if(phase==PHASE_MAIN_FULL)            
		accept_system(main_sistem);
	if(phase==PHASE_SUBSTRUCT_FULL)       
		accept_systems(sistemi, sys_num, sys_ind);
	if(phase==PHASE_SUBSTRUCT_RHS)        
		accept_rhs_ind(sistemi, sys_num, sys_ind); 
	if(phase==PHASE_MAIN_RHS)             
		accept_rhs(main_sistem); 
    if(phase==PHASE_MAIN_VALS)
	{           
		accept_vals(main_sistem);  
	    accept_rhs(main_sistem);
	} 
}

void Communication::respond(int phase,equationSystem& main_sistem,vector<equationSystem>& sistemi, int sys_num, vector<int>& sys_ind)
{
	if(phase==PHASE_MAIN_FULL || phase==PHASE_MAIN_RHS || phase==PHASE_MAIN_VALS)  
	{ 
		sendArray(client_socket, &main_sistem.rhs[0], main_sistem.eq_count*sizeof(double),BUF_SIZE);
	}
	if(phase==PHASE_SUBSTRUCT_FULL || phase==PHASE_SUBSTRUCT_RHS)
	{
		for(int i=0;i<sys_num;i++)           
		    sendArray(client_socket, &sistemi[sys_ind[i]].rhs[0], sistemi[sys_ind[i]].eq_count*sizeof(double),BUF_SIZE);
	}
}
