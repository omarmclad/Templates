bool isprime(int n){
    if(n<2) return 0;
    for(int p : {2,3,5,7,11,13,17,19,23,29,31,37}){
        if(n%p==0) return n==p;
    }
    int d = n-1;
    int s = 0;
    while((d & 1) == 0){ d >>= 1; s++; }
    auto powmod = [](int a, int b, int m){
        int r=1;
        a %= m;
        while(b){
            if(b&1) r = (__int128)r * a % m;
            a = (__int128)a * a % m;
            b >>=1;
        }
        return r;
    };
    for(int a : {2,3,5,7,11,13,17,19,23,29,31,37}){
        if(a>=n) break;
        int x = powmod(a,d,n);
        if(x==1 || x==n-1) continue;
        bool composite = true;
        for(int r=1;r<s;r++){
            x = (__int128)x * x % n;
            if(x==n-1){ composite=false; break; }
        }
        if(composite) return false;
    }
    return true;
}
