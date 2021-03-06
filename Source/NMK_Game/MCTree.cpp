/*
 * Tree.cpp
 *
 *  Created on: Jul 26, 2020
 *      Author: Alonso Vega
 */

#include "MCTree.h"

#define EXPAND_DIST 'N'
#define SIGMA        0.85

int MCTree::rel_level = 0;
const int MCTree::print_space = 20;

MCTree::MCTree()
{
	root = NULL;

    int j_1, j_2;
    LOOP(j_1, 0, TTT_SIZE)
        LOOP(j_2, 0, TTT_SIZE) U_0.insert(std::make_tuple(j_1, j_2));
}

MCTree::~MCTree()
{
	delete root;
}

void MCTree::add_node(TTT_state state, TNode* parent)
{
	if (parent == NULL)
	{
		if (root == NULL)
		{
			root = new TNode(state, NULL);
			root->set_input_control(INF, INF);
		}
		else thrrow("ERROR IN ADDING NODE");
	}
	else parent->insert__child(state);
}

TNode* MCTree::get_root()
{
	return root;
}

bool MCTree::is_empty()
{
	return root == NULL;
}

int MCTree::size(TNode* v)
{
	if (v == NULL) return 0;

    int i;
	int count = 1;
	vector< TNode* > offspring = v->get_children();

	LOOP(i, 0, offspring.size())
		count = count + size(offspring.at(i));

	return count;
}


void MCTree::breath_first_print(vector< TNode* > abuelos)
{
	if (abuelos.empty()) cout << "TREE IS EMPTY";
	else
	{
		vector< TNode* > hijos;

		cout << string(rel_level*print_space,' ') << "Relative Level: " << rel_level << string(rel_level*print_space,' ') << "\n";

		int i, k;
		LOOP(i, 0, abuelos.size())
		{
			cout << string(rel_level*print_space + 2,' ') << "N = [ ";
			LOOP(k, 0, abuelos.at(i)->get_N_values().size()) cout << abuelos.at(i)->get_N_values()[k] << "  " ;
			cout << "] \n";
			cout << string(rel_level*print_space + 2,' ') << "u_t-1 = (" << std::get<0>(abuelos.at(i)->get_input_control()) << "," << std::get<1>(abuelos.at(i)->get_input_control()) << ") \n";
			cout << string(rel_level*print_space + 2,' ') << "Q = [ ";
			LOOP(k, 0, abuelos.at(i)->get_Q_values().size()) cout << abuelos.at(i)->get_Q_values()[k] << "  ";
			cout << "] \n \n";

			abuelos.at(i)->get_state().print_board(print_space*rel_level + 4);

			if (!abuelos.at(i)->is_leaf())
			{
				vector< TNode* > unos_nietos = abuelos.at(i)->get_children();
				hijos.insert(hijos.end(), unos_nietos.begin(), unos_nietos.end());
			}
		}
		if (!hijos.empty())
		{
			rel_level++;
			breath_first_print(hijos);
		}
	}

}


//--------------------------------------------------UCT--------------------------------------------------------

// UCT SEARCH
tuple<int, int> MCTree::UCT_search(TTT_state x_t, double c_p, int N_root)
{
	if (x_t.get_termina()) thrrow("ERROR UCT: STATE IS TERMINAL");
	if (root != NULL) thrrow("ERROR UCT: TREE IS NOT EMPTY");

	add_node(x_t, NULL);								// add root to search tree

	TNode* v_l;											// children nodes
	double Δ;											// terminal state performance metric

	while (root->get_N_values()[0] < N_root)				// search is conditioned on number of simulations
	{
		v_l = tree_policy(root, c_p);					// Selection and Exploration
		Δ = default_policy(v_l->get_state());			// Simulation
		back_up(v_l, Δ);								// Update statistics
	}

//    if (best_child_ind(root, 0) == best_child_ind_N(root)) 						// finished only if node with max score is also most explored node
		return root->get_children()[best_child_ind(root, 0)]->get_input_control();
    /*
	else
	{
		delete root;
		root = NULL;

		return UCT_search(x_t, c_p + 0.85*(1.0-c_p), N_root + (N_root/2));			// redo search with diff. paramters
	}
//	*/
}


// TREE POLICY
TNode* MCTree::tree_policy(TNode* v, double c_p)
{
	while (!v->get_state().get_termina())
	{
		if (v->expandable())	return expand(v);
		else					v = v->get_children()[best_child_ind(v, c_p)];
	}
	return v;
}


// EXPANSION																		// deterministic input control
TNode* MCTree::expand(TNode* v_t, int i_1, int i_2)
{
    if (v_t == NULL) thrrow("EXPANSION: NODE DOES NOT EXIST");

	TTT_state x_tPlus1(v_t->get_state());											// create new state
	if (!x_tPlus1.in_contorlSpace(i_1, i_2)) thrrow("NOT A PROPER CONTROL INPUT");

	x_tPlus1.transition(i_1, i_2);													// x_t+1 = x_t + u_t    u_t ε U_t

	add_node(x_tPlus1, v_t);														// add node v_t+1   s.t. v_t is its parent

	v_t->get_children().back()->set_input_control(i_1, i_2);						// u_t data is held by v_t+1
	v_t->expanded_control(std::make_tuple(i_1, i_2));								// maintain set of expanded actions for each parent

	return v_t->get_children().back();
}

