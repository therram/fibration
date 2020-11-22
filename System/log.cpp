#include "log.hpp"

#include <string>
#include <cstdio>
#include <cstdarg>

void Log::error(const std::string &context, const char *fmt, ... )
{
	if(LOG_ENABLED)
	{
		va_list arglist;
		va_start(arglist, fmt);
		log(LOG_TYPE_ERROR, context, fmt, arglist);
		va_end(arglist);
	}
};

void Log::warning(const std::string &context, const char *fmt, ... )
{
	if(LOG_ENABLED)
	{
		va_list arglist;
		va_start(arglist, fmt);	
		log(LOG_TYPE_WARNING, context, fmt, arglist);
		va_end(arglist);
	}
};

void Log::message(const std::string &context, const char *fmt, ... )
{
	if(LOG_ENABLED)
	{
		va_list arglist;
		va_start(arglist, fmt);	
		log(LOG_TYPE_MESSAGE, context, fmt, arglist);
		va_end(arglist);
	}
};

void Log::syserr(const std::string &context, const char *fmt, ... )
{
	if(LOG_ENABLED)
	{
		va_list arglist;
		va_start(arglist, fmt);	
		log(LOG_TYPE_SYSERR, context, fmt, arglist);
		va_end(arglist);
	}
};

void Log::log(Type type, 
	const std::string &context, const char *fmt, va_list arglist)
{
	if(LOG_ENABLED)
	{
		// todo real timeStamp
		const std::string timeStamp = "0.123"; // ms	

		enum ColorType { COLOR, TYPE, COLORTYPE_SIZE };
		// try just char pointer array... maybe more efficient?
		const std::string arColorAndType[LOG_TYPES_SIZE][COLORTYPE_SIZE] = {
			{"\e[39m", "SYSERR"}, // white
			{"\e[31m", "ERROR"}, // red
			{"\e[33m", "WARNING"}, // yellow
			{"\e[36m", "MESSAGE"}, // cyan
		};

		/* todo could be very inefficient using std::string (memory and 
		perfomance wise), test this. maybe try https://github.com/fmtlib/fmt 
		for printig and formatting */
		printf("%s%s\t%s\t%s\t", 
			arColorAndType[type][COLOR].c_str(),
			timeStamp.c_str(), 
			arColorAndType[type][TYPE].c_str(), 
			context.c_str());
			
		vprintf(fmt, arglist);
		printf("\e[0m\n\r");
	}
};
