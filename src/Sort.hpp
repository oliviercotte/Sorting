///////////////////////////////////////////////////////////////////////////////
/// @file Sort.hpp
/// @author Olivier Cotte
/// @date February 2014
///////////////////////////////////////////////////////////////////////////////

#ifndef _SORT_HPP_
#define _SORT_HPP_

#include <vector>

/*
 *********************************************************************************************************
 *                                              CONSTANTS
 *********************************************************************************************************
 */

static const size_t CUTOFF = 256; // Minimum size require to use the quick sort (recursively), else bubble sort

/*
 *********************************************************************************************************
 *                                         FUNCTION PROTOTYPES
 *********************************************************************************************************
 */

template <class T>
static void swap(T& a, T& b);

template <class T>
static void _bubble(T a[], size_t size);

template <class T>
static void _counting(T a[], size_t size);

template <class T>
static size_t partitioning(T a[], size_t left, size_t right);

template <class T>
static void _quick(T a[], size_t left, size_t right);

template <class T>
static void _quick2(T a[], size_t left, size_t right);

template <class T>
static void median3(T a[], size_t left, size_t right);

template <class T>
static size_t partitionMed(T A[], size_t left, size_t right);

template <class T>
static void _quickMed(T a[], size_t left, size_t right);

/*
 *********************************************************************************************************
 *                                                  SORT.CPP
 *********************************************************************************************************
 */

template <class T>
void bubble(std::vector<T>& a)
{
	_bubble<T>(&a[0], a.size());
}

template <class T>
void counting(std::vector<T>& a)
{
	_counting<T>(&a[0], a.size());
}

template <class T>
void quick(std::vector<T>& a)
{
	_quick<T>(&a[0], 0, a.size()-1);
}

template <class T>
void quick2(std::vector<T>& a)
{
	_quick2<T>(&a[0], 0, a.size());
}

template <class T>
void quickMed(std::vector<T>& a)
{
	_quickMed<T>(&a[0], 0, a.size()-1);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn template <class T> void swap(T& a, T& b)
///
/// Exchange the value of a and b.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
template <class T>
void swap(T& a, T& b)
{
	T c(a);
	a = b;
	b = c;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn template <class T> void _bubble(T a[], size_t size)
///
/// Implementation of the bubble sort algorithm.
///	The array received as a parameter is sorted by the call.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
template <class T>
void _bubble(T a[], size_t size)
{
	for(size_t i = 0; i < size; ++i) {
		bool is_sorted = true;
		for(size_t j = 0; j < size - i - 1; ++j) {
			if(a[j] > a[j + 1]) {
				swap(a[j], a[j + 1]);
				is_sorted = false;
			}
		}
		if(is_sorted) return;
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn template <class T> void _counting(T a[], size_t size)
///
/// Implementation of the counting sort algorithm
///	The array received as a parameter is sorted by the call.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
template <class T>
void _counting(T a[], size_t size)
{
	size_t i, j, k;
	int idx = 0;
	int min, max;

	min = max = a[0];
	for(i = 1; i < size; i++) {
		min = (a[i] < min) ? a[i] : min;
		max = (a[i] > max) ? a[i] : max;
	}

	// Initialization of the counting vector to 0
	k = max - min + 1;
	std::vector<T> B(k, 0);

	// Creating the sorted array
	for(i = 0; i < size; i++) {
		B[a[i] - min]++;
	}

	for(i = min; i <= max; i++) {
		for(j = 0; j < B[i - min]; j++) {
			a[idx++] = i;
		}
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn template <class T> size_t partitionMed(T a[], size_t left, size_t right)
///
/// For each sub-array, a new pivot is defined.
/// At the end of this function, the pivot is at the correct position in the sorted array.
///
/// @return The position of the pivot.
///
////////////////////////////////////////////////////////////////////////
template <class T>
size_t partitionMed(T a[], size_t left, size_t right)
{
	int m = right;
	swap(a[left], a[m]);
	int pivot = a[left];
	int low = left + 1, high = right;
	while(low <= high) {
		while(a[high] > pivot) --high;
		while(low <= high && a[low] <= pivot) ++low;
		if(low <= high) {
			swap(a[low],a[high]);
			++low; --high;
		}
	}
	swap(a[left], a[high]);
	return high;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn template <class T> void median3(T a[], size_t left, size_t right)
///
/// place the pivot at the end (arbitrarily), changing it with the last element of the sub-array;
/// place all the elements below the pivot at the beginning of the sub-array;
/// place the pivot at the end of the moved elements.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
template <class T>
void median3(T a[], size_t left, size_t right)
{
	size_t center = ( left + right ) / 2;
	if ( a[ center ] < a[ left ] )
		swap( a[ left ], a[ center ] );
	if ( a[ right ] < a[ left ] )
		swap( a[ left ], a[ right ] );
	if ( a[ right ] < a[ center ] )
		swap( a[ center ], a[ right ] );

	swap( a[ center ], a[ right - 1 ] ); // Place the pivot at the (right - 1) position
}

////////////////////////////////////////////////////////////////////////
///
/// @fn template <class T> int partitioning(T A[], size_t left, size_t right)
///
/// For each sub-array, define a new pivot and repeat the partitioning operation.
/// This process is repeated recursively until all elements are sorted.
/// That is, the elements smaller than the pivot are on one side and the other (larger) on the other.
///
/// @return Final position of the pivot.
///
////////////////////////////////////////////////////////////////////////
template <class T>
size_t partitioning(T A[], size_t left, size_t right)
{
	T v = A[right];
	size_t i = left;
	size_t j = right-1;

	while (1) {
		while (A[i] < v) i++;
		while (A[j] > v) j--;
		if (i < j) {
			swap(A[i], A[j]);
			i++; j--;
		}
		else
			break;
	}

	swap(A[i], A[right]);
	return i;
}

// Different realization for quick sort algorithm
////////////////////////////////////////////////////////////////////////
template <class T>
void _quick(T a[], size_t left, size_t right)
{
	if(left + CUTOFF < right) {
		int q = partitionMed(a, left, right);
		_quick(a, left, q - 1);
		_quick(a, q + 1, right);
	} else {
		_bubble(&a[left], right - left + 1);
	}
}

////////////////////////////////////////////////////////////////////////
template <class T>
void _quick2(T a[], size_t left, size_t right)
{
	while(left < right) {
		int q = partition(a, left, right);
		if(q-left < right-q) {
			_quick2(a, left, q-1);
			left = q+1;
		}
		else {
			_quick2(a, q+1, right);
			right = q-1;
		}
	}
}

////////////////////////////////////////////////////////////////////////
template <class T>
void _quickMed(T a[], size_t left, size_t right)
{
	if(left + CUTOFF < right) {
		median3(a, left, right);
		int q = partitionMed(a, left + 1, right - 1);
		_quickMed(a, left, q - 1);
		_quickMed(a, q + 1, right);
	} else {
		_bubble(&a[left], right - left + 1);
	}
}

#endif // _SORT_HPP_
