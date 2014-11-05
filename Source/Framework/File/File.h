#pragma once
#ifndef __FILE_H__
#define __FILE_H__


#include "myString.h"


namespace myengine {

	class Aabox;
		
	class File
	{
	public:
		enum OpenMode {
			OPENMODE_INPUT,
			OPENMODE_OUTPUT,
			OPENMODE_APPEND
		};
		
			//File();
			virtual ~File();

		static File			*createFile();

		//Common functions
		virtual bool		open(const char *filename, OpenMode openMode) = 0;
		virtual void		close() = 0;
		virtual bool		isOpen() const = 0;
		virtual unsigned	getSize() const = 0;
		virtual unsigned	getPosition() const = 0;
		virtual void		setPosition(unsigned position) = 0;
		
		//Read functions
		virtual bool		eof() const = 0;
		virtual unsigned	read(unsigned char *data, unsigned size) = 0;
		void				readChar(char &data);
		void				readUnsignedChar(unsigned char &data);
		void				readInt(int &data);
		void				readUnsignedInt(unsigned &data);
		void				readFloat(float &data);
		void				readString(String &string);
		void				readLine(String &string);

		//Write functions
		virtual void		flush() = 0;
		virtual void		write(const unsigned char *data, unsigned size) = 0;
		void				writeChar(char data);
		void				writeUnsignedChar(unsigned char data);
		void				writeInt(int data);
		void				writeUnsignedInt(unsigned data);
		void				writeFloat(float data);
		void				writeString(const char *string);
		void				writeStringWithNull(const char *string);
		void				writeLine(const char *string);

		//Static functions
		static bool			getFileSize(const char *filename, unsigned &size);
		static bool			readFile(const char *filename, unsigned char **data, unsigned &size);
	};
	
		
	//=========================================================================
	// Reads a char
	//=========================================================================
	inline void File::readChar(char &data)
	{
		read(reinterpret_cast<unsigned char*>(&data), sizeof(char));
	}

	//=========================================================================
	// Reads an unsigned char
	//=========================================================================
	inline void File::readUnsignedChar(unsigned char &data)
	{
		read(reinterpret_cast<unsigned char*>(&data), sizeof(unsigned char));
	}

	//=========================================================================
	// Reads an integer
	//=========================================================================
	inline void File::readInt(int &data)
	{
		read(reinterpret_cast<unsigned char*>(&data), sizeof(int));
	}

	//=========================================================================
	// Reads an unsigned integer
	//=========================================================================
	inline void File::readUnsignedInt(unsigned &data)
	{
		read(reinterpret_cast<unsigned char*>(&data), sizeof(unsigned));
	}

	//=========================================================================
	// Reads a float
	//=========================================================================
	inline void File::readFloat(float &data)
	{
		read(reinterpret_cast<unsigned char*>(&data), sizeof(float));
	}
	
	//=========================================================================
	// Writes an integer
	//=========================================================================
	inline void File::writeChar(char data)
	{
		write(reinterpret_cast<unsigned char*>(&data), sizeof(char));
	}

	//=========================================================================
	// Writes an unsigned integer
	//=========================================================================
	inline void File::writeUnsignedChar(unsigned char data)
	{
		write(reinterpret_cast<const unsigned char*>(&data), sizeof(unsigned char));
	}

	//=========================================================================
	// Writes an integer
	//=========================================================================
	inline void File::writeInt(int data)
	{
		write(reinterpret_cast<const unsigned char*>(&data), sizeof(int));
	}

	//=========================================================================
	// Writes an unsigned integer
	//=========================================================================
	inline void File::writeUnsignedInt(unsigned data)
	{
		write(reinterpret_cast<const unsigned char*>(&data), sizeof(unsigned));
	}

	//=========================================================================
	// Writes a float
	//=========================================================================
	inline void File::writeFloat(float data)
	{
		write(reinterpret_cast<const unsigned char*>(&data), sizeof(float));
	}

}


#endif //!__FILE_H__