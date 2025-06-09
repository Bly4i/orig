#include <iostream>
#include <mutex>
#include <shared_mutex>

using namespace std;

class Data
{
public:
    int value;
    mutex mtx;

    Data(int v) : value(v) {}
};

void swap_with_lock(Data& a, Data& b)
{
    lock(a.mtx, b.mtx);
    swap(a.value, b.value);
    a.mtx.unlock();
    b.mtx.unlock();
}

void swap_with_scoped_lock(Data& a, Data& b)
{
    scoped_lock lock(a.mtx, b.mtx);
    swap(a.value, b.value);
}

void swap_with_unique_lock(Data& a, Data& b)
{
    unique_lock lock_a(a.mtx, defer_lock);
    unique_lock lock_b(b.mtx, defer_lock);
    lock(lock_a, lock_b);
    swap(a.value, b.value);
}

int main()
{
    Data d1(10), d2(20);

    cout << "Before swap: d1 = " << d1.value << ", d2 = " << d2.value << endl;

    swap_with_lock(d1, d2);
    cout << "After swap_with_lock: d1 = " << d1.value << ", d2 = " << d2.value << endl;

    swap_with_scoped_lock(d1, d2);
    cout << "After swap_with_scoped_lock: d1 = " << d1.value << ", d2 = " << d2.value << endl;

    swap_with_unique_lock(d1, d2);
    cout << "After swap_with_unique_lock: d1 = " << d1.value << ", d2 = " << d2.value << endl;

    return 0;
}
