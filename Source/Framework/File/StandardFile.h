#pragma once
#ifndef __STANDARD_FILE_H__
#define __STANDARD_FILE_H__


#include <fstream>
#undef FLT_EPSILON //Conflict in Math.h

#include "File.h"

namespace myengine {

	class StandardFile: public File, std::fstream
	{
	public:
			//StandardFile();
			//~StandardFile();

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
	};

}


#endif //!__STANDARD_FILE_H__