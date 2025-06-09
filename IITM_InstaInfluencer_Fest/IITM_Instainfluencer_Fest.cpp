#include<iostream>
#include<vector>
#include<algorithm>
#include<stack>
#include<map>
#include<queue>
#include<unordered_set>

using namespace std;

/*bool cmp(pair<int,int>a,pair<int,int>b) {
    return a > b; 
}*/
bool compare(pair<int,int>a,pair<int,int>b){
    return a.first>b.first || (a.first == b.first && a.second > b.second);
}

class Graph{
    public:
        int V; //no of vertices
        vector<vector<int>>adjlist;//adjacent list
        vector<int>cardinalities;//size == no of cycles
        vector<int>color;//white = 0,Grey = 1,Black = 2
        vector<pair<int,int>>time;//discovery and finish time
        int timer;//time variable
        vector<vector<int>>adjrev;//adjacent list of transpose graph
        vector<int>DFSvers;//vertices on running DFS
        vector<vector<int>>SCCs;//vector of SCCs
        int noscc,maxscc;//noscc = number of SCCs,maxscc = maximum cardinality of SCCs 
        vector<int>hype;//hype values of vertices
        vector<int>scchype;//hype values of SCCs
        map<int,int>nodescc;//scc as a vertex of graph(vertex value to corresponding SCC)
        vector<vector<int>>condensed;//condensed graph of SCCs
        
        //constructor
        Graph(int N,vector<vector<int>>adj,vector<int>h){
            V = N;
            adjlist = adj;
            color.assign(N+1,0); //white
            hype = h;
            time.assign(N+1,{0,0});
            adjrev.resize(N+1);
            timer = 0;
        }
        
        //to find if cycles existed or not if existed store cardinalities
        void DFS(){
            int t = 0;
            
            for(int i = 1;i<=V;i++){
                if(color[i]==0){
                    int d = 0;//cycle length
                    DFSvers.push_back(i);
                    DFSvisit(i,t,d);
                }
            }
        }
        void DFSvisit(int u,int &t,int &d){
            t = t+1;
            d = d+1;
            time[u].first = t;
            color[u] = 1;//grey
            for(auto v : adjlist[u]){
                if(color[v] == 0){
                    DFSvers.push_back(v);
                    DFSvisit(v,t,d);
                }else if(color[v] == 1){
                    int cycleLength = t-time[v].first + 1;
                    cardinalities.push_back(cycleLength);
                }
            }
            t = t+1;
            time[u].second = t;
            color[u] = 2;
        }
        void DFS2(vector<pair<int,int>>& v){
            int t = 0;
            
            for(auto i : v){
                if(color[i.second]==0){
                    int d = 0;//cycle length
                    vector<int>vec2;
                    DFS2visit(i.second,t,d,v,vec2);
                    SCCs.push_back(vec2);
                }
            }
        }
        void DFS2visit(int u,int &t,int &d,vector<pair<int,int>>& vec,vector<int>&vec2){
            t = t+1;
            d = d+1;
            vec2.push_back(u);
            time[u].first = t;
            color[u] = 1;//grey
            for(auto v : adjlist[u]){
                if(color[v] == 0){
                    
                    DFS2visit(v,t,d,vec,vec2);
                }else if(color[v] == 1){
                    int cycleLength = t - time[v].first + 1;
                     cardinalities.push_back(cycleLength);
                }
            }
            t = t+1;
            time[u].second = t;
            color[u] = 2;
        }
        void SCCcalci(){
            DFS();
            vector<pair<int,int>> DFSftime;
            for(int i=0;i<V;i++){
                DFSftime.push_back({time[DFSvers[i]].second,DFSvers[i]});
            }
            sort(DFSftime.begin(),DFSftime.end(),compare);
            transpose();
            Graph rev(V,adjrev,hype);
            rev.DFS2(DFSftime);
            SCCs = rev.SCCs;
            noscc = rev.SCCs.size();
            maxscc = 0;
            for(auto p : rev.SCCs){
                if(maxscc<(int)p.size()){
                    maxscc=(int)p.size();
                }
                maxscc = max(maxscc,(int)p.size());
            }
            
        }
        //transpose graph of G
        void transpose(){
            for(int i=1;i<V+1;i++){
                for(auto v:adjlist[i]){
                    adjrev[v].push_back(i);
                }
            }
        }
        //condensed graph
        //vertices as SCCs
        void shrink(){
            condensed.resize(noscc);
            scchype.assign(noscc,0);
            for(int i=0;i<noscc;i++){
                for(int j : SCCs[i]){
                    nodescc[j] = i;
                    scchype[i] += hype[j-1];
                }
            }
            //to not get duplicate edges
            vector<unordered_set<int>>temp_graph(noscc);
            for(int u=1;u<=V;u++){
                for(int v : adjlist[u]){
                    int sccu = nodescc[u];
                    int sccv = nodescc[v];
                    if(sccu != sccv){
                        temp_graph[sccu].insert(sccv);
                    }
                }
            }
            //unordered_set to vector
            for (int i = 0; i < noscc; i++) {
                condensed[i] = vector<int>(temp_graph[i].begin(),temp_graph[i].end());
            }
        }
        
};

