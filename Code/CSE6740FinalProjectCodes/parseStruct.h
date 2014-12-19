#ifndef __CSE6740FinalProject__parseStruct__
#define __CSE6740FinalProject__parseStruct__

#include <string>

int dim; //n: number of vertices
struct edge
{
    int begin; //source
    int end;// target
    float weight; // weight
};
edge **parsetsp(const std::string &filename, int a);

#endif /* defined(__CSE6740FinalProject__parseStruct__) */
