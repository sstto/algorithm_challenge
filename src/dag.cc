#include "dag.h"



Dag::Dag(const Graph &query, const CandidateSet &cs) {
    graph_id_ = query.GetGraphID();
    num_vertices_ = query.GetNumVertices();
    num_edges_ = query.GetNumEdges();
    num_labels_ = query.GetNumLabels();

// find a root vertex using argmin of |C|/|Degree|
    size_t root_vertex = 0;
    size_t factor = cs.GetCandidateSize(0)/query.GetDegree(0);
    for (size_t i = 0; i < num_vertices_; ++i) {
        if (factor > cs.GetCandidateSize(i)/query.GetDegree(i)){
            factor = cs.GetCandidateSize(i)/query.GetDegree(i);
            root_vertex = i;
        }
    }
    std::cout << "root is " << root_vertex << std::endl;
    visited.resize(num_vertices_,false);
    std::vector<bool> qVisited(num_vertices_, false);
    //make dag using bfs.
    adj_list.resize(num_vertices_);

    // check adjarray of query
//    for(size_t i = 0; i<2*query.GetNumEdges();i++){
//        std::cout << query.GetNeighbor(i) <<std::endl;
//    }

    std::queue<size_t> q;
    q.push(root_vertex);
    qVisited[root_vertex] = true;

    while(!q.empty()){
        size_t x = q.front();
        q.pop();
        visited[x] = true;
        size_t startOffset = query.GetNeighborStartOffset(x);
        size_t endOffset = query.GetNeighborEndOffset(x);

        for(size_t i = startOffset; i< endOffset;i++){ //check i <= or <
            size_t v_visit = query.GetNeighbor(i);
            if (!visited[v_visit]){
                adj_list[x].push_back(v_visit);
            }
            if (!qVisited[v_visit]){
                q.push(v_visit);
                qVisited[v_visit] = true;
            }
        }
    }



}

int32_t Dag::GetGraphID() const {
    return graph_id_;
}

size_t Dag::GetNumVertices() const {
    return num_vertices_;
}

size_t Dag::GetNumEdges() const {
    return num_edges_;
}

size_t Dag::GetNumLabels() const {
    return num_labels_;
}


//void Dag::bfs(const Vertex &v, const Graph& query) {
//
//
//}
void Dag::printAdjList() {
    for(size_t i = 0; i< adj_list.size(); i++){
        std::cout << "vertex " << i << ' ' << std::endl;
        for(size_t j = 0; j<adj_list[i].size();j++){
            std::cout << adj_list[i][j] << std::endl;
        }
        std::cout << "==============="<<std::endl;
    }
}

Dag::~Dag() {

}
