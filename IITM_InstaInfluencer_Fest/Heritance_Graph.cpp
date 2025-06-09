#include "Graph.cpp"

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
