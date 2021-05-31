/**
 * @file backtrack.cc
 *
 */

#include <dag.h>
#include "backtrack.h"
#include <limits.h>

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
  //visited: embedding
  //ec : embedding candidate : we have to check whether it is embedding or not. True -> check candidate size
  std::set<Vertex> ec;

  Vertex root_vertex = dag.GetRootVertex();




  for(size_t i = 0; i < cs.GetCandidateSize(root_vertex); i++) {
      size_t c = cs.GetCandidate(root_vertex, i);
      visited[root_vertex] = c;
      //backtracking(root_vertex, c, data, query, cs, dag, visited);
      for(size_t j = 0; j<dag.GetDirectedNeighborSize(root_vertex); j++){
          ec.insert(dag.GetDirectedNeighbor(root_vertex)[j]);
      }

      while(!ec.empty()){
          Vertex next_vertex;size_t cs_size = ULONG_MAX;
          for(auto it = ec.begin();it != ec.end();it++){
              auto parents = dag.GetInvDirectedNeighbor(*it);
              bool isValid = true;
              for(auto it_p = parents.begin(); it_p != parents.end(); it_p++){
                  if(visited[*it_p] == -1){
                      isValid = false;
                      break;
                  }
              }
              if(isValid){
                  if(cs_size > cs.GetCandidateSize(*it)){
                      next_vertex = *it;
                      cs_size = cs.GetCandidateSize(*it);
                  }
              }
          }

          ec.erase(next_vertex);



      }


  }
}

void Backtrack::backtracking(const Vertex &u, const size_t &c, const Graph &data, const Graph &query, const CandidateSet &cs, const Dag &dag,
                             std::vector<size_t> visited) {

    for(size_t j = 0; j<dag.GetDirectedNeighborSize(u); j++){
        Vertex u2 = dag.GetDirectedNeighbor(u)[j];
        std::vector<size_t> parentOfU2 = dag.GetInvDirectedNeighbor(u2);
        size_t parentOfU2Size = dag.GetInvDirectedNeighborSize(u2);
        for(size_t k = 0; k<cs.GetCandidateSize(u2); k++){
            size_t next_c = cs.GetCandidate(u2, k);

            // condition 3 : check the edge of candidate space
            bool condition3 = data.IsNeighbor(c,next_c);
            if(condition3){
                for(size_t l = 0; l<parentOfU2Size; l++){
                    condition3 = data.IsNeighbor(visited[parentOfU2[l]], next_c);
                    if(!condition3) break;
                }
            }

            // condition 1
            bool condition1 = true;
            for(size_t i = 0; i<visited.size(); i++) {
                if(visited[i] == next_c){
                    condition1 = false;
                    break;
                }
            }
            //condition3

            if(condition1 & condition3){
                visited[u2] = next_c;
                backtracking(u2, next_c, data, query, cs, dag, visited);
            }
        }

    }


}


