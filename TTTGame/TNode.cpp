/*
 * TNode.cpp
 *
 *  Created on: Jul 26, 2020
 *      Author: Alonso Vega
 */

#include "TNode.h"

TNode::TNode(TTT_state x_t, TNode* v_t_minus_1)
{
	state = x_t;
	parent = v_t_minus_1;


	int N_vect = x_t.get_controlSpace().size();
	if (N_vect == 0)
	{
		Q_values.assign(1, 0);
		N_value.assign(1, 0);
	}
	else
	{
		Q_values.assign(N_vect, 0);
		N_value.assign(N_vect, 0);
	}
}

TNode::TNode()
{
	parent = NULL;
	N_value.assign(TTT_SIZE*TTT_SIZE, 0);
	Q_values.assign(TTT_SIZE*TTT_SIZE, 0);
}

TNode::TNode(const TNode& v)
{
	state = v.state;
	parent = v.parent;
	children = v.children;

	N_value = v.N_value;
	Q_values = v.Q_values;

}

TNode::~TNode()
{
	int i;
	LOOP(i, 0, children.size())
	{
		delete children[i];
		children[i] = NULL;
	}
}

TTT_state TNode::get_state()
{
	return state;
}

TNode* TNode::get_parent()
{
	return parent;
}

std::vector< TNode* > TNode::get_children()
{
	return children;
}

VecInt TNode::get_N_values()
{
	return N_value;
}

VecDoub TNode::get_Q_values()
{
	return Q_values;
}


void TNode::insert__child(TTT_state v)
{
	TNode* child = new TNode(v, this);
	children.push_back(child);
}

void TNode::set_state(const TTT_state x)
{
	state = x;
}

void TNode::set_input_control(int i_1, int i_2)
{
	input_control = std::make_tuple(i_1, i_2);
}

void TNode::expanded_control(tuple<int, int> u)
{
	U_expanded.insert(u);
}

tuple<int, int> TNode::get_input_control()
{
	return input_control;
}

set< tuple<int, int> > TNode::get_U_expanded()
{
	return U_expanded;
}

bool TNode::is_leaf()
{
	return children.empty();
}

bool TNode::expandable()
{
	return U_expanded.size() != state.get_controlSpace().size();
}

void TNode::inc_N_value(int n, int ind)
{
	N_value[ind] = N_value[ind] + n;
}

void TNode::inc_Q_values(double q, int ind)
{
	Q_values[ind] = Q_values[ind] + q;
}


