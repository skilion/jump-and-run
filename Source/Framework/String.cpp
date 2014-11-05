#include "Memory.h"
#include "myString.h"
#include "Platform.h"

#include "Math/Math.h"


using namespace myengine;


//=========================================================================
// Constructor
//=========================================================================
String::String()
{
	lenght = 0;
}

String::String(const String &string)
{
	lenght = 0;
	set(string);
}

String::String(const char *string)
{
	lenght = 0;
	set(string);
}

String::String(const bool b)
{
	lenght = 0;
	set(b);
}

String::String(const char c)
{
	lenght = 0;
	set(c);
}

String::String(const int i)
{
	lenght = 0;
	set(i);
}

String::String(const float f)
{
	lenght = 0;
	set(f);
}

//=========================================================================
// Destructor
//=========================================================================
//String::~String() {}

//=========================================================================
// Overload of coversion operator const char*
//=========================================================================
/*String::operator const char*()
{
	return getString();
}*/

//=========================================================================
// Overload of operator=
//=========================================================================
String &String::operator=(const String &a)
{
	set(a);
	return *this;
}

String &String::operator=(const char *a)
{
	set(a);
	return *this;
}

String &String::operator=(const bool a)
{
	set(a);
	return *this;
}

String &String::operator=(const char a)
{
	set(a);
	return *this;
}

String &String::operator=(const int a)
{
	set(a);
	return *this;
}

String &String::operator=(const float a)
{
	set(a);
	return *this;
}

//=========================================================================
// Overload of operator+=
//=========================================================================
String &String::operator+=(const String &a)
{
	append(a);
	return *this;
}

String &String::operator+=(const char *a)
{
	append(a);
	return *this;
}

String &String::operator+=(const bool a)
{
	append(a);
	return *this;
}

String &String::operator+=(const char a)
{
	append(a);
	return *this;
}

String &String::operator+=(const int a)
{
	append(a);
	return *this;
}

String &String::operator+=(const float a)
{
	append(a);
	return *this;
}

//=========================================================================
// Overload of operator+
//=========================================================================
const String String::operator+(const String &a) const
{
	return String(*this) += a;
}

const String String::operator+(const char *a) const
{
	return String(*this) += a;
}

const String String::operator+(const bool a) const
{
	return String(*this) += a;
}

const String String::operator+(const char a) const
{
	return String(*this) += a;
}

const String String::operator+(const int a) const
{
	return String(*this) += a;
}

const String String::operator+(const float a) const
{
	return String(*this) += a;
}

//=========================================================================
// Overload of operator==
//=========================================================================
bool String::operator==(const String &a) const
{
	return strcmp(data, a.data) == 0;
}

bool String::operator==(const char *a) const
{
	return strcmp(data, a) == 0;
}

//=========================================================================
// Overload of operator!=
//=========================================================================
bool String::operator!=(const String &a) const
{
	return strcmp(data, a.data) != 0;
}

bool String::operator!=(const char *a) const
{
	return strcmp(data, a) != 0;
}

//=========================================================================
// Overload of operator<
//=========================================================================
bool String::operator<(const String &a) const
{
	if(isEmpty() || a.isEmpty()) return false;
	return (strcmp(data, a.data) < 0) ? true : false;
}

bool String::operator<(const char *a) const
{
	if(isEmpty() || !a) return false;
	return (strcmp(data, a) < 0) ? true : false;
}

//=========================================================================
// Overload of operator>
//=========================================================================
bool String::operator>(const String &a) const
{
	return (strcmp(data, a.data) > 0) ? true : false;
}

bool String::operator>(const char *a) const
{
	return (strcmp(data, a) > 0) ? true : false;
}

//=========================================================================
// Overload of operator[]
//=========================================================================
/*char &String::operator[](unsigned index)
{
	return data[index];
}

char String::operator[](unsigned index) const
{
	return data[index];
}*/

//=============================================================================
// Appends the specified value to the end of the string
//=============================================================================
void String::append(const String &string)
{
	if(!string.isEmpty()) {
		unsigned oldLenght = lenght;
		lenght += string.lenght;
		ensureSize(lenght + 1, true);
		Memory::memcpy(data + oldLenght, string.data, string.lenght);
	}
}

