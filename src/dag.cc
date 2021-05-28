#include "dag.h"



Dag::Dag(const Graph &query) {
    graph_id_ = query.GetGraphID();
    num_vertices_ = query.GetNumVertices();
    num_edges_ = query.GetNumEdges();
    num_labels_ = query.GetNumLabels();
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

