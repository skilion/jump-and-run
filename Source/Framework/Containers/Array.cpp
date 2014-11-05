#include "Array.h"
#include "myString.h"


namespace myengine {

//=============================================================================
// Optimized replacement of Array<T>::reallocate for strings
//=============================================================================
template<>
void Array<char>::reallocate(unsigned size)
{
	char *data2 = new char[size];

	if(data) {
		String::strcpy(data2, data);
		this->size = size;
	} else {
		allocate(size);
	}

	delete [] data;
	data = data2;
}

}