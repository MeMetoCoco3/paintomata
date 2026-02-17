
#include <sstream>
#include "vgeneral.h"
#include <fstream>


std::string LoadFile(const std::string& path)
{
	std::ifstream file(path);

	if (!file.is_open())
	{
		throw std::runtime_error("FAILED TO OPEN FILE: " + path);
	} 
	
	std::stringstream buff;
	buff << file.rdbuf();
	file.close();
	return buff.str();
}
