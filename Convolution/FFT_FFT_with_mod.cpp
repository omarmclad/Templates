// compact fft

typedef complex<double> cd;
const double PI = acos(-1);

void fft(vector<cd> &a, bool inv) {
    int n = a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (inv ? -1 : 1);
        cd wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            cd w(1);
            for (int j = 0; j < len / 2; j++) {
                cd u = a[i + j], v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
    if (inv) for (cd &x : a) x /= n;
}

vector<int> multiply(const vector<int> &a, const vector<int> &b) {
    int need = a.size() + b.size() - 1, n = 1;
    while (n < need) n <<= 1;
    vector<cd> p(n);
    for (size_t i = 0; i < a.size(); i++) p[i].real(a[i]);
    for (size_t i = 0; i < b.size(); i++) p[i].imag(b[i]);
    
    fft(p, false);
    for (int i = 0; i < n; i++) p[i] *= p[i];
    fft(p, true);
    
    vector<int> res(need);
    for (int i = 0; i < need; i++) res[i] = round(p[i].imag() / 2.0);
    return res;
}

//Arbitrary Modulo Multiplication (Works for 10^9+7 or any dynamic MOD)
vector<long long> multiply_mod(const vector<long long> &a, const vector<long long> &b, long long MOD) {
    int need = a.size() + b.size() - 1, n = 1;
    while (n < need) n <<= 1;

    const long long C = 32768; // 2^15 ≈ sqrt(MOD)

    vector<cd> fa(n), fb(n);
    for (size_t i = 0; i < a.size(); i++) {
        long long x = (a[i] % MOD + MOD) % MOD;
        fa[i] = cd(x % C, x / C); // real = A_lo, imag = A_hi
    }
    for (size_t i = 0; i < b.size(); i++) {
        long long x = (b[i] % MOD + MOD) % MOD;
        fb[i] = cd(x % C, x / C); // real = B_lo, imag = B_hi
    }

    // 1. Forward FFTs
    fft(fa, false);
    fft(fb, false);

    // 2. Unpack components using complex symmetry and recombine
    vector<cd> fa_lo_hi(n), fb_lo_hi(n);
    for (int i = 0; i < n; i++) {
        int j = (n - i) % n;
        
        cd a_lo = (fa[i] + conj(fa[j])) * cd(0.5, 0);
        cd a_hi = (fa[i] - conj(fa[j])) * cd(0, -0.5);

        cd b_lo = (fb[i] + conj(fb[j])) * cd(0.5, 0);
        cd b_hi = (fb[i] - conj(fb[j])) * cd(0, -0.5);

        fa_lo_hi[i] = a_lo * b_lo + cd(0, 1) * a_lo * b_hi;
        fb_lo_hi[i] = a_hi * b_lo + cd(0, 1) * a_hi * b_hi;
    }

    // 3. Inverse FFTs
    fft(fa_lo_hi, true);
    fft(fb_lo_hi, true);

    // 4. Combine parts modulo MOD
    vector<long long> res(need);
    for (int i = 0; i < need; i++) {
        long long lo_lo = (long long)round(fa_lo_hi[i].real()) % MOD;
        long long lo_hi = (long long)round(fa_lo_hi[i].imag()) % MOD;
        long long hi_lo = (long long)round(fb_lo_hi[i].real()) % MOD;
        long long hi_hi = (long long)round(fb_lo_hi[i].imag()) % MOD;

        long long t1 = lo_lo;
        long long t2 = ((lo_hi + hi_lo) % MOD) * C % MOD;
        long long t3 = (hi_hi * ((C * C) % MOD)) % MOD;

        res[i] = (t1 + t2 + t3) % MOD;
    }

    return res;
}