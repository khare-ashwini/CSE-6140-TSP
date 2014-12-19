#include "parseStruct.h"
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
using namespace std;


#define R 6383;//Radius of earth
#define PI 3.141592653;


//to read in data from txt file to a two-dimensional vector G
edge **parsetsp(const string &filename, int a)
{
    float pi = PI;
    float r = R;
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
        cout<<line<<endl;
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
                
                w = (int) (r * acos(0.5*((1+q1)*q2-(1-q1)*q3))+1);
            }
            else w = sqrt(pow(nodeset[i][0]-nodeset[j][0],2)+pow(nodeset[i][1]-nodeset[j][1],2)); //Euc Distance
            nd.begin = i;
            nd.weight = w;
            nd.end = j;
            distance[i][j] = nd;
            //cout<<nd.weight<<endl;
        }
    }
    return distance;
}