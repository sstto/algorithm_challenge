/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include <limits.h>
#include <algorithm>


bool compare(const std::pair<Vertex,size_t> &a, const std::pair<Vertex,size_t> &b)
{
    return a.second < b.second;
}


Backtrack::Backtrack(){
    count = 0;
}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
    std::cout << "t " << query.GetNumVertices() << "\n";

    std::vector<Vertex> nextVertex;
    std::vector<Vertex> seqVertex;
    std::vector<std::vector<Vertex>> nextVisited;
    std::vector<std::vector<Vertex>> seqVisited;

    nextVertex.resize(query.GetNumVertices(), INT32_MAX);
    nextVisited.resize(query.GetNumVertices());

    // find a root_vertex

//    size_t factor = cs.GetCandidateSize(0)/query.GetDegree(0);
//    for (size_t i = 0; i < query.GetNumVertices(); ++i) {
//        if (factor > cs.GetCandidateSize(i)/query.GetDegree(i)){
//            factor = cs.GetCandidateSize(i)/query.GetDegree(i);
//            root_vertex = i;
//        }
//    }
//    std::cout << root_vertex <<std::endl;
//    std::vector<Vertex> toVisit;
//    std::vector<Vertex> visited;
//    std::cout << "start" <<std::endl;
//    toVisit.push_back(root_vertex);
//    while(!toVisit.empty()){
//        Vertex u = toVisit.back();
//        seqVertex.push_back(u);
//        seqVisited.push_back(visited);
//        toVisit.pop_back();
//
//        visited.push_back(u);
//
//        std::vector<Vertex> neighbor;
//        std::vector<std::pair<Vertex,size_t>> v;
//        size_t startOffset = query.GetNeighborStartOffset(u);
//        size_t endOffset = query.GetNeighborEndOffset(u);
//        for(size_t i = startOffset; i <endOffset; i++) {
//            Vertex uNeighbor = query.GetNeighbor(i);
//            v.push_back(std::pair<Vertex, size_t>(uNeighbor,cs.GetCandidateSize(uNeighbor)/query.GetDegree(uNeighbor)));
//
//        }
//
////        sort(neighbor.begin(), neighbor.end(), compare);
//
//
//        sort(v.begin(),v.end(),compare);
//
//        for(auto it = v.begin(); it < v.end();it++) {
//            neighbor.push_back((*it).first);
//        }
//        for(auto it = neighbor.begin(); it < neighbor.end(); it++){
//            if(std::find(visited.begin(), visited.end(),*it) == visited.end()){
////                visited.push_back(*it);
//                if(std::find(toVisit.begin(), toVisit.end(),*it) == toVisit.end()){
//                    toVisit.push_back(*it);
//                }
//            }
//        }
//    }
    std::vector<std::pair<Vertex,size_t>> v;
    for(Vertex i = 0; i <query.GetNumVertices(); i++) {
            v.push_back(std::pair<Vertex, size_t>(i,cs.GetCandidateSize(i)/query.GetDegree(i)));
    }
    sort(v.begin(), v.end(), compare);
    for(auto it = v.begin(); it < v.end();it++) {
            seqVertex.push_back((*it).first);
    }

    std::vector<Vertex> visit;
    for(auto it = seqVertex.begin(); it < seqVertex.end();it++){
        seqVisited.push_back(visit);
        visit.push_back(*it);
    }

    Vertex j = 0;
    for(auto it = seqVertex.begin(); it < seqVertex.end(); it++){
        if(it != seqVertex.end()-1){nextVertex[*it] = *(it+1);}
        std::vector<Vertex> a;
        for(auto it_v = seqVisited[j].begin();it_v < seqVisited[j].end();it_v++){
            if(query.IsNeighbor(*it_v, *it)){
                a.push_back(*it_v);
            }
        }
        nextVisited[*it] = a;
        j++;
    }
//    std::cout << "start" <<std::endl;
//    for(auto it = seqVertex.begin(); it < seqVertex.end(); it++){
//        std::cout << *it << std::endl;
//    }
//    for(auto it = nextVisited.begin(); it <nextVisited.end(); it++){
//        for(auto itr = (*it).begin();itr<(*it).end();itr++){
//            std::cout << *itr << ", ";
//        }
//        std::cout <<std::endl;
//    }

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
    Vertex root_vertex = seqVertex[0];
    backtracking(data, query, cs, root_vertex, embedding, nextVertex, nextVisited, INT32_MAX);
    //=========================print ===============================
//    std::cout << "back tracking end" << std::endl;
}

void Backtrack::backtracking(const Graph &data, const Graph &query, const CandidateSet &cs,
                             Vertex u, std::vector<size_t> embedding, std::vector<Vertex>& nV, std::vector<std::vector<Vertex>>& nVisit, Vertex prev) {
    Vertex nextVertex = nV[u];
    std::vector<Vertex> visited = nVisit[u];
    //=========================print ===============================
    //std::cout << "next vertex of "<< u << " : "<< next_vertex << std::endl;
    size_t csSize = cs.GetCandidateSize(u);
    for(size_t j = 0 ; j<csSize; j++){
        bool isValid = true;
        // std::cout << "loop :"<< j << std::endl;

        // injective
        size_t candidate = cs.GetCandidate(u, j);

//        if(prev != INT32_MAX && query.IsNeighbor(prev, u) && !data.IsNeighbor(embedding[prev],candidate)){
//            continue;
//        }

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

        if((nextVertex == INT32_MAX)&& embedding[u] != ULONG_MAX){
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
            if(j == cs.GetCandidateSize(u)-1){
                return;
            }
        }else{
//            std::cout << "back" << std::endl;
            backtracking(data, query, cs, nextVertex, embedding,nV, nVisit, u);
        }
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


