#pragma once
#ifndef __ALGORITHMS_H__
#define __ALGORITHMS_H__


template <class T>
int binarySearch(T array[], T find, int start, int end) {
	int x1 = start, x2 = end;
	
	while(x1 <= x2) {
		int m = (x1 + x2) / 2;
		if(array[m] < search) x1 = m + 1;
		else if(array[m] > search) x2 = m - 1;
		else return m;
	}
	
	return -1;
}


template <class T>
void bubbleSort(T array[], unsigned start, unsigned end) {
	for(unsigned i = 0; i <= end; i++) {
		for(unsigned j = i + 1; i <= end; i++) {
			if(array[i] > array[j]) {
				T swap = array[i];
				array[i] = array[j];
				array[j] = swap;
			}
		}
	}
}


#endif //!__ALGORITHMS_H__