/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#include <limits.h>
#include <algorithm>
#include <queue>

// query tree
nodeQ::nodeQ(Vertex v, std::vector<Vertex> *next, std::vector<Vertex> *prev)
:v(v), next(next),prev(prev){}



// global vaarible
Vertex qRootVertex;
std::vector<nodeQ*> q;
std::vector<Vertex> q_sort;
std::vector<std::vector<std::pair<Vertex, size_t>*>*> cost;//cost.first : node, cost.second : cost
std::vector<nodeQ*> csInfo;
std::vector<bool> dataVertexVisitInfo;
int count = 0;


Backtrack::Backtrack() {
}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
    std::cout << "t " << query.GetNumVertices() << "\n";
    //=============================debug===============================
    clock_t start = clock();
    //=============================debug===============================

    //make a q
    for (Vertex i = 0; i < query.GetNumVertices(); i++)
    {
        std::vector<Vertex>* prev = new std::vector<Vertex>();
        std::vector<Vertex>* next = new std::vector<Vertex>();
        q.push_back(new nodeQ(i, next, prev));
    }


    //find root of q
    size_t factorCs = ULONG_MAX;size_t factorDegree = 1;
    for (Vertex i = 0; i < query.GetNumVertices(); i++){
        if(query.GetDegree(i) == 0)continue;
        // argmin(cs/degree)
        if (cs.GetCandidateSize(i)*factorDegree < factorCs*query.GetDegree(i)){
            factorDegree =  factorCs*query.GetDegree(i);
            factorCs = cs.GetCandidateSize(i);
            qRootVertex = i;
        }
    }

    // make dag Tree with q
    std::queue<Vertex> quForBfs;
    std::vector<bool> qVisited(query.GetNumVertices(), false);
    std::vector<bool> visited(query.GetNumVertices(), false);
    quForBfs.push(qRootVertex);
    qVisited[qRootVertex] = true;

    while(!quForBfs.empty()){
        Vertex x = quForBfs.front();
        quForBfs.pop();
        visited[x] = true;
        q_sort.push_back(x);

        size_t startOffset = query.GetNeighborStartOffset(x);
        size_t endOffset = query.GetNeighborEndOffset(x);
        for(size_t i = startOffset; i< endOffset;i++){ //check i <= or <
            size_t v_visit = query.GetNeighbor(i);
            if (!visited[v_visit]){
                q[x]->addNext(v_visit); q[v_visit]->addPrev(x);
            }
            if (!qVisited[v_visit]){
                quForBfs.push(v_visit);
                qVisited[v_visit] = true;
            }
        }
    }
    // we have to find inv Root for path size ordering => next null, less degree
    Vertex qInvRootVertex = qRootVertex;
    size_t factor = ULONG_MAX;
    for(auto elem : q){
        if(elem->next->empty() && factor > query.GetDegree(elem->GetVertex())){
            factor = query.GetDegree(elem->GetVertex());
            qInvRootVertex = elem->GetVertex();
        }
    }
    // make q tree with candidate

    for(auto elem : q){
        elem->candidates->reserve(cs.GetCandidateSize(elem->GetVertex()));
        for(size_t i=0; i<cs.GetCandidateSize(elem->GetVertex());i++){
            elem->candidates->push_back(cs.GetCandidate(elem->GetVertex(),i));
        }
        std::sort(elem->candidates->begin(), elem->candidates->end());
        csInfo.push_back(elem);
    }
    for(size_t i = 0; i< q.size();i++){
        nodeQ* cur = q[i];
        for(Vertex nextElem : *cur->next){
           cur->nextNodes->push_back(csInfo.at(nextElem));
        }
        for(Vertex prevElem : *cur->next){
            cur->prevNodes->push_back(csInfo.at(prevElem));
        }
    }
    nodeQ* qRootNode = csInfo[qRootVertex];

    // make a cost for path size ordering
    for (Vertex i = 0; i < (Vertex) query.GetNumVertices(); i++) {
        auto * nCost = new std::vector<std::pair<Vertex, size_t> *>();
        for (int &cs : *csInfo[i]->candidates) {
            nCost->push_back(new std::pair<Vertex, size_t>(cs, 1));
        }
        cost.push_back(nCost);
    }

    for (size_t i = query.GetNumVertices() - 1; i >= 0; --i) {
        Vertex u = q_sort[i];
        //using dp
        if (q[u]->prev->size() == 1) {
            for (auto costOfPrev : *cost.at(q[u]->prev->front())) {
                int costValue = 0;
                for (auto costOfU : *cost.at(u)) {
                    if (data.IsNeighbor(costOfPrev->first, costOfU->first)) {
                        costValue += costOfU->second;
                    }
                }
                if (costValue != 0) {
                    if (costOfPrev->second == 1) costOfPrev->second = costValue;
                    else if (costValue < costOfPrev->second) costOfPrev->second = costValue;
                }
            }
        }
    }

    dataVertexVisitInfo.resize(data.GetNumVertices(), false);
    std::vector<Vertex> embedding;
    std::vector<bool> embeddingInfo;
    backtracking(data, query, embedding, embeddingInfo, 0);

//    finish = clock();clock
//    double duration = (double)(finish-start) / CLOCKS_PER_SEC;
//    std::cout << duration << " sec" << std::endl;

}
size_t nextCost(Vertex a){
    size_t c = 0;
    for (int i = 0; i< csInfo[a]->candidates->size(); i++){
        if (!dataVertexVisitInfo[csInfo[a]->candidates->at(i)]){
            c += cost.at(a)->at(i)->second;
        }
    }
    return c;
}

