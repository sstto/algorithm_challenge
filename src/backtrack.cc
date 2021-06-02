/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include <limits.h>
#include <algorithm>

Backtrack::Backtrack() {
    count = 0;
}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
    std::cout << "t " << query.GetNumVertices() << "\n";

    // find a root_vertex
    size_t root_vertex = 0;
    size_t factor = cs.GetCandidateSize(0)/query.GetDegree(0);
    for (size_t i = 0; i < query.GetNumVertices(); ++i) {
        if (factor > cs.GetCandidateSize(i)/query.GetDegree(i)){
            factor = cs.GetCandidateSize(i)/query.GetDegree(i);
            root_vertex = i;
        }
    }
    //=========================print ===============================
//    std::cout << "root vertex : " << root_vertex << std::endl;
    std::vector<size_t> embedding;
    embedding.resize(query.GetNumVertices(),ULONG_MAX);
//    for(auto it = embedding.begin(); it != embedding.end();it++){
//        std::cout << "a "<< *it << " ";
//    }
    //std::cout << std::endl;
    //=========================print ===============================
    //std::cout << "back tracking start" << std::endl;
    backtracking(data, query, cs, root_vertex, embedding);
    //=========================print ===============================
//    std::cout << "back tracking end" << std::endl;
}

void Backtrack::backtracking(const Graph &data, const Graph &query, const CandidateSet &cs,
                             Vertex u, std::vector<size_t> embedding) {
    std::vector<Vertex> visited;
    size_t startOffset = query.GetNeighborStartOffset(u);
    size_t endOffset = query.GetNeighborEndOffset(u);
    Vertex next_vertex; bool isEnd = true;
    size_t factor = ULONG_MAX;
    for(size_t i = startOffset; i <endOffset; i++){
        Vertex uNeighbor = query.GetNeighbor(i);
        if(embedding[uNeighbor] != ULONG_MAX){
            visited.push_back(uNeighbor);
        }else{
            //visited neigbor X
            size_t argmin = cs.GetCandidateSize(uNeighbor)/query.GetDegree(uNeighbor);
            if(factor > argmin){
                next_vertex = uNeighbor;
                factor = argmin;
                isEnd = false;
            }
        }
    }
    embedding[u] = ULONG_MAX-1;
    auto it_v = std::find(embedding.begin(),embedding.end(), ULONG_MAX);
    if(isEnd && (it_v != embedding.end())){
        size_t factor = ULONG_MAX;
        for (size_t i = 0; i < query.GetNumVertices(); ++i) {
            if(embedding[i] == ULONG_MAX) {
                if (factor > cs.GetCandidateSize(i) / query.GetDegree(i)) {
                    factor = cs.GetCandidateSize(i) / query.GetDegree(i);
                    next_vertex = i;
                }
            }
        }
    }
        //=========================print ===============================
    //std::cout << "next vertex of "<< u << " : "<< next_vertex << std::endl;

    for(size_t j = 0 ; j<cs.GetCandidateSize(u); j++){
        bool isValid = true;

        // injective
        size_t candidate = cs.GetCandidate(u, j);
        //std::cout << "vertex : " << u << " ,candidate v id : " << candidate << std::endl;
        auto it = std::find(embedding.begin(),embedding.end(), candidate);
        if(!(it == embedding.end())) continue;

        // common edge existence
        if(!visited.empty()) {
            for(auto it_visited = visited.begin(); it_visited != visited.end();it_visited++) {
                if (!data.IsNeighbor(embedding[*it_visited], candidate)) {
                    isValid = false;
                    break;
                }
            }
        }

        if(!isValid){
            continue;
        }else{
            embedding[u] = candidate;
        }

        //std::cout<<"back"<<std::endl;

        if(isEnd && (it_v == embedding.end())){
            std::cout << count <<std::endl;
            std::cout << "a ";
            for(auto it = embedding.begin(); it != embedding.end();it++){
                std::cout << *it << " ";
            }
            std::cout << std::endl;
            //check(data, query, embedding);
//            std::cout << "isReal : " << check(data, query, embedding) << std::endl;
            count +=1;
            if(count == 100000){
                std::cout << "count = 100000" << std::endl;
                exit(0);
            }
            return;
        }

        backtracking(data, query, cs, next_vertex, embedding);

    }
    embedding[u] = ULONG_MAX;
}

