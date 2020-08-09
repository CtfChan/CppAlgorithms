#include <iostream>
#include <vector>
#include <math.h>


class MinSparseTable {
private:
    int n_;
    int P_;
    std::vector<int> log2_; // fast log2 lookup for 1 <= i <= n
    std::vector<std::vector<long>> dp_; // sparse table itself, holds best value in range
    std::vector<std::vector<long>> it_; // get best index for query in a range

public:
    MinSparseTable(std::vector<long>& values) {
        n_ = values.size();
        P_ = static_cast<int>(std::log(n_) / std::log(2));
        dp_ = std::vector<std::vector<long>>(P_ + 1, std::vector<long>(n_, 0));
        it_ =  std::vector<std::vector<long>>(P_ + 1, std::vector<long>(n_, 0));
        log2_ = std::vector<int>(n_ + 1, 0);

        // init dp and log table
        for (int i = 0; i < n_; ++i) {
            dp_[0][i] = values[i];
            it_[0][i] = i;
        }

        for (int i = 2; i <= n_; ++i) {
            log2_[i] = log2_[i / 2] + 1;
        }

        // build sparse table
        for (int p = 1; p <= P_; ++p) {
            for (int i = 0; i + (1 << p) <= n_; ++i) {
                long left_interval = dp_[p - 1][i];
                long right_interval = dp_[p - 1][i + (1 << (p - 1))];
                dp_[p][i] = std::min(left_interval, right_interval);
                it_[p][i] = (left_interval <= right_interval) ? 
                    it_[p - 1][i] : it_[p - 1][i + (1 << (p-1))];  
            }
        }
    }

    int queryMin(int l, int r) {
        int length = r - l + 1;
        int p = log2_[length];
        int k = 1 << p; // 2^p
        return std::min(dp_[p][l], dp_[p][r - k + 1]); // min is overlap friendly 
    }

    long queryMinIndex(int l, int r) {
        int length = r - l + 1;
        int p = log2_[length];
        int k = 1 << p;
        long left_interval = dp_[p][l];
        long right_interval = dp_[p][r - k + 1];
        return (left_interval <= right_interval) ? it_[p][l] : it_[p][r - k + 1];
    }

};

int main() {
    std::vector<long> values = {1, 2, -3, 2, 4, -1, 5};
    MinSparseTable sparseTable(values);

    std::cout << sparseTable.queryMin(1, 5) << std::endl; // 3
    std::cout << sparseTable.queryMinIndex(1, 5) << std::endl; // 2

    std::cout << sparseTable.queryMin(3, 3) << std::endl; // 2
    std::cout << sparseTable.queryMinIndex(3, 3) << std::endl; // 3
 
    std::cout << sparseTable.queryMin(3, 6) << std::endl; // - 1
    std::cout << sparseTable.queryMinIndex(3, 6) << std::endl; // 5

    return 0;
}