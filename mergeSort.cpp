#include <iostream>
#include <algorithm>
#include <iterator>
#include <cstring>
#include <ctime>
using namespace std;

template <typename T>
void merge(T arr[], int begin, int mid, int end)
{
  int len = end - begin + 1;
  T *temp = new T[len];
  int i = begin;
  int j = mid + 1;
  int k = 0;
  while (i <= mid && j <= end)
    {
      if(arr[i] <= arr[j])
	temp[k++] = arr[i++];
      else
	temp[k++] = arr[j++];
    }
  while (i <= mid)
    temp[k++] = arr[i++];
  while(j <= end)
    temp[k++] = arr[j++];

  for (int iter = 0; iter < len; ++iter)
    arr[begin+iter] = temp[iter];
  delete []temp;
}


//merge sort
template <typename T>
void mergeSort(T arr[], int begin, int end)
{
  if (begin >= end)
    return;

  int mid = (end + begin) / 2;
  mergeSort(arr, begin, mid);
  mergeSort(arr, mid + 1, end);
  merge(arr, begin, mid, end);
}


int main()
{
  srand(static_cast<unsigned int>(time(NULL)));
  int arr[10];
  int N = sizeof(arr) / sizeof(int);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 100);

  for_each(arr, arr + N, [&](int &val){ val = dis(gen); });

  copy(arr, arr+N, ostream_iterator<int>(cout, " "));
  cout<<endl;

  mergeSort(arr, 0, N - 1);
  
  copy(arr, arr+N, ostream_iterator<int>(cout, " "));
  cout<<endl;
}