void String::append(const char *string)
{
	if(string) {
		unsigned lenght1 = lenght;
		unsigned lenght2 = strlen(string);
		lenght += lenght2;
		ensureSize(lenght + 1, true);
		Memory::memcpy(data + lenght1, string, lenght2 + 1);
	}
}

void String::append(const bool b)
{
	return b ? append("true") : append("false");
}

void String::append(const char c)
{
	lenght += 1;
	ensureSize(lenght + 1, true);
	data[lenght - 1] = c;
	data[lenght] = 0;
}

void String::append(const int i)
{
	char string[24];
	integerToAscii(i, string);
	append(string);
}

void String::append(const float f)
{
	char string[32];
	floatToAscii(f, string);
	append(string);
}

//=============================================================================
// Returns the character at the specified index
//=============================================================================
/*char String::get(unsigned index) const
{
	return data[index];
}*/

//=============================================================================
// Returns a pointer to the string
//=============================================================================
const char *String::getString() const
{
	return data;
}

//=============================================================================
// Clears the string without freeing the memory
//=============================================================================
void String::clear()
{
	lenght = 0;
	if(data) {
		*data = 0;
	}
}

//=============================================================================
// Compares two string
//=============================================================================
int String::compare(const String &s) const
{
	return strcmp(data, s.data);
}

int String::compare(const char *string) const
{
	return strcmp(data, string);
}

//=============================================================================
// Frees the memory occupied by the string
//=============================================================================
void String::free()
{
	lenght = 0;
	Array<char>::free();
}

//=============================================================================
// Returns the lenght of the string
//=============================================================================
unsigned String::getLenght() const
{
	return lenght;
}

//=============================================================================
// Returns the hash value of the string
//=============================================================================
unsigned String::hash() const
{
	if(data) {
		return hash(data);
	}
	return 0;
}

//=============================================================================
// Inserts the specified string at the specified position
//=============================================================================
void String::insert(unsigned pos, const String &string)
{
	if(!string.isEmpty() && (pos <= lenght)) {
		char *newData = new char[lenght + string.lenght];

		Memory::memcpy(newData, data, pos);
		Memory::memcpy(newData + pos, string.data, string.lenght);
		Memory::memcpy(newData + pos + string.lenght, data + pos, lenght - pos);
		set(newData);

		delete [] newData;
	}
}

void String::insert(unsigned pos, const char *string)
{
	if(string) {
		int lenght2 = strlen(string);
		char *newData = new char[lenght + lenght2 + 1];

		Memory::memcpy(newData, data, pos);
		Memory::memcpy(newData + pos, string, lenght2);
		Memory::memcpy(newData + pos + lenght2, data + pos, lenght - pos + 1);
		set(newData);

		delete [] newData;
	}
}

void String::insert(unsigned pos, char c) //TODO: test !
{
	if(c != 0) {
		char *newData = new char[lenght + 2];

		Memory::memcpy(newData, data, pos);
		newData[pos] = c;
		Memory::memcpy(newData + pos + 1, data + pos, lenght - pos + 1);
		set(newData);

		delete [] newData;
	}
}

//=============================================================================
// Returns true if the string is empty
//=============================================================================
bool String::isEmpty() const
{
	return lenght == 0;
}

//=========================================================================
// Sets the value of the string
//=========================================================================
void String::set(const String &a)
{
	if(a.isEmpty()) {
		clear();
	} else {
		lenght = a.lenght;
		ensureSize(lenght + 1, false);
		strcpy(data, a.data);
	}
}

void String::set(const char *a)
{
	if(!a) {
		clear();
	} else {
		lenght = strlen(a);
		ensureSize(lenght + 1, false);
		strcpy(data, a);
	}
}

void String::set(const bool a)
{
	return a ? set("true") : set("false");
}

void String::set(const char a)
{
	ensureSize(2, false);
	data[0] = a;
	data[1] = 0;
}

