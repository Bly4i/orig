#include <iostream>
#include <vector>
#include <Windows.h> 
#include <algorithm> 
#include<numeric>
#include<list>
#include <future>
#include <utility>
using namespace std;

template <typename Iterator, typename Function>

void parallel_for_each_recursive(Iterator begin, Iterator end, Function func, size_t threshold) {
    size_t size = distance(begin, end);

    if (size <= threshold) 
    {
        for_each(begin, end, func);
    }
    else {
        Iterator middle = begin;
        advance(middle, size / 2);

        future<void> left_future = async(launch::async, [begin, middle, &func, threshold]() 
            {
            parallel_for_each_recursive(begin, middle, func, threshold);
            });
        parallel_for_each_recursive(middle, end, func, threshold);

        left_future.get();
    }
}

template <typename Iterator, typename Function>
void parallel_for_each(Iterator begin, Iterator end, Function func, size_t threshold = 1000) 
{
    parallel_for_each_recursive(begin, end, func, threshold);
}

int main() {
    vector<int> data = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    cout << "Original array: ";
    for (int val : data) 
    {
       cout << val << " ";
    }
    cout << endl;

    parallel_for_each(data.begin(), data.end(), [](int& x) { x *= 2; });

    cout << "Modified array: ";
    for (int val : data) {
        cout << val << " ";
    }
    cout << endl;

    return 0;
}
