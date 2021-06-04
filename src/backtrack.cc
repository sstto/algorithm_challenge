/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include <limits.h>
#include <algorithm>

size_t ArgMin(size_t cs_size, size_t degree){
    return cs_size;
}
Backtrack::Backtrack() {
    count = 0;
}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
//    std::cout << "t " << query.GetNumVertices() << "\n";

    // find a root_vertex
    size_t root_vertex = 0;
    size_t factor = cs.GetCandidateSize(0)/query.GetDegree(0);
    for (size_t i = 0; i < query.GetNumVertices(); ++i) {
        if (factor > ArgMin(cs.GetCandidateSize(i),query.GetDegree(i))){
            factor = ArgMin(cs.GetCandidateSize(i),query.GetDegree(i));
            root_vertex = i;
        }
    }
    //=========================print ===============================
//    std::cout << "root vertex : " << root_vertex << std::endl;
    std::vector<size_t> embedding;
    embedding.resize(query.GetNumVertices(),ULONG_MAX);

    backtracking(data, query, cs, root_vertex, embedding);

}

void Backtrack::backtracking(const Graph &data, const Graph &query, const CandidateSet &cs,
                             Vertex u, std::vector<size_t> embedding) {
    //=========================print ===============================
    std::cout << "vertex 탐방 : " << u <<std::endl;
    int c = 0;
//    for(auto it = embedding.begin(); it != embedding.end();it++){
//        if(*it != ULONG_MAX){
////            std::cout << *it << ", ";
////            c++;
//            if(*it == 149){
//                std::cout << "im here" <<std::endl;
//            }
//        }
//    }
    if(u == 150){
        std::cout << embedding[149] <<std::endl;
    }
//    std::cout << c << std::endl;
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
            size_t argmin = ArgMin(cs.GetCandidateSize(uNeighbor),query.GetDegree(uNeighbor));
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
                if (factor > ArgMin(cs.GetCandidateSize(i),query.GetDegree(i))) {
                    factor = ArgMin(cs.GetCandidateSize(i),query.GetDegree(i));
                    next_vertex = i;
                }
            }
        }
        //=========================print ===============================
//        std::cout<< "next vertexs with random : " << next_vertex << std::endl;
    }else{
        //=========================print ===============================
//        std::cout << "next vertex : " << next_vertex <<std::endl;
    }
//    if(next_vertex == 24){
//        std::cout << "24 <-" << u<<std::endl;
//    }

        //=========================print ===============================
    //std::cout << "next vertex of "<< u << " : "<< next_vertex << std::endl;
    size_t csSize = cs.GetCandidateSize(u);
    for(size_t j = 0 ; j<csSize; j++){
        bool isValid = true;
       // std::cout << "loop :"<< j << std::endl;

        // injective
        size_t candidate = cs.GetCandidate(u, j);
        //std::cout << "candidate size :"<< csSize<< " ,candidate :" << candidate <<std::endl;
//        if(candidate == 937){
//            std::cout << "real ";
//            for(auto it = embedding.begin(); it != embedding.end();it++){
//                std::cout << *it << " ";
//            }
//            std::cout << std::endl;
//        }
//        std::cout << "vertex : " << u << " ,candidate v id : " << candidate;
        auto it = std::find(embedding.begin(),embedding.end(), candidate);
        if(!(it == embedding.end())) continue;

        // common edge existence
        if(!visited.empty()) {
            for(auto it_visited = visited.begin(); it_visited != visited.end();it_visited++) {
                if (!data.IsNeighbor(embedding[*it_visited], candidate)) {
                    isValid = false;
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
            if(j == cs.GetCandidateSize(u)-1){
                return;
            }
        }else{
//            std::cout << "back" << std::endl;
            backtracking(data, query, cs, next_vertex, embedding);
        }
    }
    embedding[u] = ULONG_MAX;
    return;
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



