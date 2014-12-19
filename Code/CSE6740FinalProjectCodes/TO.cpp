/***
 Local Search heuristic to solve Travelling Salesman Problem
 No Approximation but close to the solution
 ***/
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>
#include <algorithm>
#include "TO.h"

using namespace std;

/***
 Head Stuff
 ***/

int dim;
int optimum;
#define R 6383;//Radius of earth
#define PI 3.141592653;

class heap
{
public:
    edge *start;
    int size;
    heap()
    {
        start=new edge[dim*dim];
        size=0;
    }
    ~heap(){delete start;};
    void Insert(edge);
    void Remove(edge &ed);//remove the minimum node
    void siftDown(int start,int m);//downward adjustment
    void siftUp(int start);//upward adjustment
};
void heap::siftDown(int beg,int m)
{
    int i=beg,j=2*i+1;
    edge temp=start[i];
    while(j<=m)
    {
        if(j<m&&start[j].weight>start[j+1].weight)
        {
            j++;
        }
        if(temp.weight<=start[j].weight)
        {
            break;
        }
        else
        {
            start[i]=start[j];
            i=j;
            j=2*j+1;
        }
    }
    start[i]=temp;
}
void heap::siftUp(int beg)
{
    int j=beg,i=(j-1)/2;
    edge temp=start[j];
    while(j>0)
    {
        if(start[i].weight<=temp.weight)
        {
            break;
        }
        else
        {
            start[j]=start[i];
            j=i;
            i=(i-1)/2;
        }
    }
    start[j]=temp;
}
void heap::Insert(edge n)
{
    start[size]=n;
    siftUp(size);
    size++;
}
void heap::Remove(edge &ed)
{
    ed=start[0];
    start[0]=start[size-1];
    size--;
    siftDown(0,size-1);
}

vector<int> path;

//to read in data from txt file to a two-dimensional vector G
/***
 @public parsetsp
 Given a .tsp file, compute the distance matrix
 @param string filename, int type(Geo/Euc)
 @return distance
 ***/
edge **parsetsp(string filename, int a)
{
    ifstream fin;
    fin.open(filename.data(), ifstream::in);
    string line;
    size_t p1,p2;
    for(int i=0;i<6;i++)
    {
        getline(fin,line);
        if(i == 2) // get the dimension information from the 3rd line
        {
            p1 = line.find(' ',0);
            p2 = line.find(' ',p1+1);
            dim= atof(line.substr(p1 + 1,p2-p1-1).c_str());
        }
        if(i == 4){
            //get the optimum
            p1 = line.find(' ',0);
            p2 = line.find(' ',p1+1);
            optimum = atof(line.substr(p1 + 1,p2-p1-1).c_str());
            
        }
        //cout<<line<<endl;
    }
    // read the site information
    int Node;
    double Coord,Section;
    double (*nodeset)[2] = new double[dim][2];
    int i=0;
    while (fin>>Node>>Coord>>Section)
    {
        nodeset[i][0]=Coord;
        nodeset[i][1]=Section;
        i++;
    }
    // compute the distance matrix
    edge **distance = new edge*[dim];
    for (int i=0;i<dim;i++)
        distance[i]=new edge[dim];
    edge nd;
    for(int i=0;i<dim;i++)
    {
        for(int j=0;j<dim;j++)
        {
            float w;
            if (a == 1) // Geo distance
            {
                float x1,x2,y1,y2,pi=PI;
                // int r=R;
                x1 = nodeset[i][1]*pi/180;
                x2 = nodeset[j][1]*pi/180;
                y1 = nodeset[i][0]*pi/180;
                y2 = nodeset[j][0]*pi/180;
                w = acos(cos(y1)*cos(y2)*cos(x1-x2)+sin(y1)*sin(y2));
                w *= R;
                // cout << "w1 " << w;
                // double u,v;
                // u = sin((y2 - y1)/2);
                // v = sin((x2 - x1)/2);
                // cout << " Haversine Formula " ;
                // w = 2.0 * r * asin(sqrt(u * u + cos(y1) * cos(y2) * v * v));
                //cout << nodeset[i][0] << " " << nodeset[i][1] <<  endl;
            }
            else w = sqrt(pow(nodeset[i][0]-nodeset[j][0],2)+pow(nodeset[i][1]-nodeset[j][1],2)); //Euc Distance
            nd.begin = i;
            nd.weight = w;
            nd.end = j;
            //cout << w << endl;
            distance[i][j] = nd;
            //cout<<nd.weight<<endl;
        }
    }
    return distance;
}


//Prim Algorithm
//use heap to store the edges in the cutset
//use Adjacent List to store the MST

//Definition of adjacent nodes
struct NODE {
    int adjvex;// node ID
    int weight;// weight
    NODE *next;
};
//Definition of head nodes
struct HEADNODE{
    int nodeName; // node ID
    NODE *link; //points to the first node that is connected with the head node
    bool visited; //record if a node has been visited or not
};

