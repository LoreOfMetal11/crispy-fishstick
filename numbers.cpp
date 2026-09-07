#include "numbers.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>

using namespace std;

bool isnum(int number)
{
    if (number < 2)
    {
        return false;
    }

    for (int d = 2; d <= number / d; d++)
    {
        if (number % d == 0)
        {
            return false;
        }
    }

    return true;
}

void square(vector<int>& nums)
{
    transform(nums.begin(), nums.end(), nums.begin(), [](int x)
        {
            return isnum(x) ? x * x : x;
        });
}

std::vector<int> mixed(const std::vector<int>& nums)
{
    return std::vector<int>();
}

vector<int> mixed(const vector<int>& nums)
{
    vector<int> sorted = nums;

    auto middle = stable_partition(sorted.begin(), sorted.end(), [](int x)
        {
            return x % 2 != 0;
        });

    sort(sorted.begin(), middle);
    sort(middle, sorted.end(), greater<int>());

    return sorted;
}

vector<int> pickrange(const vector<int>& nums, int low, int high)
{
    vector<int> picked;

    copy_if(nums.begin(), nums.end(), back_inserter(picked),
        [low, high](int x)
        {
            return x >= low && x <= high;
        });

    sort(picked.begin(), picked.end());
    picked.erase(unique(picked.begin(), picked.end()), picked.end());

    return picked;
}

void prnums(const vector<int>& nums)
{
    for (int x : nums)
    {
        cout << x << ' ';
    }

    cout << '\n';
}