#pragma once

//nullptr definition
#if defined(_MSC_VER) && _MSC_VER < 1600 //Prior Visual Studio 2010
#define nullptr 0
#elif defined(__GNUG__)  
	#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
	//Prior GCC 4.6
	#if GCC_VERSION < 40600
	#define nullptr 0
	#endif
#endif


//Max lenght for static strings
const unsigned MAX_STRING = 1024 * 8;		//8 kBytes


//Inline assembly syntax
#if defined(WIN32) || defined(LINUX)	//Windows and Linux

	#if defined(_MSC_VER)		//Microsoft Visual C++ compiler
		#define ASM_VCC
	#elif defined(__GNUG__)		//GNU C++ compiler
		//#define ASM_GCC
	#endif

#endif


//Typedef for size-matter works
#if defined(_MSC_VER) || defined(__GNUG__)		//Microsoft Visual C++ and GNU C++ compiler
	typedef unsigned char		byte;
	typedef unsigned int		dword;

	//todo: unsigned&signed in image.h !
	typedef short					word;
	typedef unsigned short			uword;

	typedef unsigned int			udword;
	typedef long long int			qword;
	typedef unsigned long long int	uqword;

#else
	#error Provide a definition for primitive types on this compiler
#endif


//OS newline character
#if defined(WIN32)		//Windows
	const char * const	NEWLINE = "\r\n";
	const int			NEWLINE_LEN = 2;
#else
	const char * const	NEWLINE = "\n";
	const int			NEWLINE_LEN = 1;
#endif


//Breakpoint
#if defined(_MSC_VER)
	#define BREAKPOINT __asm { int 3 }
#elif defined(__GNUG__)
	#define BREAKPOINT asm("int3")
#else
	#error Provide a BREAKPOINT definition on this compiler
#endif


//ASSERT macro
#ifdef _DEBUG
	#define ASSERT(value) if((value) == 0) BREAKPOINT;
#else
	#define ASSERT(value)
#endif


/*
static bool isLittleEndian()
{
	word w = 1;
	return *reinterpret_cast<byte*>(&w) == 1;
}*/
/*
void convLittleEndian(void *ptr, unsigned size)
{
	if(isLittleEndian()) return;
	for(register unsigned i = 0; i < size; i++)
	{
		register ubyte swap = static_cast<ubyte*>(ptr)[i];
		static_cast<ubyte*>(ptr)[i] = static_cast<ubyte*>(ptr)[size - i - 1];
		static_cast<ubyte*>(ptr)[size - i - 1] = swap;
	}
}
*/