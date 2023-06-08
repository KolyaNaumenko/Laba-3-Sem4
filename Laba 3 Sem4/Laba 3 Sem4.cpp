// Laba 3 Sem4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>
#include <algorithm>
#include <cassert>

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

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
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

// Юніт-тести

void testMergeSort()
{
    std::vector<int> arr = { 4, 2, 7, 5, 1, 3, 6 };
    std::vector<int> sortedArr = { 1, 2, 3, 4, 5, 6, 7 };

    mergeSortSequential(arr, 0, arr.size() - 1);
    assert(arr == sortedArr);

    arr = { 4, 2, 7, 5, 1, 3, 6 };
    mergeSortParallel(arr, 0, arr.size() - 1, 2);
    assert(arr == sortedArr);
}

void testMergeSortEmptyArray()
{
    std::vector<int> arr;
    std::vector<int> sortedArr;

    mergeSortSequential(arr, 0, arr.size() - 1);
    assert(arr == sortedArr);

    mergeSortParallel(arr, 0, arr.size() - 1, 2);
    assert(arr == sortedArr);
}

int main()
{
    // Юніт-тести
    testMergeSort();
    testMergeSortEmptyArray();

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