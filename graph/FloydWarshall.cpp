#include <iostream>
#include <vector>
#include <limits> 
#include <math.h>

template<typename T> 
using Matrix = std::vector<std::vector<T>>;

class FloydWarshallSolver {
private:
    int n_;
    bool solved_ = false;
    Matrix<double> dp_;
    Matrix<int> next_;

    constexpr static int REACHES_NEGATIVE_CYCLE = -1;



public:
    const Matrix<double> getCostMatrix() {
        solve();
        return dp_;
    }

    FloydWarshallSolver(const Matrix<double>& matrix) {
        n_ = matrix.size();
        dp_ = Matrix<double>(n_, std::vector<double>(n_));
        next_ = Matrix<int>(n_, std::vector<int>(n_ ));

        // copy values 
        for (int src = 0; src < n_; ++src) {
            for (int dst = 0; dst < n_; ++dst) {
                if (isinf(matrix[src][dst]) == false) 
                    next_[src][dst] = dst; // in path src->dst, next node to go to is dst
                dp_[src][dst] = matrix[src][dst];
            }
        }
    }

    void solve() {
        if (solved_)
            return;

        // compute all pairs of shortest paths
        for (int mid = 0; mid < n_; ++mid) {
            for (int src = 0; src < n_; ++src) {
                for (int dst = 0; dst < n_; ++dst) {
                    // update when src->mid->dst is cheaper than src->dst so far
                    if (dp_[src][mid] + dp_[mid][dst] < dp_[src][dst]) {
                        dp_[src][dst] = dp_[src][mid] + dp_[mid][dst];
                        next_[src][dst] = next_[src][mid];
                    }
                }
            } 
        }

        // identify negative cycles
        for (int mid = 0; mid < n_; ++mid) {
            for (int src = 0; src < n_; ++src) {
                for (int dst = 0; dst < n_; ++dst) {
                    if (dp_[src][mid] + dp_[mid][dst] < dp_[src][dst]) {
                        dp_[src][dst] = -1.0 * std::numeric_limits<double>::infinity();
                        next_[src][dst] = REACHES_NEGATIVE_CYCLE;
                    }                
                }
            } 
        }
        
        solved_ = true;

    }



};

Matrix<double> createGraph(int n) {
    Matrix<double> matrix(n, std::vector<double>(n, std::numeric_limits<double>::infinity() ));
    for (int i = 0; i < n; ++i)
        matrix[i][i] = 0;
    return matrix;
}

int main() {

    int n = 7;
    auto m = createGraph(n);

    // Add some edge values.
    m[0][1] = 2;
    m[0][2] = 5;
    m[0][6] = 10;
    m[1][2] = 2;
    m[1][4] = 11;
    m[2][6] = 2;
    m[6][5] = 11;
    m[4][5] = 1;
    m[5][4] = -2;

    auto solver = FloydWarshallSolver(m);
    auto dist = solver.getCostMatrix();

    for (int src = 0; src < n; src++)
      for (int dst = 0; dst < n; dst++)
        std::cout << "This shortest path from " << src << " " << dst << " is " << dist[src][dst] << std::endl;


    return 0;
}