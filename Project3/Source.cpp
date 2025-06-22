#include <iostream>
#include <vector>
#include <Windows.h> 
#include <algorithm> 
#include<numeric>
#include<list>
#include <future>
#include <utility>

using namespace std;



void findMinIndexPromise(const vector<int>& arr, size_t start, promise<size_t>&& prom)
{
    size_t minIndex = start;
    for (size_t i = start + 1; i < arr.size(); ++i)
    {
        if (arr[i] < arr[minIndex])
        {
            minIndex = i;
        }
    }
    prom.set_value(minIndex);
}

void selectionSortAsync(vector<int>& arr)
{
    for (size_t i = 0; i < arr.size() - 1; ++i)
    {
        promise<size_t> prom;
        future<size_t> fut = prom.get_future();

        thread t(findMinIndexPromise, cref(arr), i, move(prom));

        size_t minIndex = fut.get();

        t.join();

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