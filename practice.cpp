#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define ln "\n"
void solve(){
    int n;
    cin>>n;
    //for idleness
    // try reading input instead of outputing
    //try outputing with no flush
  
    //check memory limit
    //  vector<int> v;
    // while (true) {
    //     v.push_back(1);
    // }
  
    //check what verdict this gives
    //vector<int>v(2);
    //cout<<v[3];
  
    //check runtime or WA 
    //cout<<n/0;
  
    //check Time limit or runtime
    //set<int>st;
    //st.erase(st.find(n));
  
    //Time limit also check how many operations the device can withstand in one sec
    // volatile int k=0;
    // for(int i=0;i<100000;i++){
    //     for(int j=0;j<100000;j++){
    //         k++;
    //     }
    // }
    cout<<(n%2||n==2?"NO":"YES");
}
int main(){
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int t = 1;
   // cin>>t;
    while(t--) solve();
    return 0;
}
