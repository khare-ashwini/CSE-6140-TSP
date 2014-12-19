#include "Iter.h"
#include <vector>
#include <iostream>
#include <ctime>
#include <fstream>
#include <cmath>
using namespace std;


vector<int> random_permutation(const int &n) {
    vector<int> bestTour;
    for (int i=0; i<n; i++) bestTour.push_back(i);
    
    random_shuffle(bestTour.begin(),bestTour.end());
    return bestTour;
}

int cost(int **mat, const vector<int> &tour, const int &n) {
    int dist = 0;
    for (int i = 0; i < n; i++) dist += mat[tour[i]][tour[(i+1)%n]]; return dist;
    //    vector<int>::const_iterator it;
    //    for(it = tour.begin(); it < tour.end(); it++) dist+=mat[*it][*(it+1)];
    //    return dist+mat[*tour.begin()][*tour.end()];
}

vector<int> stochastic_two_opt(const vector<int> &tour, const int &n) {
    // checked, no mutation
    vector<int> newTour;
    int v1 = rand()%n , v2 = rand()%n;
    
    while (v1==v2) v2 = rand()%n;
    if (v2 < v1) {int temp = v2; v2 = v1; v1 = temp;}
    //    cout<<"The tour is: \n";
    //    for(int i = 0;i < n; i++) cout<<tour[i]<<" ";
    //    cout<<"tour finished"<<endl;
    //    cout<<"size of tour is: "<<tour.size()<<endl;
    
    for (int i = 0; i < v1; i++)  newTour.push_back(tour[i]);
    for (int i = v2; i >= v1; i--) newTour.push_back(tour[i]);
    for (int i = v2+1; i < n; i++)  newTour.push_back(tour[i]);
    
    //    cout<<"after opt the new tour size is "<<newTour.size()<<endl;
    //    for(int i = 0;i < n-1; i++) cout<<newTour[i]<<" ";
    //    cout<<endl;
    return newTour;
}




vector<int> double_bridge_move(const vector<int> &tour, const int &n) {
    
    vector<int> newTour;
    int m = n / 4;
    int v1 = rand()%m;
    int v2 = v1 + rand()%m;
    int v3 = v2 + rand()%m;
    
    for (int i = 0; i < v1; i++) newTour.push_back(tour[i]);
    for (int i = v3; i< n; i++)  newTour.push_back(tour[i]);
    for (int i = v2; i< v3; i++) newTour.push_back(tour[i]);
    for (int i = v1; i< v2; i++) newTour.push_back(tour[i]);
    return newTour;
}
vector<int> reverse_double_bridge_move(const vector<int> &tour, const int &n) {
    
    vector<int> newTour;
    int m = n / 4;
    int v1 = rand()%m;
    int v2 = v1 + rand()%m;
    int v3 = v2 + rand()%m;
    
    for (int i = v1; i< v2; i++) newTour.push_back(tour[i]);
    for (int i = v2; i< v3; i++) newTour.push_back(tour[i]);
    for (int i = v3; i< n; i++)  newTour.push_back(tour[i]);
    for (int i = 0; i < v1; i++) newTour.push_back(tour[i]);
    return newTour;
}

vector<int> stochastic_four_opt(const vector<int> &tour, const int &n) {
    vector<int> order = {0,1,2,3};
    random_shuffle(order.begin(),order.end());
    vector<int> newTour;
    int m = n / 4;
    int v1 = 1+rand()%m;
    int v2 = 1+v1 + rand()%m;
    int v3 = 1+ v2 + rand()%m;
    
    for (int j=0; j<4; j++) {
        if (order[j] == 0) {
            if (rand()%100 < 50)
                for (int i = v1; i< v2; i++) newTour.push_back(tour[i]);//0
            else
                for (int i = v2-1; i>=v1; i--) newTour.push_back(tour[i]);//0
        } else if (order[j] == 1) {
            if (rand()%100 < 50)
                for (int i = v2; i< v3; i++) newTour.push_back(tour[i]);//1
            else
                for (int i = v3-1; i>= v2; i--) newTour.push_back(tour[i]);//1
        } else if (order[j] == 2) {
            if (rand()%100 < 50)
                for (int i = v3; i< n; i++)  newTour.push_back(tour[i]);//2
            else
                for (int i = n-1; i>= v3; i--)  newTour.push_back(tour[i]);//2
        } else {
            if (rand()%100 < 50)
                for (int i = 0; i < v1; i++) newTour.push_back(tour[i]);//3
            else
                for (int i = v1-1; i >= 0; i--) newTour.push_back(tour[i]);//3
        }
    }
    
    return newTour;
}

