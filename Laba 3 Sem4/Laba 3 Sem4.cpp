// Laba 3 Sem4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>
#include <algorithm>

// Функція для заповнення масиву випадковими числами
void fillArrayWithRandomNumbers(std::vector<int>& arr, int min, int max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);

    for (int& num : arr) {
        num = dis(gen);
    }
}

// Функція для злиття двох підмасивів у відсортований масив
void merge(std::vector<int>& arr, int left, int middle, int right)
{
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    // Створення тимчасових підмасивів
    std::vector<int> L(n1), R(n2);

    // Копіювання даних в тимчасові підмасиви L та R
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[middle + 1 + j];

    // Злиття тимчасових підмасивів назад у відсортований масив
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Копіювання залишків елементів L, якщо такі є
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Копіювання залишків елементів R, якщо такі є
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}
// Послідовний алгоритм сортування "Merge Sort"
void mergeSortSequential(std::vector<int>& arr, int left, int right)
{
    if (left < right) {
        int middle = left + (right - left) / 2;

        mergeSortSequential(arr, left, middle);
        mergeSortSequential(arr, middle + 1, right);

        merge(arr, left, middle, right);
    }
}

// Паралельний алгоритм сортування "Merge Sort"
void mergeSortParallel(std::vector<int>& arr, int left, int right, int numThreads)
{
    if (left < right) {
        if (numThreads <= 1) {
            mergeSortSequential(arr, left, right);
        }
        else {
            int middle = left + (right - left) / 2;

            std::thread leftThread(mergeSortParallel, std::ref(arr), left, middle, numThreads / 2);
            std::thread rightThread(mergeSortParallel, std::ref(arr), middle + 1, right, numThreads / 2);

            leftThread.join();
            rightThread.join();

            merge(arr, left, middle, right);
        }
    }
}
int main()
{
    // Кількість випадкових чисел, яку введе користувач
    int numRandomNumbers;

    std::ios_base::sync_with_stdio(false); // Вимкнення синхронізації ввідно-вивідних потоків

    std::cout << "Enter the number of random numbers: ";
    std::cin >> numRandomNumbers;

    // Перевірка на недопустимі значення
    if (numRandomNumbers <= 0) {
        std::cout << "Invalid number of random numbers." << std::endl;
        return 1;
    }

    // Вхідні дані
    std::vector<int> arr(numRandomNumbers);
    fillArrayWithRandomNumbers(arr, 1, 100000000);

    // Копія масиву для мультипотокового сортування
    std::vector<int> arrParallel = arr;

    int numThreads = std::thread::hardware_concurrency();

    // Засіб для вимірювання часу виконання
    std::chrono::steady_clock::time_point start, end;
    std::chrono::duration<double> durationSeq, durationPar;

    // Послідовне сортування
    start = std::chrono::steady_clock::now();
    mergeSortSequential(arr, 0, arr.size() - 1);
    end = std::chrono::steady_clock::now();
    durationSeq = end - start;

    // Паралельне сортування з використанням всіх доступних потоків
    start = std::chrono::steady_clock::now();
    mergeSortParallel(arrParallel, 0, arrParallel.size() - 1, numThreads);
    end = std::chrono::steady_clock::now();
    durationPar = end - start;

    // Паралельне сортування з використанням одного потоку
    std::vector<int> arrSingleThread = arr;
    start = std::chrono::steady_clock::now();
    mergeSortParallel(arrSingleThread, 0, arrSingleThread.size() - 1, 1);
    end = std::chrono::steady_clock::now();
    std::chrono::duration<double> durationSingleThread = end - start;

    // Виведення результатів
    std::cout << "Sequential Sort Time: " << durationSeq.count() << " seconds" << std::endl;
    std::cout << "Parallel Sort Time (" << numThreads << " threads): " << durationPar.count() << " seconds" << std::endl;
    std::cout << "Parallel Sort Time (1 thread): " << durationSingleThread.count() << " seconds" << std::endl;

    double speedupPar = durationSeq / durationPar;
    double speedupSingleThread = durationSeq / durationSingleThread;

    std::cout << "Speedup (Parallel, " << numThreads << " threads): " << speedupPar << "x faster" << std::endl;
    std::cout << "Speedup (Parallel, 1 thread): " << speedupSingleThread << "x faster" << std::endl;

    return 0;
}
// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
