#include "Memory.h"
#include "Log.h"

#include <iostream>
#include <string>

using namespace std;

vector<MemoryTrack> Memory::memory; // Define static memory vector.

void Memory::AddTrack(MemoryTrack track)
{
	memory.push_back(track);
}

void Memory::DeleteTrack(void* pointer)
{
	if (memory.empty())
		return;

	for (vector<MemoryTrack>::iterator it = memory.begin(); it != memory.end(); it++) // Search through all tracks.
	{
		if (pointer == it->pointer)
		{
			memory.erase(it);
			break;
		}
	}
}

void Memory::Print()
{
	if (!memory.empty()) // If there are undeleted tracks.
	{
		for (vector<MemoryTrack>::iterator it = memory.begin(); it != memory.end(); it++) // Iterate through memory vector...
			cout << (it->filename) << " (" << (it->line) << ")" << endl; // ... and write each track to console.
	}
	else
		DEBUG_INFO(("There are no tracks in memory."));
}

bool Memory::IsClean()
{
	return memory.empty();
}

///////////////////////////////////////////////////////
///		Definitions for new and delete operators	///
///////////////////////////////////////////////////////

void* operator new(size_t size, const char* file, int line)
{
	void* pointer = new char[size];	                   // Make void pointer based on size.
	string filename(file);                             // Convert filename to string.

	MemoryTrack track;
	track.pointer = pointer;
	track.size = size;
	track.filename = filename;
	track.line = line;

	Memory::AddTrack(track);                           // Make note of memory track.
	return pointer;                                    // Return pointer as per normal.
}

void *operator new[](size_t size, const char* file, int line)
{
	return operator new(size, file, line); // Use above function.
}

void operator delete(void* pointer)
{
	Memory::DeleteTrack(pointer); // Remove track from manager.
	free(pointer); // Free memory as per normal.
}

void operator delete[](void* pointer)
{
	operator delete(pointer);
}