TNode* MCTree::expand(TNode* v_t)														// random input control
{
	if (!v_t->expandable()) thrrow("ERROR: NO LONGER EXPANDABLE");

	set< tuple<int, int> > U_unexpanded;
	set< tuple<int, int> > U_t = v_t->get_state().get_controlSpace();

    std::set_difference(U_t.begin(), U_t.end(),
                        v_t->get_U_expanded().begin(), v_t->get_U_expanded().end(),
                        std::inserter(U_unexpanded, U_unexpanded.begin()));

	std::random_device rand_dev;
    std::mt19937 gen_U(rand_dev());
	std::uniform_int_distribution<> unif(0, U_unexpanded.size()-1);					// select control from uniform dist. in control space
    set< tuple<int, int> >::iterator rand_control_iter;

    if ((EXPAND_DIST == 'N') && (U_unexpanded.size() > 5))
    {
        // _____________________________ gettting AI's current plays _________________________
        //set< tuple<int, int> > U_0;
        //U_0 = get_root()->get_state().get_controlSpace();
        /*_____________ for debugging purposes:
        int j_1, j_2;
        LOOP(j_1, 0, TTT_SIZE)
            LOOP(j_2, 0, TTT_SIZE)
                U_0.insert(std::make_tuple(j_1, j_2));
        // _____________ for debugging purposes*/
        //U_t = root->get_state().get_controlSpace();                                     // we only need the inital root state space
        set< tuple<int, int> > U_played;
        std::set_difference(U_0.begin(), U_0.end(),
                            U_t.begin(), U_t.end(),
                            std::inserter(U_played, U_played.begin()));                 // U_played = {u_t in U_0 \ U_t}

        /*_____________ for debugging purposes:
        set< tuple<int, int> >::iterator set_iter2;
         for (set_iter2 = U_played.begin(); set_iter2 != U_played.end(); set_iter2++)
             cout << std::get<0>(*set_iter2) << ", " << std::get<1>(*set_iter2) << "\n";
        // _____________ for debugging purposes*/

        if ((U_played.size() != 0))
        {
            //set< tuple<int, int> > U_t_AI;                                                  // U_t_AI = {u in U_played: p(u) = -1 }, -1 beging the AI player
            //set< tuple<int, int> >::iterator set_iter;
            tuple<int, int> u_t;
            //MatInt x_t = v_t->get_state().get_state();
            int i_1, i_2;
            /*for (set_iter = U_played.begin(); set_iter != U_played.end(); set_iter++)
            {
                if (U_t_AI.size() >= std::ceil(((double)U_played.size())/2)) break;
                u_t = *set_iter;
                i_1 = std::get<0>(u_t);
                i_2 = std::get<1>(u_t);
                if (x_t[i_1][i_2] != 0) U_t_AI.insert(u_t);
            }*/
            // _____________________________ gettting AI's current plays _________________________
            std::uniform_int_distribution<> unif_defend(0, U_played.size()-1);		    // select control from uniform dist. in control space

            rand_control_iter = U_played.begin();
            std::advance(rand_control_iter, unif_defend(gen_U));

            cout << "Normalizing about: ( " << std::get<0>(*rand_control_iter) << ", " << std::get<1>(*rand_control_iter) << ")";

            const double i_1_N = std::get<0>(*rand_control_iter);                     // sample mean from AI's plays
            const double i_2_N = std::get<1>(*rand_control_iter);

            std::random_device rd_1{};
            std::mt19937 gen_N_1{rd_1()};
            std::normal_distribution<> n_sample_1{i_1_N, SIGMA};

            std::random_device rd_2{};
            std::mt19937 gen_N_2{rd_2()};
            std::normal_distribution<> n_sample_2{i_2_N, SIGMA};

            int normal_count = 0;
            bool NOTvalid_control;
            do
            {
                i_1 = std::round(n_sample_1(gen_N_1));                                  // sample a expanding action
                i_2 = std::round(n_sample_2(gen_N_2));
                cout << " \n  sample Normal contorl: ( " << i_1 << ", " << i_2 << ") \n";

                u_t = std::make_tuple(i_1, i_2);

                normal_count++;
                NOTvalid_control = ((i_1 < 0) || (i_1 >= TTT_SIZE) || (i_2 < 0) || (i_2 >= TTT_SIZE) || (U_unexpanded.count(u_t) == 0));
            }while ((normal_count < 25) && (NOTvalid_control));

            if (!NOTvalid_control)
            {
                cout << "   Normal contorl: ( " << i_1 << ", " << i_2 << ") \n";
                cout << "sample count: " << normal_count << "\n";
                return expand(v_t, i_1, i_2);
            }
        }
    }

    rand_control_iter = U_unexpanded.begin();
    std::advance(rand_control_iter, unif(gen_U));

    cout << "Uniform contorl: ( " << std::get<0>(*rand_control_iter) << ", " << std::get<1>(*rand_control_iter) << ") \n";

    const int i_1 = std::get<0>(*rand_control_iter);
    const int i_2 = std::get<1>(*rand_control_iter);
    return expand(v_t, i_1, i_2);
}


