#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <Windows.h> 

using namespace std;

mutex cout_mutex;

void set_cursor_position(int x, int y) 
{
    COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void progress_bar(int thread_id, int total_steps, int sleep_time_ms)
{
    string bar = "";
    auto start_time = chrono::high_resolution_clock::now();

    for (int i = 0; i <= total_steps; ++i)
    {
        this_thread::sleep_for(chrono::milliseconds(sleep_time_ms));

        bar += "=";

        {
            lock_guard<mutex> lock(cout_mutex);

            set_cursor_position(0, thread_id - 1);

            cout << "Thread " << setw(2) << thread_id << " | ID: " << setw(5) << this_thread::get_id()
                << " | [" << setw(total_steps) << left << bar << "] "
                << setw(3) << i * 100 / total_steps << "%" << flush;
        }
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;

    {
        lock_guard<mutex> lock(cout_mutex);

        set_cursor_position(0, thread_id - 1);
        
        cout << "Thread " << setw(2) << thread_id << " | ID: " << setw(5) << this_thread::get_id()
            << " | Time: " << fixed << setprecision(2) << elapsed.count() << " seconds" << endl;
    }
}

int main()
{
    setlocale(LC_ALL, "ru");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int num_threads = 4;
    int total_steps = 50;
    int sleep_time_ms = 50;

    vector<thread> threads;

    for (int i = 0; i < num_threads; ++i)
    {
        threads.emplace_back(progress_bar, i + 1, total_steps, sleep_time_ms);
    }

    for (auto& t : threads)
    {
        t.join();
    }

    cout << "\nВсе потоки завершили работу." << endl;

    return 0;
}
