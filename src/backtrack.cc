/**
 * @file backtrack.cc
 *
 */

#include <dag.h>
#include "backtrack.h"

Backtrack::Backtrack() {
}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
  std::cout << "t " << query.GetNumVertices() << "\n";

  // implement your code here.

  // make dag using query & candidateSet
  Dag dag(query, cs);
  std::vector<size_t> visited;
  visited.resize(query.GetNumVertices(),-1);
  //
  Vertex root_vertex = dag.GetRootVertex();
  for(size_t i = 0; i < cs.GetCandidateSize(root_vertex); i++) {
      size_t c = cs.GetCandidate(root_vertex, i);
      visited[root_vertex] = c;
      backtracking(root_vertex, c, data, query, cs, dag, visited);
  }
}

void Backtrack::backtracking(const Vertex &u, const size_t &c, const Graph &data, const Graph &query, const CandidateSet &cs, const Dag &dag,
                             std::vector<size_t> visited) {
    //std::vector<size_t> candidateSetOfU = cs.GetCandidate(u,0 ~ #GetCandidateSize(u));
        // find edge of candidate space

    for(size_t j = 0; j<dag.GetDirectedNeighborSize(u); j++){
        Vertex u2 = dag.GetDirectedNeighbor(u)[j];
        for(size_t k = 0; k<cs.GetCandidateSize(u2); k++){
            size_t next_c = cs.GetCandidate(u2, k);
            if(data.IsNeighbor(c,next_c)){
                //1,3 조건 통과

                visited[u2] = next_c;
                backtracking(u2, next_c, data, query, cs, dag, visited);
            }
        }

    }

    }
}


