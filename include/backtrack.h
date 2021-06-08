/**
 * @file backtrack.h
 *
 */

#ifndef BACKTRACK_H_
#define BACKTRACK_H_

#include "candidate_set.h"
#include "common.h"
#include "graph.h"

class Backtrack {
 public:
  Backtrack();
  ~Backtrack();

  void PrintAllMatches(const Graph &data, const Graph &query,
                       const CandidateSet &cs);
  void backtracking(const Graph &data, const Graph &query, Vertex path[], size_t path_size);
  bool check(const Graph &data, const Graph &query, const std::vector<size_t> embedding);


};

class nodeQ{
public:
    nodeQ(Vertex v, std::vector<Vertex>* next = nullptr, std::vector<Vertex>* prev = nullptr);
    ~nodeQ(){
        delete next;delete prev;
    }
    std::vector<Vertex> *prev;
    std::vector<Vertex> *next;
    std::vector<nodeQ*>* nextNodes= new std::vector<nodeQ*>();
    std::vector<nodeQ*>* prevNodes= new std::vector<nodeQ*>();
    std::vector<Vertex> *candidates= new std::vector<Vertex>();

    inline Vertex GetVertex();

    inline void addNext(Vertex v);
    inline void addPrev(Vertex v);
private:
    Vertex v;
};
inline Vertex nodeQ::GetVertex(){return v;}
inline void nodeQ::addNext(Vertex v){next->push_back(v);}
inline void nodeQ::addPrev(Vertex v){prev->push_back(v);}
#endif  // BACKTRACK_H_
