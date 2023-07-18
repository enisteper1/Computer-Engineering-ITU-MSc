#include <iostream>
#include <queue>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <bits/stdc++.h>
#include <string>
#include <stack>

using namespace std;

class City
{
    public:
        int x;
        int y;
        double pount;
};

class Graph
{
    public:
        // city num placeholder
        int cities_num;
        // baker num placeholder
        int bakers_num;
        // bakers placeholder
        int* bakers;
        // cities placeholder
        City* cities;
        // threshold to check plow
        double threshold;
        // graph to keep plow values between cities
        int** graph;
        // check if the node is visited
        map<int, bool> is_visited;
        // connections for each baker
        map<int, list<int> > connections;
        // visited connections of each backer
        map<int, list<int>> visited_connections;
        // destructor to free the memory
        ~Graph();
        // read txt file and load data into memory
        void read_txt(fstream&, string);
        // initialize graph by making connections via considering requirements
        void init_graph();
        // adds edge between given 2 node
        void add_edge(int, int);
        // Runs the prim algorithm
        void run_prim();
        // writes the result to txt
        void write_prim();
        
};
// Free memory with destructor
Graph::~Graph()
{
    for (int i = 0; i < cities_num; i++) {
      delete[] graph[i];
    }
    delete[] graph;

    delete[] bakers;
}

void Graph::read_txt(fstream& file, string delimiter)
{
    // to get parsed str 
    size_t pos = 0;
    // placeholders
    string line, parsed;
    // to track line
    int line_idx = 0;
    // to check first splitted str
    bool is_f_parsed = true;
    // to keep parsed index to assign bakers
    int baker_idx = 0;
    // to keep parsed index to assign cities
    int city_idx = 0;
    while (std::getline(file, line))
    {
        is_f_parsed = true;
        // If first line
        if(line_idx == 0)
        {
            while ((pos = line.find(delimiter)) != std::string::npos)
            {
                // parse str
                parsed = line.substr(0, pos);
                // assign total kid number
                if(is_f_parsed)
                {
                    // convert string to integer
                    cities_num = stoi(parsed);
                    // Allocate required array space for graph
                    graph = new int*[cities_num];
                    for (int i=0; i<cities_num; i++)
                        graph[i] = new int[cities_num];
                    // Allocate required array space for cities
                    cities = new City[cities_num];
                }
                // get bakers number and generate the required storage
                else
                {
                    bakers_num = stoi(parsed);
                    bakers = new int[bakers_num];
                }
                // remove obtained str to move next one
                line.erase(0, pos + delimiter.length());
                is_f_parsed = false; 
            }
            threshold = stod(line);
        }

        else if (line_idx == 1)
        {
            while ((pos = line.find(delimiter)) != std::string::npos)
            {
                // parse str
                parsed = line.substr(0, pos);
                // assign y location
                bakers[baker_idx] = stoi(parsed);
                // remove obtained str to move next one
                line.erase(0, pos + delimiter.length());
                baker_idx++;
            }
            bakers[baker_idx] = stoi(line);
            baker_idx++;
        }

        else
        {
            while ((pos = line.find(delimiter)) != std::string::npos)
            {
                // parse str
                parsed = line.substr(0, pos);
                // assign y location
                if(is_f_parsed)
                    cities[city_idx].x = stoi(parsed);
                // assign y location
                else
                    cities[city_idx].y = stoi(parsed);
                // remove obtained str to move next one
                line.erase(0, pos + delimiter.length());
                is_f_parsed = false; 
            }
            cities[city_idx].pount = stoi(line);
            city_idx++;
        }

        line_idx++;

    }
}

void Graph::add_edge(int cur_node, int next_node)
{
    // Add connections of next nodes to current node 
    connections[cur_node].push_back(next_node);

}


void Graph::init_graph()
{
    int plow;
    for(int i=0; i < cities_num; i++)
    {
        for(int j=0; j < cities_num; j++)
        {
            // To pass distance calculation of node itself
            if(i == j)
                continue;
            // plow calculation between 2 city
            plow = abs(cities[i].pount - cities[j].pount);
            
            // If plow requirement is satisfied then set edge between that 2 city
            if(plow <= threshold * (cities[i].pount + cities[j].pount) / 2 && plow != 0)
            {
                // save plow between 2 city
                graph[i][j] = plow;
                // make connection between 2 city
                add_edge(i, j);
            }

        }
    }
}


void Graph::run_prim()
{
    int baker_idx = bakers[0];

    // priority queue based map
    unordered_map<int, priority_queue<pair<int, int>, vector<pair<int, int>>>> pqueue_map;
    // define initial baker stores
    for(int k=0; k < bakers_num; k++)
    {
        // why we inserted indexes as minus is explained at line 218-219
        baker_idx = -bakers[k];
        pqueue_map[baker_idx].push({0, baker_idx});
    }

    // placeholder of current node
    int cur_node = 0;
    // placeholder of next node
    int next_node;

    // to prevent to check neighbors of already visited neighbors
    int done_baker = 0;
    while(done_baker != bakers_num)
    {
        done_baker = 0;
        for(int k=0; k < bakers_num; k++)
        {
            // since the priority queue uses higher to lower insertion indexes
            // are inserted as minus to keep abs(lower index) on top
            baker_idx = -bakers[k];
            if(pqueue_map[baker_idx].empty())
            {
                done_baker++;
                continue;
            }
            // get index of current node of related baker
            cur_node = pqueue_map[baker_idx].top().second;
            // remove top node from that index
            pqueue_map[baker_idx].pop();
            if(!is_visited[abs(cur_node)])
            {
                // set that node as visited
                is_visited[abs(cur_node)] = true;
                // insert that node as visited connections of that baker
                visited_connections[baker_idx].push_back(abs(cur_node));
            }
            else
            {
                // to dont let that baker pass if the current node is already visited 
                k--;
                continue;
            }

            // Iterate through nodes 
            for (auto i = connections[abs(cur_node)].begin(); i != connections[abs(cur_node)].end(); ++i)
            {
                // Set next node
                next_node = *i;

                // If node is not visited insert it to queue
                if (!is_visited[next_node])
                    pqueue_map[baker_idx].push({graph[abs(cur_node)][next_node], -next_node});
            }
        }

    }

    // write results to txt
    write_prim();
}


void Graph::write_prim()
{
    // string to write txt
    string result;
    // iterator index
    int baker_idx;
    // save each baker connections to txt
    for(int k=0; k < bakers_num; k++)
    {
        // get the baker index
        baker_idx = -bakers[k];
        // index of the baker
        result += "k" + to_string(k) + " " + to_string(visited_connections[baker_idx].size()) + "\n";
        // connections of that baker
        for(auto idx = visited_connections[baker_idx].begin(); idx != visited_connections[baker_idx].end(); idx++)
            result += to_string(*idx) + "->";
        // finally remove "->" and give horizontal space for next backer
        result = result.substr(0, result.length() - 2) + "\n";
    }
    
    // Writing to txt
    ofstream prim_txt;
    // Open txt
    prim_txt.open ("prim.txt");
    // Write result into txt
    prim_txt << result;
    // Close it
    prim_txt.close();
}

int main(int argc, char *argv[]){
    /*
        Keep your solution for Prim's algorithm Part in this file!
        Program Compile Command: g++ -std=c++11 -Wall -Werror prim.cpp -o prim
        Program Run Command: ./prim <input.txt>
        Expected input: /cases/case{$n}/input{$n}.txt
        Expected output: prim.txt
        Please, try to write clean and readable code. Remember to comment!!
    */
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

    // run prim algorithm
    graph.run_prim();

    return 0;
    exit(0);
}