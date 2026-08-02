#include <bits/stdc++.h>
using namespace std;

struct SparseTable {
    int n;
    int max_log;
    vector<vector<int>> st;
    vector<int> lg;

    // Initialize and build the table
    void build(const vector<int>& arr) {
        n = arr.size();
        if (n == 0) return; // Fix: Prevent UB with __builtin_clz(0) if arr is empty

        // __builtin_clz finds the number of leading zeros, used to calculate log2 quickly
        max_log = 32 - __builtin_clz(n); 
        st.assign(n, vector<int>(max_log));
        lg.assign(n + 1, 0);

        // Precompute logarithms for O(1) query time
        for (int i = 2; i <= n; i++) {
            lg[i] = lg[i / 2] + 1;
        }

        // Base case: intervals of length 1
        for (int i = 0; i < n; i++) {
            st[i][0] = arr[i];
        }

        // Compute values for larger intervals
        for (int j = 1; j < max_log; j++) {
            for (int i = 0; i + (1 << j) <= n; i++) {
                // =========================================================
                // CHANGE OPERATION HERE (e.g., min, max, std::gcd, &, |)
                // =========================================================
                st[i][j] = max(st[i][j - 1], st[i + (1 << (j - 1))][j - 1]); 
            }
        }
    }

    // O(1) Query for range [L, R] (inclusive)
    int query(int L, int R) {
        int len = R - L + 1;
        int k = lg[len];
        // =========================================================
        // CHANGE OPERATION HERE (must match the operation above!)
        // =========================================================
        return max(st[L][k], st[R - (1 << k) + 1][k]); 
    }
};
