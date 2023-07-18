#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <bits/stdc++.h>
#include <queue>

using namespace std;

class Kid{
    public:
        int x;
        int y;
        int strength;
};

class Graph
{
    public:
        int kid_num;
        int source_kid;
        int target_kid;
        bool first_step;
        bool is_done;
        int** graph;
        Kid* kids;
        vector <bool> is_discovered;
        vector<int> dfs_cycle;
        map<int, bool> is_visited;
        map<int, list<int> > connections;
        ~Graph();
        Graph();
        void read_txt(fstream&, string);
        void init_graph();
        void write_graph();
        void depth_first_search(int);
        void add_edge(int, int);
        void write_dfs();
        void breadth_first_search(int, int);
        void write_bfs(int, int, unordered_map<int, int>&);
        
};
Graph::Graph()
{
    // Set first step to true to get source kid check twice for dfs algorithm
    first_step = true;
    // Set is done to false to check is first cycle found
    is_done = false;
}
// Free memory with destructor
Graph::~Graph()
{
    for (int i = 0; i < kid_num; i++) {
      delete[] graph[i];
    }
    delete[] graph;

    delete[] kids;
}

void Graph::read_txt(fstream& file, string delimiter)
{
    // to get parsed str 
    size_t pos = 0;
    // placeholders
    string line, parsed;
    // to check is first line
    bool is_first = true;
    // to check first splitted str
    bool is_f_parsed = true;
    // to assign kids properties
    int kid_idx;
    while (std::getline(file, line))
    {
        is_f_parsed = true;
        // If first line
        if(is_first)
        {
            while ((pos = line.find(delimiter)) != std::string::npos)
            {
                // parse str
                parsed = line.substr(0, pos);
                // assign total kid number
                if(is_f_parsed)
                {
                    // convert string to integer
                    kid_num = stoi(parsed);
                    // Allocate required array space for graph
                    graph = new int*[kid_num];
                    for (int i=0; i<kid_num; i++)
                        graph[i] = new int[kid_num];
                    // Allocate required array space for kids
                    kids = new Kid[kid_num];
                }
                // assign source kid
                else
                    source_kid = stoi(parsed);
                // remove obtained str to move next one
                line.erase(0, pos + delimiter.length());
                is_f_parsed = false; 
            }
            target_kid = stoi(line);
            is_first = false;
        }

        else
        {
            while ((pos = line.find(delimiter)) != std::string::npos)
            {
                // parse str
                parsed = line.substr(0, pos);
                // assign y location
                if(is_f_parsed)
                    kids[kid_idx].x = stoi(parsed);
                // assign y location
                else
                    kids[kid_idx].y = stoi(parsed);
                // remove obtained str to move next one
                line.erase(0, pos + delimiter.length());
                is_f_parsed = false; 
            }
            kids[kid_idx].strength = stoi(line);
            kid_idx++;
        }

    }
}


void Graph::init_graph()
{
    int distance;
    for(int i=0; i<kid_num; i++)
    {
        for(int j=0; j<kid_num; j++)
        {
            // To pass distance calculation of node itself
            if(i == j)
                continue;
            // squared distance between two kid
            distance = pow(abs(kids[i].x - kids[j].x),2) + pow(abs(kids[i].y - kids[j].y), 2);
            // If both sides are able to throw ball back to each other then set edge
            if(distance <= kids[i].strength && distance <= kids[j].strength)
            {
                graph[i][j] = 1;
                add_edge(i, j);
            }

        }
    }
}

void Graph::write_graph()
{
    string total_str = "Graph:\n";
    string line;
    for(int i=0; i<kid_num; i++)
    {
        line = "";
        for(int j=0; j<kid_num; j++)
        {
            line += to_string(graph[i][j]) + " ";
        }
        line = line.substr(0, line.length() - 1) + "\n";
        total_str += line;
    }

    ofstream graph_txt;
    graph_txt.open ("graph.txt");
    graph_txt << total_str;
    graph_txt.close();
}

void Graph::add_edge(int cur_node, int next_node)
{
    // Add connections of next nodes to current node 
    connections[cur_node].push_back(next_node);
}

void Graph::depth_first_search(int node)
{
    // If cycle is found return 
    if(is_done)
        return;

    // Set as visited to that node
    is_visited[node] = true;
    // Insert node to cycle
    dfs_cycle.push_back(node);

    list<int>::iterator iter;

    for (iter = connections[node].begin(); iter != connections[node].end(); ++iter)
    {   
        if(is_visited[*iter] && *iter == source_kid)
        {
            // Boolean to check source node twice
            if(first_step)
                first_step = false;
            // Set done to end loop 
            else
            {
                is_done = true;
                dfs_cycle.push_back(*iter);
            }
        }

    }

    
    for (iter = connections[node].begin(); iter != connections[node].end(); ++iter)
    {
        if (!is_visited[*iter])
        {
            // Iterate through nodes
            depth_first_search(*iter);
            // If source node is found the end loop
            if(is_done)
                return;
            // If moving back to previous node pop that last node
            dfs_cycle.pop_back();
        }
            
    }
}

