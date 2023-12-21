#include <iostream>
#include <cstdlib>
#include <vector>

#define SIZE 1000
#define SEED 10

using namespace std;

struct stats {
    size_t comp_count = 0;
    size_t copy_count = 0;

    friend stats operator+(const stats& left, const stats& right)
    {
        stats stats;
        stats.comp_count = left.comp_count + right.comp_count;
        stats.copy_count = left.copy_count + right.copy_count;
        return stats;
    }
    stats& operator=(const stats& right) {
        comp_count = right.comp_count;
        copy_count = right.copy_count;
        return *this;
    }
};

void print(const vector<int> arr) {
    for (size_t i = 0; i < arr.size(); ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

/// 
/// 
/// Сортировка выбором
///  
/// 
void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

stats selection_sort(std::vector<int>& arr)
{
    stats stats;
    int i, j, min_idx;

    for (i = 0; i < arr.size() - 1; i++)
    {
        min_idx = i;
        for (j = i + 1; j < arr.size(); j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
            stats.comp_count += 1;
        }
        stats.copy_count += 1;
        swap(&arr[min_idx], &arr[i]);
    }

    return stats;
}

/// 
/// 
/// Быстрая сортировка
///  
/// 

void quick_sort(vector<int>& arr, int left, int right, stats& sts)
{
    int i = left, j = right;
    int pivot = arr[left];
    while (i <= j)
    {
        while (arr[i] < pivot)
        {
            sts.comp_count++;
            i++;
        }
        while (arr[j] > pivot)
        {
            sts.comp_count++;
            j--;
        }
        if (i <= j)
        {
            swap(arr[i], arr[j]);
            i++;
            j--;
            sts.copy_count++;
        }
    }

    if (left < j)
        quick_sort(arr, left, j, sts);
    if (i < right)
        quick_sort(arr, i, right, sts);
}

stats quick_sort(vector<int>& arr, int size)
{
    stats sts;
    if (size < 2)
    {
        return sts;
    }
    quick_sort(arr, 0, size - 1, sts);
    return sts;
}

/// 
/// 
/// Сортировка естественным двухпутевым слиянием
///  
/// 

stats merge(vector<int>& arr, vector<int>& left, vector<int>& right, stats& sts)
{
    size_t left_size = left.size();
    size_t right_size = right.size();
    size_t i = 0, j = 0, k = 0;
    while (i < left_size && j < right_size)
    {
        sts.comp_count++;
        if (left[i] <= right[j])
        {
            arr[k] = left[i];
            ++i;
        }
        else
        {
            arr[k] = right[j];
            ++j;
        }
        sts.copy_count++;
        ++k;
    }
    while (i < left_size)
    {
        arr[k] = left[i];
        ++i;
        sts.copy_count++;
        ++k;
    }

    while (j < right_size)
    {
        arr[k] = right[j];
        ++j;
        sts.copy_count++;
        ++k;
    }
    return sts;
}

stats merge_sort(vector<int>& arr)
{
    size_t size = arr.size();
    stats sts;
    if (size < 2)
    {
        return sts;
    }
    size_t mid = size / 2;
    vector<int> left(arr.begin(), arr.begin() + mid);
    vector<int> right(arr.begin() + mid, arr.end());
    sts = merge_sort(left);
    sts = merge_sort(right);
    merge(arr, left, right, sts);
    return sts;
}

/// 
/// 
/// Создание тестовых векторов
///  
/// 

vector<int> sorted_vector(const size_t size)
{
    vector<int> arr;
    for (size_t i = 0; i < size; ++i)
    {
        arr.push_back(i);
    }
    return arr;
}

vector<int> reverse_vector(const size_t size)
{
    vector<int> arr = sorted_vector(size);
    std::reverse(arr.begin(), arr.end());
    return arr;
}

vector<int> random_vector(const size_t size, const int seed)
{
    vector<int> arr;
    srand(seed);
    for (size_t i = 0; i < size; ++i)
    {
        arr.push_back(rand() % 100);
    }
    return arr;
}


int main()
{
    {
        cout << "Selection sort for 10 random arrays, 1 sorted and 1 reversed" << endl << endl;
        size_t sum_comparison = 0, sum_copy = 0;
        for (int i = 0; i < 10; ++i) {
            vector<int> arr = random_vector(SIZE, SEED);
            stats tmp = selection_sort(arr);
            sum_comparison += tmp.comp_count;
            sum_copy += tmp.copy_count;
        }
        cout << "size: " << SIZE << endl;
        cout << "Average comps: " << sum_comparison / 10 << endl;
        cout << "Average copies: " << sum_copy / 10 << endl << endl;

        vector<int> sorted = sorted_vector(SIZE);
        stats sorted_stats = selection_sort(sorted);
        cout << "Sorted comps: " << sorted_stats.comp_count << endl;
        cout << "Sorted copies: " << sorted_stats.copy_count << endl << endl;

        vector<int> reversed = reverse_vector(SIZE);
        stats reversed_stats = selection_sort(reversed);
        cout << "Reversed comps: " << reversed_stats.comp_count << endl;
        cout << "Reversed copies: " << reversed_stats.copy_count << endl << endl;
    }
     {
        cout << "Quick sort for 10 random arrays, 1 sorted and 1 reversed" << endl << endl;
        size_t sum_comparison = 0, sum_copy = 0;
        for (int i = 0; i < 10; ++i) {
            vector<int> arr = random_vector(SIZE, SEED);
            stats tmp = quick_sort(arr, arr.size());
            sum_comparison += tmp.comp_count;
            sum_copy += tmp.copy_count;
        }
        cout << "size: " << SIZE << endl;
        cout << "Average comps: " << sum_comparison / 100 << endl;
        cout << "Average copies: " << sum_copy / 100 << endl << endl;

        vector<int> sorted = sorted_vector(SIZE);
        stats sorted_stats = quick_sort(sorted, sorted.size());
        cout << "Sorted comps: " << sorted_stats.comp_count << endl;
        cout << "Sorted copies: " << sorted_stats.copy_count << endl << endl;

        vector<int> reversed = reverse_vector(SIZE);
        stats reversed_stats = quick_sort(reversed, reversed.size());
        cout << "Reversed comps: " << reversed_stats.comp_count << endl;
        cout << "Reversed copies: " << reversed_stats.copy_count << endl << endl;
    }
    {
        cout << "Selection sort for 10 random arrays, 1 sorted and 1 reversed" << endl << endl;
        size_t sum_comparison = 0, sum_copy = 0;
        for (int i = 0; i < 10; ++i) {
            vector<int> arr = random_vector(SIZE, SEED);
            stats tmp = merge_sort(arr);
            sum_comparison += tmp.comp_count;
            sum_copy += tmp.copy_count;
        }
        cout << "size: " << SIZE << endl;
        cout << "Average comps: " << sum_comparison / 100 << endl;
        cout << "Average copies: " << sum_copy / 100 << endl << endl;

        vector<int> sorted = sorted_vector(SIZE);
        stats sorted_stats = merge_sort(sorted);
        cout << "Sorted comps: " << sorted_stats.comp_count << endl;
        cout << "Sorted copies: " << sorted_stats.copy_count << endl << endl;

        vector<int> reversed = reverse_vector(SIZE);
        stats reversed_stats = merge_sort(reversed);
        cout << "Reversed comps: " << reversed_stats.comp_count << endl;
        cout << "Reversed copies: " << reversed_stats.copy_count << endl << endl;
    }
}
