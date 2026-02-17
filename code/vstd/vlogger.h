#ifndef LOGGER
#define LOGGER

#include "vtypes.h"

#define V_LOG_INFO(msg) Logger::Info(msg, call_location{__FILE__, __func__, __LINE__})  
#define V_LOG_WARN(msg) Logger::Warn(msg, call_location{__FILE__, __func__, __LINE__})  
#define V_LOG_ERROR(msg) Logger::Error(msg, call_location{__FILE__, __func__, __LINE__})  

typedef struct call_location{
    const char * file;
    const char * func;
    int loc;
} call_location;


class Logger 
{
    public: 
	enum Level: u8{
	    L_INFO = 1<<0, 
	    L_WARN = 1<<1, 
	    L_ERROR = 1<<2 
	};

    private:
    public:
	static u8 m_LogLevel;
	static void SetLevelDefault(void)
	{
		m_LogLevel = (L_INFO | L_WARN | L_ERROR);
	}
	static void SetLevel(u8 level)
	{
	    m_LogLevel = level;
	}
	
	static void Warn(const char *msg, call_location c_loc)
	{
	    if ((m_LogLevel & L_WARN) == L_WARN)
	    {
		printf("[WARNING]: %s|| %s %s Line:%i\n", msg, c_loc.file, c_loc.func, c_loc.loc);
	    }
	}

	static void Error(const char *msg, call_location c_loc)
	{
	    if ((m_LogLevel & L_ERROR) == L_ERROR)
	    {
		printf("[ERROR]  : %s|| %s %s Line:%i\n", msg, c_loc.file, c_loc.func, c_loc.loc);
	    }
	}

	static void Info(const char *msg, call_location c_loc)
	{
	    if ((m_LogLevel & L_INFO) == L_INFO)
	    {
		printf("[INFO]   : %s|| %s %s Line:%i\n", msg, c_loc.file, c_loc.func, c_loc.loc);
	    }
	}


};

#endif
