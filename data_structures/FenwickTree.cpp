#include <vector>
#include <iostream>

class FenwickTree {
private:
    int N_;
    std::vector<long> tree_;

    int lsb(int i ) {
        return i & -i;
    }

    // keep cascading down
    long prefixSum(int i ) {
        long sum = 0;
        while (i != 0) {
            sum += tree_[i];
            i &= ~lsb(i);
        }
        return sum;
    }

public:
    // assume values array is ONE based, so values[0] = 0
    FenwickTree(std::vector<long>& values) : tree_(values), N_(values.size()) {
        for (int i = 1; i < N_; ++i) {
            int parent = i + lsb(i);
            if (parent < N_) 
                tree_[parent] += tree_[i];
        }
    }

    long sum(int left, int right) {
        return prefixSum(right) - prefixSum(left - 1);
    }

    // add to idx i, value v
    void add(int i, long v) {
        // cascade upwards
        while (i < N_) {
            tree_[i] += v;
            i += lsb(i);
        }
    }

    // set tree[i] = v
    void set(int i, long v) {
        add(i, v - sum(i, i));
    }

};

int main() {

    return 0;
}