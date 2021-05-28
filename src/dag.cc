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
            root_vertex = i;
        }
    }

    visited.resize(num_vertices_,false);

    //make dag using bfs.
    adj_list.resize(num_vertices_);

    std::queue<size_t> q;
    q.push(root_vertex);

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
                q.push(v_visit);
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