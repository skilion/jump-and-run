#include "Memory.h"
#include "WindowsFile.h"

using namespace myengine;


//=============================================================================
// Creates a WindowsFile class
//=============================================================================
File *File::createFile()
{
	return new WindowsFile;
}


//=============================================================================
// Constructor
//=============================================================================
WindowsFile::WindowsFile()
{
	hFile = INVALID_HANDLE_VALUE;
	endfile = true;
	Memory::memset(&overlapped, 0, sizeof(OVERLAPPED));
}

//=============================================================================
// Destructor
//=============================================================================
//WindowsFile::~WindowsFile() {}

//=============================================================================
// Opens a file
//=============================================================================

//=============================================================================
// Opens the file
//=============================================================================
bool WindowsFile::open(const char *filename, OpenMode openMode)
{
	DWORD access = 0, creation = 0;
	switch(openMode)
	{
	case OPENMODE_INPUT:
		access = GENERIC_READ;
		creation = OPEN_EXISTING;
		break;
	case OPENMODE_OUTPUT:
		access = GENERIC_WRITE;
		creation = CREATE_ALWAYS;
		break;
	case OPENMODE_APPEND:
		access = GENERIC_READ | GENERIC_WRITE;
		creation = OPEN_ALWAYS;
		break;
	}

	hFile = CreateFile(filename, access, FILE_SHARE_READ, 0, creation, FILE_FLAG_OVERLAPPED | FILE_FLAG_SEQUENTIAL_SCAN, 0);
	endfile = false;
	
	if(hFile == INVALID_HANDLE_VALUE) return false;

	return true;
}

//=============================================================================
// Returns if the file is open
//=============================================================================
bool WindowsFile::isOpen() const
{
	return hFile != INVALID_HANDLE_VALUE;
}

//=============================================================================
// Returns the file position
//=============================================================================
unsigned WindowsFile::getPosition() const
{
	return overlapped.Offset;
}

//=============================================================================
// Sets the file position
//=============================================================================
void WindowsFile::setPosition(unsigned position)
{
	overlapped.Offset = position;
}

//=============================================================================
// Returns the size of the file
//=============================================================================
unsigned WindowsFile::getSize() const
{
	return GetFileSize(hFile, 0);
}

//=============================================================================
// Closes the file
//=============================================================================
void WindowsFile::close()
{
	CloseHandle(hFile);
	hFile = INVALID_HANDLE_VALUE;
	endfile = true;
}

//=============================================================================
// Returns true if the end has been reched
//=============================================================================
bool WindowsFile::eof() const
{
	return endfile;
}

//=============================================================================
// Try Reads to read 'size' bytes, returns the bytes effectively read
//=============================================================================
unsigned WindowsFile::read(unsigned char *data, unsigned size)
{
	if(!(isOpen() && size != 0)) return 0;
	
	DWORD read;
	if(!ReadFile(hFile, data, size, &read, &overlapped)) {
		switch(GetLastError())
		{
		case ERROR_HANDLE_EOF:
			endfile = true;
			break;

		case ERROR_IO_PENDING:
			if(!GetOverlappedResult(hFile, &overlapped, &read, true)) {
				if(GetLastError() == ERROR_HANDLE_EOF) endfile = true;
			}
		}
	}

	overlapped.Offset += read;

	return read;
}

//=============================================================================
// Flush the output buffer
//=============================================================================
void WindowsFile::flush()
{
	FlushFileBuffers(hFile);
}

//=============================================================================
// Writes 'size' bytes
//=============================================================================
void WindowsFile::write(const unsigned char *data, unsigned size)
{
	if(!isOpen()) return;
	WriteFile(hFile, data, size, 0, &overlapped);
	overlapped.Offset += size;
}