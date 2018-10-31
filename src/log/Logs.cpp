#include "Logs.h"
#include "../comm/comm_phases.h"	

void Logs::init()  { f=fopen("log.txt","a");}
void Logs::clear() { fclose(f);}

void Logs::print_ip(struct sockaddr_in client)
{
    fprintf(f," client: %s\n", inet_ntoa(client.sin_addr));
}

void Logs::print_time()
{
    time_t timer; 
	char buffer[26];
	time(&timer);
	strftime(buffer, 26, "%Y/%m/%d %H:%M:%S", localtime(&timer));
    fprintf(f,"%s", buffer);
}

void Logs::print(string str)
{
    print_time();
    fprintf(f," %s\n",str.c_str());
	fflush(f);
}

void Logs::print_break()
{
    fprintf(f,"---------------------------------------------------\n");
	fflush(f);
}

void Logs::start(int phase)
{
    if(phase==PHASE_MAIN_FULL)      print("Accepting full main system");
    if(phase==PHASE_SUBSTRUCT_FULL) print("Accepting full substructures systems");
    if(phase==PHASE_MAIN_RHS)       print("Accepting just rhs of main system");
    if(phase==PHASE_SUBSTRUCT_RHS)  print("Accepting just rhs of substructures systems");
    if(phase==PHASE_DONE)  		    print("Successfully responded to all requests");
}

void Logs::end() { print("Responded."); }
