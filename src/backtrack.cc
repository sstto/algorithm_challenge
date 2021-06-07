/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include <limits.h>
#include <algorithm>
//clock_t start, finish;
Backtrack::Backtrack() {
    count = 0;
}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {

//    start = clock();
    std::cout << "t " << query.GetNumVertices() << "\n";
    nV.resize(query.GetNumVertices(), INT32_MAX);
    nVisit.resize(query.GetNumVertices());
    // find a root_vertex
    size_t root_vertex = 0;
    size_t factor = cs.GetCandidateSize(0)/query.GetDegree(0);
    for (size_t i = 0; i < query.GetNumVertices(); ++i) {
        if (factor > cs.GetCandidateSize(i)/query.GetDegree(i)){
            factor = cs.GetCandidateSize(i)/query.GetDegree(i);
            root_vertex = i;
        }
    }

    std::vector<size_t> embedding;
    embedding.resize(query.GetNumVertices(),ULONG_MAX);


    backtracking(data, query, cs, root_vertex, embedding);
//    finish = clock();clock
//    double duration = (double)(finish-start) / CLOCKS_PER_SEC;
//    std::cout << duration << " sec" << std::endl;

}

void Backtrack::backtracking(const Graph &data, const Graph &query, const CandidateSet &cs,
                             Vertex u, std::vector<size_t>& embedding) {
//    std::cout << "vertex 탐방 : " << u <<std::endl;
    std::vector<Vertex> visited;Vertex next_vertex;bool isEnd = true;
    embedding[u] = ULONG_MAX-1;
    auto it_v = std::find(embedding.begin(),embedding.end(), ULONG_MAX);
    if(nV[u] == INT32_MAX){
        size_t startOffset = query.GetNeighborStartOffset(u);
        size_t endOffset = query.GetNeighborEndOffset(u);

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

        if(isEnd && (it_v != embedding.end())){
            factor = ULONG_MAX;
            for (size_t i = 0; i < query.GetNumVertices(); ++i) {
                if(embedding[i] == ULONG_MAX) {
                    if (factor > cs.GetCandidateSize(i) / query.GetDegree(i)) {
                        factor = cs.GetCandidateSize(i) / query.GetDegree(i);
                        next_vertex = i;
                    }
                }
            }
        }
        nV[u] = next_vertex;
        nVisit[u] = visited;
    }else{

        next_vertex = nV[u];
        visited = nVisit[u];
    }

    for(size_t j = 0 ; j<cs.GetCandidateSize(u); j++){
        bool isValid = true;

        // injective
        size_t candidate = cs.GetCandidate(u, j);

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

        if(isEnd && (it_v == embedding.end())){
//            std::cout << count <<std::endl;
            std::cout << "a ";
            for(auto it = embedding.begin(); it != embedding.end();it++){
                std::cout << *it << " ";
            }
            std::cout << std::endl;
            //check(data, query, embedding);
            count +=1;
            if(count == 100000){
                std::cout << "count = 100000" << std::endl;
//                finish = clock();
//                double duration = (double)(finish-start) / CLOCKS_PER_SEC;
//                std::cout << duration << " sec" << std::endl;
                exit(0);
            }

        }else{
            backtracking(data, query, cs, next_vertex, embedding);
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