void String::set(const int a)
{
	char string[24];
	integerToAscii(a, string);
	set(string);
}

void String::set(const float a)
{
	char string[32];
	floatToAscii(a, string);
	set(string);
}

//=============================================================================
// Reduces the lenght of the string
//=============================================================================
void String::shrink(unsigned newLenght)
{
	if(newLenght == 0) clear();
	else if(newLenght < lenght) {
		lenght = newLenght;
		data[lenght] = 0;
	}
}

void String::shrink(unsigned pos1, unsigned pos2)
{
	//todo
	/*if((pos1 <= pos2) && (pos2 < lenght)) {
		lenght = pos2 - pos1;
		Memory::memcpy(data, data + pos1, lenght);
		lenght++;

		data[lenght] = 0;
	}*/
}

//=============================================================================
// Splits the string into tokens
//=============================================================================
Array<String> String::split(const char *delimiters, unsigned expectedToken) const
{
	Array<String> tokens(expectedToken);
	if(isEmpty()) return tokens;

	unsigned	tokensNo = 0;		//Number of tokens found
	unsigned	start;				//Start of the token
	bool		beginning = true;	//True if finding a new token
	bool		isDelimiter;		//True if the current character is a delimiter

	for(unsigned i = 0; i <= lenght; i++)
	{
		//Check if it is a delimiter (NULL included)
		isDelimiter = false;
		register const char *cp = delimiters;
		do {
			if(*cp == data[i]) {
				isDelimiter = true;
				break;
			}
		} while(*cp++);

		if(beginning)	//Finding the start of the token
		{
			if(!isDelimiter) {
				start = i;
				beginning = false;
			}
		}
		else			//Finding the end of the token
		{
			if(isDelimiter) {
				beginning = true;
				tokens[tokensNo++] = substr(start, i - 1);
			}
		}
	}

	return tokens;
}

//=============================================================================
// Strips the specified char from the front and the end of the string
//=============================================================================
void String::strip(const char c)
{
	stripLeading(c);
	stripTrailing(c);
}

String String::strip(const char c) const
{
	String tmp(*this);
	tmp.stripLeading(c);
	tmp.stripTrailing(c);
	return tmp;
}

//=============================================================================
// Strip a char from front as many times as the char occurs
//=============================================================================
void String::stripLeading(const char c) {
	if(!isEmpty() && c != 0) {
		const char *cp = data;
		while(*cp == c) cp++;

		if(cp != data) {
			lenght -= cp - data;
			Memory::memcpy(data, cp, lenght);
			data[lenght] = 0;
		}
	}
}

String String::stripLeading(const char c) const
{
	String tmp(*this);
	tmp.stripLeading(c);
	return tmp;
}

//=============================================================================
// Strip a char from end as many times as the char occurs
//=============================================================================
void String::stripTrailing(const char c) {
	if(!isEmpty() && c != 0) {
		const char *cp = data + lenght - 1;
		while(*cp == c) cp--;

		if(cp < data) lenght = 0;
		else lenght = cp - data + 1;

		data[lenght] = 0;
	}
}

String String::stripTrailing(const char c) const
{
	String tmp(*this);
	tmp.stripTrailing(c);
	return tmp;
}

//=============================================================================
// Strip a char from front as many times as the char occurs
//=============================================================================
String String::substr(unsigned start, unsigned end) const
{
	ASSERT(start <= end);
	ASSERT(end < lenght);

	String substr;
	substr.lenght = end - start + 1;
	substr.ensureSize(substr.lenght + 1);

	Memory::memcpy(substr.data, data + start, substr.lenght);
	substr.data[substr.lenght] = 0;

	return substr;
}

//=============================================================================
// Converts the string to lowercase
//=============================================================================
void String::toLower()
{
	if(!isEmpty()) {
		strlwr(data);
	}
}

String String::toLower() const
{
	String tmp(*this);
	tmp.toLower();
	return tmp;
}

//=============================================================================
// Converts the string to uppercase
//=============================================================================
void String::toUpper()
{
	if(!isEmpty()) {
		strupp(data);
	}
}

