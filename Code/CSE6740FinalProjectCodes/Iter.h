#ifndef __CSE6740FinalProject__Iter__
#define __CSE6740FinalProject__Iter__

#include <vector>
#include <string>
void iter_search(int **adjMat, const int &n, const std::string &filename, const std::string &method,
                 const int &cutoff, const int &runID);
int cost(int **mat, const std::vector<int> &tour, const int &n);

#endif /* defined(__CSE6740FinalProject__Iter__) */
