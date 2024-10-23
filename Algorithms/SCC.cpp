#include<vector>
#include<iostream>
class SCC
{
public:
    bool dfs(std::vector<std::vector<int>>& adj,int curr, int target, std::vector<int>& visited)
    {
        if (curr==target) return true;
        visited[curr] = 1;
        for (int x : adj[curr]) 
        {
            if (!visited[x]) 
            {
                if (dfs(adj,x,target,visited)) return true;
            }
        }
        return false;
    }

    bool pathExists(int src, std::vector<std::vector<int>>& adj, int target)
    {
        std::vector<int> visited(adj.size() + 1, 0);
        return dfs(adj,src,target,visited);
    }

    std::vector<std::vector<int>> findSCC(int n, std::vector<std::vector<int>>& a)
    {
        std::vector<std::vector<int>> SCCs;
        std::vector<int> isSCC(n+1, 0);
        std::vector<std::vector<int>> adj(n+1);

        for (int i=0; i< a.size(); i++) 
        {
            adj[a[i][0]].push_back(a[i][1]);
        }
        for (int i=1; i<=n; i++) {

            if (!isSCC[i]) 
            {
                std::vector<int> scc;
                scc.push_back(i);

                for (int j = i + 1; j <= n; j++) 
                {
                    if (!isSCC[j] && pathExists(i, adj, j) && pathExists(j, adj, i)) 
                    {
                        isSCC[j] = 1;
                        scc.push_back(j);
                    }
                }
                SCCs.push_back(scc);
            }
        }
        return SCCs;
    }
};

int main()
{

    SCC collection;
    int numV = 7;
    std::vector<std::vector<int>> edges{{1,3},{1,5},{3,5},{3,2},{4,5},{6,7},{1,7},{3,7}};
    std::vector<std::vector<int>>SCC=collection.findSCC(numV, edges);
    for(int i=0; i<SCC.size(); i++)
    {
        std::cout<<"SCC No."<<i+1<<std::endl;
        for (auto x : SCC) 
        {
            for (auto y : x) 
            {
                std::cout<<y<<" ";
            }
            std::cout << "\n";
        }
    }
}