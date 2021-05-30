#ifndef SUBGRAPH_MATCHING_CANDIDATE_SPACE_H
#define SUBGRAPH_MATCHING_CANDIDATE_SPACE_H

#include "common.h"
#include "graph.h"
#include "candidate_set.h"
#include "dag.h"
#include <queue>
#include <map>

class CandidateSpace:CandidateSet {
public:
    explicit CandidateSpace(const std::string& filename,const CandidateSet& cs, const Graph& data, const Dag& dag);
    ~CandidateSpace();

    inline std::vector<size_t> GetCandidateEdges(Vertex startDagVertex, Vertex endDagVertex, size_t candidateVertexID);


private:
    std::vector<std::vector<std::map<Vertex ,std::vector<size_t>>>> cs_edge;
    Dag dag;

};

inline std::vector<size_t> CandidateSpace::GetCandidateEdges(Vertex startDagVertex, Vertex endDagVertex, size_t candidateVertexID){
    std::vector<size_t> endDag = dag.GetDirectedNeighbor(startDagVertex);
    return cs_edge[startDagVertex][].find(candidateVertexID);
}




#endif //SUBGRAPH_MATCHING_CANDIDATE_SPACE_H
