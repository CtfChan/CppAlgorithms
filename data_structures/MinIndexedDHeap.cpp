#include <vector>
#include <iostream>
#include <exception>

class MinIndexedDHeap {
private:
    
    int size_ = 0;

    // more efficient to preallocate
    int max_size_; 

    int degree_;

    std::vector<int> child_, parent_;

    // position map: ki -> node #
    std::vector<int> pm_;

    // inverse map: node # -> ki
    std::vector<int> im_;

    std::vector<int> values_; // assume input will be integers (can template this)

    void keyExistsOrThrow(int ki) {
        if (contains(ki) == false)
            throw std::invalid_argument("ki does not exist");
    }

    void keyInBoundsOrThrow(int ki) {
        if (ki < 0 || ki >= max_size_)
            throw std::range_error("ki value out of range");
    }

    void valueNotNullOrThrow(int value) {
        if (value == -1)
            throw std::invalid_argument("value is invalid");
    }

    void keyExistsAndValueNotNullOrThrow(int ki, int value) {
        keyExistsOrThrow(ki);
        valueNotNullOrThrow(value);
    }

    bool less(int node_num_a, int node_num_b) {
        return values_[im_[node_num_a]] < values_[im_[node_num_b]];
    }

    void swap(int node_num_a, int node_num_b) {
        // swap position map values
        pm_[im_[node_num_a]] = node_num_b;
        pm_[im_[node_num_b]] = node_num_a;

        // swap inverse map values
        int tmp = im_[node_num_a] ;
        im_[node_num_a] = im_[node_num_b];
        im_[node_num_b] = tmp;

        // // more elegant
        // std::swap(pm_[im_[node_num_a]],  pm_[im_[node_num_b]]);
        // std::swap(im_[node_num_a],  im_[node_num_b]);
    }

    // bubble up 
    void swim(int node_num) {
        // keep bubbling up node when it's less than its parent in value
        while(less(node_num, parent_[node_num])) {
            swap(node_num, parent_[node_num]);
            node_num = parent_[node_num];
        }
    }

    // find minimum child below node_num
    // returns -1 if no child found
    int minChild(int node_num) {
        int smallest_child = -1;
        int from = child_[node_num];
        int to = std::min(size_, from + degree_);
        for (int kid = from; kid < to; ++kid) {
            if (less(kid, node_num)) {
                smallest_child = kid;
                node_num = kid;
            }
        }
        return smallest_child;
    }

    // keep swapping a node with its smallest child 
    void sink(int node_num) {
        for (int child = minChild(node_num); child != -1; ) {
            swap(node_num, child);
            node_num = child;
            child = minChild(node_num); // or child = minChild(child)
        }
    }

    void isNotEmptyOrThrow() {
        if (isEmpty())
            throw std::domain_error("heap is empty");
    }

public:
    MinIndexedDHeap(int degree, int max_size) {
        degree_ = std::max(2, degree);
        max_size_ = std::max(degree_ + 1, max_size_);

        child_.resize(max_size_);
        parent_.resize(max_size_);
        pm_.resize(max_size_);
        im_.resize(max_size_);
        values_.resize(max_size_);

        for (int i = 0; i < max_size_; ++i) {
            parent_[i] = (i - 1) / degree_;
            child_[i] = i * degree_ + 1;
            pm_[i] = -1;
            im_[i] = -1;
        }
    }

    int size() {
        return size_;
    }

    bool isEmpty() {
        return size_ == 0;
    }

    bool contains(int ki) {
        keyInBoundsOrThrow(ki);
        return pm_[ki] != -1;
    }

    int peekMinKeyIndex() {
        isNotEmptyOrThrow();
        return im_[0];
    }

    int pollMinKeyIndex() {
        int min_ki = peekMinKeyIndex();
        remove(min_ki);
        return min_ki;
    }

    int peekMinValue() {
        isNotEmptyOrThrow();
        return values_[im_[0]];
    }

    int pollMinValue() {
        int min_value = peekMinValue();
        remove(peekMinKeyIndex());
        return min_value;
    }

    void insert(int ki, int value) {
        if (contains(ki))
            throw std::invalid_argument("ki already in use");

        pm_[ki] = size_;
        im_[size_] = ki;
        values_[ki] = value;
        swim(size_);
        ++size_;
    }

    int valueOf(int ki) {
        keyExistsOrThrow(ki);
        values_[ki];
    }

    int remove(int ki) {
        keyExistsOrThrow(ki);
        int node_num = pm_[ki];

        // swap node for ki with element at the end of heap
        // make node move up or down, depending on value
        swap(node_num, --size_);
        sink(node_num);
        swim(node_num);

        // clean up arrays and return value
        int value = values_[ki];
        values_[ki] = -1;
        pm_[ki] = -1;
        im_[size_] = -1;
        return value;
    }

    int update(int ki, int value) {
        keyExistsAndValueNotNullOrThrow(ki, value);
        int node_num = pm_[ki];
        int old_value = values_[ki];

        // update value of node, move it up/down accordingly
        values_[ki] = value;
        sink(node_num);
        swim(node_num);

        return old_value;
    }

    void decrease(int ki, int value) {
        keyExistsAndValueNotNullOrThrow(ki, value);
        if (less(value, values_[ki])) {
            values_[ki] = value;
            swim(pm_[ki]);
        }
    }

    void increase(int ki, int value) {
        keyExistsAndValueNotNullOrThrow(ki, value);
        if (less(values_[ki], value)) {
            values_[ki] = value;
            sink(pm_[ki]);
        }
    }

};


int main() {

    MinIndexedDHeap min_heap(2, 10);

    return 0;
}