struct ftime{
    Graph &graph;
    ftime(Graph &g):graph(g){}
    //comparator functor
    bool operator()(int u,int v){
        //for topological order we need to sort the DFS order by finish time
        //so compare finish times of vertices 
        return graph.time[u].second > graph.time[v].second;
    }
};

//parent class
class GraphAlgorithm{
    public:
        virtual void Query(Graph &graph,vector<int>&hype)=0;
}; 
//chlid classes
//Qeury 1
class isCycle : public GraphAlgorithm{
    public:
        //Detects cycles in the graph.
        void Query(Graph &graph,vector<int>&hype) override {
            graph.DFS();
            if(!graph.cardinalities.empty()){
                cout << "YES" << endl;
            }else{
                cout << "NO" << endl;
            }
            return;
        }
};
//Query 2
class indepComponent : public GraphAlgorithm{
    public:
        
        void Query(Graph &graph,vector<int>&hype) override {
            graph.SCCcalci();
            cout << graph.noscc << " " << graph.maxscc << endl;
            return;
        }

};
//Qeury 3
class validOrder : public GraphAlgorithm{
    public:
        //Computes a valid topological order if possible.
        void Query(Graph &graph,vector<int>&hype) override {
            vector<int>valid;//final order vector
            vector<int>children(graph.V+1,0);//to keep trak of how many parents are there
            for(int i=1;i<graph.V+1;i++){
                for(auto v : graph.adjlist[i]){
                    children[v]++;
                }
            }
            priority_queue<int,vector<int>,greater<int>>orphan;
            for(int i=1;i<graph.V+1;i++){
                if(children[i]==0){
                    orphan.push(i);//pushing the parents(source where indegree == 0)
                }
            }
            while(!orphan.empty()){
                int u = orphan.top();
                orphan.pop();//remove the parent
                valid.push_back(u);
                for(auto v : graph.adjlist[u]){
                    children[v]--;//removing the parent decreases the the chilren's value
                    if(children[v] == 0){
                        orphan.push(v);//pushing the orphans(parent less children)
                    }
                }
            }
            //if size == V then all are independent 
            if(valid.size() != graph.V){
                cout << "NO" << endl;
            }else{
                for(auto i : valid){
                    cout << i << " ";
                }
                cout << endl;
            }
            return;
        }
};
//Qeury 4       
class maxHype : public GraphAlgorithm{
    public:
        //Computes maximum hype points on DAGs.
        void Query(Graph &graph,vector<int>&hype) override {
            graph.SCCcalci(); // Compute SCCs
            graph.shrink();
            //Using dynamic programing(DP)
            vector<int>maxhype(graph.noscc,0);
            vector<int>enter(graph.noscc,0);
            for(int i=0;i<graph.noscc;i++){
                for(int v : graph.condensed[i]){
                    enter[v]++;
                }
            }
            // Topological sort using queue
            queue<int>q;
            for(int i=0;i<graph.noscc;i++){
                if(enter[i] == 0){
                    q.push(i);
                    maxhype[i] = graph.scchype[i];
                }
            }
            int maxHS = 0;
            while(!q.empty()){
                int u = q.front();
                q.pop();
                maxHS = max(maxHS,maxhype[u]);
                for(int v : graph.condensed[u]){
                    maxhype[v] = max(maxhype[v],maxhype[u]+graph.scchype[v]);
                    enter[v]--;
                    if(enter[v] == 0){
                        q.push(v);
                    }
                }
            }
            cout << maxHS << endl;//final
        }
};

int main(){
    int N,M;//nof vetices,no of directed edges
    cin >> N >> M;
    vector<int>hype(N);//vector of hype points
    for(int i=0;i<N;i++){
        cin >> hype[i];
    }
    vector<vector<int>>adjlist(N+1);
    int a,b;
    // input of directed edges
    for(int i=0;i<M;i++){
        cin >> a >> b;
        adjlist[a].push_back(b);
    }
    Graph graph(N,adjlist,hype);
    int Q;//no of queries
    cin >> Q;
    int type;//Qeury type       
    for(int i=0;i<Q;i++){
        cin >> type;
        GraphAlgorithm* qry;
        if(type == 1){
            qry = new isCycle(); 
            qry->Query(graph,hype);
        }else if(type == 2){
            qry = new indepComponent();
            qry->Query(graph,hype);
        }else if(type == 3){
            qry = new validOrder();
            qry->Query(graph,hype);
        }else if(type == 4){
            qry = new maxHype();
            qry->Query(graph,hype);
        }
    }
    return 0;
}
