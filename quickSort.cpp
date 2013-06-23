// wy.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

int partion(int arr[], int left, int right)
{
	int i = left;
	int j = right;
	int target = arr[i];

	while (i < j)
	{
		if (arr[j] > target)
			j--;
		arr[i] = arr[j];

		if (arr[i] <= target)
			i++;
		arr[j] = arr[i];
	}
	arr[j] = target;
	return j;
}


void quickSort(int arr[], int start, int end)
{
	if(start >= end)
		return;

	int pos = partion(arr, start, end);

	quickSort(arr, start, pos);
	quickSort(arr, pos+1, end);
}

int kth_element(int arr[], int start, int end, int k)
{
	int pos = partion(arr, start, end);
	if (pos == k)
		return arr[pos];
	else if (pos < k)
		kth_element(arr, pos + 1, end, k);
	else
		kth_element(arr, start, pos, k);
}
struct print
{
	void operator()(int a)
	{
		cout<<a<<" ";
	}
};
int main()
{
	int arr[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	int len = sizeof(arr) / sizeof(*arr);

	quickSort(arr, 0, len - 1);

	for_each(arr, arr + len, print());
	cout<<endl;

	cout<<"k = 2:  "<<kth_element(arr, 0, len - 1, len - 2)<<endl;
}