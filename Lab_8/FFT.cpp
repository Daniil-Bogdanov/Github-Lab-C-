#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <windows.h>
#include <stdexcept>

using namespace std;

// определяем тип для комплексных чисел
typedef complex<double> Complex;
typedef vector<Complex> CArray;

// функция для выполнения БПФ
void fft(CArray &x) {
    try {
        if (x.empty()) {
            throw runtime_error("Входной массив пуст");
        }

        // проверка, является ли размер массива степенью двойки
        int N = x.size();
        if ((N & (N - 1)) != 0) {
            throw runtime_error("Размер массива должен быть степенью двойки");
        }

        if (N <= 1) return; // базовый случай

        // разделяем массив на четные и нечетные элементы
        CArray even(N / 2), odd(N / 2);
        for (int i = 0; i < N / 2; ++i) {
            even[i] = x[i * 2];
            odd[i] = x[i * 2 + 1];
        }

        // рекурсивно применяем БПФ к четным и нечетным частям
        fft(even);
        fft(odd);

        // объединяем результаты
        for (int k = 0; k < N / 2; ++k) {
            Complex t = polar(1.0, -2 * M_PI * k / N) * odd[k];
            x[k] = even[k] + t;
            x[k + N / 2] = even[k] - t;
        }
    } catch (const exception& e) {
        cerr << "Ошибка в функции FFT: " << e.what() << endl;
        throw;
    }
}

// функция для вычисления модуля спектра
vector<double> computeMagnitude(const CArray &x) {
    try {
        if (x.empty()) {
            throw runtime_error("Входной массив пуст");
        }

        vector<double> magnitude(x.size());
        for (size_t i = 0; i < x.size(); ++i) {
            magnitude[i] = abs(x[i]);
        }
        return magnitude;
    } catch (const exception& e) {
        cerr << "Ошибка при вычислении модуля спектра: " << e.what() << endl;
        throw;
    }
}

// функция для вывода массива комплексных чисел
void printArray(const CArray &x) {
    try {
        if (x.empty()) {
            throw runtime_error("Массив пуст");
        }
        for (const auto &c : x) {
            cout << c << " ";
        }
        cout << endl;
    } catch (const exception& e) {
        cerr << "Ошибка при выводе массива: " << e.what() << endl;
    }
}

// функция для вывода массива действительных чисел
void printMagnitude(const vector<double> &magnitude) {
    try {
        if (magnitude.empty()) {
            throw runtime_error("Массив пуст");
        }
        for (const auto &m : magnitude) {
            cout << m << " ";
        }
        cout << endl;
    } catch (const exception& e) {
        cerr << "Ошибка при выводе модуля спектра: " << e.what() << endl;
    }
}

int main() {
    try {
        // устанавливаем кодовую страницу консоли на UTF-8
        if (!SetConsoleOutputCP(CP_UTF8)) {
            throw runtime_error("Не удалось установить кодировку UTF-8");
        }

        // создаем массив комплексных чисел
        CArray data = {
            {0, 0}, {1, 0}, {2, 0}, {3, 0},
            {4, 0}, {5, 0}, {6, 0}, {7, 0}
        };

        cout << "Исходные данные:" << endl;
        printArray(data);

        // применяем БПФ
        fft(data);

        cout << "Результат БПФ:" << endl;
        printArray(data);

        // вычисляем и выводим модуль спектра
        vector<double> magnitude = computeMagnitude(data);
        cout << "Модуль спектра:" << endl;
        printMagnitude(magnitude);

        return 0;
    } catch (const exception& e) {
        cerr << "Критическая ошибка: " << e.what() << endl;
        return 1;
    }
}
