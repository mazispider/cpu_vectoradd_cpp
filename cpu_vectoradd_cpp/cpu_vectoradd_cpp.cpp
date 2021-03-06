// cpu_vectoradd_cpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <thread>

void vecAdd(std::vector<int>& a, std::vector<int>& b, std::vector<int>& c, int threadnum, int tid) {
	int idx = tid;
	while (idx < a.size()) {
		c[idx] = a[idx] + b[idx];
		idx += threadnum;
	}
}
//if I don't add std::ref to thread arguments it gives "'std::invoke': no matching overloaded function found" so I've read about referencing argument in thread constructor --> "The arguments to the thread function are moved or copied by value. If a reference argument needs to be passed to the thread function, it has to be wrapped (e.g., with std::ref or std::cref)."
//https://stackoverflow.com/questions/62187098/stdinvoke-no-matching-overloaded-function-found
void vecAddThreads(std::vector<int>& a, std::vector<int>& b,std::vector<int>& c, int threadnum) {
	std::vector<std::thread> thr;
	for (int i = 0; i < threadnum; i++)
		thr.emplace_back(vecAdd, std::ref(a), std::ref(b), std::ref(c), threadnum, i);
	for (auto& th : thr) {
		th.join();
	}
}
void printVector(std::vector<int>& input) {
	
	for (auto i : input) {
		std::cout << i << std::endl;
	}
}

int main()
{
	const int retry = 10;
	std::vector<int> x, y, z;
	const int vecsize = 10000000;
	for (int i = 0; i < vecsize; i++) {
		x.push_back(i);
		y.push_back(i);
		z.push_back(0);
	}
	for (int i = 1; i <= (1<<8); i *= 2) {
	int startTime = clock();
	for (int j = 0; j < retry; j++) {
		vecAddThreads(x, y, z, i);
	}
	int stopTime = clock();

	//printVector(z);
	std::cout << "Elapsed Time for " << i << " thread(s): " << 1000 * (double)(stopTime - startTime) / (double)CLOCKS_PER_SEC / retry << " ms" << std::endl;
    }
	std::cin.get();
    return 0;
}