String String::toUpper() const
{
	String tmp(*this);
	tmp.toUpper();
	return tmp;
}

//=============================================================================
// Replace all non-printable characters with a space
//=============================================================================
void String::ensurePrintable()
{
	register char *cp = data;
	while(*cp) {
		if(*cp < ' ' || *cp > '~') *cp = ' ';
		cp++;
	}
}


//=============================================================================
//
//		Common string functions
//
//=============================================================================


//=============================================================================
// Return the integer value of the given string
//=============================================================================
int String::atoi(const char *string)
{
	if(!string) return 0;

	int value;
	strval(string, value);
	return value;
}

//=============================================================================
// Return the float value of the given string
//=============================================================================
float String::atof(const char *string)
{
	if(!string) return 0;

	int integerValue;
	string = strval(string, integerValue);

	bool negative = false;
	if(integerValue < 0) {
		negative = true;
		integerValue = -integerValue;
	}

	float value = static_cast<float>(integerValue);

	if(*string == '.') {
		const char *afterDecimal = ++string;
		int decimalValue;
		afterDecimal = strval(string, decimalValue);
		value += static_cast<float>(decimalValue) / pow(10, afterDecimal - string);
		string = afterDecimal;
	}

	if(*string == 'e' || *string == 'E') {
		string++;
		value *= pow(10.0f, atoi(string));
	}

	return negative ? (-value) : value;
}

//=============================================================================
// Appends a copy of the source string to the destination string
//=============================================================================
char *String::strcat(char *destination, const char *source)
{
	if(!(destination && source)) return 0;

#if defined(ASM_VCC)
	__asm {
		mov		eax, destination
		mov		edi, eax
		mov		esi, source

loop1:	cmp		byte ptr [edi], 0
		je		end1
		add		edi ,1
		jmp		loop1
end1:

loop2:	mov		al, byte ptr[esi]
		mov		byte ptr[edi], al
		cmp		al, 0
		je		end2
		add		esi, 1
		add		edi, 1
		jmp		loop2
end2:
	}
#elif defined(ASM_GCC)
asm("	mov		%[destination], %%eax\n"
"		mov		%%eax, %%edi\n"
"		mov		%[source], %%esi\n"

"1:		test	%%edi, %%edi\n"
"		jz		2f\n"
"		add		1, %%edi\n"
"		jmp		1b\n"
"2:\n"

"3:		movb	(%%esi), %%al\n"
"		movb	%%al, (%%edi)\n"
"		cmp		$0, %%al\n"
"		je		4f\n"
"		add		1, %%esi\n"
"		add		1, %%edi\n"
"		jmp		3b\n"
"4:\n"
:
: [destination] "m" (destination), [source] "m" (source));
#else
	register char *copy1 = destination;
	while(*copy1) copy1++;
	register const char *copy2 = source;
	while(*copy1++ = *copy2++);
	return destination;
#endif
}

//=============================================================================
// Returns a value indicating the relationship between the strings:
// A zero value indicates that both strings are equal.
// A return value greater than zero indicates that the first
// character that does not match has a greater value in string1 than
// in string2, a value less than zero indicates the opposite.
//=============================================================================
int String::strcmp(const char *string1, const char *string2)
{
	if(!(string1 && string2)) return 0;
	if(string1 == string2) return 0;

#if defined(ASM_VCC)
	__asm {
		mov		edi, string1
		mov		esi, string2

loop1:	mov		al, byte ptr [edi]
		mov		ah, byte ptr [esi]
		cmp		al, 0
		je		end
		cmp		ah, 0
		je		end
		cmp		al, ah
		jne		end
		add		edi, 1
		add		esi, 1
		jmp		loop1
end:

		sub		al, ah
		movsx	eax, al
	}
#elif defined(ASM_GCC)
asm("	mov		%[string1], %%edi\n"
"		mov		%[string2], %%esi\n"

"1:		movb	(%%edi), %%al\n"
"		mov		(%%esi), %%ah\n"
"		cmp		$0, %%al\n"
"		je		2f\n"
"		cmp		$0, %%ah\n"
"		je		2f\n"
"		cmp		%%ah, %%al\n"
"		jne		2f\n"
"		add		$1, %%edi\n"
"		add		$1, %%esi\n"
"		jmp		1b\n"

"2:		sub		%%ah, %%al\n"
"		movsx	%%al, %%eax\n"
:
: [string1] "m" (string1), [string2] "m" (string2));
#else
	const register char *copy1 = string1;
	const register char *copy2 = string2;
	while(*copy1 && *copy2 && (*copy1 == *copy2)) {
		copy1++;
		copy2++;
	}
	return *copy1 - *copy2;
#endif
}

