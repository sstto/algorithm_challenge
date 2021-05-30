/**
 * @file backtrack.cc
 *
 */

#include <dag.h>
#include "backtrack.h"

Backtrack::Backtrack() {
    Dag dag(query, candidate_set);
}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
  std::cout << "t " << query.GetNumVertices() << "\n";

  // implement your code here.
}
