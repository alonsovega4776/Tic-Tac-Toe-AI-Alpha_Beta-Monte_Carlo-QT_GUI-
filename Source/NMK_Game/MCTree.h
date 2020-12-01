/*
 * Tree.h
 *
 *  Created on: Jul 26, 2020
 *      Author: Alonso Vega
 */

#ifndef MCTREE_H_
#define MCTREE_H_

#include "TNode.h"

class MCTree
{
	public:
		static int rel_level;
		const static int print_space;

		MCTree();
		virtual ~MCTree();

		void add_node(TTT_state state, TNode* parent);
		TNode* expand(TNode* v_t, int i_1, int i_2);
		TNode* expand(TNode* v_t);

		TNode* get_root();
		bool is_empty();
		int size(TNode* v);
		void breath_first_print(vector< TNode* > abuelos);

		tuple<int, int> UCT_search(TTT_state x_t, double c_p, int N_root);
		TNode* tree_policy(TNode* v, double c_p);
		int best_child_ind(TNode* v, double c);
        int best_child_ind_N(TNode* v);
		int default_policy(TTT_state x);
		void back_up(TNode* v, int Δ);
		double util_hat(TNode* v_t, TNode* v_tPlus1, double c);

        template<typename T>
        static int duplicate_rand_index(vector<T> v, T value);


	private:
		TNode* root;
        set< tuple<int, int> > U_0;

};

#endif /* MCTREE_H_ */
