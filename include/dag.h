//
// Created by 심영인 on 2021/05/28.
//

#ifndef SUBGRAPH_MATCHING_DAG_H
#define SUBGRAPH_MATCHING_DAG_H

#include "common.h"
#include "graph.h"
#include "candidate_set.h"
#include <queue>

class Dag {
public:
    explicit Dag(const Graph& query, const CandidateSet& cs);
    ~Dag();

    inline int32_t GetGraphID() const;

    inline size_t GetNumVertices() const;
    inline size_t GetNumEdges() const;
    inline size_t GetNumLabels() const;
    inline std::vector<size_t> GetDirectedNeighbor(Vertex v) const;
    inline size_t GetDirectedNeighborSize(Vertex v) const;
    inline std::vector<size_t> GetInvDirectedNeighbor(Vertex v) const;
    inline size_t GetInvDirectedNeighborSize(Vertex v) const;
    inline Vertex GetRootVertex() const;
    void printAdjList();
//    void bfs(const Vertex& v, const Graph& query);
private:
    int32_t graph_id_;

    size_t num_vertices_;
    size_t num_edges_;
    size_t num_labels_;
    Vertex root_vertex;

    std::vector<std::vector<size_t>> adj_list;
    std::vector<std::vector<size_t>> adj_list_inv;
    std::vector<bool> visited;
};
inline std::vector<size_t> Dag::GetDirectedNeighbor(Vertex v) const{
    return adj_list[v];
}
inline Vertex Dag::GetRootVertex() const{
    return root_vertex;
}
inline size_t Dag::GetDirectedNeighborSize(Vertex v) const{
    return adj_list[v].size();
}
inline std::vector<size_t> Dag::GetInvDirectedNeighbor(Vertex v) const{
    return adj_list_inv[v];
}
inline size_t Dag::GetInvDirectedNeighborSize(Vertex v) const{
    return adj_list_inv[v].size();
}

#endif //SUBGRAPH_MATCHING_DAG_H
