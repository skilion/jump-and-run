#include "Memory.h"
#include "Platform.h"

using namespace myengine;


//=============================================================================
// Copies the values of num bytes from the location pointed by source directly
// to the memory block pointed by destination
//=============================================================================
void *Memory::memcpy(void *destination, const void *source, unsigned num)
{
	if(destination == source) return destination;
/*
#if defined(ASM_INTEL)
	__asm {
		mov		edi, destination
		mov		esi, source
		mov		ecx, num
		mov		edx, 4

loop1:	cmp		ecx, edx
		jb		end1
		mov		eax, dword ptr[esi]
		mov		dword ptr[edi], eax
		sub		ecx, 4
		add		edi, 4
		add		esi, 4
		jmp		loop1
end1:

loop2:	test	ecx, ecx
		jz		end2
		mov		al, byte ptr[esi]
		mov		byte ptr[edi], al
		sub		ecx, 1
		add		edi, 1
		add		esi, 1
		jmp		loop2
end2:

		mov		eax, destination
	}
#elif defined(ASM_GCC)
asm("	mov		%[destination], %%edi\n"
"		mov		%[source], %%esi\n"
"		mov		%[num], %%ecx\n"
"		mov		$4, %%edx\n"

"1:		cmp		%%edx, %%ecx\n"
"		jb		2f\n"
"		movl	(%%esi), %%eax\n"
"		movl	%%eax, (%%edi)\n"
"		sub		$4, %%ecx\n"
"		add		$4, %%edi\n"
"		add		$4, %%esi\n"
"		jmp		1b\n"
"2:\n"

"3:		test	%%ecx, %%ecx\n"
"		jz		4f\n"
"		movb	(%%esi), %%al\n"
"		movb	%%al, (%%edi)\n"
"		sub		$1, %%ecx\n"
"		add		$1, %%edi\n"
"		add		$1, %%esi\n"
"		jmp		3b\n"
"4:\n"

"		mov		%[destination], %%eax\n"
:
: [destination] "m" (destination), [source] "m" (source), [num] "m" (num));
#else*/
	const register byte *copy1 = static_cast<const byte*>(source);
	register byte *copy2 = static_cast<byte*>(destination);
	while(num--) {
		*copy2++ = *copy1++;
	}
	return destination;
//#endif
}

//=============================================================================
// Compares the first num bytes of the block of memory pointed by ptr1 to the
// first num bytes pointed by ptr2, returning zero if they all match or a value
// different from zero representing which is greater if they do not.
//=============================================================================
int Memory::memcmp(const void *ptr1, const void *ptr2, unsigned num)
{
	if((ptr1 == ptr2) || (num == 0)) return 0;

/*#if defined(ASM_VCC)
	__asm {
		mov		edi, ptr1
		mov		esi, ptr2
		mov		ecx, num
		mov		edx, 4

loop1:	cmp		ecx, edx
		jb		end1
		mov		eax, dword ptr[esi]
		test	dword ptr[edi], eax
		jnz		end1
		sub		ecx, 4
		add		edi, 4
		add		esi, 4
		jmp		loop1
end1:

		xor		eax, eax
loop2:	test	ecx, ecx
		jz		end2
		mov		al, byte ptr [edi]
		sub		al, byte ptr [esi]
		jne		end2
		add		edi, 1
		add		esi, 1
		sub		ecx, 1
		jmp		loop2

end2:	movsx	eax, al
	}
#elif defined(ASM_GCC)
asm("	mov		%[ptr1], %%edi\n"
"		mov		%[ptr2], %%esi\n"
"		mov		%[num], %%ecx\n"
"		mov		$4, %%edx\n"

"1:		cmp		%%edx, %%ecx\n"
"		jb		2f\n"
"		movl	(%%esi), %%eax\n"
"		test	%%eax, (%%edi)\n"
"		jnz		2f\n"
"		sub		$4, %%ecx\n"
"		add		$4, %%edi\n"
"		add		$4, %%esi\n"
"		jmp		1b\n"
"2:\n"

"		xor		%%eax, %%eax\n"
"3:		test	%%ecx, %%ecx\n"
"		jz		4f\n"
"		movb	(%%edi), %%al\n"
"		sub		(%%esi), %%al\n"
"		jne		4f\n"
"		add		$1, %%edi\n"
"		add		$1, %%esi\n"
"		sub		$1, %%ecx\n"
"		jmp		3b\n"

"4:		movsx	%%al, %%eax\n"
:
: [ptr1] "m" (ptr1), [ptr2] "m" (ptr2), [num] "m" (num));
#else*/
	const register byte *copy1 = static_cast<const byte*>(ptr1);
	const register byte *copy2 = static_cast<const byte*>(ptr2);
	while(--num) {
		if(*copy1 != *copy2) break;
		copy1++;
		copy2++;
	}
	return *copy1 - *copy2;
//#endif
}

