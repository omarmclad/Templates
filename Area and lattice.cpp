int twiceArea (vector<pair<int,int>>v){
    v.push_back(v[0]);
    int n=v.size();
    int ans=0;
    for(int i=0;i<n-1;i++){
        ans+=(v[i].first*v[i+1].second);
        ans-=(v[i].second*v[i+1].first);
    }
    return(abs(ans));
}
int boundaryPoints(vector<pair<int,int>>v){
    int ans=0;
    int x=v.size();
    for(int i=0;i<x;i++){
        int j=(i+1)%x;
        ans+=abs(__gcd(v[j].first-v[i].first,v[j].second-v[i].second));
    }
    return ans;
}
int latticeInPolygon(vector<pair<int,int>>v){
    int b=boundaryPoints(v);
    int a=twiceArea(v)/2;
    return a-b/2+1;
}
