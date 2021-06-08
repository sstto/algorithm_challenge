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
        : v(v), next(next), prev(prev) {}


// global vaarible
Vertex qRootVertex;
std::vector<nodeQ *> q;
std::vector<Vertex> q_sort;
std::vector<std::vector<std::pair<Vertex, int> *> *> cost;//cost.first : node, cost.second : cost
std::vector<nodeQ *> csInfo;
bool *dataVertexVisitInfo;
int count = 0;


Backtrack::Backtrack() {
}

Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
    std::cout << "t " << query.GetNumVertices() << "\n";
    //=============================debug===============================
//    clock_t start = clock();
    //=============================debug===============================

    //make a q
    for (Vertex i = 0; i < query.GetNumVertices(); i++) {
        std::vector<Vertex> *prev = new std::vector<Vertex>();
        std::vector<Vertex> *next = new std::vector<Vertex>();
        q.push_back(new nodeQ(i, next, prev));
    }

    //find root of q
    size_t factorCs = ULONG_MAX;
    size_t factorDegree = 1;
    for (Vertex i = 0; i < query.GetNumVertices(); i++) {
        if (query.GetDegree(i) == 0)continue;
        // argmin(cs/degree)
        if (cs.GetCandidateSize(i) * factorDegree < factorCs * query.GetDegree(i)) {
            factorDegree = factorCs * query.GetDegree(i);
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

    while (!quForBfs.empty()) {
        Vertex x = quForBfs.front();
        quForBfs.pop();
        visited[x] = true;
        q_sort.push_back(x);

        size_t startOffset = query.GetNeighborStartOffset(x);
        size_t endOffset = query.GetNeighborEndOffset(x);
        for (size_t i = startOffset; i < endOffset; i++) { //check i <= or <
            size_t v_visit = query.GetNeighbor(i);
            if (!visited[v_visit]) {
                q[x]->addNext(v_visit);
                q[v_visit]->addPrev(x);
            }
            if (!qVisited[v_visit]) {
                quForBfs.push(v_visit);
                qVisited[v_visit] = true;
            }
        }
    }

    // we have to find inv Root for path size ordering => next null, less degree
    Vertex qInvRootVertex = qRootVertex;
    size_t factor = ULONG_MAX;
    for (auto elem : q) {
        if (elem->next->empty() && factor > query.GetDegree(elem->GetVertex())) {
            factor = query.GetDegree(elem->GetVertex());
            qInvRootVertex = elem->GetVertex();
        }
    }
    // make q tree with candidate

    for (auto elem : q) {
        size_t n = cs.GetCandidateSize(elem->GetVertex());
        elem->candidates->reserve(n);
        for (size_t i = 0; i < n; i++) {
            elem->candidates->push_back(cs.GetCandidate(elem->GetVertex(), i));
        }
        std::sort(elem->candidates->begin(), elem->candidates->end());
        csInfo.push_back(elem);
    }

    for (size_t i = 0; i < q.size(); i++) {
        nodeQ *cur = q[i];
        for (Vertex nextElem : *cur->next) {
            cur->nextNodes->push_back(csInfo.at(nextElem));
        }
        for (Vertex prevElem : *cur->next) {
            cur->prevNodes->push_back(csInfo.at(prevElem));
        }
    }

    // make a cost for path size ordering
    for (Vertex i = 0; i < (Vertex) query.GetNumVertices(); i++) {
        auto *insOfPair = new std::vector<std::pair<Vertex, int> *>();
        auto  it = csInfo[i]->candidates;
        for (auto itr = it->begin(); itr<it->end();itr++) {
            insOfPair->push_back(new std::pair<Vertex, int>(*itr, 1));
        }
        cost.push_back(insOfPair);
    }
    for (int i = (int) query.GetNumVertices() - 1; i >= 0; --i) {
        Vertex u = q_sort[i];
        if (q[u]->prev->size() == 1) {
            Vertex prevV = q[u]->prev->front();
            for (auto costOfP : *cost.at(prevV)) {
                int nCost = 0;
                for (auto CostOfU : *cost.at(u)) {
                    if (data.IsNeighbor(costOfP->first, CostOfU->first))
                        nCost += CostOfU->second;
                }

                if (nCost != 0 && costOfP->second == 1) costOfP->second = nCost;
                else if (nCost != 0 && nCost < costOfP->second) costOfP->second = nCost;

            }
        }
    }

    Vertex embedding[query.GetNumVertices() * 2];
    dataVertexVisitInfo = (bool *) malloc(sizeof(bool) * data.GetNumVertices());

    for (size_t i = 0; i < data.GetNumVertices(); i++) {
        dataVertexVisitInfo[i] = false;
    }

    backtracking(data, query, embedding, 0);

    //=============================debug===============================
//    clock_t finish = clock();
//    double duration = (double)(finish-start) / CLOCKS_PER_SEC;
//    std::cout << duration << " sec" << std::endl;
//    std::cout <<count << std::endl;
    //=============================debug===============================
}

size_t nextCost(Vertex a) {
    size_t c = 0;
    for (int i = 0; i < csInfo[a]->candidates->size(); i++) {
        if (!dataVertexVisitInfo[csInfo[a]->candidates->at(i)]) {
            c += cost.at(a)->at(i)->second;
        }
    }
    return c;
}

bool compare(Vertex a, Vertex b) {
    return nextCost(a) > nextCost(b);
}

Vertex CalculateNextVertex(const Graph &query, const Vertex embedding[], size_t size) {
    std::vector<Vertex> cds;

    if (size == 0) {
        return qRootVertex;
    }

    bool isVisited[query.GetNumVertices()];
    for (size_t i = 0; i < query.GetNumVertices(); i++) {
        isVisited[i] = false;
    }


    for (size_t i = 0; i < size; i++) {
        isVisited[embedding[2 * i]] = true;
    }
    for (size_t i = 0; i < size; i++) {
        for (nodeQ *cs : *csInfo.at(embedding[2 * i])->nextNodes) {
            if (!isVisited[cs->GetVertex()]) {
                cds.push_back(cs->GetVertex());
            }
        }
    }
    sort(cds.begin(), cds.end(), compare);

    return cds.back();
}

void Backtrack::backtracking(const Graph &data, const Graph &query, Vertex embedding[], size_t size) {
    if (count >= 100000)
        return;
    if (query.GetNumVertices() == size) {
        count++;
        std::vector<size_t> realEmbedding;
        realEmbedding.resize(query.GetNumVertices());
        for (size_t i = 0; i < query.GetNumVertices(); i++) {
            realEmbedding[embedding[2 * i]] = embedding[2 * i + 1];
        }

        std::cout << "a ";
        for (size_t i = 0; i < query.GetNumVertices(); i++) {
            std::cout << realEmbedding[i] << " ";
        }
        std::cout << std::endl;
        return;
    }

    Vertex nextVertex = CalculateNextVertex(query, embedding, size);

    embedding[2 * size] = nextVertex;
    for (Vertex candidate : *(csInfo.at(nextVertex))->candidates) {
        if (!dataVertexVisitInfo[candidate]) {
            bool isEnd = true;
            for (size_t i = 0; i < size; i++) {
                if (query.IsNeighbor(embedding[2 * i], nextVertex) &&
                    !data.IsNeighbor(embedding[2 * i + 1], candidate)) {
                    isEnd = false;
                    break;
                }
            }
            if (isEnd) {
                embedding[size * 2 + 1] = candidate;
                dataVertexVisitInfo[candidate] = true;
                backtracking(data, query, embedding, size + 1);
                dataVertexVisitInfo[candidate] = false;
            }
        }
    }
}


bool Backtrack::check(const Graph &data, const Graph &query, const std::vector<size_t> embedding) {
    bool isValid = true;
    for (size_t i = 0; i < query.GetNumVertices(); i++) {
        size_t startOffset = query.GetNeighborStartOffset(i);
        size_t endOffset = query.GetNeighborEndOffset(i);

        for (size_t j = startOffset; j < endOffset; j++) {
            Vertex uNeighbor = query.GetNeighbor(j);
            if (!data.IsNeighbor(embedding[i], embedding[uNeighbor]))
                isValid = false;
        }
    }
    return isValid;
}


