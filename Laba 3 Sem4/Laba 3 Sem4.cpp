// Laba 3 Sem4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <thread>

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

        // Рекурсивно сортуємо першу та другу половину масиву
        mergeSortSequential(arr, left, middle);
        mergeSortSequential(arr, middle + 1, right);

        // Злиття відсортованих підмасивів
        merge(arr, left, middle, right);
    }
}

// Паралельний алгоритм сортування "Merge Sort"
void mergeSortParallel(std::vector<int>& arr, int left, int right, int numThreads)
{
    if (numThreads <= 1 || right - left < 1000) {
        mergeSortSequential(arr, left, right);
        return;
    }

    int middle = left + (right - left) / 2;

    // Створення потоків для рекурсивного сортування підмасивів
    std::thread leftThread(mergeSortParallel, std::ref(arr), left, middle, numThreads / 2);
    std::thread rightThread(mergeSortParallel, std::ref(arr), middle + 1, right, numThreads / 2);

    // Очікування завершення обох потоків
    leftThread.join();
    rightThread.join();

    // Злиття відсортованих підмасивів
    merge(arr, left, middle, right);
}

int main()
{
    // Вхідний масив
    std::vector<int> arr = { 9, 3, 2, 7, 1, 5, 6, 4, 8 };

    int numThreads = std::thread::hardware_concurrency(); // Отримання кількості доступних потоків

    // Паралельне сортування масиву
    mergeSortParallel(arr, 0, arr.size() - 1, numThreads);

    // Виведення відсортованого масиву
    std::cout << "Sorted array: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

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
