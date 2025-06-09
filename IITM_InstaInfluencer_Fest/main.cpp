#include "Heritance_Graph.cpp"


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
