#include <iostream>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <vector>
#include <fstream>
using std::cout;
using std::endl;

// problem 1
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


// problem 2
template <typename T>
void reverse(T* arr, int begin, int end)
{
  int i = begin;
  int j = end - 1;
  while(i < j)
    {
      std::swap(arr[i], arr[j]);
      i++; j--;
    }	
}

template <typename T>
void revrot(T* arr, int rotpos, int len)
{
  reverse(arr, 0, rotpos);
  reverse(arr, rotpos, len);
  reverse(arr, 0, len);
}


//problem 3
void hadleAnagrams(string inFile, string outFile)
{
  ifstream in(inFile);
  string str, temp;
  typedef pair<string, string> StrPair;
  vector<StrPair> vec;
  while (in>>str)
    {
      temp = str;
      sort(temp.begin(), temp.end());
      vec.push_back(make_pair(temp, str));
    }
  in.close();

  sort(vec.begin(), vec.end(), [](const StrPair& left, const StrPair& right) { return left.first < right.first; } );

  ofstream out(outFile);
  string tagStr;
  for_each(vec.begin(), vec.end(), [&](const StrPair& element){
      if(tagStr.empty()){
	out<<element.first<<": "<<element.second;
	tagStr = element.first;
      }
      else{
	if(element.first == tagStr)
	  out<<" "<<element.second;
	else{
	  out<<endl;
	  out<<element.first<<": "<<element.second;
	  tagStr = element.first;
	}
      }
    });
  out.close();
}

int main()
{

  // test problem 1
  constexpr int N = 10;
  int arr[N];
  int i = 1;
  std::for_each(arr, arr + N, [&](int& v) {v = i++;} );
	
  assert(4  == binSearch(arr, 0, N, 5));
  assert(-1 == binSearch(arr, 0, N, 100));
  assert(5  == binSearch(arr, 0, N, 6));

  // test problem 2
  char str[100];
  memset(str, '\0', 100);
  strcpy(str, "abcdefghijk");
  int len = strlen(str);
  revrot(str, 3, len);
  cout<<str<<endl;

  // test problem 3
  //input file:  pans pots opt snap stop tops
  //output file: anps: pans snap
  //             opst: pots stop tops
  //              opt: opt
  hadleAnagrams(string("wy.txt"), string("a.txt"));
}
