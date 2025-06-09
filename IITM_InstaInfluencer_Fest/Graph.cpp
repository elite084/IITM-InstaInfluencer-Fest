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
