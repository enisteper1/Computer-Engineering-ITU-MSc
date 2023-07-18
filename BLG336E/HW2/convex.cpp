#include <iostream>
#include <queue>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <string>
#include <math.h>

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
        // city number placeholder
        int cities_num;
        // placeholder of all cities
        City* cities;
        // inversed dictionary to reach indexes of points
        map<int, int> inverse_dict;
        // read txt file to load data into memory
        void read_txt(fstream&, string);
        // sort cities by their locations
        vector<pair<int, int>> sort_cities();
        // save result to txt
        void write_convex(vector<pair<int, int>>);
        // merge 2 convex to generate more general one
        vector<pair<int, int>> convex_merger(vector<pair<int, int>>,
                            vector<pair<int, int>>);
        // generate minimal convex
        vector<pair<int, int>> basic_convex(vector<pair<int, int>>);
        // divide points from middle to for divide & conquer approach
        vector<pair<int, int>> divide(vector<pair<int, int>>);
        // check tangents if they are ok
        bool point_checker(pair<int, int>, 
                        pair<int, int>,
                        pair<int, int>);
};


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
                // assign total city number
                if(is_f_parsed)
                {
                    // convert string to integer
                    cities_num = stoi(parsed);
                    // Allocate required array space for cities
                    cities = new City[cities_num];
                }
                // remove obtained str to move next one
                line.erase(0, pos + delimiter.length());
                is_f_parsed = false; 
            }
        }

        else if (line_idx > 1)
        {
            while ((pos = line.find(delimiter)) != std::string::npos)
            {
                // parse str
                parsed = line.substr(0, pos);
                // assign y location
                if(is_f_parsed)
                {
                    inverse_dict[stoi(parsed)] = city_idx;
                    cities[city_idx].x = stoi(parsed);

                }
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

 
// Check the area of the point 
int area_checker(pair<int, int> point)
{
    // I
    if (point.first >= 0 && point.second >= 0)
        return 1;
    // II
    else if (point.first <= 0 && point.second >= 0)
        return 2;
    // III
    else if (point.first <= 0 && point.second <= 0)
        return 3;
    // IV
    else
        return 4;
}

pair<int, int> mid_point;
bool point_comparer(pair<int, int> first_point, pair<int, int> second_point)
{
    // calculate first point relatively to mid
    pair<int, int> p1 = make_pair(first_point.first - mid_point.first,
                                first_point.second - mid_point.second);
    // calculate second point relatively to mid
    
    pair<int, int> p2 = make_pair(second_point.first - mid_point.first,
                                second_point.second - mid_point.second);
 
    int p1_area = area_checker(p1);
    int p2_area = area_checker(p2);
    bool area_result;

    // If areas are not equal
    // set true if p1 area lower than p2, false otherwise
    if (p1_area != p2_area)
        area_result = p1_area < p2_area;
    // If areas are equal
    // check relatively and set true if multiplying y1*x2 lower than x1*y2, false otherwise 
    else
        area_result = p1.second*p2.first < p1.first * p2.second;
    // return result
    return area_result;
}

bool Graph::point_checker(pair<int, int> p1, 
                        pair<int, int> p2,
                        pair<int, int> checkpoint)
{
    // y = slope * x + constant
    // slope = (y2 - y1) / (x2 - x1)
    double slope = 1.0 * (p2.second - p1.second) / (p2.first - p1.first);
    // y - y1 = slope * (x - x1) -> y = slope * x - slope * x1 + y1
    // constant = y1 - slope * x1
    double constant = p1.second - slope * p1.first;
    // since we have found the line now its time to check where the next point is below or above the line
    double y = slope * checkpoint.first + constant;
    // If y >= then the checkpoint it means its an upper tangent
    // Otherwise lower tangent
    return (y >= checkpoint.second);

}


vector<pair<int, int>> Graph::convex_merger(vector<pair<int, int> > polygon1,
                            vector<pair<int, int> > polygon2)
{

    // placeholder of left polygon
    int p1_size = polygon1.size();
    // placeholder of the size of the left polygon
    int p2_size = polygon2.size();

    // placeholder of rightmost point of polygon1
    int p1_rmost = 0;
    // placeholder of leftmost point of polygon2
    int p2_lmost = 0;

    // finds the left most point of polygon2
    for (int i=1; i<p2_size; i++)
        if (polygon2[i].first < polygon2[p2_lmost].first)
            p2_lmost=i;

    // finds the right most point of polygon1
    for (int i=1; i<p1_size; i++)
        if (polygon1[i].first > polygon1[p1_rmost].first)
            p1_rmost = i;
 
    
    // iterator point of polygon1
    int p1_tmp = p1_rmost;
    // iterator point of polygon2
    int p2_tmp = p2_lmost;
    // Checks the loop is finished or not
    bool is_finished = false;

    // Loop to find upper tangent
    while (!is_finished)
    {
        is_finished = true;
        // find most suitable left point of polygon2 for upper tangent
        // This loop just iterates all points by going left most point to mid-upper one by one
        while(!point_checker(polygon1[p1_tmp], polygon2[p2_tmp], polygon2[(p2_size+p2_tmp-1)%p2_size]))
            p2_tmp = (p2_size+p2_tmp-1)%p2_size;

        // finds most suitable right point of polygon1 for upper tangent
        // This loop just iterates all points by going right most point to mid-upper one by one
        while(!point_checker(polygon1[p1_tmp], polygon2[p2_tmp], polygon1[(p1_tmp+1)%p1_size]))
        {
            p1_tmp = (p1_tmp + 1) % p1_size;
            // If point changes set is finished to false
            is_finished = false;
        }  
    }
    // upper point of polygon1
    int p1_upper = p1_tmp;
    // upper point of polygon2
    int p2_upper = p2_tmp;
    // assign leftmost and rightmost points again to find lower tangent
    p1_tmp = p1_rmost;
    p2_tmp=p2_lmost;

    is_finished = false;
    while (!is_finished)
    {
        
        is_finished = true;
        // find most suitable left point of polygon2 for lower tangent
        // This loop just iterates all points by going left most point to mid-lower one by one
        while(point_checker(polygon1[p1_tmp], polygon2[p2_tmp], polygon2[(p2_tmp+1)%p2_size]))
            p2_tmp = (p2_tmp+1)%p2_size;
        
        // find most suitable left point of polygon1 for lower tangent
        // This loop just iterates all points by going rightmost point to mid-lower one by one
        while(point_checker(polygon1[p1_tmp], polygon2[p2_tmp], polygon1[(p1_size + p1_tmp - 1)%p1_size]))
        {
            p1_tmp = (p1_size + p1_tmp - 1) % p1_size;
            is_finished = false;
        }

    }
    
    // lower point of polygon1
    int p1_lower = p1_tmp;
    // lower point of polygon2
    int p2_lower = p2_tmp;
    // Result convex
    vector<pair<int, int>> res_convex;

    // Add from upper point to lower one of polygon1
    int iter_point = p1_upper;
    res_convex.push_back(polygon1[p1_upper]);
    while (iter_point != p1_lower)
    {
        // anti-clockwise iteration from upper to lower point
        iter_point = (iter_point+1)%p1_size;
        res_convex.push_back(polygon1[iter_point]);
    }
    
    // Add from lower point to upper point polygon2
    iter_point = p2_lower;
    res_convex.push_back(polygon2[p2_lower]);
    while (iter_point != p2_upper)
    {
        // anti-clockwise iteration from lower to upper point
        iter_point = (iter_point+1)%p2_size;
        res_convex.push_back(polygon2[iter_point]);
    }
    return res_convex;
 
}
 

vector<pair<int, int>> Graph::basic_convex(vector<pair<int, int>> convex)
{
    
    // set mid_pint to origin
    mid_point = {0, 0};
    int convex_size = convex.size();
    // for each point increase mid to divide convex by 2
    for (int i=0; i < convex_size; i++)
    {
        // increase x point
        mid_point.first += convex[i].first;
        // increase y point
        mid_point.second += convex[i].second;
        // since we don't divide the midpoint
        // In order to keep ratio with points and mid point
        // each point multiplied by size
        convex[i].second *= convex_size;
        convex[i].first *= convex_size;
        
    }
    // quicksort algorithm with implemented function to sort
    sort(convex.begin(), convex.end(), point_comparer);

    // generate the final convex
    for (int i=0; i<convex_size; i++)
        convex[i] = make_pair(convex[i].first/convex_size, convex[i].second/convex_size);

 
    return convex;
}


vector<pair<int, int>> Graph::divide(vector<pair<int, int>> convex)
{
    // Since 3 point represents the minimal convex hull
    // We generate the basic convex
    if (convex.size() <= 3)
        return basic_convex(convex);

    // placeholder of left and right polygons
    vector<pair<int, int>> left, right;
    // insert left points to left side
    for (int i=0; i<static_cast<int>(convex.size()/2); i++)
        left.push_back(convex[i]);

    // insert right points to right side
    for (int i=convex.size()/2; i<static_cast<int>(convex.size()); i++)
        right.push_back(convex[i]);

    // convex hull for the left and right sets
    vector<pair<int, int>>left_hull = divide(left);
    vector<pair<int, int>>right_hull = divide(right);
 
    // merging the convex hulls
    return convex_merger(left_hull, right_hull);
}

void Graph::write_convex(vector<pair<int, int>> convex)
{
    // will be modified 
    vector<pair<int, int>> modified_convex = convex;
    // get left most point
    int min_x = static_cast<int>(INFINITY);
    for(auto p:convex)
    {
        if(p.first < min_x)
            min_x = p.first;
    }
    // Operations below are done to start from leftmost point of the convex
    // pop items from list until left most x point
    queue<pair<int, int>> q;
    for(auto p:convex)
    {
        if(p.first == min_x)
            break;
        q.push(make_pair(p.first, p.second));
        modified_convex.erase(modified_convex.begin());
    }

    // move first segments to end to start from left most point
    pair<int, int> p;
    while(!q.empty())
    {
        p = q.front();
        q.pop();
        modified_convex.push_back(p);
    }
    // add last item
    modified_convex.push_back(modified_convex.front());
    // convert size of the convex to string 
    // first line
    string result = to_string(modified_convex.size() - 1) + "\n";

    // Iterate points and convert them to string
    for (auto p:modified_convex)
        result += to_string(inverse_dict[p.first]) + "->";

    // finally remove last "->" word
    result = result.substr(0, result.length() - 2) + "\n";
    
    // Writing to txt
    ofstream convex_txt;
    // Open txt
    convex_txt.open ("convex.txt");
    // Write result into txt
    convex_txt << result;
    // Close it
    convex_txt.close();

}

vector<pair<int, int>> Graph::sort_cities()
{
    // placeholder of sorted cities
    vector<pair<int, int>> tmp_cities;
    // add each city one by one
    for(int i=0; i < cities_num; i++)
        tmp_cities.push_back(make_pair(cities[i].x, cities[i].y));

    // sorting the set of points according to the x-coordinate
    // with the quicksort algorithm
    sort(tmp_cities.begin(), tmp_cities.end());
    
    return tmp_cities;
}

int main(int argc, char *argv[]){
    /*
        Keep your solution for Convex Hull Part in this file!
        Program Compile Command: g++ -std=c++11 -Wall -Werror conve.cpp -o convex
        Program Run Command: ./convex <input.txt>
        Expected input: /cases/case{$n}/input{$n}.txt
        Expected output: convex.txt
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
    // Read data and generate graph & arrays
    graph.read_txt(file, delimiter);
    // sort loaded cities by their locations x, y priority
    vector<pair<int, int>> sorted_cities = graph.sort_cities();
    // Divide & Conquer with Bitantent Hull Merging Approach
    vector<pair<int, int>> result_convex = graph.divide(sorted_cities);
    // Write result to txt
    graph.write_convex(result_convex);
    
    return 0;
    exit(0);
}