bool Backtrack::check(const Graph &data, const Graph &query, const std::vector<size_t> embedding)
{
    bool isValid = true;
    for(size_t i = 0; i<query.GetNumVertices(); i++)
    {
        size_t startOffset = query.GetNeighborStartOffset(i);
        size_t endOffset = query.GetNeighborEndOffset(i);

        for(size_t j = startOffset; j <endOffset; j++) {
            Vertex uNeighbor = query.GetNeighbor(j);
            if(!data.IsNeighbor(embedding[i],embedding[uNeighbor]))
                isValid = false;
        }
    }
    return isValid;
}
//void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
//                                const CandidateSet &cs) {
//  std::cout << "t " << query.GetNumVertices() << "\n";
//
//  // implement your code here.
//
//  // make dag using query & candidateSet
//  Dag dag(query, cs);
//  std::vector<size_t> visited;
//  visited.resize(query.GetNumVertices(),-1);
//  //visited: embedding
//  //ec : embedding candidate : we have to check whether it is embedding or not. True -> check candidate size
//  std::set<Vertex> ec;
//
//  Vertex root_vertex = dag.GetRootVertex();
//
//
//
//
//  for(size_t i = 0; i < cs.GetCandidateSize(root_vertex); i++) {
//      size_t c = cs.GetCandidate(root_vertex, i);
//      visited[root_vertex] = c;
//      //backtracking(root_vertex, c, data, query, cs, dag, visited);
//      for(size_t j = 0; j<dag.GetDirectedNeighborSize(root_vertex); j++){
//          ec.insert(dag.GetDirectedNeighbor(root_vertex)[j]);
//      }
//
//      while(!ec.empty()){
//          Vertex next_vertex;size_t cs_size = ULONG_MAX;
//          for(auto it = ec.begin();it != ec.end();it++){
//              auto parents = dag.GetInvDirectedNeighbor(*it);
//              bool isValid = true;
//              for(auto it_p = parents.begin(); it_p != parents.end(); it_p++){
//                  if(visited[*it_p] == -1){
//                      isValid = false;
//                      break;
//                  }
//              }
//              if(isValid){
//                  if(cs_size > cs.GetCandidateSize(*it)){
//                      next_vertex = *it;
//                      cs_size = cs.GetCandidateSize(*it);
//                  }
//              }
//          }
//
//          ec.erase(next_vertex);
//
//
//
//      }
//
//
//  }
//}
//
//void Backtrack::backtracking(const Vertex &u, const size_t &c, const Graph &data, const Graph &query, const CandidateSet &cs, const Dag &dag,
//                             std::vector<size_t> visited) {
//
//    for(size_t j = 0; j<dag.GetDirectedNeighborSize(u); j++){
//        Vertex u2 = dag.GetDirectedNeighbor(u)[j];
//        std::vector<size_t> parentOfU2 = dag.GetInvDirectedNeighbor(u2);
//        size_t parentOfU2Size = dag.GetInvDirectedNeighborSize(u2);
//        for(size_t k = 0; k<cs.GetCandidateSize(u2); k++){
//            size_t next_c = cs.GetCandidate(u2, k);
//
//            // condition 3 : check the edge of candidate space
//            bool condition3 = data.IsNeighbor(c,next_c);
//            if(condition3){
//                for(size_t l = 0; l<parentOfU2Size; l++){
//                    condition3 = data.IsNeighbor(visited[parentOfU2[l]], next_c);
//                    if(!condition3) break;
//                }
//            }
//
//            // condition 1
//            bool condition1 = true;
//            for(size_t i = 0; i<visited.size(); i++) {
//                if(visited[i] == next_c){
//                    condition1 = false;
//                    break;
//                }
//            }
//            //condition3
//
//            if(condition1 & condition3){
//                visited[u2] = next_c;
//                backtracking(u2, next_c, data, query, cs, dag, visited);
//            }
//        }
//
//    }
//
//
//}


