/*
 * DescisionAlgorithms.h
 *
 *  Created on: Aug 2, 2020
 *      Author: Alonso Vega
 */

#ifndef MINMAX_H_
#define MINMAX_H_

#include "TTT_state.h"

class MinMax
{
	public:
		static tuple<int, int> min_max_search(TTT_state x_t);
		static tuple<int, int> αβ_search(TTT_state x_t);
		static int min_value(TTT_state x_t);
		static int min_value(TTT_state x_t,int α, int β);
		static int max_value(TTT_state x_t);
		static int max_value(TTT_state x_t,int α, int β);
};

#endif /* MINMAX_H_ */
