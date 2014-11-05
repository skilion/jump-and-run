#include "Log.h"
#include "Config.h"

#include "File/File.h"

#include "memdbg.h"

using namespace myengine;


Config myengine::config;


//=============================================================================
// Constructor
//=============================================================================
Config::Config(): elements(16)
{
	count = 0;
}

//=============================================================================
// Destructor
//=============================================================================
//Config::~Config() {}

//=============================================================================
// Returns the reference to the value of the specified variable
//=============================================================================
String &Config::get(const String &name)
{
	Element *p = find(name);
	if(p) return p->value;
	
	//Create a new element
	elements.ensureSize(++count);
	elements[count - 1].name = name;
	return elements[count - 1].value;
}

//=============================================================================
// Finds a variable
//=============================================================================
Config::Element *Config::find(const String &name)
{
	Element *p = 0;
	
	for(unsigned i = 0; i < count; i++) {
		if(elements[i].name == name) {
			p = &elements[i];
			break;
		}
	}
	
	return p;
}

//=============================================================================
// Reads the configuration from a file
//=============================================================================
bool Config::readFromFile(const char *filename)
{
	File *file = File::createFile();
	file->open(filename, File::OPENMODE_INPUT);
	if(!file->isOpen()) {
		log->print("Can't open the configuration file for reading");
		delete file;
		return false;
	}
	
	String line;
	unsigned lineNo = 0;
	Array<String> tokens;

	while(!file->eof()) {
		lineNo++;

		file->readLine(line);
		tokens = line.split("=", 2);
		tokens[0].trim();
		tokens[1].trim();

		if(tokens[0].isEmpty() && tokens[1].isEmpty()) continue;
		if(tokens[0].isEmpty() || tokens[1].isEmpty()) {
			log->printf("Error in the configuration file %s at line: %i", filename, lineNo);
			continue;
		}
		get(tokens[0]) = tokens[1];
	}

	file->close();
	delete file;
	
	return true;
}

//=============================================================================
// Saves the configuration into a file
//=============================================================================
bool Config::saveToFile(const char *filename)
{
	File *file = File::createFile();
	file->open(filename, File::OPENMODE_OUTPUT);
	if(!file->isOpen()) {
		log->print("Can't open the configuration file for writing");
		delete file;
		return false;
	}
	
	for(unsigned i = 0; i < count; i++) {
		if(!elements[i].value.isEmpty()) {
			file->writeString(elements[i].name);
			file->writeString(" = ");
			file->writeLine(elements[i].value);
		}
	}

	file->close();
	delete file;
		
	return true;
}