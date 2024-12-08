#include <thread> // работа с патоками
#include <iostream>
#include <vector>
#include <mutex> //работа с блокировками

using namespace std;

mutex coutMutex; // глобальный объект блокировки

//параметры передаваемые в конструктор копируются
void print_hello(int id, double& sum)
{
    //добавление блокировки
    coutMutex.lock();
    //вывод ID потока
    sum +=id;
    cout<<"Thread: "<<id<<" sum: "<<sum<<endl;
    coutMutex.unlock();
}

//версия кода с блокировками
int main()
{
    double sum =0;
    cout <<"MAAAAAAAAAAAAAAAAAAAAAAIN"<<endl;
    vector<thread> threads;
    for (int i=0; i<8; i++)
    {
        //передаём по ссылке значение
        threads.push_back(thread(print_hello, i, ref(sum)));
    }
    //ожидаем присоединения потока
    for (auto& th: threads)
    {
        if (th.joinable())
        {
            th.join();
        }
    }

    return 0;
}

//создание массива потоков
// потоки будут в состоянии гонки, поэтому текст будет выводиться не одинаково, возможно даже будет так: Hello from thread 2Hello from thread 2
// int main()
// {
//     vector<thread> threads;
//     for (int i=0; i<8; i++)
//     {
//         threads.push_back(thread(print_hello));
//     }
//     cout <<"MAAAAAAAAAAAAAAAAAAAAAAIN"<<endl;
//     //присоединение потока
//     for (auto& th: threads)
//     {
//         if (th.joinable())
//         {
//             th.join();
//         }
//     }

//     return 0;
// }

//создание потока

// int main()
// {
//     cout <<"MAAAAAAAAAAAAAAAAAAAAAAIN"<<endl;
//     thread thread2(print_hello);
//     thread2.join();
//     return 0;
// }