void Graph::write_dfs()
{
    string result="DFS: \n";
    // If could not generate the cycle
    // If cycle length equal or below 3 (source node counted twice)
    if(dfs_cycle.back() != source_kid || dfs_cycle.size() <= 3)
        result += "-1";
    else
    {
        // Unsigned long int values are casted to integer
        // At first cycle length is given
        result += to_string(static_cast<int>(dfs_cycle.size())-1) + " ";
        // Then cycle is written in loop 
        for(int i = 0; i < static_cast<int>(dfs_cycle.size()); i++)
        {
            result += to_string(dfs_cycle[static_cast<int>(i)]) + "->";
        }
        // Finally last "->" is removed
        result = result.substr(0, result.length() - 2);
    } 
    // Writing to txt
    ofstream dfs_txt;
    // Open txt
    dfs_txt.open ("dfs.txt");
    // Write result into txt
    dfs_txt << result;
    // Close it
    dfs_txt.close();

}


void Graph::breadth_first_search(int source_kid, int target_kid)
{
    
    // To check is node visited 
    bool* bfs_visited = new bool[kid_num];
    // set all to false
    for (int i = 0; i < kid_num; i++)
        bfs_visited[i] = false;

    // Queue for breadth first search
    queue<int> bfs_queue;

    // Is visited array to check is node visited
    bfs_visited[source_kid] = true;
    bfs_queue.push(source_kid);

    // Unordered map to keep information of previous connections
    unordered_map<int, int> previous;
    previous[source_kid] = -1;
    // placeholder of current node
    int cur_node;
    // placeholder of next node
    int next_node;
    // Until queue is empty run iteration
    while (!bfs_queue.empty())
    {
        // Get first node 
        cur_node = bfs_queue.front();
        // Pop it to iterate following nodes
        bfs_queue.pop();

        // If path to target kid is found then finish the loop
        if (cur_node == target_kid) {
            write_bfs(source_kid, target_kid, previous);
            return;
        }
        // Iterate through nodes 
        for (auto i = connections[cur_node].begin(); i != connections[cur_node].end(); ++i)
        {
            // Set next node
            next_node = *i;
            // If node is not visited
            if (!bfs_visited[next_node])
            {
                // Set visited
                bfs_visited[next_node] = true;
                // Insert it to queue
                bfs_queue.push(next_node);
                // Set previous node of next node to current node
                previous[next_node] = cur_node;
            }
        }
    }
}

void Graph::write_bfs(int source_kid, int target_kid, unordered_map<int, int>& previous)
{
    // Source to target path
    stack<int> throw_path;
    
    // Append target node
    throw_path.push(target_kid);
    // Init txt string
    string result = "BFS: \n" ;

    // Iterate from target to source kid
    int cur_node = target_kid;
    while (cur_node != source_kid)
    {
        throw_path.push(previous[cur_node]);
        cur_node = previous[cur_node];
    }
    result += to_string(throw_path.size() - 1) + " ";
    while (!throw_path.empty())
    {
        result += to_string(throw_path.top()) + "->";
        throw_path.pop();
    }
    result = result.substr(0, result.length() - 2);
    // Writing to txt
    ofstream bfs_txt;
    // Open txt
    bfs_txt.open ("bfs.txt");
    // Write result into txt
    bfs_txt << result;
    // Close it
    bfs_txt.close();
}

int main(int argc, char *argv[]){

    // Check input path is passed
    if(argc <= 1)
    {
        cout<<"Input txt file should be passed!"<<endl;
        return 0;
    }
    // Read Input path
    fstream file(argv[1]);
    // Delimiter
    string delimiter = " ";
    // Init graph class
    Graph graph;
    // Read txt

    graph.read_txt(file, delimiter);
    
    // Init graph
    graph.init_graph();
    
    // Write graph to txt
    graph.write_graph();

    // Run depth first search to find first cycle between kids
    graph.depth_first_search(graph.source_kid);
    // Write results of depth first search cycle
    
    graph.write_dfs();
    
    // Find path between source and target kid and write it to txt
    graph.breadth_first_search(graph.source_kid, graph.target_kid);

    exit(0);
}