//=============================================================================
// Like strcmp but case insensitive
//=============================================================================
int String::stricmp(const char *string1, const char *string2)
{
	if(!(string1 && string2) || string1 == string2) return 0;

	while(*string1 && *string2 && ((*string1 == *string2) || (toLower(*string1) == toLower(*string2)))) {
		string1++;
		string2++;
	}

	if(isAlpha(*string1) && isAlpha(*string2))
		return toLower(*string1) - toLower(*string2);

	return *string1 - *string2;
}

//=============================================================================
// Copies a string from source to destination
//=============================================================================
char *String::strcpy(char *destination, const char *source)
{
	if(!(destination && source)) return 0;
	if(destination == source) return destination;

#if defined(ASM_VCC)
	__asm {
		mov		eax, destination
		mov		edi, eax
		mov		esi, source

loop1:	mov		al, byte ptr[esi]
		mov		byte ptr[edi], al
		cmp		al, 0
		je		end
		add		esi, 1
		add		edi, 1
		jmp		loop1
end:
	}
#elif defined(ASM_GCC)
asm("	mov		%[destination], %%eax\n"
"		mov		%%eax, %%edi\n"
"		mov		%[source], %%esi\n"

"1:		movb	(%%esi), %%al\n"
"		movb	%%al, (%%edi)\n"
"		cmp		$0, %%al\n"
"		je		2f\n"
"		add		$1, %%esi\n"
"		add		$1, %%edi\n"
"		jmp		1b\n"
"2:\n"
:
: [destination] "m" (destination), [source] "m" (source));
#else
	register char *copy1 = destination;
	const register char *copy2 = source;
	while(*copy1++ = *copy2++);
	return destination;
#endif
}

//=============================================================================
// Same as strcpy but allocates itself the space for destination
//=============================================================================
char *String::strcpyAlloc(char **destination, const char *source)
{
	if(!source) {
		*destination = 0;
		return 0;
	}

	*destination = new char[strlen(source) + 1];
	return strcpy(*destination, source);
}

//=============================================================================
// Returns the lenght of the string
//=============================================================================
unsigned String::strlen(const char *string)
{
	if(!string) return 0;

#if defined(ASM_VCC)
	__asm {
		mov		eax, string
		mov		edx, 01010101h
		mov		edi, 80808080h

loop1:	mov		ebx, dword ptr [eax]	//Find the zero byte
		add		eax, 4

		mov		ecx, ebx
		not		ecx

		sub		ebx, edx
		and		ebx, ecx

		test	ebx, edi
		jz		loop1

		test	ebx, 00008080h
		jnz		x1
		shr		ebx, 16
		sub		eax, 1
		jmp		x2

x1:		sub		eax, 3
x2:		shl		bl, 1
		sbb		eax, string
	}
#elif defined(ASM_GCC)
asm("	mov		%[string], %%eax\n"
"		mov		$0x01010101, %%edx\n"
"		mov		$0x80808080, %%edi\n"

"1:		movl	(%%eax), %%ebx\n"
"		add		$4, %%eax\n"

"		mov		%%ebx, %%ecx\n"
"		not		%%ecx\n"

"		sub		%%edx, %%ebx\n"
"		and		%%ecx, %%ebx\n"

"		test	%%edi, %%ebx\n"
"		jz		1b\n"

"		test	$0x00008080, %%ebx\n"
"		jnz		2f\n"
"		shr		$16, %%ebx\n"
"		sub		$1, %%eax\n"
"		jmp		3f\n"

"2:		sub		$3, %%eax\n"
"3:		shl		$1, %%bl\n"
"		sbb		%[string], %%eax\n"
:
: [string] "m" (string));
#else
	register const char *copy = string;
	while(*copy) copy++;
	return copy - string;
#endif
}

