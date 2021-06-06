/**
 * @file main.cc
 *
 */

#include "backtrack.h"
#include "candidate_set.h"
#include "common.h"
#include "graph.h"
#include <sstream>

int main(int argc, char* argv[]) {
  if (argc < 4) {
    std::cerr << "Usage: ./program <data graph file> <query graph file> "
                 "<candidate set file>\n";
    return EXIT_FAILURE;
  }
  clock_t start, finish;
  start = clock();
  std::string data_file_name = argv[1];
  std::string query_file_name = argv[2];
  std::string candidate_set_file_name = argv[3];

  Graph data(data_file_name);
  Graph query(query_file_name, true);
  CandidateSet candidate_set(candidate_set_file_name);
  Backtrack backtrack;

  backtrack.PrintAllMatches(data, query, candidate_set);
  finish = clock();
  double duration = (double)(finish-start) / CLOCKS_PER_SEC;
  std::cout << duration << " sec" << std::endl;
//  std::string as = "937 2330 1293 161 303 1126 541 1106 5008 3088 1469 221 5672 5671 5670 5669 211 4723 1684 1376 110 687 266 86 131 1168 1379 2806 2805 684 685 4455 4468 123 5138 0 5137 1517 4700 404 160 831 280 1932 1929 179 8785 69 43 1995";
//  std::istringstream ss(as);
//  std::string stringBuffer;
//  std::vector<size_t> ch;
//  while (getline(ss, stringBuffer, ' '))
//  {
//      ch.push_back(std::stoul(stringBuffer));
//  }
//  std::cout << backtrack.check(data, query, ch);
  return EXIT_SUCCESS;
}