vector<int> local_search(int **mat, const vector<int> &tour,
                         const int &tourCost, const int &max_no_improv,
                         const int &n) {
    int count = 0;
    vector<int> candiTour,bestTour;
    int candiCost,bestCost = tourCost;
    while (count<max_no_improv) {
        count++;
        if (rand()%100 < 95)
            candiTour = stochastic_two_opt(tour, n);
        else
            candiTour = stochastic_four_opt(tour, n);
        candiCost = cost(mat,candiTour,n);
        if (candiCost < bestCost) {
            bestTour = candiTour;
            bestCost = candiCost;
        }
    }
    return bestTour;
}

void iter_search(int **adjMat, const int &n, const string &filename, const string &method, const int &cutoff, const int &runID) {
    srand(runID); // random initialization
    
    //set parameters
    int max_iter = 10e10, max_no_improv = n*n/log(n);
    
    //generate initial candidate soluiton
    vector<int> bestTour = random_permutation(n);
    int         bestCost = cost(adjMat, bestTour, n);
    
    //perform local search on bestTour
    bestTour = local_search(adjMat, bestTour, bestCost, max_no_improv, n);
    bestCost = cost(adjMat, bestTour, n);
    
    vector<int> candiTour;
    int         candiCost;
    vector<int> perturbedCandiTour;
    int         perturbedCandiCost;
    
    
    //file processing
    ofstream solution, trace;
    string name = filename.substr(0,filename.size()-4);
    solution.open(name+"_"+method+"_"+to_string(cutoff)+"_"+to_string(runID)+".sol");
    trace.open(name+"_"+method+"_"+to_string(cutoff)+"_"+to_string(runID)+".trace");
    //start timing
    clock_t start = clock();
    
    for (int i=0; i<max_iter; i++) {
        if ( (clock() - start) / (float)CLOCKS_PER_SEC > cutoff) break;
        
        candiTour.clear(); //clear content
        
        int prob = rand()%100;
        
        if ( prob < 45) {
            candiTour = double_bridge_move(bestTour, n); // perturbation key part I
        } else if (prob < 90){
            candiTour = reverse_double_bridge_move(bestTour, n);
        } else {
            candiTour = stochastic_four_opt(bestTour, n);
        }
        
        candiCost = cost(adjMat, candiTour, n);
        
        perturbedCandiTour.clear();
        perturbedCandiTour = local_search(adjMat, candiTour, candiCost, max_no_improv, n);
        perturbedCandiCost = cost(adjMat, candiTour,n);
        //
        if (perturbedCandiCost < bestCost) {
            bestTour = perturbedCandiTour;
            bestCost = perturbedCandiCost;
            //            printf("> iteration: %d, best cost: %d, time elapsed: %f\n",
            //                   i+1,bestCost,(clock() - start) / (float)CLOCKS_PER_SEC);
            trace<<(clock() - start)/(float)CLOCKS_PER_SEC<<", "<<bestCost<<endl;
        }
    }
    
    //output result to solution handler
    solution<<bestCost<<endl;
    for (int i = 0; i < n; i++) solution<<bestTour[i]<<",";
    solution<<bestTour[0]<<endl;
    solution.close();
    trace.close();
}