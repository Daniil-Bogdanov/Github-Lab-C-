#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;


class HeatConduction1D {
private:
    int N;                   // кол-во узлов
    double L;                // толщина пластины
    double lambda;           // коэфф. теплопроводности
    double rho;              // плотность материала
    double c;                // удельная теплоемкость
    double T0;               // начальная температура
    double Tl;               // температура на левом краю
    double Tr;               // температура на правом краю
    double t_end;            // время, до которого нужно считать
    double h;                // шаг по пространственной координате
    double tau;              // шаг по времени
    vector<double> T;   // вектор для хранения температур
    vector<double> alpha, beta;  // прогоночные коэффициенты

public:
    // конструктор инициализации параметров задачи
    HeatConduction1D(int nodes, double length, double conductivity, double density, double heatCapacity,
                     double initialTemp, double leftTemp, double rightTemp, double endTime)
        : N(nodes), L(length), lambda(conductivity), rho(density), c(heatCapacity),
          T0(initialTemp), Tl(leftTemp), Tr(rightTemp), t_end(endTime) {

        h = L / (N - 1);            // расчет шага по пространству
        tau = t_end / 100.0;        // задаем шаг по времени
        T.resize(N, T0);
        alpha.resize(N, 0.0);
        beta.resize(N, 0.0);
    }

    // выполнение численного решения уравнения теплопроводности
    void solve() {
        double time = 0.0;
        while (time < t_end) {
            time += tau;

            // устанавление граничных условий
            alpha[0] = 0.0;
            beta[0] = Tl;

            //прогоночные коэффициенты alpha и beta для внутренней области
            for (int i = 1; i < N - 1; ++i) {
                double ai = lambda / (h * h);
                double bi = 2.0 * lambda / (h * h) + rho * c / tau;
                double ci = lambda / (h * h);
                double fi = -rho * c * T[i] / tau;

                alpha[i] = ai / (bi - ci * alpha[i - 1]);
                beta[i] = (ci * beta[i - 1] - fi) / (bi - ci * alpha[i - 1]);
            }

            // правое граничное условие
            T[N - 1] = Tr;

            // прогонка для определения температуры
            for (int i = N - 2; i >= 0; --i) {
                T[i] = alpha[i] * T[i + 1] + beta[i];
            }
        }
    }


    void saveResultsToTextFile(const string &filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            file << "Распределение температуры на момент времени t = " << t_end << " секунд:\n";
            for (int i = 0; i < N; ++i) {
                file << "x = " << i * h << ", T = " << T[i] << " C\n";
            }
            file.close();
            cout << "Результаты сохранены в файл " << filename << endl;
        } else {
            cerr << "Ошибка открытия файла для записи!" << endl;
        }
    }
    void saveResultsToTXTFile(const string& filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            file << "x,temperature\n";
            for (int i = 0; i < N; ++i) {
                file << i * h << "," << T[i] << "\n";
            }
            file.close();
            cout << "Результаты сохранены в файл " << filename << endl;
        } else {
            cerr << "Ошибка открытия файла для записи!" << endl;
        }
    }
};

// основная функция
int main() {
    SetConsoleOutputCP(CP_UTF8);
    int N;
    double L, lambda, rho, c, T0, Tl, Tr, t_end;

    cout << "Введите количество узлов по пространственной координате, N: ";
    cin >> N;
    cout << "Введите конечное время, t_end (с): ";
    cin >> t_end;
    cout << "Введите толщину пластины, L (м): ";
    cin >> L;
    cout << "Введите коэффициент теплопроводности, lambda (Вт/(м*С)): ";
    cin >> lambda;
    cout << "Введите плотность, rho (кг/м^3): ";
    cin >> rho;
    cout << "Введите удельную теплоемкость, c (Дж/(кг*С)): ";
    cin >> c;
    cout << "Введите начальную температуру, T0 (С): ";
    cin >> T0;
    cout << "Введите температуру на левом краю, Tl (С): ";
    cin >> Tl;
    cout << "Введите температуру на правом краю, Tr (С): ";
    cin >> Tr;

    // создаем объект и выполняем расчет
    HeatConduction1D heatConduction(N, L, lambda, rho, c, T0, Tl, Tr, t_end);
    heatConduction.solve();

    // сохраняем результаты в текстовый файл
    heatConduction.saveResultsToTextFile("oop_temp.txt");

    // сохраняем результаты в txt
    heatConduction.saveResultsToTXTFile("oop_res.txt");

    return 0;
}