//ESTIMATED UTILITY
double MCTree::util_hat(TNode* v_t, TNode* v_tPlus1, double c)
{
	int N_t = v_t->get_N_values()[0];
	//int Q_t = v_t->get_Q_values()[0];

	int N_tPlus1 = v_tPlus1->get_N_values()[0];
	int Q_tPlus1 = v_tPlus1->get_Q_values()[0];

	double exploitation = (double)Q_tPlus1 / N_tPlus1;								// Explotation
	double exploration = sqrt((2*std::log1p(N_t))/(N_tPlus1));						// Exploration
	double util_hat = exploitation + c*exploration;

	/*
	cout << "\nc = " << c << "\n";
	cout << "N_t = " << N_t << "\n";
	cout << "< Q_t+1/N_t+1 = "<< Q_tPlus1 << "/" << N_tPlus1 << "= "<< exploitation << ", "<< exploration << "> \n";
	cout << "est. utility = " << util_hat << "\n";
	*/

	return util_hat;
}


//BEST CHILD: Using Score Metric
int MCTree::best_child_ind(TNode* v, double c)
{
	vector< TNode* > hijos = v->get_children();										// get all children of v
	if (hijos.empty()) thrrow("ERROR: NO BEST CHILD");

	vector< double > util_hat_vect;
	util_hat_vect.reserve(hijos.size());

	int i;
	LOOP(i, 0, hijos.size()) util_hat_vect.push_back(util_hat(v, hijos.at(i), c));	// calc. each children's score metric

	std::vector<double>::iterator max_index_iter
			= std::max_element(util_hat_vect.begin(), util_hat_vect.end());			// get max score

	int max_index = std::distance(util_hat_vect.begin(), max_index_iter);
    max_index = duplicate_rand_index(util_hat_vect, util_hat_vect[max_index]);

    /*
    cout << "\n util hat vector = ";
    LOOP(i, 0, util_hat_vect.size()) cout << "  " << util_hat_vect[i];
    cout << "\n max index = " << max_index;
    */

	return max_index;																// return index of best child
}


//BEST CHILD: Using Most Explored Node
int MCTree::best_child_ind_N(TNode* v)
{
	vector< TNode* > hijos = v->get_children();
	if (hijos.empty()) thrrow("ERROR: NO BEST CHILD");

	vector< int > N_vect;
	N_vect.reserve(hijos.size());

	int i;
	LOOP(i, 0, hijos.size()) N_vect.push_back(hijos.at(i)->get_N_values()[0]);

	std::vector<int>::iterator max_index_iter = std::max_element(N_vect.begin(), N_vect.end());

	int max_index = std::distance(N_vect.begin(), max_index_iter);
    max_index = duplicate_rand_index(N_vect, N_vect[max_index]);

	return max_index;
}


template<typename T>
int MCTree::duplicate_rand_index(vector<T> v, T value)
{
    vector<int> duplicate_indexes;
    duplicate_indexes.reserve(v.size());

    typename vector<T>::iterator ptr = v.begin();
    typename vector<T>::iterator ind_ptr;

    unsigned long int ind = 0;

    while (ind < v.size())
    {
        ind_ptr = std::find(ptr, v.end(), value);
        ind = std::distance(v.begin(), ind_ptr);

       //cout << "\n \n current index pointer:" << *ind_ptr;
        //cout << "\n current index pushed: " << ind;

        if (ind >= v.size()) break;
        duplicate_indexes.push_back(ind);

        ptr = ind_ptr;
        std::advance(ptr, 1);
    }

    /*
    int i;
    cout << "\n number of duplicates: " << duplicate_indexes.size();
    cout << "\n duplicates at : ";
    LOOP(i, 0, duplicate_indexes.size()) cout << ", " << duplicate_indexes.at(i);
    //*/


    std::random_device rand_dev;
    std::mt19937 gen(rand_dev());
    std::uniform_int_distribution<> unif(0, duplicate_indexes.size()-1);

    return duplicate_indexes[unif(gen)];
}


//DEFAULT POLICY
int MCTree::default_policy(TTT_state x)
{
    while (!x.get_termina()) x.transition();

	return x.utility();
}


void MCTree::back_up(TNode* v, int Δ)
{
	int ind;
	if (Δ > 0) ind = 0;
	else ind = 1;

	while (v != NULL)
	{
		v->inc_N_value(1, 0);
		v->inc_Q_values(Δ, 0);
		//Δ = -Δ;
		v = v->get_parent();
	}
}
//--------------------------------------------------UCT--------------------------------------------------------



