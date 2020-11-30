/*
 * TTT_state.h
 *
 *  Created on: Jul 10, 2020
 *  Author: Alonso Vega
 */

#ifndef TTT_STATE_H_
#define TTT_STATE_H_
#define TTT_SIZE 3
#define TTT_WIN_SIZE 3

#include "Graph.h"
#include <tuple>
#include <random>

class TTT_state
{
public:

	static std::vector< std::tuple<int, int> > w_1;
	static std::vector< std::tuple<int, int> > w_2;

	static Graph winningPaths;
	static vector<int> Z_TTT_SIZE_vec;
	static const std::map<int, int> toPrint;

	TTT_state();
	TTT_state(char first_play);
	TTT_state(const TTT_state& y);
	virtual ~TTT_state();

	MatInt get_state();
	set< tuple<int, int> > get_controlSpace();
	bool in_contorlSpace(int i_1, int i_2);

	void print_controlSpace();

	void transition();
	void transition(int i, int j);
	bool terminal(int r, int c, int player);
	bool terminal(int i_c, int j_c, int player_num, int K);
	double evaluation();
	int utility();

	Graph state_graphs();

	int get_time();
	bool get_termina();
	char get_player();
	string get_outcome();

	void print_state();
	void print_board();
	void print_board(int n_space);

	void gen_const();

	MatInt chess_board(int i_c, int j_c, int d);


private:
	MatInt state;
	int time;
	set< tuple<int, int> > controlSpace;
	bool termina;
	char player;
	string outcome;
};

#endif /* TTT_STATE_H_ */
