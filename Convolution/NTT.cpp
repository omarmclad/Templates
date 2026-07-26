//NTT 
const int MOD = 998244353, G = 3;

long long qpow(long long b, long long e) {
    long long r = 1;
    for (b %= MOD; e > 0; e >>= 1, b = b * b % MOD)
        if (e & 1) r = r * b % MOD;
    return r;
}

void ntt(vector<long long> &a, bool inv) {
    int n = a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        long long wlen = qpow(G, inv ? MOD - 1 - (MOD - 1) / len : (MOD - 1) / len);
        for (int i = 0; i < n; i += len) {
            long long w = 1;
            for (int j = 0; j < len / 2; j++) {
                long long u = a[i + j], v = a[i + j + len / 2] * w % MOD;
                a[i + j] = (u + v >= MOD ? u + v - MOD : u + v);
                a[i + j + len / 2] = (u - v < 0 ? u - v + MOD : u - v);
                w = w * wlen % MOD;
            }
        }
    }
    if (inv) {
        long long inv_n = qpow(n, MOD - 2);
        for (long long &x : a) x = x * inv_n % MOD;
    }
}

vector<long long> multiply(vector<long long> a, vector<long long> b) {
    int need = a.size() + b.size() - 1, n = 1;
    while (n < need) n <<= 1;
    a.resize(n); b.resize(n);

    ntt(a, false); ntt(b, false);
    for (int i = 0; i < n; i++) a[i] = a[i] * b[i] % MOD;
    ntt(a, true);

    a.resize(need);
    return a;
}