//=============================================================================
// Converts a string to its lowercase equivalent
//=============================================================================
char *String::strlwr(char *string)
{
	if(string) {
		register char *copy = string;
		while(*copy) {
			if(isUpper(*copy))
				*copy += 'a' - 'A';
			copy++;
		}
	}
	return string;
}

//=============================================================================
// Same as String::compare but it stops after count characters are compared
//=============================================================================
int	String::strncmp(const char *string1, const char *string2, unsigned count)
{
	if(!(string1 && string2)) return 0;
	if(string1 == string2) return 0;

#if defined(ASM_VCC)
	__asm {
		mov		edi, string1
		mov		esi, string2
		mov		ecx, count

		xor		eax, eax
		test	ecx, ecx
		jz		end2
		dec		ecx

loop1:	mov		al, byte ptr [edi]
		mov		ah, byte ptr [esi]
		cmp		al, 0
		je		end1
		cmp		ah, 0
		je		end1
		cmp		al, ah
		jne		end1
		test	ecx, ecx
		jz		end1
		add		edi, 1
		add		esi, 1
		dec		ecx
		jmp		loop1

end1:	sub		al, ah
		movsx	eax, al
end2:
	}
#elif defined(ASM_GCC)
asm("	mov		%[string1], %%edi\n"
"		mov		%[string2], %%esi\n"
"		mov		%[count], %%ecx\n"

"		xor		%%eax, %%eax\n"
"		test	%%ecx,%%ecx\n"
"		jz		3f\n"
"		dec		%%ecx\n"

"1:		movb	(%%edi), %%al\n"
"		movb	(%%esi), %%ah\n"
"		cmp		$0, %%al\n"
"		je		2f\n"
"		cmp		$0, %%ah\n"
"		je		2f\n"
"		cmp		%%ah, %%al\n"
"		jne		2f\n"
"		test	%%ecx, %%ecx\n"
"		jz		2f\n"
"		add		$1, %%edi\n"
"		add		$1, %%esi\n"
"		dec		%%ecx\n"
"		jmp		1b\n"

"2:		sub		%%ah, %%al\n"
"		movsx	%%al, %%eax\n"
"3:\n"
:
: [string1] "m" (string1), [string2] "m" (string2), [count] "m" (count));
#else
	register const char *copy1 = string1;
	register const char *copy2 = string2;
	register unsigned copy3 = count;
	if(copy3) {
		while(*copy1 && *copy2 && (*copy1 == *copy2) && --copy3) {
			copy1++;
			copy2++;
		}
		return *copy1 - *copy2;
	}
	return 0;
#endif
}

//=============================================================================
// Returns a pointer to the first occurrence of string2 in string1
//=============================================================================
const char *String::strstr(const char *string1, const char *string2)
{
	if(!(string1 && string2)) return 0;
#if defined(ASM_VCC)
	__asm {
		xor		eax, eax
		mov		ecx, string1
		mov		edx, string2

loop1:	cmp		byte ptr [ecx], 0
		je		end1

		mov		edi, ecx
		mov		esi, edx

loop2:	mov		bl, byte ptr [esi]
		cmp		bl, byte ptr [edi]
		jne		end2

		add		edi, 1
		add		esi, 1

		cmp		byte ptr [esi], 0 //End of string 2
		jne		no
		mov		eax, ecx
		jmp		end1
no:

		jmp		loop2
end2:

		add		ecx, 1
		jmp		loop1
end1:
	}
#elif defined(ASM_GCC)
asm("	xor		%%eax, %%eax\n"
"		mov		%[string1], %%ecx\n"
"		mov		%[string2], %%edx\n"

"1:		cmpb	$0, (%%ecx)\n"
"		je		5f\n"

"		mov		%%ecx, %%edi\n"
"		mov		%%edx, %%esi\n"

"2:		mov		(%%esi), %%bl\n"
"		cmp		(%%edi), %%bl\n"
"		jne		4f\n"

"		add		$1, %%edi\n"
"		add		$1, %%esi\n"

"		cmp		$0, (%%esi)\n"
"		jne		3f\n"
"		mov		%%ecx, %%eax\n"
"		jmp		5f\n"
"3:\n"

"		jmp		2b\n"
"4:\n"

"		add		$1, %%ecx\n"
"		jmp		1b\n"
"5:\n"
:
: [string1] "m" (string1), [string2] "m" (string2));
#else
	while(*string1) {
		register const char *cp1 = string1, *cp2 = string2;
		while(*cp1++ == *cp2++)
			if(!*cp2) return string1;
		string1++;
	}
	return 0;
#endif
}

