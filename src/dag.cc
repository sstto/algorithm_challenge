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

