#include "Log.h"
#include <cstdarg>

using namespace std;

void Log::Info(const char* text, ...)
{
	char buffer[256];
	va_list arguments; // Type to hold information about variable arguments.

	va_start(arguments, text); // Macro to initialize variable argument list.
	int check = vsprintf(buffer, text, arguments); // Move text from variable argument list to string.
	va_end(arguments); // End using variable argument list.

	if (check < 0)
		cout << "Debug failed to format text." << endl;
	else
		cout << buffer; // Finally print message.
}

void Log::File(const char* file, int line)
{
	string filename = GetFile(file);
	cout << "(" << filename << ": " << line << ")";
}

string Log::GetFile(const char* file)
{
	string path(file); // Contains full file-path now.
	size_t last = path.find_last_of("/\\") + 1; // Find last part of path.
	size_t amount = path.size() - last; // How long filename is.

	string filename;
	filename.append(path, last, amount); // Copy appropriate part from path.

	if (filename.empty())
	{
		cout << "Debug failed to format filename." << endl;
		return string();
	}
	else
		return filename;
}