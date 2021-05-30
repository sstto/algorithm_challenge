//
// Created by 심영인 on 2021/05/30.
//

#include <candidate_space.h>


CandidateSpace::CandidateSpace(const std::string& filename,const CandidateSet &cs, const Graph &data, const Dag &dag)
: CandidateSet(filename), dag(dag)
{
}

