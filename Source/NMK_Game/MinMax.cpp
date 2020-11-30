/*
 * DescisionAlgorithms.cpp
 *
 *  Created on: Aug 2, 2020
 *      Author: Alonso Vega
 */

#include "MinMax.h"


//-----------------------------MiniMax Search----------------------------------------------------
tuple<int, int> MinMax::min_max_search(TTT_state x_t)
{
	TTT_state x_tPlus1(x_t);
	set< tuple<int, int> > U_t = x_t.get_controlSpace();
	vector< tuple<int, int> > U_t_vect;
	vector< int > values;
	U_t_vect.reserve(U_t.size());
	values.reserve(U_t.size());

	for (auto u_pt=U_t.begin(); u_pt != U_t.end(); ++u_pt)
	{
		x_tPlus1 = x_t;
		//cout << "\n ("<< std::get<0>(*u_pt) << ", " << std::get<1>(*u_pt) << ") \n";

		U_t_vect.push_back(*u_pt);
		x_tPlus1.transition(std::get<0>(*u_pt), std::get<1>(*u_pt));

		//x_tPlus1.print_board();

		values.push_back(min_value(x_tPlus1));
	}
	vector<int>::iterator max_pt = std::max_element(values.begin(), values.end());
	int max_index = std::distance(values.begin(), max_pt);
	// WHAT IF A SUBSET OF THE VALUES HAVE SAME NUMBER ?

	/*
	cout << "\nvalues = [";
	LOOP(i, 0, values.size()) cout << values[i] << ",";
	cout << "]\n";
	cout << "size of values: " << values.size() << "\n";
	cout << "size of U_t_vect: " << U_t_vect.size() << "\n";
	cout << "size of U_t:" << U_t.size() << "\n";
	//*/

	return U_t_vect[max_index];
}


int MinMax::min_value(TTT_state x_t)
{
	if (x_t.get_termina()) return x_t.utility();

	int v = INF;
	set< tuple<int, int> > U_t = x_t.get_controlSpace();
	TTT_state x_tPlus1(x_t);

	for (auto u_pt=U_t.begin(); u_pt != U_t.end(); ++u_pt)
	{
		x_tPlus1 = x_t;
		x_tPlus1.transition(std::get<0>(*u_pt), std::get<1>(*u_pt));

		v = MIN(v, max_value(x_tPlus1));
	}

	return v;
}


int MinMax::max_value(TTT_state x_t)
{
	if (x_t.get_termina()) return x_t.utility();

	int v = NEG_INF;
	set< tuple<int, int> > U_t = x_t.get_controlSpace();
	TTT_state x_tPlus1(x_t);

	for (auto u_pt=U_t.begin(); u_pt != U_t.end(); ++u_pt)
	{
		x_tPlus1 = x_t;
		x_tPlus1.transition(std::get<0>(*u_pt), std::get<1>(*u_pt));

		v = MAX(v, min_value(x_tPlus1));
	}

	return v;
}


//-----------------------------αβ Search----------------------------------------------------
tuple<int, int> MinMax::αβ_search(TTT_state x_t)
{
	TTT_state x_tPlus1(x_t);
	set< tuple<int, int> > U_t = x_t.get_controlSpace();
	vector< tuple<int, int> > U_t_vect;
	vector< int > values;
	U_t_vect.reserve(U_t.size());
	values.reserve(U_t.size());

	for (auto u_pt=U_t.begin(); u_pt != U_t.end(); ++u_pt)
	{
		x_tPlus1 = x_t;
		//cout << "\n ("<< std::get<0>(*u_pt) << ", " << std::get<1>(*u_pt) << ") \n";

		U_t_vect.push_back(*u_pt);
		x_tPlus1.transition(std::get<0>(*u_pt), std::get<1>(*u_pt));

		//x_tPlus1.print_board();

		values.push_back(max_value(x_tPlus1, NEG_INF, INF));
	}
	vector<int>::iterator max_pt = std::max_element(values.begin(), values.end());
	int max_index = std::distance(values.begin(), max_pt);
	// WHAT IF A SUBSET OF THE VALUES HAVE SAME NUMBER ?

	/*
	cout << "\nvalues = [";
	LOOP(i, 0, values.size()) cout << values[i] << ",";
	cout << "]\n";
	cout << "size of values: " << values.size() << "\n";
	cout << "size of U_t_vect: " << U_t_vect.size() << "\n";
	cout << "size of U_t:" << U_t.size() << "\n";
	//*/

	return U_t_vect[max_index];
}

int MinMax::max_value(TTT_state x_t, int α, int β)
{
	if (x_t.get_termina()) return x_t.utility();

	int v = NEG_INF;
	set< tuple<int, int> > U_t = x_t.get_controlSpace();
	TTT_state x_tPlus1(x_t);

	for (auto u_pt=U_t.begin(); u_pt != U_t.end(); ++u_pt)
	{
		x_tPlus1 = x_t;
		x_tPlus1.transition(std::get<0>(*u_pt), std::get<1>(*u_pt));

		v = MAX(v, min_value(x_tPlus1, α, β));
		if (v >= β) return v;
		α = MAX(α, v);
	}

	return v;
}

int MinMax::min_value(TTT_state x_t, int α, int β)
{
	if (x_t.get_termina()) return x_t.utility();

	int v = INF;
	set< tuple<int, int> > U_t = x_t.get_controlSpace();
	TTT_state x_tPlus1(x_t);

	for (auto u_pt=U_t.begin(); u_pt != U_t.end(); ++u_pt)
	{
		x_tPlus1 = x_t;
		x_tPlus1.transition(std::get<0>(*u_pt), std::get<1>(*u_pt));

		v = MIN(v, max_value(x_tPlus1, α, β));
		if (v <= α) return v;
		β = MIN(β, v);
	}

	return v;
}





