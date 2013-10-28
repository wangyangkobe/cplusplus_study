#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
using namespace std;

class Counter
{
public:
	Counter(int _value):value(_value){}
	void increaseValue()
	{
		std::lock_guard<std::mutex> lock(mutex);
		++value;
	}

	void decreaseValue()
	{
		std::lock_guard<std::mutex> lock(mutex);
		if(value == 0)
			throw "Value cannot be less than 0";
		--value;
	}
	int value;
	std::mutex mutex;
};


int main()
{
	Counter counter(0);

	std::vector<thread> threadVec;

	for (int i = 0; i < 5; ++i)
	{
		threadVec.push_back(thread([&counter]()
			{
				for(int j = 0; j < 1000; ++j)
				{
					counter.increaseValue();
				}
			}
		));
	}

	for (auto& thread : threadVec)
	{
		thread.join();
	}
	cout<<counter.value<<endl;
}