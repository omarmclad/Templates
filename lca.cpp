const int LOG=32;
const int x=2e5;
int up[x][LOG];
vector<int>children[x];
int depth[x];
bool vis[x];
void dfs(int a){
    vis[a]=1;
    for(int b:children[a]){
        if(vis[b])continue;
        up[b][0]=a;
        depth[b]=depth[a]+1;
        for(int j=1;j<LOG;j++){
            up[b][j]=up[up[b][j-1]][j-1];
        }
        dfs(b);
    }
}
int get_lca(int a,int b){
    if(depth[a]<depth[b])swap(a,b);
    int k=depth[a]-depth[b];
    for(int j=LOG-1;j>=0;j--){
        if(k&(1<<j)){
            a=up[a][j];
        }
    }
    if(a==b){
        return a;
    }
    for(int j=LOG-1;j>=0;j--){
        if(up[a][j]!=up[b][j]){
            a=up[a][j];
            b=up[b][j];
        }
    }
    return up[a][0];
}
void init(int n){
    for(int i=0;i<=n;i++){
        children[i].clear();
        depth[i]=0;
        vis[i]=0;
        for(int j=0;j<LOG;j++){
            up[i][j]=0;
        }
    }
}
int get_kth_ancestor(int u, int dist) {
    for (int j = 0; j < LOG; j++) {
        if ((dist >> j) & 1) {
            u = up[u][j];
        }
    }
    return u;
}
