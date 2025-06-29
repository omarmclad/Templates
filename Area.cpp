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