bool compare(Vertex a, Vertex b){
    return nextCost(a) > nextCost(b);
}
Vertex CalculateNextVertex(const Graph &query, std::vector<Vertex> embedding,std::vector<bool> embeddingInfo, size_t embeddingCtr){
    if(embeddingCtr == 0) return qRootVertex;
    std::vector<bool> isQVisited;
    isQVisited.resize(query.GetNumVertices(),false);

    std::vector<Vertex> cd;

    for (size_t i=0; i<embeddingCtr; i++){
        isQVisited[embeddingInfo[i]] = true;
    }
    for (size_t i=0; i<embeddingCtr; i++){
        nodeQ* cElem = csInfo[embeddingInfo[i]];
        for (nodeQ* cs : *cElem->nextNodes){
            if (!isQVisited[cs->GetVertex()]){
                cd.push_back(cs->GetVertex()); //// TODO:: 효율성 높이기.
            }
        }
    }
    sort(cd.begin(), cd.end(), compare);
    return cd.back();
}
void Backtrack::backtracking(const Graph &data, const Graph &query,std::vector<Vertex> embedding,std::vector<bool> embeddingInfo, size_t embeddingCtr) {
    if(count >= 100000) return;
    if(embeddingCtr == query.GetNumVertices()){
        std::cout << "a ";
        for(auto it = embedding.begin(); it != embedding.end();it++){
            std::cout << *it << " ";
        }
        std::cout << std::endl;
        //check(data, query, embedding);
        count +=1;
        return;
    }

    Vertex nextVertex = CalculateNextVertex(query,embedding,embeddingInfo,embeddingCtr);

    embeddingInfo[embeddingCtr] = nextVertex;
    nodeQ* q_cs_node = csInfo.at(nextVertex);
    for (Vertex candidate : *q_cs_node->candidates){
        if (!dataVertexVisitInfo[candidate]){
            bool is_connected_identically = true;
            for (size_t i=0; i<embeddingCtr; i++){
                Vertex already_visited_q = embeddingInfo[2*i];
                Vertex already_visited_d = embedding[2*i + 1];
                if (query.IsNeighbor(already_visited_q, nextVertex)){
                    if (!data.IsNeighbor(already_visited_d, candidate)){
                        is_connected_identically = false;
                        break;
                    }
                }
            }
            if (is_connected_identically){
                embedding[embeddingCtr] = candidate;
                dataVertexVisitInfo[candidate] = true;
                backtracking(data, query, embedding, embeddingInfo, embeddingCtr + 1);
                dataVertexVisitInfo[candidate] = false;
            }
        }
    }
//    std::cout << "vertex 탐방 : " << u <<std::endl;
//    std::vector<Vertex> visited;Vertex next_vertex;bool isEnd = true;
//    embedding[u] = ULONG_MAX-1;
//    auto it_v = std::find(embedding.begin(),embedding.end(), ULONG_MAX);
//    if(nV[u] == INT32_MAX){
//        size_t startOffset = query.GetNeighborStartOffset(u);
//        size_t endOffset = query.GetNeighborEndOffset(u);
//
//        size_t factor = ULONG_MAX;
//        for(size_t i = startOffset; i <endOffset; i++){
//            Vertex uNeighbor = query.GetNeighbor(i);
//            if(embedding[uNeighbor] != ULONG_MAX){
//                visited.push_back(uNeighbor);
//            }else{
//                //visited neigbor X
//                size_t argmin = cs.GetCandidateSize(uNeighbor)/query.GetDegree(uNeighbor);
//                if(factor > argmin){
//                    next_vertex = uNeighbor;
//                    factor = argmin;
//                    isEnd = false;
//                }
//            }
//        }
//
//        if(isEnd && (it_v != embedding.end())){
//            factor = ULONG_MAX;
//            for (size_t i = 0; i < query.GetNumVertices(); ++i) {
//                if(embedding[i] == ULONG_MAX) {
//                    if (factor > cs.GetCandidateSize(i) / query.GetDegree(i)) {
//                        factor = cs.GetCandidateSize(i) / query.GetDegree(i);
//                        next_vertex = i;
//                    }
//                }
//            }
//        }
//        nV[u] = next_vertex;
//        nVisit[u] = visited;
//    }else{
//
//        next_vertex = nV[u];
//        visited = nVisit[u];
//    }
//
//    for(size_t j = 0 ; j<cs.GetCandidateSize(u); j++){
//        bool isValid = true;
//
//        // injective
//        size_t candidate = cs.GetCandidate(u, j);
//
//        auto it = std::find(embedding.begin(),embedding.end(), candidate);
//        if(!(it == embedding.end())) continue;
//
//        // common edge existence
//        if(!visited.empty()) {
//            for(auto it_visited = visited.begin(); it_visited != visited.end();it_visited++) {
//                if (!data.IsNeighbor(embedding[*it_visited], candidate)) {
//                    isValid = false;
//                    break;
//                }
//            }
//        }
//
//        if(!isValid){
//            continue;
//        }else{
//            embedding[u] = candidate;
//        }
//
//        if(isEnd && (it_v == embedding.end())){
////            std::cout << count <<std::endl;
//            std::cout << "a ";
//            for(auto it = embedding.begin(); it != embedding.end();it++){
//                std::cout << *it << " ";
//            }
//            std::cout << std::endl;
//            //check(data, query, embedding);
//            count +=1;
//            if(count == 100000){
////                std::cout << "count = 100000" << std::endl;
////                finish = clock();
////                double duration = (double)(finish-start) / CLOCKS_PER_SEC;
////                std::cout << duration << " sec" << std::endl;
//                exit(0);
//            }
//
//        }else{
//            backtracking(data, query, cs, next_vertex, embedding);
//        }
//    }
//    embedding[u] = ULONG_MAX;
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


