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
    void printAdjList();
//    void bfs(const Vertex& v, const Graph& query);
private:
    int32_t graph_id_;

    size_t num_vertices_;
    size_t num_edges_;
    size_t num_labels_;

    std::vector<std::vector<size_t>> adj_list;
    std::vector<bool> visited;
};

#endif //SUBGRAPH_MATCHING_DAG_H
