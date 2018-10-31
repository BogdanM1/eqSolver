#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>                  		
#include <sys/fcntl.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h> 
#include <arpa/inet.h>
#include <time.h>

using namespace std;

class Logs
{
	public: 
	void init();
	void print(string str);
	void start(int phase);
	void end();
	void print_break();	
	void print_ip(struct sockaddr_in client);
	void clear();
	static Logs& get()
	{
		static Logs instance;
		return instance;		
	} 		
	
    private:
    Logs() {}   
    Logs(Logs const&);
    void operator=(Logs const&);
	void print_time();	
	FILE *f;
};

