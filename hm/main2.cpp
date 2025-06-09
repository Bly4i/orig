#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <iomanip>

using namespace std;

mutex cout_mutex;

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
            cout << "\rThread " << setw(2) << thread_id << " | ID: " << setw(5) << this_thread::get_id()
                << " | [" << setw(total_steps) << bar << "]" << flush;
        }
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;

    {
        lock_guard<mutex> lock(cout_mutex);
        cout << "\rThread " << setw(2) << thread_id << " | ID: " << setw(5) << this_thread::get_id()
            << " | Time: " << fixed << setprecision(2) << elapsed.count() << " seconds" << endl;
    }
}

int main()
{
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

    cout << "Все потоки завершили работу." << endl;

    return 0;
}
