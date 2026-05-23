#define adj(head,u,e,v) for(int e=head[u],v;~e &&((v=to[e]),1);e=nxt[e])
const int N=2e5+5;
const int M=4e5+5;
int head[N],to[M],nxt[M],ne;
int dfsT[N], low[N],cmpId[N],stk[N],sz,curT,cmpcnt;
int n;
void initGraph(int nNodes){
    n=nNodes;ne=0;
    memset(head,-1,n*sizeof(head[0]));
}
void addEdge(int u,int v){
    to[ne]=v;nxt[ne]=head[u];head[u]=ne++;
}
void tarjanDFS(int u){
    dfsT[u]=low[u]=curT++;
    stk[sz++]=u;
    adj(head,u,e,v){
        if(dfsT[v]==-1){ //unvisited ->tree edge
            tarjanDFS(v);
            low[u]=min(low[u],low[v]);
        } else if(cmpId[v]==-1){  // visited &still active -> back edge
            low[u]=min(low[u],low[v]);
        }
        // cmpId[v]!=-1 -> finished SCC, skip
    }
    if(low[u]==dfsT[u]){ //u is SCC root
        do{
            cmpId[stk[--sz]]=cmpcnt;
        }while(stk[sz]!=u);
        cmpcnt++;
    }
}
void tarjan(){
    curT=sz=cmpcnt=0;
    memset(dfsT,-1,n*sizeof(dfsT[0]));
    memset(cmpId,-1,n*sizeof(cmpId[0]));
    for(int i=0;i<n;i++){
        if(dfsT[i]==-1)tarjanDFS(i);
    }
}
/*=========================================================*/
#define adj(head,u,e,v) for(int e=head[u],v;~e &&((v=to[e]),1);e=nxt[e])
const int N=2e5+5;
const int M=4e5+5;
int head[N],rhead[N],to[M],rto[M],rnxt[M],nxt[M],ne,rne;
bool vis[N];
int stk[N],topOrder,cmpId[N];
int n;
void initGraph(int nNodes){
    n=nNodes;ne=rne=0;
    memset(head,-1,n*sizeof(head[0]));
    memset(rhead,-1,n*sizeof(rhead[0]));
}
void addEdge(int u,int v){
    to[ne]=v;nxt[ne]=head[u];head[u]=ne++;
    rto[ne]=v;rnxt[ne]=rhead[u];rhead[u]=rne++;
}
void dfs1(int u){
    vis[u]=1;
    adj(head,u,e,v){if(!vis[v])dfs1(v);}
    stk[topOrder++]=u;    //post-order
}
void dfs2(int u,int c){
    cmpId[u]=c;
    adj(rhead,u,e,v){if(cmpId[v]==-1)dfs2(v,c);}
}
int kosaraju(){
    memset(vis,0,n*sizeof(vis[0]));
    topOrder=0;
    for(int i=0;i<n;i++)if(!vis[i])dfs1(i);
    memset(cmpId,-1,n*sizeof(cmpId[0]));
    int cmpCnt=0;
    for(int i=topOrder-1;i>=0;i--){
        int u=stk[i];
        if(cmpId[u]==-1)dfs2(u,cmpCnt++);
    }
    return cmpCnt;
}
