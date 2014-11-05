#include "StandardFile.h"

using namespace std;
using namespace myengine;


//=============================================================================
// Creates a StandardFile class
//=============================================================================
File *File::createFile()
{
	return new StandardFile;
}


//=============================================================================
// Constructor
//=============================================================================
//StandardFile::StandardFile() { }

//=============================================================================
// Destructor
//=============================================================================
//StandardFile::~StandardFile() { }

//=============================================================================
// Opens a file
//=============================================================================
bool StandardFile::open(const char *filename, OpenMode openMode)
{
	fstream::openmode stdMode = fstream::binary;
	switch(openMode)
	{
	case OPENMODE_INPUT:
		stdMode |= fstream::in;
		break;
	case OPENMODE_OUTPUT:
		stdMode |= fstream::out;
		break;
	case OPENMODE_APPEND:
		stdMode |= fstream::app;
		break;
	}

	fstream::open(filename, stdMode);
	if(!is_open()) return false;

	return true;
}

//=============================================================================
// Returns if the file is open
//=============================================================================
bool StandardFile::isOpen() const
{
	return fstream::is_open();
}

//=============================================================================
// Returns the file position
//=============================================================================
unsigned StandardFile::getPosition() const
{
	return static_cast<unsigned>(const_cast<StandardFile*>(this)->fstream::tellg());
}

//=============================================================================
// Sets the file position
//=============================================================================
void StandardFile::setPosition(unsigned position)
{
	seekg(position);
}

//=============================================================================
// Returns the size of the file
//=============================================================================
unsigned StandardFile::getSize() const
{
	streamoff pos = const_cast<StandardFile*>(this)->tellg();

	const_cast<StandardFile*>(this)->seekg(0, ios::end);
	streamoff length = const_cast<StandardFile*>(this)->tellg();
	const_cast<StandardFile*>(this)->seekg(pos);

	return static_cast<unsigned>(length);
}

//=============================================================================
// Closes the file
//=============================================================================
void StandardFile::close()
{
	fstream::close();
}

//=============================================================================
// Returns true if we have reached the end
//=============================================================================
bool StandardFile::eof()const
{
	return fstream::eof();
}

//=============================================================================
// Reads 'size' bytes
//=============================================================================
unsigned StandardFile::read(unsigned char *data, unsigned size)
{
	streamoff pos = tellg();
	fstream::read(reinterpret_cast<char*>(data), size);
	return static_cast<unsigned>(tellg() - pos);
}

//=============================================================================
// Flush output buffer
//=============================================================================
void StandardFile::flush()
{
	fstream::flush();
}

//=============================================================================
// Writes 'size' bytes
//=============================================================================
void StandardFile::write(const unsigned char *data, unsigned size)
{
	fstream::write(reinterpret_cast<const char*>(data), size);
}
