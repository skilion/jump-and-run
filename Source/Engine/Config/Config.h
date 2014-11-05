#pragma once
#ifndef __CONFIG_H__
#define __CONFIG_H__


#include "myString.h"
#include "Containers/Array.h"


namespace myengine {

	class String;

	class Config
	{
	public:
			Config();
			//~Config();
		
		String		&get(const String &name);
		
		
		bool		readFromFile(const char *filename);
		bool		saveToFile(const char *filename);

		
	private:
		struct Element {
			String	name;
			String	value;
		};
		
		Element			*find(const String &name);

		unsigned		count;
		Array<Element>	elements;
	};

	extern Config config;

}


#endif //!__CONFIG_H__