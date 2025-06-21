#include <iostream>
#include <vector>
#include <Windows.h> 
#include <algorithm> 
#include<numeric>
#include<list>
#include <future>
#include <utility>

using namespace std;



future<size_t> findMinIndexAsync(const vector<int>& arr, size_t start) 
{
    return async(launch::async, [&arr, start]() 
        {
        size_t minIndex = start;
        for (size_t i = start + 1; i < arr.size(); ++i) 
        {
            if (arr[i] < arr[minIndex]) 
            {
                minIndex = i;
            }
        }
        return minIndex;
        });
}

void selectionSortAsync(std::vector<int>& arr) 
{
    for (size_t i = 0; i < arr.size() - 1; ++i) 
    {
        future<size_t> future = findMinIndexAsync(arr, i);

        size_t minIndex = future.get();

        if (minIndex != i) 
        {
            swap(arr[i], arr[minIndex]);
        }
    }
}

int main() 
{
    vector<int> data = { 5, 2, 8, 1, 9, 4, 7, 3, 6 };
    cout << "Original array: ";
    for (int val : data) 
    {
        cout << val << " ";
    }
    cout << endl;

    selectionSortAsync(data);

   cout << "Sorted array: ";
    for (int val : data) 
    {
        cout << val << " ";
    }
    cout << endl;

    return 0;
}