//=============================================================================
// Sets the first num bytes of the block of memory pointed by ptr1 to the
// specified value (interpreted as an unsigned char)
//=============================================================================
void *Memory::memset(void *ptr1, int value, unsigned num)
{
#if defined(ASM_VCC)
	__asm {
		mov		al, byte ptr value		//byte -> dword
		mov		ah, al
		mov		bl, al
		bswap	eax
		mov		al, bl
		mov		ah, bl

		mov		edx, 4
		mov		ecx, num		//counter
		mov		edi, ptr1

loop1:	cmp		ecx, edx
		jb		end1
		mov		dword ptr [edi], eax
		add		edi, 4
		sub		ecx, 4
		jmp		loop1
end1:

loop2:	test	ecx, ecx
		jz		end2
		mov		byte ptr [edi], al
		add		edi, 1
		sub		ecx, 1
		jmp		loop2
end2:

		mov		eax, ptr1
	}
#elif defined(ASM_GCC)
asm("	movb	%[value], %%al\n"
"		mov		%%al, %%ah\n"
"		mov		%%al, %%bl\n"
"		bswap	%%eax\n"
"		mov		%%bl, %%al\n"
"		mov		%%bl, %%ah\n"

"		mov		$4, %%edx\n"
"		mov		%[num], %%ecx\n"
"		mov		%[ptr1], %%edi\n"

"1:		cmp		%%edx, %%ecx\n"
"		jb		2f\n"
"		mov		%%eax, (%%edi)\n"
"		add		$4, %%edi\n"
"		sub		$4, %%ecx\n"
"		jmp		1b\n"
"2:\n"

"3:		test	%%ecx, %%ecx\n"
"		jz		4f\n"
"		mov		%%al, (%%edi)\n"
"		add		$1, %%edi\n"
"		sub		$1, %%ecx\n"
"		jmp		3b\n"
"4:\n"

"		mov		%[ptr1], %%eax\n"
:
: [ptr1] "m" (ptr1), [value] "m" (value), [num] "m" (num));
#else
	register byte *copy1 = static_cast<byte*>(ptr1);
	register byte copy2 = static_cast<byte>(value);

	while(num--) {
		*copy1++ = copy2;
	}

	return ptr1;
#endif
}

//=============================================================================
// Memcpy for 4 bytes aligned memory
//=============================================================================
/*void *Memory::memcpy4B(void *destination, const void *source, unsigned num)
{
#if defined(ASM_VCC)
	__asm {
		mov		edi, destination
		mov		esi, source
		mov		ecx, num
		mov		edx, 4

loop1:	test	ecx, ecx
		jz		end1
		mov		eax, dword ptr [esi]
		mov		dword ptr[edi], eax
		sub		ecx, edx
		add		esi, edx
		add		edi, edx
		jmp		loop1
end1:
		mov		eax, destination
	}
#elif defined(ASM_GCC)
asm("	mov		%[destination], %%edi\n"
"		mov		%[source], %%esi\n"
"		mov		%[num], %%ecx\n"
"		mov		$4, %%edx\n"

"1:		test	%%ecx, %%ecx\n"
"		jz		2f\n"
"		movl	(%%esi), %%eax\n"
"		movl	%%eax, (%%edi)\n"
"		sub		%%edx, %%ecx\n"
"		add		%%edx, %%esi\n"
"		add		%%edx, %%edi\n"
"		jmp		1b\n"
"2:\n"
"		mov		%[destination], %%eax\n"
:
: [destination] "m" (destination), [source] "m" (source), [num] "m" (num));
#else
	while(num) {
		*static_cast<dword*>(destination) = *static_cast<const dword*>(source);
		num -= 4;
		source += 4;
		destination += 4;
	}
	return destination;
#endif
}*/
