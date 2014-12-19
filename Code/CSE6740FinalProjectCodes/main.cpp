#include <iostream>
#include <fstream>
#include "parse.h"
#include "Iter.h"
#include "FI.h"
//#include "MSTApprox.h"
//#include "SA.h"
//#include "parseStruct.h"
#include "TO.h"

using namespace std;


int main(int argc, char *argv[]) {
    ////////////////////// temporary parameter //////////  passed as variables in Xcodes
    //      argv[1] = "burma14.tsp"; argv[2] = "LS_Iter";
    //      argv[3] = "4"; argv[4] = "5";
    ////////////////////// temporary parameter //////////
    /////////////////////////////////////////////////////
    string filename = argv[1], method = argv[2],
    _cutoff =  argv[3], _runID = argv[4];
    int cutoff = stoi(_cutoff), runID = stoi(_runID);
    ///////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////
    string filenames[6] = {"berlin52.tsp","burma14.tsp","ch150.tsp",
        "gr202.tsp","kroA100.tsp","ulysses16.tsp"};
    int option[6] = {0,1,0,1,0,1}; //Geo or Euc
    int dim[6] = {52,14,150,202,100,16};
    
    int idx;
    for (idx = 0; idx < 6; idx++)
        if (filenames[idx] == filename) break;
    //////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////
    
    if (method == "Iter") {
        
        cout<<"running Iter"<<endl;
        int **adjacencyMatrix = parse_mat(filenames[idx], option[idx],dim[idx]);
        iter_search(adjacencyMatrix, dim[idx], filename, method, cutoff, runID);
        cout<<"OK...";
    } else if (method == "FI") {
        cout<<"OK"<<endl;
        int **adjacencyMatrix = parse_mat(filenames[idx], option[idx],dim[idx]);
        farthestInsertion(adjacencyMatrix, dim[idx], filename, method, cutoff, runID);
        cout<<"finished"<<endl;
    } else if (method == "MST") {
        edge **distance = parsetsp(filename, option[idx]);//parse file
        run_MST(distance, dim[idx], filename, method, cutoff, runID);
        cout<<"finished"<<endl;
    } else if (method == "SA") {
        edge **distance = parsetsp(filename, option[idx]);//parse file
        run_SA(distance, dim[idx], filename, method, cutoff, runID);
        cout<<"finished...";
    } else if (method == "TwoOpt") {
        cout<<filename<<" "<<option[idx]<<endl;
        edge **distance = parsetsp(filename, option[idx]);//parse file
        
        run_two_opt(distance, dim[idx], filename, method, cutoff, runID);

        cout<<"finished"<<endl;
    }
    return 0;
}
