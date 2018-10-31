#include "config_parser.h"
#include "../solvers/solverOptions.h"

cJSON* ConfParser::conf;

void ConfParser::loadConf()
{	
	stringstream configurationStr;
	ifstream conf_stream("config.cfg");
	configurationStr << conf_stream.rdbuf();	
	conf = cJSON_Parse(configurationStr.str().c_str());
	conf_stream.close();
}

void ConfParser::loadSolverOptions()
{
	cJSON* json = cJSON_GetObjectItem(conf,"solverOptions");
	solverOptions::get().isym = cJSON_GetObjectItem(json,"sym")->valueint;
	
	cJSON* mumpsjson = cJSON_GetObjectItem(json,"mumps");            
	solverOptions::get().mumps.imsgs      = cJSON_GetObjectItem(mumpsjson,"imsgs")->valueint;	
	solverOptions::get().mumps.ordering   = cJSON_GetObjectItem(mumpsjson,"ordering")->valueint;
	solverOptions::get().mumps.relaxparam = cJSON_GetObjectItem(mumpsjson,"relaxparam")->valueint;

	cJSON* cusolverjson = cJSON_GetObjectItem(json,"cusolver");
	solverOptions::get().cusolver.reordering = cJSON_GetObjectItem(cusolverjson,"reordering")->valueint;	
	solverOptions::get().cusolver.tolerance  = cJSON_GetObjectItem(cusolverjson,"tolerance")->valueint;
	solverOptions::get().cusolver.filltype  = cJSON_GetObjectItem(cusolverjson,"filltype")->valuestring;

	cJSON* petscjson = cJSON_GetObjectItem(json,"petsc");
	solverOptions::get().petsc.precond     = cJSON_GetObjectItem(petscjson,"precond")->valuestring;	
	solverOptions::get().petsc.precondseq  = cJSON_GetObjectItem(petscjson,"precondseq")->valuestring;
}

void ConfParser::unloadConf()
{
	cJSON_Delete(conf);	
}