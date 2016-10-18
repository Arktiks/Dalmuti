#include "Log.h"
#include <cstdarg>
using namespace std;

void Log::Info(const char* text, ...)
{
	char buffer[256];
	va_list arguments;
	va_start(arguments, text);
	int check = vsprintf(buffer, text, arguments);
	va_end(arguments);
	cout << buffer;
}

void Log::File(const char* file, int line)
{
	string filename = GetFile(file);
	cout << "(" << filename << ": " << line << ")";
}

string Log::GetFile(const char* file)
{
	string path(file); // Contains full file-path now.
	int last = path.find_last_of("/\\") + 1; // Find last part of path.
	int amount = path.size() - last; // How long filename is.

	string filename;
	filename.append(path, last, amount); // Copy appropriate part from path.
	return filename;
}