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
  void backtracking(const Vertex &v, const size_t &c, const Graph &data, const Graph &query,
                    const CandidateSet &cs, const Dag &dag, std::vector<size_t> visited);
};

#endif  // BACKTRACK_H_
