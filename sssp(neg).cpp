//Input : n vertices (0-indexed), edge list ,source src
//Output: dist[v]=shortest dist from src (INF = unreachable)
//Return: true = no negative cycle reachable from src
//        false = negative cycle detected
//Complexity : O(nm) time, O(n+m) space
//Template slots:
//  EDGE_SET  -- what edges exist (given, or constructed)
//  SCHEDULE  -- all edges each round (-> SPFA: layered queue)
//  STOP_COND -- n-1 rounds           (-> SPFA:queue empties before nth layer)
//  QUERY     --dist[] values or cycle detection boolean

struct Edge{
    int u,v,w;
};
bool bellmanFord(int n,const vector<Edge>& edges,int src ,vector<int>& dist){
    dist.assign(n,INF);
    dist[src]=0;
    vector<int> parent(n,-1);
    int lst_relax;
    //SCHEDULE: n-1 rounds over all edges
    for(int i=0;i<n;i++){    //STOP_COND
        lst_relax=-1;
        for(auto &[u,v,w]:edges){
            //Gaurd: never relax through unreachable node (INF + w overflows)
            if(dist[u]<INF && dist[u]+w<dist[v]){
                dist[v]=dist[u]+w;
                lst_relax=v;
                parent[v]=u;
            }
        }
        if(lst_relax==-1)break;
    }

    //QUERY: n-th pass = negative cycle detection 
    if (lst_relax != -1) {
        // Cycle detected. Backtrack N times to enter the cycle loop.
        int curr = lst_relax;
        for (int i = 0; i < n; i++) curr = parent[curr];
        vector<int> cycle;
        for (int v = curr; ; v = parent[v]) {
            cycle.push_back(v);
            if (v == curr && cycle.size() > 1) break;
        }
        reverse(all(cycle));
        for (int node : cycle) cout << node << " ";
        cout << el;
        return false;
    }
    return 1;
}
bool spfa(const vector<vector<pair<int,int>>>& adj,int src){
    int n=adj.size(); 
    vector<int>dist(n,INF),parent(n,-1),cnt(n,0);
    vector<bool>inqueue(n);
    dist[src]=0;
    queue<int>q;
    q.push(src);
    inqueue[src]=1;
    while(!q.empty()){     //outer: n-- counts BF rounds
        int u=q.front();
        q.pop();
        inqueue[u]=false;
        for(auto &[v,w]:adj[u]){
            int d=dist[u]+w;
            if(dist[v]>d){  //same relaxation as BF
                dist[v]=d;
                parent[v]=u;
                if(!inqueue[v]){
                    inqueue[v]=1;
                    q.push(v);
                    cnt[v]++;
                    if(cnt[v]>=n){
                        // Negative cycle detected at node v
                        int curr = v;
                        for (int i = 0; i < n; i++) curr = parent[curr];
                        
                        vector<int> cycle;
                        for (int node = curr; ; node = parent[node]) {
                            cycle.push_back(node);
                            if (node == curr && cycle.size() > 1) break;
                        }
                        reverse(all(cycle));
                        
                        for (int x : cycle) cout << x << " ";
                        cout << el;
                        return false;
                    }
                }
            }
        }
    }
    return 1;
}
