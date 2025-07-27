#include <bits/stdc++.h>
using namespace std;
#define int long long
const int MOD = 1e9 + 7;
const int MAX = 1000;  // Change based on max n you expect

int C[MAX + 1][MAX + 1]; // Pascal Triangle for binomial coefficients
int fib[MAX + 2];        // Fibonacci numbers

// Precompute all factorial-based binomial coefficients
void precompute_binomial() {
    for (int n = 0; n <= MAX; ++n) {
        C[n][0] = C[n][n] = 1;
        for (int k = 1; k < n; ++k) {
            C[n][k] = C[n - 1][k - 1] + C[n - 1][k];
            C[n][k] %= MOD;
        }
    }
}

// Symmetry Rule: C(n, k) == C(n, n - k)
int symmetry_rule(int n, int k) {
    return C[n][k]; // same as C[n][n-k]
}

// Factoring: C(n, k) = n / k * C(n-1, k-1)
int factoring_rule(int n, int k) {
    if (k == 0 || k == n) return 1;
    return (n * C[n - 1][k - 1] / k) % MOD;
}

// ∑ C(n, k) = 2^n
int sum_over_k(int n) {
    return (1LL << n) % MOD;
}

// ∑ C(n, k) over n = C(n+1, k+1)
int sum_over_n(int n, int k) {
    return C[n + 1][k + 1];
}

// ∑ C(n + k, k) = C(n + m + 1, m)
int sum_over_n_and_k(int n, int m) {
    return C[n + m + 1][m];
}

// Sum of squares: ∑ C(n, k)^2 = C(2n, n)
int sum_of_squares(int n) {
    return C[2 * n][n];
}

// Weighted sum: ∑ k * C(n, k) = n * 2^(n - 1)
int weighted_sum(int n) {
    return (n * (1LL << (n - 1))) % MOD;
}

// Connection with Fibonacci numbers
// C(n, 0) + C(n-1, 1) + ... + C(n-k, k) = Fib(n+1)
void precompute_fib() {
    fib[0] = 0, fib[1] = 1;
    for (int i = 2; i <= MAX + 1; ++i)
        fib[i] = (fib[i - 1] + fib[i - 2]) % MOD;
}
int binomial_fib_sum(int n) {
    return fib[n + 1]; // directly from Fibonacci
}
