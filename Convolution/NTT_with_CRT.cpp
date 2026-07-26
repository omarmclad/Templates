/*
  ==============================================================================
  WHEN TO USE THIS TEMPLATE (3-Prime NTT + Garner's CRT)
  ==============================================================================
  1. Modulo M is Arbitrary / Dynamic:
     Use when M is NOT an NTT-friendly prime (e.g., M = 10^9 + 7, M = 10^9, 
     or M is read at runtime from input).

  2. Pure Integer Precision Required (No Floating-Point Errors):
     Use when precision loss from double-based MTT (FFT) is unacceptable or risky.

  ==============================================================================
  DECISION MATRIX (WHICH ALGORITHM TO PICK?)
  ==============================================================================
  - Is M = 998,244,353? 
    --> DO NOT USE THIS. Use Standard 1-Pass NTT (3x faster, much smaller code).

  - Is M arbitrary, but Time Limit is strict (TL < 0.5s) / Memory tight?
    --> Use Arbitrary Modulo FFT (MTT / 4-pass complex FFT). 
        MTT needs 4 transform calls vs 9 calls in 3-Prime NTT.

  - Is M arbitrary AND 100% exact integer math is needed?
    --> USE THIS TEMPLATE (3-Prime NTT + CRT).
  ==============================================================================
*/

// Standard 3-Prime Set (All share primitive root G = 3)
const long long P1 = 998244353, P2 = 469762049, P3 = 167772161;
const long long G = 3;

long long qpow(long long b, long long e, long long mod) {
    long long r = 1;
    for (b %= mod; e > 0; e >>= 1, b = (__int128)b * b % mod)
        if (e & 1) r = (__int128)r * b % mod;
    return r;
}

// Single-modulus NTT
void ntt(vector<long long> &a, bool inv, long long mod) {
    int n = a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        long long wlen = qpow(G, inv ? mod - 1 - (mod - 1) / len : (mod - 1) / len, mod);
        for (int i = 0; i < n; i += len) {
            long long w = 1;
            for (int j = 0; j < len / 2; j++) {
                long long u = a[i + j];
                long long v = (__int128)a[i + j + len / 2] * w % mod;
                a[i + j] = (u + v >= mod ? u + v - mod : u + v);
                a[i + j + len / 2] = (u - v < 0 ? u - v + mod : u - v);
                w = (__int128)w * wlen % mod;
            }
        }
    }
    if (inv) {
        long long inv_n = qpow(n, mod - 2, mod);
        for (long long &x : a) x = (__int128)x * inv_n % mod;
    }
}

// Helper to run multiplication under a specific prime modulus
vector<long long> ntt_single(vector<long long> a, vector<long long> b, int n, long long mod) {
    a.resize(n); b.resize(n);
    ntt(a, false, mod);
    ntt(b, false, mod);
    for (int i = 0; i < n; i++) a[i] = (__int128)a[i] * b[i] % mod;
    ntt(a, true, mod);
    return a;
}

// Arbitrary Modulo Polynomial Multiplication using 3-Prime NTT + Garner's CRT
vector<long long> multiply_arb_mod(const vector<long long> &a, const vector<long long> &b, long long MOD) {
    int need = a.size() + b.size() - 1;
    int n = 1;
    while (n < need) n <<= 1;

    // 1. Run NTT under P1, P2, P3
    vector<long long> r1 = ntt_single(a, b, n, P1);
    vector<long long> r2 = ntt_single(a, b, n, P2);
    vector<long long> r3 = ntt_single(a, b, n, P3);

    // Precompute constants for Garner's Algorithm
    long long inv_P1_P2 = qpow(P1, P2 - 2, P2);
    long long inv_P1P2_P3 = qpow((__int128)P1 * P2 % P3, P3 - 2, P3);

    vector<long long> res(need);

    // 2. Reconstruct coefficients using Garner's CRT
    for (int i = 0; i < need; i++) {
        long long c1 = r1[i];
        long long c2 = r2[i];
        long long c3 = r3[i];

        // Garner's step 1: x1 = c1
        long long x1 = c1;

        // Garner's step 2: x2 = (c2 - x1) * inv(P1) mod P2
        long long x2 = (__int128)(c2 - x1 % P2 + P2) % P2 * inv_P1_P2 % P2;

        // Garner's step 3: x3 = ((c3 - x1) * inv(P1) - x2) * inv(P2) mod P3
        long long x3 = (__int128)((c3 - x1 % P3 + P3) % P3 - x2 * P1 % P3 + P3) % P3 * inv_P1P2_P3 % P3;

        // 3. Final answer = (x1 + x2*P1 + x3*P1*P2) % MOD
        long long ans = x1 % MOD;
        ans = (ans + (__int128)x2 * (P1 % MOD)) % MOD;
        ans = (ans + (__int128)x3 * ((__int128)P1 * P2 % MOD)) % MOD;

        res[i] = ans;
    }

    return res;
}