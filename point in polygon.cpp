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
struct P
{
    int x, y;

    void input() {
        cin >> x >> y;
    }
    P operator-(const P &b) const 
    {
        return {x - b.x, y - b.y};
    }
    double operator*(const P &b) const 
    {
        return x * b.y - y * b.x;
    }
    double triangle(const P &b, const P &c) const 
    {
        return (b - *this) * (c - *this);
    }
    bool on_segment(const P &a, const P &b) const {
        int area = (a - *this) * (b - *this);
        if (area != 0) return false;
        return min(a.x, b.x) <= x && x <= max(a.x, b.x) &&
               min(a.y, b.y) <= y && y <= max(a.y, b.y);
    }
};
void VoidVortexOfNothingness(){
    int n,q;
    cin>>n>>q;
    vector<P>v(n);
    for(int i=0;i<n;i++){
        v[i].input();
    }
    while(q--){
        P a;
        a.input();
        P b={a.x+1,2000'000'0001};
        int c=0;
        bool m=0;
        for(int i=0;i<n;i++){
            P p1=v[i];
            P p2=v[(i+1)%n];
            if(a.on_segment(v[i],v[(i+1)%n])){
                m=1;
                break;
            }
            if (p1.y > p2.y) swap(p1, p2);

            if (p1.y <= a.y && a.y < p2.y) {
                long long det = (p2 - p1) * (a - p1);
                if (det > 0) c++;
            }
        }
        if(m){
            cout<<"BOUNDARY\n";
        }
        else{
            if(c%2){
                cout<<"INSIDE\n";
            }
            else{
                cout<<"OUTSIDE\n";
            }
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
