#pragma once
#ifndef __WINDOWS_FILE_H__
#define __WINDOWS_FILE_H__


#define NOCOMM
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef min
#undef max

#include "File.h"


namespace myengine {

	class WindowsFile: public File
	{
	public:
			WindowsFile();
			//~WindowsFile();

		//Common functions
		virtual bool		open(const char *filename, OpenMode openMode);
		virtual void		close();
		virtual bool		isOpen() const;
		virtual unsigned	getSize() const;
		virtual unsigned	getPosition() const;
		virtual void		setPosition(unsigned position);
		
		//Read functions
		virtual bool		eof() const;
		virtual unsigned	read(unsigned char *data, unsigned size);

		//Write functions
		virtual void		flush();
		virtual void		write(const unsigned char *data, unsigned size);
		

	private:
		HANDLE		hFile;
		OVERLAPPED	overlapped;
		bool		endfile;
	};

}


#endif //!__WIN_FILE_H__