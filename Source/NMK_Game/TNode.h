/*
 * TNode.h
 *
 *  Created on: Jul 26, 2020
 *      Author: Alonso Vega
 */

#ifndef TNODE_H_
#define TNODE_H_

#include "TTT_state.h"

class TNode
{
public:

	TNode();
	TNode(TTT_state x_t, TNode* v_t_minus_1);
	TNode(const TNode& v);
	virtual ~TNode();

	TTT_state get_state();
	tuple<int, int> get_input_control();
	set< tuple<int, int> > get_U_expanded();

	TNode* get_parent();
	std::vector< TNode* > get_children();

	bool is_leaf();
	bool expandable();

	void set_state(const TTT_state x);
	void set_input_control(int i_1, int i_2);
	void expanded_control(tuple<int, int> u);

	void insert__child(TTT_state v);

	VecInt get_N_values();
	VecDoub get_Q_values();

	void inc_N_value(int n, int ind);
	void inc_Q_values(double q, int ind);



private:
	TTT_state state;
	tuple<int, int> input_control;
	set< tuple<int, int> > U_expanded;

	TNode* parent;
	vector< TNode* > children;


	VecDoub Q_values;
	VecInt N_value;



};

#endif /* TNODE_H_ */
