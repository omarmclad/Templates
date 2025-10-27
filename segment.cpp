#include <bits/stdc++.h>
using namespace std;
#define endl '\n'
#define int long long 
#define francesco_bernoulli  ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
#define yes {cout << "YES\n";}
#define no {cout << "NO\n";}
#define sortx(X) sort(X.begin(),X.end());
#define allr(x) (x).rbegin(),(x).rend()
#define all(x) (x).begin(), (x).end()
#define fx(n) fixed<<setprecision(n)
#define mine(v) *min_element(all(v))
#define maxe(v) *max_element(all(v))
/*---------------------------------------------------------------------------------------------------------------------------------------*/
// bool temp(int a,int b){
// if(a<=b){
//     return true;
// }
// }
struct seg{
    int s;
    vector<int>sum;
    void init(int n){
        s=1;
        while(s<n){
            s*=2;
        }
        sum.assign(2*s,0ll);
    }
    void build(vector<int> &a,int x,int lx,int rx){
        if(rx-lx==1){
            if(lx<a.size()){
                sum[x]=a[lx];
            }
            return;
        }
        int m=(lx+rx)/2;
        build(a,2*x+1,lx,m);
        build(a,2*x+2,m,rx);
        sum[x]=sum[2*x+1]+sum[2*x+2];
    }
    void build(vector<int> &a){
        build(a,0,0,s);
    }
    void set(int i,int v,int x,int lx,int rx){
        if(rx-lx==1){
            sum[x]=v;
            return;
        }
        int m=(lx+rx)/2;
        if(i<m){
            set(i,v,2*x+1,lx,m);
        }
        else{
            set(i,v,2*x+2,m,rx);
        }
        sum[x]=sum[2*x+1]+sum[2*x+2];
    }
    void set(int i,int v){
        set(i,v,0,0,s);
    }
    int sm(int l,int r,int x,int lx,int rx){
        if(lx>=r||l>=rx)return 0;
        if(lx>=l&&rx<=r)return sum[x];
        int m=(lx+rx)/2;
        int s1=sm(l,r,2*x+1,lx,m);
        int s2=sm(l,r,2*x+2,m,rx);
        return s1+s2;
    }
    int sm(int l,int r){
        return sm(l,r,0,0,s);
    }
};
void VoidVortexOfNothingness(){
    int n,m;
    cin>>n>>m;
    seg sg;
    sg.init(n);
    vector<int>v(n);
    for(int i=0;i<n;i++){
        cin>>v[i];
    }
    sg.build(v);
    while(m--){
        int c;
        int l,r;
        cin>>c>>l>>r;
        if(c==1){
            sg.set(l,r);
        }
        else{
            cout<<sg.sm(l,r)<<endl;
        }
    }
}
signed main()
{
    //freopen("mex.in", "r", stdin);
    //freopen("window.in", "r", stdin);
    //freopen("output.txt", "w", stdout);
   francesco_bernoulli
   int t=1;
    // cin>>t;
   while(t--){
   VoidVortexOfNothingness();
   }
    return 0;
}
//CIAO_McQUEEN//
