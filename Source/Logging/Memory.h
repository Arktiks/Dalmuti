#ifdef _DEBUG
#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include <stdlib.h>
#include <string>

// Prevent linker errors.
#pragma comment(linker, "/nodefaultlib:libc.lib")
#pragma comment(linker, "/nodefaultlib:libcd.lib")

// Contains data where new-operator was called.
struct MemoryTrack
{
public:
	void* pointer;			// Pointer to data.
	size_t size;			// Size of data type.
	std::string filename;	// File where new was called.
	int line;				// Line number where new was called.
};

class Memory
{
public:
	static void AddTrack(MemoryTrack track);
	static void DeleteTrack(void* pointer);
	static void Print();
	static bool IsClean();

private:
	static std::vector<MemoryTrack> memory;
};

// Overload new and delete operators to track memory leaks.
// Functions are defined in Memory.cpp to avoid compilation errors.
extern void* operator new(size_t size, const char* file, int line);
extern void* operator new[](size_t size, const char* file, int line);
extern void  operator delete(void* pointer) noexcept;
extern void  operator delete[](void* pointer) noexcept;

// Macro calling overloaded new operator.
#define NEW new(__FILE__, __LINE__)

#endif // MEMORY_H
#endif // _DEBUG


#ifdef NDEBUG
#define NEW new
#endif