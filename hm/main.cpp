#include <iostream>
#include <thread>
#include <Windows.h> 
#include <chrono>
#include <mutex>
#include <atomic>

using namespace std;

atomic<long long> client_count{ 0 }; 
int max_clients;
mutex cout_mutex;

void client_thread()
{
    while (true)
    {
        if (client_count.load(memory_order_acquire) < max_clients)
        {
            client_count.fetch_add(1, memory_order_release);
            {
                lock_guard<mutex> lock(cout_mutex);
                cout << "Клиент пришел. Всего клиентов: " << client_count.load() << endl;
            }
            Sleep(1000);
        }
        else
        {
            {
                lock_guard<mutex> lock(cout_mutex);
                cout << "Клиенты больше не приходят" << endl;
            }
            break;
        }
    }
}

void operator_thread()
{
    while (true)
    {
        if (client_count.load(memory_order_acquire) > 0)
        {
            client_count.fetch_sub(1, memory_order_release);
            {
                lock_guard<mutex> lock(cout_mutex);
                cout << "Клиент обслужен. Всего клиентов: " << client_count.load() << endl;
            }
            Sleep(2000);
        }
        else
        {
            if (max_clients <= 0) {
                break;
            }
            Sleep(100);
        }
    }
    {
        lock_guard<mutex> lock(cout_mutex);
        cout << "Операционист закончил работу" << endl;
    }
}

int main()
{
    setlocale(LC_ALL, "ru");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    cout << "Введите максимальное количество клиентов: ";
    cin >> max_clients;

    thread client(client_thread);
    thread oper(operator_thread);

    client.join();
    oper.join();

    cout << "Все потоки завершили работу." << endl;

    return 0;
}
