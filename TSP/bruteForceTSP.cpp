#include <bits/stdc++.h>
using namespace std;
#include<time.h>
#include<vector>
#define V 4 //number of cities / vertices
 
int TSP(int graph[][V], int s)
{
    vector<int> cities;

    for (int i = 0; i < V; i++)
        if (i != s)
            cities.push_back(i);
 
    // minimum weight Hamiltonian Cycle.
    int min_path = INT_MAX;
    do 
    {
        int current_pathweight = 0;
        int k = s;

        for (int i = 0; i < cities.size(); i++) {
            current_pathweight += graph[k][cities[i]];
            k = cities[i];
        }
        current_pathweight += graph[k][s];
 
        min_path = min(min_path, current_pathweight);
 
    } 
    while(next_permutation(cities.begin(), cities.end()));
 
    return min_path;
}
 
// Driver Code
int main()
{
    // clock_t start, end;
    // double timeTaken;

    int graph[4][4] = { { 0, 10, 15, 20 },
                       { 10, 0, 35, 25 },
                       { 15, 35, 0, 30 },
                       { 20, 25, 30, 0 } };

    // int graph[5][5] = { { 0, 10, 15, 12, 17 },
    //                     { 10, 0, 35, 25, 23 },
    //                     { 15, 35, 0, 30, 15 },
    //                     { 12, 25, 30, 0, 36 },
    //                     { 17, 23, 15, 36, 0 } };
    int s = 0;
    // start = clock();
    cout << TSP(graph, s) << endl;

    // end = clock();
    // timeTaken = ((double) (end - start)) / CLOCKS_PER_SEC;

    // std::cout<<end<<endl;
    // std::cout<<start<<endl;
    // std::cout<<"Time taken: "<<timeTaken<<" seconds"<<std::endl;
    return 0;
}