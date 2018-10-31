extern "C" {
#include "cJSON.h"
}
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
using namespace std;

class ConfParser
{
	private:
	static cJSON* conf;
	
	public: 
	static void loadConf();
	static void unloadConf();
	static void loadSolverOptions();
};
