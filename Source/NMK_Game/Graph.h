/*
 * Graph.h
 *
 *  Created on: June 22, 2020
 *      Author: Alonso Vega
 */

#include <set>
#include <map>
#include "nr3.h"

#ifndef GRAPH_H_
#define GRAPH_H_

#define LOOP(var, i, end_plus1) for (var = i; var < end_plus1; var++)
#define INF std::numeric_limits<int>::max()
#define NEG_INF std::numeric_limits<int>::min()


class Graph
{

public:

	static std::map< std::tuple<int, int>, int > vertexMap;

	Graph(int num_vertices);

	void add_nodes(std::vector< std::tuple<int, int> > ω_1, std::vector< std::tuple<int, int> > ω_2);

	virtual ~Graph();

	void Init(int n);

	int get_num_vertices();
	int get_num_edges();

	int first(int x);
	int next(int x_1, int x_2);
    std::set<int> neighbors(int x);

	void set_edge(int x_1, int x_2, int weight);
	void set_edge(std::tuple<int, int> r_1, std::tuple<int, int> r_2, int weight);
	void del_edge(std::tuple<int, int> r_1, std::tuple<int, int> r_2);
	void del_edge(int x_1, int x_2);

	bool is_edge(int x_1, int x_2);

	int get_weight(int x_1, int x_2);

	int get_mark(int x);
	void set_mark(int x, int value);

	int get_fill(int x);
	void set_fill(int x, int value);

	void gen_const();

private:

	int num_vertices;
	int num_edges;
	int **matrix;
	int *mark;
	int *fill;

};

#endif /* GRAPH_H_ */
