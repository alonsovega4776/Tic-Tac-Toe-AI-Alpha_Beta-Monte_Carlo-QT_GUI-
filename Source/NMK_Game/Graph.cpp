/*
 * Graph.cpp
 *
 *  Created on: June 22, 2020
 *      Author: Αlonso Vega
 */

#include "Graph.h"

std::map< std::tuple<int, int>, int > Graph::vertexMap = {};

void Graph::gen_const()
{
    int N = sqrt(num_vertices);
    int row, col;

    LOOP(row, 0, N)
        LOOP(col, 0, N)
            vertexMap.insert(std::pair< std::tuple<int, int>, int >(std::make_tuple(row, col), col + row*N));
    /*
    LOOP(row, 0, N)
        LOOP(col, 0, N)
            cout <<  vertexMap.at(std::make_tuple(row, col)) << " <--- (" << row << "," << col << ") \n";
//  */
}



Graph::Graph(int num_vertices)							//Constructor
{
    Init(num_vertices);									//number of nodes is constant
    if(vertexMap.empty()) gen_const();
}


Graph::~Graph()											//Destructor
{
    delete [] mark;
    delete [] fill;

    int i;
    LOOP(i, 0, num_vertices)
        delete [] matrix[i];

    delete [] matrix;
}

void Graph::add_nodes(std::vector< std::tuple<int, int> > ω_1, std::vector< std::tuple<int, int> > ω_2)							//Constructor
{
    if (ω_1.size() != ω_2.size()) thrrow("Graph Constructor: WRONG VECTOR SIZES");


    std::vector< std::tuple<int, int> >::iterator iter_1 = ω_1.begin();
    std::vector< std::tuple<int, int> >::iterator iter_2 = ω_2.begin();

    LOOP(iter_1, ω_1.begin(), ω_1.end())
    {
        set_edge(*iter_1, *iter_2, 1);
        iter_2++;
    }
}

void Graph::Init(int n)									//Initialization
{
    num_vertices = n;
    num_edges = 0;

    int i, j;
    mark = new int[n];
    LOOP(i, 0, num_vertices)
        mark[i] = 0;									//mark as unvisited

    fill = new int[n];
        LOOP(i, 0, num_vertices)
            fill[i] = 0;								//empty mark

    matrix = (int ** ) new int * [num_vertices];
    LOOP(i, 0, num_vertices)
        matrix[i] = new int[num_vertices];

    LOOP(i, 0, num_vertices)
        LOOP(j, 0, num_vertices)
            matrix[i][j] = 0;
}

//--------------------------------------------Nodes-------------------------------------------------------------

int Graph::get_num_vertices() 							//get number of nodes
{
    return num_vertices;
}

int Graph::get_fill(int x)
{
    return fill[x];
}

void Graph::set_fill(int x, int value)
{
    fill[x] = value;
}

//--------------------------------------------Hood-------------------------------------------------------------

int Graph::first(int x)
{
    int i;
    LOOP(i, 0, num_vertices)
        if (matrix[x][i] != 0) return i;
    return INF;
}

int Graph::next(int x_1, int x_2)
{
    if (x_2 >= num_vertices) return INF;

    int i;
    LOOP(i, x_2+1, num_vertices)
        if (matrix[x_1][i] != 0) return i;
    return INF;
}

std::set<int> Graph::neighbors(int x)
{
    std :: set<int> hood;

    int y = first(x);
    if (y >= num_vertices) return hood;
    hood.insert(y);

    y = next(x, y);
    while (y < num_vertices)
    {
        hood.insert(y);
        y = next(x, y);
    }

    return hood;
}


//----------------------------------------------Εdge------------------------------------------------------------

int Graph::get_num_edges()								//get number of edges
{
    return num_edges;
}

void Graph::set_edge(int x_1, int x_2, int weight)		//insert undirected edge
{
    if (matrix[x_1][x_2] == 0) num_edges++;

    matrix[x_1][x_2] = weight;
    matrix[x_2][x_1] = weight;
}
void Graph::set_edge(std::tuple<int, int> r_1, std::tuple<int, int> r_2, int weight)
{
    int x_1 = vertexMap.at(r_1);
    int x_2 = vertexMap.at(r_2);

    set_edge(x_1, x_2, weight);
}

void Graph::del_edge(int x_1, int x_2)					//delete undirected edge
{
    if (matrix[x_1][x_2] != 0)
        num_edges--;
    matrix[x_1][x_2] = 0;
    matrix[x_2][x_1] = 0;
}

void Graph::del_edge(std::tuple<int, int> r_1, std::tuple<int, int> r_2)
{
        int x_1 = vertexMap.at(r_1);
        int x_2 = vertexMap.at(r_2);

        del_edge(x_1, x_2);
}


bool Graph::is_edge(int x_1, int x_2)					//there exist an edge {x_1, x_2}
{
    return matrix[x_1][x_2] != 0;
}

int Graph::get_weight(int x_1, int x_2)
{
    return matrix[x_1][x_2];
}

//-------------------------------------------Mark--------------------------------------------------------------
int Graph::get_mark(int x)
{
    return mark[x];
}

void Graph::set_mark(int x, int value)
{
    mark[x] = value;
}
