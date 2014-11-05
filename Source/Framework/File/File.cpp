#include "File.h"
#include "Platform.h"

using namespace myengine;


//=============================================================================
// Constructor
//=============================================================================
//File::File() {}

//=============================================================================
// Destructor
//=============================================================================
File::~File() {}

//=============================================================================
// Reads a string
//=============================================================================
void File::readString(String &string)
{
	string.ensureSize(32, false);
	string.clear();
	char c = 0;
	while(!eof()) {
		read(reinterpret_cast<unsigned char*>(&c), sizeof(char));
		if(c == 0) break;
		string.append(c);
	}
}

//=============================================================================
// Reads a line
//=============================================================================
void File::readLine(String &string)
{
	string.ensureSize(32, false);
	string.clear();
	char c = 0;
	bool endOfLine = false;
	while(!eof()) {
		read(reinterpret_cast<unsigned char*>(&c), sizeof(char));
		if(!c) break;

		//Stop at "\n" or "\r\n"
		if(!endOfLine) {
			if(c =='\n') break;
			else if(c =='\r') endOfLine = true;
			else string.append(c);
		} else {
			if(c =='\n') break;
			else {
				endOfLine = false;
				string.append('\r');
				string.append(c);
			}
		}
	}
}

//=============================================================================
// Writes a string with the null character
//=============================================================================
inline void File::writeStringWithNull(const char *string)
{
	if(string) {
		write(reinterpret_cast<const unsigned char*>(string), String::strlen(string) + 1);
	} else writeChar(0);
}

//=============================================================================
// Writes a line
//=============================================================================
void File::writeLine(const char *string)
{
	writeString(string);
	write(reinterpret_cast<const unsigned char*>(NEWLINE), NEWLINE_LEN);
}

//=============================================================================
// Writes a string
//=============================================================================
void File::writeString(const char *string)
{
	if(string) {
		write(reinterpret_cast<const unsigned char*>(string), String::strlen(string));
	}
}

//=============================================================================
// Gets the size of the specified file
//=============================================================================
bool File::getFileSize(const char *filename, unsigned &size)
{
	File *file = createFile();
	if(!file->open(filename, OPENMODE_INPUT)) return false;

	size = file->getSize();
	file->close();
	delete file;

	return true;
}

//=============================================================================
// Reads the content of the specified file
//=============================================================================
bool File::readFile(const char *filename, unsigned char **data, unsigned &size)
{
	File *file = createFile();
	if(!file->open(filename, OPENMODE_INPUT)) return false;

	size = file->getSize();
	if(size == 0) {
		*data = 0;
	} else {
		*data = new unsigned char[size];
		file->read(*data, size);
	}

	file->close();
	delete file;

	return true;
}
