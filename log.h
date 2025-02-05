#ifndef _LOG_H
#define _LOG_H

//[INCLUDES]
#include <string>


//[NAMESPACE]
namespace Log 
{
	//[VARIABLES]

	//[FUNCTIONS]
	void Message(std::string _msg, ...);
	void Warning(std::string _msg, ...);
	void Error(std::string _msg, ...);
}

#endif // !_LOG_H