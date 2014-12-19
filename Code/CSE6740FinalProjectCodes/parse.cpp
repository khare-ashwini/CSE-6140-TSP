#include "parse.h"
#include <fstream>
#include <iostream>
#include <cmath>
using namespace std;

#define R 6383;
//Radius of earth
#define PI 3.141592653;


int **parse_mat(string filename, int a, int dim) {
    ifstream fin;
    fin.open(filename.data(), ifstream::in);
    string line;
    
    
    for(int i=0;i<6;i++)
    {
        getline(fin,line);
        //        cout<<line<<endl;
    }
    
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
    
    int **distance = new int*[dim];
    for (int i=0;i<dim;i++)
        distance[i]=new int[dim];
    
    int w;
    float pi = PI;
    float r = R;
    for(int i=0;i<dim;i++) {
        for(int j=0;j<dim;j++) {
            
            if (a == 1) // Geo distance
            {
                float x1=nodeset[i][0],x2=nodeset[j][0],y1=nodeset[i][1],y2=nodeset[j][1];
                
                int deg = int(x1);
                float min = x1 - deg;
                x1 = pi*(deg+5*min/3)/180;
                
                deg = int(y1);
                min = y1 - deg;
                y1 = pi*(deg+5*min/3)/180;
                
                deg = int(x2);
                min = x2 - deg;
                x2 = pi*(deg+5*min/3)/180;
                
                deg = int(y2);
                min = y2 - deg;
                y2 = pi*(deg+5*min/3)/180;
                
                float q1 = cos(y1-y2);
                float q2 = cos(x1-x2);
                float q3 = cos(x1+x2);
                
                w = (int) (r*acos(0.5*((1+q1)*q2-(1-q1)*q3))+1);
                
            }
            else w = sqrt(pow(nodeset[i][0]-nodeset[j][0],2)+pow(nodeset[i][1]-nodeset[j][1],2)); //Euc Distance
            if (i == j)
                distance[i][j] = 0;
            else
                distance[i][j] = w;
            
            
            
        }
    }
    return distance;
}