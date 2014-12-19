#ifndef __CSE6740FinalProject__TO__
#define __CSE6740FinalProject__TO__

#include <string>

struct edge {
    int begin; //source
    int end;// target
    float weight; // weight
};

edge **parsetsp(std::string filename, int a);

void run_two_opt(edge **distance, const int &n, const std::string &filename,
            const std::string &method, const int &cutoff, const int &runID );

void run_SA (edge **distance, const int &n, const std::string &filename,
             const std::string &method, const int &cutoff, const int &runID );

void run_MST(edge **distance, const int &n, const std::string &filename, const std::string &method,
             const int &cutoff, const int &runID);
#endif /* defined(__CSE6740FinalProject__TO__) */
