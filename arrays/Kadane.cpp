#include <iostream>
#include <vector>

using namespace std;

int maxSubArray(vector<int> &nums)
{
    int curr_max = nums[0]; // max ending here
    int max_so_far = nums[0];
    for (int i = 1; i < nums.size(); ++i)
    {
        curr_max = max(nums[i], curr_max + nums[i]);
        max_so_far = max(max_so_far, curr_max);
    }
    return max_so_far;
}

int main()
{
    vector<int> vec = {-2,1,-3,4,-1,2,1,-5,4};
    std::cout << maxSubArray(vec) << std::endl; // 6
    return 0;
}