#pragma once

#include "Containers/Array.h"


namespace myengine {
	
	class String: public Array<char>
	{
	public:
			String();
			String(const String &string);
			String(const char *string);
			String(const bool b);
			String(const char c);
			String(const int i);
			String(const float f);
			//~String();

		String			&operator=(const String &a);
		String			&operator=(const char *a);
		String			&operator=(const bool a);
		String			&operator=(const char a);
		String			&operator=(const int a);
		String			&operator=(const float a);

		String			&operator+=(const String &a);
		String			&operator+=(const char *a);
		String			&operator+=(const bool a);
		String			&operator+=(const char a);
		String			&operator+=(const int a);
		String			&operator+=(const float a);

		const String	operator+(const String &a) const;
		const String	operator+(const char *a) const;
		const String	operator+(const bool a) const;
		const String	operator+(const char a) const;
		const String	operator+(const int a) const;
		const String	operator+(const float a) const;

		bool			operator==(const String &a) const;
		bool			operator==(const char *a) const;

		bool			operator!=(const String &a) const;
		bool			operator!=(const char *a) const;

		bool			operator<(const String &a) const;
		bool			operator<(const char *a) const;
		bool			operator>(const String &a) const;
		bool			operator>(const char *a) const;
		
		void			append(const String &string);
		void			append(const char *string);
		void			append(const bool b);
		void			append(const char c);
		void			append(const int i);
		void			append(const float f);

		const char		*getString() const;

		void			clear();

		int				compare(const String &string) const;
		int				compare(const char *string) const;

		void			free();

		unsigned		getLenght() const;

		unsigned		hash() const;

		void			insert(unsigned pos, const String &string);
		void			insert(unsigned pos, const char *string);
		void			insert(unsigned pos, char c);

		bool			isEmpty() const;

		void			set(const String &string);
		void			set(const char *string);
		void			set(const bool b);
		void			set(const char c);
		void			set(const int i);
		void			set(const float f);

		void			shrink(unsigned newLenght);
		void			shrink(unsigned pos1, unsigned pos2);

		Array<String>	split(const char *delimiters, unsigned expectedToken = 4) const;

		void			strip(const char c);
		String			strip(const char c) const;
		void			stripLeading(const char c);
		String			stripLeading(const char c) const;
		void			stripTrailing(const char c);
		String			stripTrailing(const char c) const;

		String			substr(unsigned pos1, unsigned pos2) const;

		float			toFloat() const;
		int				toInt() const;

		void			toLower();
		String			toLower() const;
		void			toUpper();
		String			toUpper() const;

		void			trim();
		String			trim() const;

		void			ensurePrintable();

		/*bool endsWith(char *string);
		bool startsWith(char *string);*/

		//Common string functions
		static int			atoi(const char *string);
		static float		atof(const char *string);
		static bool			isAlpha(char c);
		static bool			isLower(char c);
		static bool			isNum(char c);
		static bool			isUpper(char c);
		static char			*strcat(char *destination, const char *source);
		static int			strcmp(const char *string1, const char *string2);
		static int			stricmp(const char *string1, const char *string2);
		static char			*strcpy(char *destination, const char *source);
		static char			*strcpyAlloc(char **destination, const char *source);
		static unsigned		strlen(const char *string);
		static char			*strlwr(char *string);
		static int			strncmp(const char *string1, const char *string2, unsigned count);
		static const char	*strstr(const char *string1, const char *string2);
		static char			*strtok(char *string, const char *delimiters);
		static char			*strupp(char *string);
		static const char	*strval(const char *string, int &value);
		static char			toLower(char c);
		static char			toUpper(char c);

		static char			*integerToAscii(int value, char *string);
		static char			*floatToAscii(float value, char *string, unsigned precision = 4);

		static unsigned		hash(const char *string);
		static unsigned		countChar(const char *string, char c);


	private:
		unsigned	lenght;		//lenght of the string
	};


	//=========================================================================
	// Returns the float value of the string
	//=========================================================================
	inline float String::toFloat() const
	{
		return atof(data);
	}

	//=========================================================================
	// Returns the integer value of the string
	//=========================================================================
	inline int String::toInt() const
	{
		return atoi(data);
	}

	//=========================================================================
	// Trim whitespace from the beginning and end of a string
	//=========================================================================
	inline void String::trim()
	{
		return strip(' ');
	}

	inline String String::trim() const
	{
		return strip(' ');
	}

	//=========================================================================
	// Returns true if the character is a number
	//=========================================================================
	inline bool String::isAlpha(char c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}

	//=========================================================================
	// Returns true if the character is lowercase
	//=========================================================================
	inline bool String::isLower(char c)
	{
		return c >= 'a' && c <= 'z';
	}

	//=========================================================================
	// Returns true if the character is a number
	//=========================================================================
	inline bool String::isNum(char c)
	{
		return c >= '0' && c <= '9';
	}

	//=========================================================================
	// Returns true if the character is uppercase
	//=========================================================================
	inline bool String::isUpper(char c)
	{
		return c >= 'A' && c <= 'Z';
	}
	
	//=========================================================================
	// Returns the character in lowercase
	//=========================================================================
	inline char String::toLower(char c)
	{
		return isUpper(c) ? (c + 'a' - 'A') : c;
	}
	
	//=========================================================================
	// Returns the character in uppercase
	//=========================================================================
	inline char String::toUpper(char c)
	{
		return isLower(c) ? (c - 'a' - 'A') : c;
	}

}
