#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <windows.h>
using namespace std;

const int mf = 500;

// класс для вектора
class Vector {
private:
    vector<double> data;
public:
    Vector(int size) : data(size) {}

    double& operator[](int index) {
        return data[index];
    }

    const double& operator[](int index) const {
        return data[index];
    }

    int size() const {
        return data.size();
    }
};

// ввод параметров
void inputParameters(int &N, double &t_end, double &L, double &lamda, double &ro, double &c, double &T0, double &Tl, double &Tr) {
    cout << "Введите количество пространственных узлов, N: ";
    cin >> N;
    cout << "Введите окончание по времени, t_end: ";
    cin >> t_end;
    cout << "Введите толщину пластины, L: ";
    cin >> L;
    cout << "Введите коэффициент теплопроводности материала пластины, lamda: ";
    cin >> lamda;
    cout << "Введите плотность материала пластины, ro: ";
    cin >> ro;
    cout << "Введите теплоемкость материала пластины, c: ";
    cin >> c;
    cout << "Введите начальную температуру, T0: ";
    cin >> T0;
    cout << "Введите температуру на границе x=0, Tl: ";
    cin >> Tl;
    cout << "Введите температуру на границе x=L, Tr: ";
    cin >> Tr;
}

// инициализации температуры
void initializeTemperature(vector<double> &T, int N, double T0) {
    for (int i = 0; i < N; i++) {
        T[i] = T0;
    }
}

// интегрирование нестационарного уравнения теплопроводности
void integrateHeatEquation(vector<double> &T, int N, double lamda, double ro, double c, double h, double tau, double Tl, double Tr, double t_end) {
    vector<double> alfa(mf), beta(mf);
    double time = 0;

    while (time < t_end) {
        time += tau;

        // начальные прогоночные коэффициенты
        alfa[0] = 0.0;
        beta[0] = Tl;

        // определение прогоночных коэффициентов
        for (int i = 1; i < N - 1; i++) {
            double ai = lamda / (h * h);
            double bi = 2.0 * lamda / (h * h) + ro * c / tau;
            double ci = lamda / (h * h);
            double fi = -ro * c * T[i] / tau;

            alfa[i] = ai / (bi - ci * alfa[i - 1]);
            beta[i] = (ci * beta[i - 1] - fi) / (bi - ci * alfa[i - 1]);
        }

        // температура на правой границе
        T[N - 1] = Tr;

        // определение поля температуры
        for (int i = N - 2; i >= 0; i--) {
            T[i] = alfa[i] * T[i + 1] + beta[i];
        }
    }
}

// вывод результатов в файл
void outputResults(const vector<double> &T, int N, double L, double lamda, double ro, double c, double T0, double Tl, double Tr, double h, double tau, double t_end) {
    ofstream f("res.txt");
    f << fixed << setprecision(4);
    f << "Толщина пластины L = " << L << endl;
    f << "Число узлов по координате N = " << N << endl;
    f << "Коэффициент теплопроводности материала пластины lamda = " << lamda << endl;
    f << "Плотность материала пластины ro = " << ro << endl;
    f << "Теплоемкость материала пластины c = " << c << endl;
    f << "Начальная температура T0 = " << T0 << endl;
    f << "Температура на границе x = 0, Tl = " << Tl << endl;
    f << "Температура на границе x = L, Tr = " << Tr << endl;
    f << "Результат получен с шагом по координате h = " << h << endl;
    f << "Результат получен с шагом по времени tau = " << tau << endl;
    f << "Температурное поле в момент времени t = " << t_end << endl;
    f.close();
}

// вывод температурного поля в файл
void outputTemperatureField(const vector<double> &T, int N, double h) {
    ofstream g("tempr.txt");
    g << fixed << setprecision(3);
    for (int i = 0; i < N; i++) {
        g << setw(6) << h * i << "   " << setw(8) << T[i] << endl;
    }
    g.close();
}

int main() {
    // устанавливление кодовой страницы консоли на UTF-8
    SetConsoleOutputCP(CP_UTF8);
    int N;
    double t_end, L, lamda, ro, c, T0, Tl, Tr;

    // ввод параметров
    inputParameters(N, t_end, L, lamda, ro, c, T0, Tl, Tr);

    // расчетный шаг по пространственной координате и времени
    double h = L / (N - 1);
    double tau = t_end / 100.0;

    // вектор для температуры
    vector<double> T(mf);

    // инициализация температуры
    initializeTemperature(T, N, T0);

    // интегрирование уравнения теплопроводности
    integrateHeatEquation(T, N, lamda, ro, c, h, tau, Tl, Tr, t_end);

    // вывод результатов
    outputResults(T, N, L, lamda, ro, c, T0, Tl, Tr, h, tau, t_end);
    outputTemperatureField(T, N, h);

    return 0;
}