void Prim(edge **G, HEADNODE *P)
{
    //initialize head nodes
    for (int i=0;i<dim;i++)
    {
        P[i].nodeName = i;
        P[i].link = NULL;
        P[i].visited = false;
    }
    heap Heap;
    int num=0;
    bool *ls = new bool[dim];// to record if a node has been added to the cut
    for(int i=0;i<dim;i++)
        ls[i]=false; //initialization
    //insert the edges that are connected with first node into heap.
    for(int i=0;i<dim;i++)
    {
        Heap.Insert(G[0][i]);
    }
    ls[0]=true;
    //construct the MST until the number of edge reach dim-1
    while(num<(dim-1))
    {
        edge Node = Heap.start[0];
        int tmp=Node.end;
        Heap.Remove(Node);
        if(ls[Node.end]==false)
        {
            ls[Node.end]=true;
            for(int j=0;j<dim;j++)
            {
                Heap.Insert(G[tmp][j]);
            }
            int b=Node.begin;
            int e=Node.end;
            int w=Node.weight;
            //add the target to the list of the source
            NODE *nd1 = new NODE;
            nd1->adjvex = e;
            nd1->weight = w;
            nd1->next = P[b].link;
            P[b].link = nd1;
            //add the source to the list of the target
            NODE *nd2 = new NODE;
            nd2->adjvex = b;
            nd2->weight = w;
            nd2->next = P[e].link;
            P[e].link = nd2;
            num++;
        }
    }
}

int back=1;
void DFSTreeWalk(HEADNODE *P,int i)
{
    if(P[i].link)
    {
        NODE *p = P[i].link;
        int n = p->adjvex;
        P[i].link=p->next; //delete the edge from MST
        delete p;
        if(P[n].visited==false)
        {
            back=1;
            path.push_back(n);
            P[n].visited=true;
            DFSTreeWalk(P,n);
        }
        else {DFSTreeWalk(P,i); }
    }
    else if(path.size()<dim)
    {
        back++;
        DFSTreeWalk(P,path[path.size()-back]); //if no more unvisited adjacent nodes, backtrack to previous nodes.
    }
    else path.push_back(path[0]); // add the first vertice to the end of the path to form the cycle.
}

long long getDistance(vector<int> p, edge** distance){
    
    long long sumWeight=0;
    for (int i=0;i<path.size()-1;i++)
        sumWeight += distance[p[i]][p[i+1]].weight;
    return sumWeight;
    
}

vector<int> twoOptSwap(int u, int v){
    
    vector<int> p;
    p = path;
    for(int x=u+1, y=v; x<y; ++x, --y)
        swap(p[x], p[y]);
    return p;
    
}

clock_t threshold = 2000;

void twoOpt(edge** distance, clock_t startTime,ostream &trace){
    
    vector<int> p;
//    int size = path.size();
    vector<int>::size_type size = path.size();
    long long best = getDistance(path, distance);
    //    int max = pow(2, size/8);
    for(int n=0; n<1000*size; n++){
        
        int u = rand() % (size-1);
        int v = rand() % (size-1);
        
        if(u==v) continue;
        //2-opt swap
        if(u>v){
            swap(u,v);
        }
        p = twoOptSwap(u,v);
        
        if(getDistance(p, distance) < getDistance(path, distance)){
            best = getDistance(p, distance);
            trace<<(clock() - startTime)/(float)CLOCKS_PER_SEC<<", "<<best<<endl;
            path = p;
        }
    }
}



