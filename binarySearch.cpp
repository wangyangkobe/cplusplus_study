#include <iostream>
#include <algorithm>
#include <cassert>
using std::cout;
using std::endl;

template <typename T>
int binSearch(T arr[], int begin, int end, T target)
{
	int mid = 0;
	while(begin < end)
	{
		mid = begin + (end - begin)/2;
		if(arr[mid] == target)
			return mid;	
		else if(arr[mid] < target)
			begin = mid + 1;
		else
			end = mid;
	}
	return -1; //no find		
}
int main()
{
	constexpr int N = 10;
	int arr[N];
	int i = 1;
	std::for_each(arr, arr + N, [&](int& v) {v = i++;} );
	
	assert(4  == binSearch(arr, 0, N, 5));
	assert(-1 == binSearch(arr, 0, N, 100));
	assert(5  == binSearch(arr, 0, N, 6));	
}