//=============================================================================
// Split string into tokens
//=============================================================================
char *String::strtok(char *string, const char *delimiters)
{
	static char *last = 0;
	register char *start = string ? string : last;

	if(!start) return 0;

	//Skip initial delimiters
	while(*start) {
		register char *cp = const_cast<char *>(delimiters);
		while(*cp) {
			if(*cp++ == *start) break;
		}
		if(!*cp) break;
		start++;
	}

	register char *end = start;
	while(*end) {
		register char *cp = const_cast<char *>(delimiters);
		while(*cp) {
			if(*cp == *end) break;
			cp++;
		}
		if(*cp) break;
		end++;
	}

	if(*end) last = end + 1;
	else last = 0;
	*end = 0;

	return start;
}

//=============================================================================
// Converts a string to its uppercase equivalent
//=============================================================================
char *String::strupp(char *string)
{
	if(string) {
		register char *copy = string;
		while(*copy) {
			if(isLower(*copy))
				*copy -= 'a' - 'A';
			copy++;
		}
	}
	return string;
}

//=============================================================================
// Compute the value of the given string and returns the position after the number
//=============================================================================
const char *String::strval(const char *string, int &value)
{
	if(!string) return 0;

	bool negative = false;
	switch(*string)
	{
	case '-':
		negative = true;
	case '+':
		string++;
	}

	value = 0;
	while(*string) {
		if(isNum(*string)) {
				value *= 10;
				value += *string - '0';
		}
		else break;

		string++;
	}

	if(negative) value = -value;

	return string;
}

//=============================================================================
// Converts an integer value into an ASCII string
//=============================================================================
char *String::integerToAscii(int value, char *string)
{
	register char *cp = string;
	if(value < 0) {
		*cp++ = '-';
		value = -value;
	}

	if(value == 0) *cp++ = '0';
	else {
		register bool skipZero = true;
		for(int i = 1000000000; i > 0; i /= 10) {
			register char n = value / i;
			if(!skipZero || n > 0) {
				skipZero = false;
				value -= n * i;
				*cp++ = '0' + n;
			}
		}
	}
	*cp = 0;
	return string;
}

//=============================================================================
// Converts a float value into an ASCII string
//=============================================================================
char *String::floatToAscii(float value, char *string, unsigned precision)
{
	integerToAscii(static_cast<int>(value), string);
	if(precision == 0) return string;

	char *cp = string;
	cp += strlen(cp);
	*cp++ = '.';

	value = abs<float>(value);

	while(precision--) {
		value -= static_cast<int>(value);
		value *= 10;
		*cp++ = '0' + static_cast<char>(value);
	}

	*cp = 0;
	return string;
}

//=============================================================================
// Simple hash function
//=============================================================================
unsigned String::hash(const char *string)
{
	unsigned hash = 0;
	const char *cp = string;
	for(int i = 0; *cp; cp++, i++) {
		hash += (*cp - 32) * i + 32;
	}

	return hash;
}

//=============================================================================
// Counts all the occurrences of the specified char
//=============================================================================
unsigned String::countChar(const char *string, char c)
{
	if(!string) return 0;

	unsigned count = 0;
	while(*string) {
		if(*string == c) count++;
	}

	return count;
}
