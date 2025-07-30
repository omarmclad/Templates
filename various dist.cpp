double dist_Tri_Tri(vector<P> a,vector<P> b){
    double ans=1e10;
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            double hold=P::dist_segments(a[i],a[(i+1)%3],b[j],b[(j+1)%3]);
            ans=min(hold,ans);
        }
    }
    return ans;
}
double dist_Tri_sqr(vector<P> a,vector<P> b){
    double ans=1e10;
    for(int i=0;i<3;i++){
        for(int j=0;j<4;j++){
            double hold=P::dist_segments(a[i],a[(i+1)%3],b[j],b[(j+1)%4]);
            ans=min(hold,ans);
        }
    }
    return ans;
}
double dist_sqr_sqr(vector<P> a,vector<P> b){
    double ans=1e10;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            double hold=P::dist_segments(a[i],a[(i+1)%4],b[j],b[(j+1)%4]);
            ans=min(hold,ans);
        }
    }
    return ans;
}
double dist_circle_sqr(vector<circle> a,vector<P> b){
    double ans=1e10;
    for(auto [p,r]:a){
        for(int j=0;j<4;j++){
            P pp={p.x,p.y};
            double hold=pp.dist_to_segment(b[j],b[(j+1)%4]);
            ans=min(hold-r,ans);
        }
    }
    return ans;
}
double dist_circle_tri(vector<circle> a,vector<P> b){
    double ans=1e10;
    for(auto [p,r]:a){
        for(int j=0;j<3;j++){
            P pp={p.x,p.y};
            double hold=pp.dist_to_segment(b[j],b[(j+1)%3]);
            ans=min(hold-r,ans);
        }
    }
    return ans;
}
double dist_circle_circle(circle c,vector<circle> a){
    double ans=1e10;
    for(auto it:a){
        if(it==c)continue;
        double hold=c.p.dist(it.p);
        hold-=c.r;
        hold-=it.r;
        ans=min(hold,ans);
    }
    return ans;
}
