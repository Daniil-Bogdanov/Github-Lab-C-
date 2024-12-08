#include <iostream>
#include <vector>
#include <thread>
#include <numeric>
#include <mutex>
#include <chrono>

using namespace std;
using namespace std::chrono;

// глобальная переменная для синхронизации вывода
mutex output_mutex;

// функция для вычисления суммы части массива
double partial_sum(const vector<double>& arr, size_t start, size_t end) {
    return accumulate(arr.begin() + start, arr.begin() + end, 0.0);
}

void thread_function(const vector<double>& arr, size_t start, size_t end, double& result) {
    result = partial_sum(arr, start, end);

    // синхронизация вывода
    {
        lock_guard<mutex> lock(output_mutex);
        cout << "Thread ID: " << this_thread::get_id() << ", Partial sum from " << start << " to " << end << ": " << result << endl;
    }
}

int main() {
    const size_t SIZE = 10'000'000; // размер массива
    vector<double> arr(SIZE, 1.0); // инициализация массива значениями 1.0

    int mode;
    cout << "Choose mode (1 - single thread, 2 - multi-threaded): ";
    cin >> mode;

    if (mode == 1) {
        // однопоточная работа
        auto start_time = high_resolution_clock::now(); // начало измерения времени
        double total_sum = partial_sum(arr, 0, SIZE);
        auto end_time = high_resolution_clock::now(); // конец измерения времени

        auto duration = duration_cast<microseconds>(end_time - start_time); // вычисление продолжительности
        cout << "Total sum: " << total_sum << endl;
        cout << "Execution time (single thread): " << duration.count() << " microseconds" << endl;
    } else if (mode == 2) {
        // многопоточная работа
        size_t num_threads;
        cout << "Enter the number of threads: ";
        cin >> num_threads;

        // проверка на корректность ввода
        if (num_threads <= 0) {
            cerr << "Number of threads must be positive." << endl;
            return 1;
        }

        vector<thread> threads;
        vector<double> results(num_threads);
        size_t chunk_size = SIZE / num_threads;

        auto start_time = high_resolution_clock::now(); // начало измерения времени

        // запуск потоков
        for (size_t i = 0; i < num_threads; ++i) {
            size_t start = i * chunk_size;
            size_t end = (i == num_threads - 1) ? SIZE : start + chunk_size; // последний поток берет остаток
            threads.emplace_back(thread_function, ref(arr), start, end, ref(results[i]));
        }

        // ожидание завершения всех потоков
        for (auto& thread : threads) {
            thread.join();
        }

        // суммируем результаты
        double total_sum = accumulate(results.begin(), results.end(), 0.0);
        auto end_time = high_resolution_clock::now(); // конец измерения времени

        auto duration = duration_cast<microseconds>(end_time - start_time); // вычисление продолжительности
        cout << "Total sum: " << total_sum << endl;
        cout << "Execution time (multi-threaded): " << duration.count() << " microseconds" << endl;
    } else {
        cerr << "Invalid mode selected." << endl;
        return 1;
    }

    return 0;
}