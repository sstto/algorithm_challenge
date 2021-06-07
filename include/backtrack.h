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
  void backtracking(const Graph &data, const Graph &query, const CandidateSet &cs,
                    Vertex u, std::vector<size_t>& embedding);
  bool check(const Graph &data, const Graph &query, const std::vector<size_t> embedding);


private:
    int count = 0;
    std::vector<Vertex> nV;
    std::vector<std::vector<Vertex>> nVisit;
};

#endif  // BACKTRACK_H_