void run_two_opt(edge **distance, const int &n, const string &filename,
                 const string &method, const int &cutoff, const int &runID ) {
    
    int bestResult = optimum;
    cout << filename << endl;
    //////////////////////////////////////////////////////////////////////////////
    //file processing
    ofstream solution, trace;
    string name = filename.substr(0,filename.size()-4);
    solution.open(name+"_"+method+"_"+to_string(cutoff)+"_"+to_string(runID)+".sol");
    trace.open(name+"_"+method+"_"+to_string(cutoff)+"_"+to_string(runID)+".trace");
    //start timing
    clock_t start = clock();
    /////////////////////////////////////////////////////////////////////////
    
        HEADNODE *P = new HEADNODE[dim];
        Prim(distance,P);//compute MST
        //int b=rand() % dim;
        path.push_back(0);
        P[0].visited=true;
        DFSTreeWalk(P,0);//compute preorder of MST
        
        cout<<"OPT is: "<<optimum<<" best is"<<bestResult<<endl;
        twoOpt(distance, start, trace);
    ////////////////////////////////////////////////////////////////////////////////
    solution<<optimum<<endl;
    for(int i = 0;i<dim;i++) solution<<path[i]<<",";
    solution<<path[0]<<endl;

    ////////////////////////////////////////////////////////////////////////////////
    path.clear();
    
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// simulated anealing

//Prim Algorithm
//use heap to store the edges in the cutset
//use Adjacent List to store the MST
void SA(edge **distance,float T, float r, clock_t startTime, int cutoff, ostream &trace)
{
    int S=0;
    for (int i=0;i<path.size()-1;i++)
    S+=distance[path[i]][path[i+1]].weight;
    S+=distance[path[dim-1]][path[0]].weight;
    int u,v,CurrentSum=S,BestSum=S;
    float Tr = T*0.1;
    vector<int> bestpath;
    clock_t start = clock();
    float timespent;
    while(timespent<cutoff)//T>0.00001)//timespent<60)//T > 0.00001)
    {
        //randomly generate 2 indexes
        u = rand() % (path.size()-3);
        v = u +1 + rand() % (path.size()-u-2);
        int E = distance[path[u]][path[u+1]].weight + distance[path[v]][path[v+1]].weight;
        // reverse the sequence between u and v+1.
        reverse(path.begin()+u+1,path.begin()+v+1);
        
        // cout<<"reverse weight "<<S<<endl;
        int Enew = distance[path[u]][path[u+1]].weight + distance[path[v]][path[v+1]].weight;
        int delta_E =  Enew - E;
        //sum of weight of current path
        CurrentSum += delta_E;
        if ( delta_E > 0)
        {
            float pr = rand()/(RAND_MAX+1.0);
            float p = exp((-1)*delta_E/T);
            if( p < pr) // with the probability (1-p), jump to the original state
            {
                reverse(path.begin()+u+1,path.begin()+v+1);
                CurrentSum = CurrentSum - delta_E;
            }
        }
        if(CurrentSum<BestSum)
        {
            BestSum=CurrentSum;
            bestpath=path;
            trace<<(clock() - startTime)/(float)CLOCKS_PER_SEC<<", "<<BestSum<<endl;
        }
        
        T = T*r; // decrease the temperature
        if(T < 0.00001) T=Tr;
        timespent = (clock()-start)/CLOCKS_PER_SEC;
    }
    path = bestpath;
}

void run_SA (edge **distance, const int &n, const string &filename,
              const string &method, const int &cutoff, const int &runID ) {
        srand(0);//random seed   time(NULL));
        HEADNODE *P = new HEADNODE[dim];
        int Sum=0;
        //////////////////////////////////////////////////////////////////////////////
        //file processing
        ofstream solution, trace;
        string name = filename.substr(0,filename.size()-4);
        solution.open(name+"_"+method+"_"+to_string(cutoff)+"_"+to_string(runID)+".sol");
        trace.open(name+"_"+method+"_"+to_string(cutoff)+"_"+to_string(runID)+".trace");
        //start timing
        clock_t start = clock();
        /////////////////////////////////////////////////////////////////////////
        for (int rep=0;rep<1;rep++)
        {
            Prim(distance,P);//compute MST
            //int b=rand() % dim;
            path.push_back(rep);
            P[rep].visited=true;
            DFSTreeWalk(P,rep);//compute preorder of MST
            
            float T = 10*distance[0][1].weight*dim;//initial temperature
            float r = 0.9999; //decay rate of the temperature
            SA(distance,T,r,start, cutoff, trace); // do simulated anealling
            
            int SumWeight=0;
            for (int i=0;i<path.size()-1;i++)
                SumWeight+=distance[path[i]][path[i+1]].weight;
            SumWeight+=distance[path[dim-1]][path[0]].weight;
            //cout<<"Simulated Anealling:"<<" "<<SumWeight<<endl;
            path.clear();
            Sum+=SumWeight;
        }
    solution<<Sum<<endl;
    for(int i = 0;i<dim;i++) solution<<path[i]<<",";
    solution<<path[0]<<endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MST

void run_MST(edge **distance, const int &n, const string &filename, const string &method,
         const int &cutoff, const int &runID) {
    srand(runID);
    //////////////////////////////////////////////////////////////////////////////
    //file processing
    ofstream solution, trace;
    string name = filename.substr(0,filename.size()-4);
    solution.open(name+"_"+method+"_"+to_string(cutoff)+"_"+to_string(runID)+".sol");
    trace.open(name+"_"+method+"_"+to_string(cutoff)+"_"+to_string(runID)+".trace");
    //start timing
    clock_t start = clock();
    /////////////////////////////////////////////////////////////////////////
    HEADNODE *P = new HEADNODE[dim];
    int Sum=0;
    int bestCost = 2e9;
    for (int rep=0;rep<dim;rep++)
    {
        Prim(distance,P);//compute MST
        int SumWeight=0;
        //int b=rand() % dim;
        path.push_back(rep);
        P[rep].visited=true;
        DFSTreeWalk(P,rep);//compute preorder of MST
        //compute the weight sum of the approximate solution
        for (int i=0;i<path.size()-1;i++)
            SumWeight += distance[path[i]][path[i+1]].weight;
        path.clear();
        //cout<<SumWeight<<endl;
        Sum+=SumWeight;
        if (Sum < bestCost) {
            trace<<(clock() - start)/(float)CLOCKS_PER_SEC<<", "<<bestCost<<endl;
            bestCost = Sum;
        }
    }
    solution<<Sum<<endl;
    for(int i = 0;i<dim;i++) solution<<path[i]<<",";
    solution<<path[0]<<endl;
}

