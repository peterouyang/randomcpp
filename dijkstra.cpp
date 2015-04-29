//
//  dijkstra.cpp
//
// Implementation of Dijkstra's algorithm for finding distances between
// nodes of a graph
//
// call program as ./a.out filename number_of_nodes start_node
//
// distances must be positive
// distance of -1 is treated as infinity

#include <iostream>
#include <fstream>	
#include <string>
#include <vector>
#include <list>
using namespace std;

struct edge{
	int start;
	int finish;
	int distance;
};

void read_graph(istream & is, vector<edge> & vec);
void update_distances(const int current_node, vector<edge> & gr,
					  vector<int> & temp_dist);
void update_visited(int cn, list<int> & visited, list<int> & unvisited);
void display_distances(vector<int> & distances);
void initialize(int start_node, int nodes, list<int> & unvisited, vector<int> & distances);
int next_node(vector<int> distances, list<int> unvisited);


int main(int argc, const char * argv[]) {

	list<int> visited, unvisited;
	vector<int> distances;
	vector<edge> graph;
	int num_nodes;
	int current_node;
	
	ifstream fin(argv[1]);
	num_nodes = stoi(argv[2]);
	current_node = stoi(argv[3]);

	read_graph(fin, graph);
	
	initialize(current_node, num_nodes, unvisited, distances);
	
	// main loop
	while( !unvisited.empty() )
	{
		update_visited(current_node, visited, unvisited);
		update_distances(current_node, graph, distances);
		current_node = next_node(distances, unvisited);
	}
	
	display_distances(distances);

    return 0;
}

//
// functions follow
//

int next_node(vector<int> distances, list<int> unvisited)
{
	int x = distances[*(unvisited.begin())];
	int ret = *(unvisited.begin());
	for(list<int>::iterator it = unvisited.begin(); it != unvisited.end(); it++)
	{
		if((distances[*it] < x || x == -1) && distances[*it] != -1)
		{
			x = distances[*it];
			ret = *it;
		}
	}
	return ret;
}

// read external data file containing graph data
// in the format: (start node) (end node) (distance)
//
void read_graph(istream & is, vector<edge> & vec)
{
	edge x;
	
	while(is >> x.start)
	{
		is >> x.finish;
		is >> x.distance;
		if(x.distance >= 0)
			vec.push_back(x);
		else
			cout << "Error, distance negative!" << endl;
	}
}

void update_distances(const int current_node, vector<edge> & gr,
					  vector<int> & temp_dist)
{
	for(vector<edge>::iterator it = gr.begin(); it != gr.end(); it++)
	{
		if(it->start == current_node)
		{
			if(temp_dist[it->finish] == -1 ||
			   temp_dist[it->finish] > it->distance+ temp_dist[current_node])
				temp_dist[it->finish] = it->distance + temp_dist[current_node];
		}
	}
}

void update_visited(int cn, list<int> & visited, list<int> & unvisited)
{
	// record current node as visited
	visited.push_back(cn);
	// remove current node from unvisited
	for(list<int>::iterator it = unvisited.begin(); it != unvisited.end(); it++)
	{
		if(*it == cn)
		{
			unvisited.erase(it);
			break;
		}
	}
}

void display_distances(vector<int> & distances)
{
	for(vector<int>::iterator it = distances.begin(); it != distances.end(); it++)
		cout << *it << " ";
	cout << endl;
}

void initialize(int start_node, int nodes, list<int> & unvisited, vector<int> & distances)
{
	for(int j = 0; j < nodes; j++)
	{
		unvisited.push_back(j);
		distances.push_back(-1);
	}
	distances[start_node] = 0;
}


