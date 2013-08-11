#include <iostream>
#include <fstream>
#include <ctime>
#include <memory>
#include <string.h>
#include <iterator>
#include <cstdlib>
#include <random>
using namespace std;
/************************************************************************/
/*                           位图算法的实现                             */
/************************************************************************/
/* (1)、MASK = 0x1F = 31                                                */
/*	如果被求余数是2的整数次幂，可以用位运算来进行转换，从而得到比较高的效率。        */
/*  如求 n % 32 = ???, 可以将其转换为  n & (32-1) 或者  n - (n>>5)<<5   */
/************************************************************************/

class BitMap
{
public:
	BitMap(int n):maxNumer(n)
	{
		int length = 1 + n / BITSPERWORD;
		pData = new int[length];
		memset(pData, 0, length * sizeof(int));
	}
	void set(int i)
	{
		pData[i>>SHIFT] |= 1<<(i & MASK); // i&MASK 相当于i%32
	}
	void clear(int i)
	{
		pData[i>>SHIFT] &= ~(1<<(i & MASK)); // i>>SHIFT 相当于 i/32
	}
	bool test(int i)
	{
		return pData[i>>SHIFT] & (1<<(i & MASK));
	}
	/************************************************************************/
	/*     inputFile: 输入的文件名                                          */
	/*     outputFile： 排序后输出的文件名                                  */
	/************************************************************************/
	void sort(string inputFile, string outputFile)
	{
		ifstream read(inputFile);
		ofstream write(outputFile);
		int temp = 0;

		while (read>>temp)
			set(temp);

		for (int i = 0; i < maxNumer; ++i)
		{
			if(test(i))
				write<<i<<endl;
		}
		read.close();
		write.close();
	}
	~BitMap()
	{
		delete []pData;
		pData = NULL;
	}
private:
	int* pData;
	int maxNumer;
	enum{ SHIFT = 5, MASK = 0x1F, BITSPERWORD = 32};
};

/************************************************************************/
/* 生成没重复元素的随机数类                                             */
/* 该类为单例模式，并且使用了auto_ptr来管理分配的内存。                 */
/************************************************************************/
class GenRandomNumber 
{
public:
	static GenRandomNumber* genInstance()
	{
		if(!mInstance.get())
			mInstance.reset(new GenRandomNumber());
		return mInstance.get();
	}
	/************************************************************************/
	/*    采用交换元素的方法生成随机数                                      */
	/*    fileName ：输出的文件名                                           */
	/*    m        ：要生成的随机数个数 m<maxNumber                         */
	/*    maxNumber：上限值，每个随机数都不大于maxNumber                    */
	/************************************************************************/
	void generate1(string fileName, int m, int maxNumber)
	{
		ofstream outFile(fileName);
		int* arr = new int[maxNumber];
		for(int i = 0; i < maxNumber; i++)
			arr[i] = i;
		
		int temp = 0;
		for(int j = 0; j < m; j++)
		{
			temp = randomRange(j, maxNumber - 1);
			cout<<temp<<endl;
			swap(arr[j], arr[temp]);
		}

		copy(arr, arr + m, ostream_iterator<int>(outFile, "\n"));
		delete []arr;
		outFile.close();
	}
	/************************************************************************/
	/*    采用位图原理的方法生成随机数                                      */
	/*    fileName ：输出的文件名                                           */
	/*    m        ：要生成的随机数个数 m<maxNumber                         */
	/*    maxNumber：上限值，每个随机数都不大于maxNumber                    */
	/************************************************************************/
	void generate2(string fileName, int m, int maxNumber)
	{
		BitMap bitmap(maxNumber);
		ofstream outFile(fileName);
		int count = 0;
		int temp;
		while (count < m)
		{
			srand(time(NULL));
			temp = randomRange(0, maxNumber);
			cout<<temp<<endl;
			if (!bitmap.test(temp))
			{
				bitmap.set(temp);
				outFile<<temp<<endl;
				count++;
			}
		}
		outFile.close();
	}
private:
	GenRandomNumber(){};
	GenRandomNumber(const GenRandomNumber&);
	GenRandomNumber& operator=(const GenRandomNumber&);

	// 生成[low, high]范围内的随机数
	/*
        int randomRange(int low, int high)
	{ 
		srand(clock()); // better than srand(time(NULL))
		return low + (RAND_MAX * rand() + rand()) % (high + 1 - low);;
	}
	*/
        int randomRange(int low, int high)
        {
	  	std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(low, high);
		
		return dis(gen);
        }
	static auto_ptr<GenRandomNumber> mInstance;
};
auto_ptr<GenRandomNumber> GenRandomNumber::mInstance;



int main()
{	
	const int MAX_NUMBER = 1000000;
	GenRandomNumber *pGen = GenRandomNumber::genInstance();
	pGen->generate1("test.txt", MAX_NUMBER, MAX_NUMBER);
	BitMap bitmap(MAX_NUMBER);
	bitmap.sort("test.txt", "sort.txt");
	return 0;
}

/************************************************************************/
/* 问题描述：                                                           */
/*输入：
	输入为一个文件，至多包含n个正整数，每个正整数都要小于n，n=10^7.
	如果输入时某一个整数出现2次，就会产生一个致命的错误。 
  输出：
	增序排序摁对整数数列。
  约束：
	至多只有1MB的可以内存，但是可用磁盘空间非常充足。运行时间至多只允许几分钟 */
/************************************************************************/
