#ifdef _DEBUG
#ifndef LOG_H
#define LOG_H

#include <string>
#include <iostream>

class Log
{
public:
	static void Info(const char* text, ...); // Format given text for DEBUG_INFO.
	static void File(const char* file, int line); // Format filename and line number for DEBUG_INFO.

private:
	static std::string GetFile(const char* file); // Filter filename from __FILE__ macro.
};

// Macro used to write debug messages to console.
#define DEBUG_INFO(text) Log::Info text, std::cout << " ", Log::File(__FILE__, __LINE__), std::cout << std::endl

#endif // LOG_H
#endif // _DEBUG

#ifdef NDEBUG
#define DEBUG_INFO
#endif