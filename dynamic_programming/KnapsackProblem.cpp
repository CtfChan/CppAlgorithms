#include <iostream>
#include <vector>
#include <utility>

std::pair<int, std::vector<int>> knapsack(const int total_capacity, 
    const std::vector<int>& weights, const std::vector<int>& values ) {
    // error case
    if (total_capacity < 0 || weights.empty() || values.empty())
        return {0, {}};

    size_t num_items = weights.size();

    // add one for conveneince
    std::vector<std::vector<int>> dp(num_items+1, std::vector<int>(total_capacity+1, 0) );

    // compute optimal value
    for (int item = 1; item <= num_items; ++item) {
        for (int capacity = 1; capacity <= total_capacity; ++capacity ) {
            int item_idx = item - 1;
            int weight = weights[item_idx];
            int value = values[item_idx];
                        
            // max of not picking current item and picking current item (if you can)
            if (capacity >= weight )
                dp[item][capacity] = std::max(dp[item-1][capacity], dp[item-1][capacity-weight] + value);
            else
                dp[item][capacity] = dp[item-1][capacity];
        }
    }

    // compute items required from dp table
    // dp[item][capacity] != dp[item-1][capacity] ===> item was selected
    std::vector<int> selected_items;
    int capacity = total_capacity;
    for (int item = num_items; item > 0; --item) {
        if (dp[item][capacity] != dp[item-1][capacity]) {
            int item_idx = item - 1;
            capacity -= weights[item_idx];
            selected_items.push_back(item_idx);
        }
    }


    return {dp[num_items][total_capacity], selected_items};
}


int main() {

    // max value = 12, (v,w) = (4,3), (8,5) 
    int capacity = 10;
    std::vector<int> values = {1, 4, 8, 5};
    std::vector<int> weights = {3, 3, 5, 6};
    auto [max_val1, items1] = knapsack(capacity, weights, values);
    std::cout << max_val1 << std::endl; 
    for (int item : items1)
        std::cout << item << " ";
    std::cout << std::endl;


    // max value = 10, (v,w) = (2, 1), (5, 4), (3, 2) 
    capacity = 7;
    values = {2, 2, 4, 5, 3};
    weights = {3, 1, 3, 4, 2};
    auto [max_val2, items2] = knapsack(capacity, weights, values);
    std::cout << max_val2 << std::endl;
    for (int item : items2)
        std::cout << item << " ";
    std::cout << std::endl;

    return 0;
}