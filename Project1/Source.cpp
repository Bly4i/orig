//Задание 1 
#include <iostream>
#include <thread>
#include <Windows.h> 
#include <chrono>
#include <mutex>

using namespace std;

volatile long long client_count = 0; 
int max_clients;
mutex cout_mutex;

void client_thread() 
{
    while (true) 
    {
        if (client_count < max_clients) 
        {
            client_count++;
            {
                lock_guard<mutex> lock(cout_mutex); 
                cout << "Клиент пришел. Всего клиентов: " << client_count << endl;
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
        if (client_count > 0) 
        {
            client_count--; 
            {
                lock_guard<mutex> lock(cout_mutex); 
                cout << "Клиент обслужен. Всего клиентов: " << client_count << endl;
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

//Задание 2
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include <numeric>
#include <Windows.h> 

using namespace std;

void sum_partial_vector(const vector<int>& vec1, const vector<int>& vec2, vector<int>& result, int start, int end) 
{
    for (int i = start; i < end; ++i) 
    {
        result[i] = vec1[i] + vec2[i];
    }
}

int main()
{
    setlocale(LC_ALL, "ru");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    vector<int> sizes = { 1000, 10000, 100000, 1000000 };
    vector<int> num_threads_list = { 1, 2, 4, 8, 16 };

    cout << "Количество аппаратных ядер : " << thread::hardware_concurrency() << endl;

    
    cout << setw(15) << "Размер" << setw(10) << "Потоки";
    for (int threads : num_threads_list) 
    {
        cout << setw(15) << threads;
    }
    cout << endl;

    for (int size : sizes) 
    {
        vector<int> vec1(size, 1); 
        vector<int> vec2(size, 2);
        vector<int> result(size, 0);

        cout << setw(15) << size;

        for (int num_threads : num_threads_list) 
        {
            auto start_time = chrono::high_resolution_clock::now();

            vector<thread> threads;
            int chunk_size = size / num_threads;
            int remainder = size % num_threads;

            for (int i = 0; i < num_threads; ++i) 
            {
                int start = i * chunk_size;
                int end = (i == num_threads - 1) ? (i + 1) * chunk_size + remainder : (i + 1) * chunk_size;
                threads.emplace_back(sum_partial_vector, ref(vec1), ref(vec2), ref(result), start, end);
            }

            for (auto& thread : threads) 
            {
                thread.join();
            }

            auto end_time = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);

            cout << setw(15) << fixed << setprecision(3) << duration.count() / 1'000'000.0 << " s";
        }
        cout << endl;
    }

    return 0;
}

