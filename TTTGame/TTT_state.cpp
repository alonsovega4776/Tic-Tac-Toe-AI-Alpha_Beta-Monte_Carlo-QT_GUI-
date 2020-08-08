/*
 * TTTmat.cpp
 *
 *  Created on: Jul 10, 2020
 *      Author: Alonso Vega
 */

#include "TTT_state.h"

vector< std::tuple<int, int> > TTT_state::w_1;
vector< std::tuple<int, int> > TTT_state::w_2;

vector<int> TTT_state::Z_TTT_SIZE_vec;

const std::map<int, int> TTT_state::toPrint =
{
	{-1, 79},
	{0, 32},
	{1, 88}
};

Graph TTT_state::winningPaths(TTT_SIZE*TTT_SIZE);

void TTT_state::gen_const()
{
	int i;
	LOOP(i, 0, TTT_SIZE) Z_TTT_SIZE_vec.push_back(i);

	vector< std::tuple<int, int> > v_1_1, v_1_2, v_1_3, v_1_4, v_1_5, v_1_6;
	vector< std::tuple<int, int> >        v_2_2,        v_2_4, v_2_5, v_2_6;

	v_1_1.reserve(TTT_SIZE);
	v_1_2.reserve(TTT_SIZE);
	v_1_3.reserve(TTT_SIZE);
	v_1_4.reserve(TTT_SIZE);
	v_1_5.reserve(TTT_SIZE-1);
	v_1_6.reserve(TTT_SIZE-1);

	v_2_2.reserve(TTT_SIZE);
	v_2_4.reserve(TTT_SIZE);
	v_2_5.reserve(TTT_SIZE-1);
	v_2_6.reserve(TTT_SIZE-1);

	LOOP(i, 0, TTT_SIZE)
	{
		v_1_1.push_back(std::make_tuple(i,0));
		v_1_2.push_back(std::make_tuple(i,1));
		v_1_3.push_back(std::make_tuple(0,i));
		v_1_4.push_back(std::make_tuple(1,i));

		v_2_2.push_back(std::make_tuple(i,2));
		v_2_4.push_back(std::make_tuple(2,i));
	}

	LOOP(i, 0, TTT_SIZE-1)
	{
		v_1_5.push_back(std::make_tuple(i,i));
		v_1_6.push_back(std::make_tuple(i,TTT_SIZE-1-i));

		v_2_5.push_back(std::make_tuple(i+1,i+1));
		v_2_6.push_back(std::make_tuple(i+1,TTT_SIZE-1-i-1));
	}

	//vector< std::tuple<int, int> > w_1, w_2;
	w_1.reserve(4*TTT_SIZE + 2*(TTT_SIZE-1));
	w_2.reserve(4*TTT_SIZE + 2*(TTT_SIZE-1));

	LOOP(i, 0, TTT_SIZE)
	{
		w_1.push_back(v_1_1[i]);
		w_1.push_back(v_1_2[i]);

		w_2.push_back(v_1_2[i]);
		w_2.push_back(v_2_2[i]);
	}
	LOOP(i, 0, TTT_SIZE)
	{
		w_1.push_back(v_1_3[i]);
		w_1.push_back(v_1_4[i]);

		w_2.push_back(v_1_4[i]);
		w_2.push_back(v_2_4[i]);
	}

	LOOP(i, 0, TTT_SIZE-1)
	{
		w_1.push_back(v_1_5[i]);
		w_1.push_back(v_1_6[i]);

	}
	LOOP(i, 0, TTT_SIZE-1)
	{
		w_2.push_back(v_2_5[i]);
		w_2.push_back(v_2_6[i]);

	}

	v_1_1.clear();
	v_1_2.clear();
	v_1_3.clear();
	v_1_4.clear();
	v_1_5.clear();
	v_1_6.clear();

	v_2_2.clear();
	v_2_4.clear();
	v_2_5.clear();
	v_2_6.clear();

	/*
	LOOP(i, 0, 4*TTT_SIZE + 2*(TTT_SIZE-1))
	{
		cout << "\n (" << std::get<0>(w_1[i]) << "," << std::get<1>(w_1[i]) << ") ";
		cout << "---> (" << std::get<0>(w_2[i]) << "," << std::get<1>(w_2[i]) << ") \n";
	}
	cout << "DONE \n";
	//*/

	winningPaths.add_nodes(w_1, w_2);
}

TTT_state::TTT_state()										// Constructor // @suppress("Class members should be properly initialized")
{
	if (Z_TTT_SIZE_vec.empty()) gen_const();

	state.assign(TTT_SIZE,TTT_SIZE,0);
	time = 0;
	termina = false;
	outcome = "NONE";

	std::random_device rand_dev;
	std::mt19937 gen(rand_dev());
	std::uniform_int_distribution<> unif(0,1);

	if (unif(gen) == 0) player = 'O';
	else player = 'X';
	//cout << "\n" << "FIRST PLAYER: " << player << " \n";

	int i_1, i_2;
	LOOP(i_1, 0, TTT_SIZE)
		LOOP(i_2, 0, TTT_SIZE)
			controlSpace.insert(std::make_tuple(i_1,i_2));
}

TTT_state::TTT_state(char first_play)										// Constructor // @suppress("Class members should be properly initialized")
{
	if (Z_TTT_SIZE_vec.empty()) gen_const();

	state.assign(TTT_SIZE,TTT_SIZE,0);
	time = 0;
	termina = false;
	player = first_play;
	outcome = "NONE";

	if (player != 'X' && player != 'O') thrrow("NOT A PLAYER");

	int i_1, i_2;
	LOOP(i_1, 0, TTT_SIZE)
		LOOP(i_2, 0, TTT_SIZE)
			controlSpace.insert(std::make_tuple(i_1,i_2));
}

TTT_state::TTT_state(const TTT_state& y) // @suppress("Class members should be properly initialized")
{
	if (Z_TTT_SIZE_vec.empty()) gen_const();

	state = y.state;
	time = y.time;
	controlSpace = y.controlSpace;
	termina = y.termina;
	player = y.player;
	outcome = y.outcome;
}




TTT_state::~TTT_state()										// Destructor
{
	//new operator was not used
}

MatInt TTT_state::get_state()         						// Get state
{
	return state;
}

set< std::tuple<int, int> > TTT_state::get_controlSpace()
{
	return controlSpace;
}

int TTT_state::get_time()									// Get time
{
	return time;
}

bool TTT_state::get_termina()									// Get time
{
	return termina;
}

char TTT_state::get_player()
{
	return player;
}

string TTT_state::get_outcome()
{
	return outcome;
}

void TTT_state::transition(int i, int j)		// Transition function
{
	int player_num = 0;

	if (player == 'X') player_num = 1;
	else if (player == 'O') player_num = -1;
	else thrrow("NOT A PLAYER");

	if (controlSpace.count(std::make_tuple(i,j)) == 0)
		thrrow("NOT A VALID ACTION");

	state[i][j] = player_num;
	controlSpace.erase(std::make_tuple(i,j));
	time++;
    termina = terminal(i, j, player_num, TTT_WIN_SIZE);	// updating terminal status

	switch (player)
	{
		case 'X': player = 'O'; break;
		case 'O': player = 'X'; break;
	}
}

void TTT_state::transition()
{
	std::random_device rand_dev;
	std::mt19937 gen(rand_dev());
	std::uniform_int_distribution<> unif(0, controlSpace.size()-1);

	set< tuple<int, int> >::iterator rand_control_iter = controlSpace.begin();
	std::advance(rand_control_iter, unif(gen));

	if (termina) thrrow("ERROR: TERMINAL STATE REACHED");
	transition(std::get<0>(*rand_control_iter), std::get<1>(*rand_control_iter));
}


void TTT_state::print_state()								// Print State
{
	int i_1, i_2;
	LOOP(i_1, 0, state.nrows())
		LOOP(i_2, 0, state.ncols())
		{
			printf("%d  ", state[i_1][i_2]);
			if (i_2 == state.ncols()-1)printf("\n");
		}
}

void TTT_state::print_board()
{
	print_board(0);
}

void TTT_state::print_board(int n_space)
{
	string space;
	space.insert(0, n_space, ' ');

	string seperator;
	seperator.insert(0, TTT_SIZE*4-3, '-');

	char player_char[TTT_SIZE][TTT_SIZE];

	int i_1, i_2;
	LOOP(i_1, 0, TTT_SIZE)
		LOOP(i_2, 0, TTT_SIZE)
			player_char[i_1][i_2] = toPrint.at(state[i_1][i_2]);

	LOOP(i_1, 0, TTT_SIZE)
	{
		cout << space;
		LOOP(i_2, 0, TTT_SIZE)
		{
			cout << player_char[i_1][i_2];
			if (i_2 != TTT_SIZE-1) cout << " | ";
		}
		cout << "\n";
		if (i_1 != TTT_SIZE-1) cout << space << seperator;
		cout << "\n";
	}
}

void TTT_state::print_controlSpace()
{
		set< std::tuple<int, int> > U_space = get_controlSpace();

		auto print = [](std::tuple<int, int> x)
				{
					std::cout << "(" << std::get<0>(x) << "," << std::get<1>(x) << "), ";
				};

		printf("{ ");
		std::for_each(U_space.cbegin(), U_space.cend(), print);
		printf("} \n");
}


bool TTT_state::in_contorlSpace(int i_1, int i_2)
{

	return controlSpace.count(std::make_tuple(i_1,i_2)) == 1;
}


bool TTT_state::terminal(int r, int c, int player_num)      // terminal funnction for N x N x N games (other terminal function is better)
{

	bool diag = (r == c);
	bool anti_diag = (r + c == TTT_SIZE - 1);
	int sum;
	int win = TTT_SIZE*player_num;
	int k;

	sum = 0;
	LOOP(k, 0, TTT_SIZE)
	{
		if (state[r][k] != player_num) break;
		sum = sum + state[r][k];
	}
	if (sum == win)
	{
		if (player_num == 1) outcome = "X wins";
		else if (player_num == -1) outcome = "O wins";
		else thrrow("ERROR IN DETERMINITING OUTCOME");
		return true;
	}

	sum = 0;
	LOOP(k, 0, TTT_SIZE)
	{
		if (state[k][c] != player_num) break;
		sum = sum + state[k][c];
	}
	if (sum == win)
	{
		if (player_num == 1) outcome = "X wins";
		else if (player_num == -1) outcome = "O wins";
		else thrrow("ERROR IN DETERMINITING OUTCOME");
		return true;
	}

	if (diag)
	{
		sum = 0;
		LOOP(k, 0, TTT_SIZE)
		{
			if (state[k][k] != player_num) break;
			sum = sum + state[k][k];
		}
		if (sum == win)
		{
			if (player_num == 1) outcome = "X wins";
			else if (player_num == -1) outcome = "O wins";
			else thrrow("ERROR IN DETERMINITING OUTCOME");
			return true;
		}
	}

	if (anti_diag)
	{
		sum = 0;
		LOOP(k, 0, TTT_SIZE)
		{
			if (state[k][TTT_SIZE-1-k] != player_num) break;
			sum = sum + state[k][TTT_SIZE-1-k];
		}
		if (sum == win)
		{
			if (player_num == 1) outcome = "X wins";
			else if (player_num == -1) outcome = "O wins";
			else thrrow("ERROR IN DETERMINITING OUTCOME");
			return true;
		}
	}

	if (controlSpace.empty()) return true;

	return false;
}

int TTT_state::utility()
{
	if (!termina) thrrow("NON-TERMINAL STATE. NO UTILITY.")

	if (outcome.compare("X wins") == 0) return 1;
	else if (outcome.compare("O wins") == 0) return -1;
	else return 0;
}


MatInt TTT_state::chess_board(int i_c, int j_c, int K)
{
	//if (K%2 == 0) thrrow("K MUST BE ODD.");
	//if (K > TTT_SIZE) thrrow("K IS TOO LARGE");

	int d = K/2;

	MatInt y;
	y.assign(K,K,0);
	y[d][d] = state[i_c][j_c];

	bool row_UB, col_UB, row_LB, col_LB;

	int k;
	LOOP(k, 1, d+1)
	{
		row_LB = (i_c - k >= 0);
		row_UB = (i_c + k < TTT_SIZE);
		
		col_LB = (j_c - k >= 0); 
		col_UB = (j_c + k < TTT_SIZE);

		if (row_LB) y[d - k][d] = state[i_c - k][j_c];		       		     // N
		if (row_UB) y[d + k][d] = state[i_c + k][j_c]; 						 // S

		if (col_UB) y[d][d + k] = state[i_c][j_c + k]; 						 // E
		if (col_LB) y[d][d - k] = state[i_c][j_c - k]; 			    		 // W


		if (row_LB && col_LB) y[d - k][d - k] = state[i_c - k][j_c - k];    // NW
		if (row_LB && col_UB) y[d - k][d + k] = state[i_c - k][j_c + k];    // NE

		if (row_UB && col_LB) y[d + k][d - k] = state[i_c + k][j_c - k];    // SW
		if (row_UB && col_UB) y[d + k][d + k] = state[i_c + k][j_c + k];    // SE
	}

//	/*
	int i_1, i_2;
	LOOP(i_1, 0, y.nrows())
		LOOP(i_2, 0, y.ncols())
		{
			printf("%d  ", y[i_1][i_2]);
			if (i_2 == y.ncols()-1)printf("\n");
		}
//	*/

	return y;
}


bool TTT_state::terminal(int i_c, int j_c, int player_num, int K)
{
	if (K > TTT_SIZE) thrrow("K IS TOO LARGE");

	int k, sum, x_nieghbor;

//____________________________Vertical__________________________________
	sum = 0;
	k = 1;
	x_nieghbor = state[i_c][j_c];
	while ((i_c - k >= 0) && (x_nieghbor == player_num))								// N
	{
		//cout << "\n sum N: " << sum << "\n x_neighbor: " << x_nieghbor << "\n k =" << k;

		x_nieghbor = state[i_c - k][j_c];

		if (x_nieghbor == player_num) sum = sum + x_nieghbor;
		if (sum + state[i_c][j_c] == K*player_num)
		{
			if (player_num == 1) outcome = "X wins";
			else if (player_num == -1) outcome = "O wins";
			else thrrow("ERROR IN DETERMINITING OUTCOME");

			return true;
		}

		k++;
	}
	//cout << "\n";
	k = 1;
	x_nieghbor = state[i_c][j_c];
	while ((i_c + k < TTT_SIZE) && (x_nieghbor == player_num))							// S
	{
		//cout << "\n sum S: " << sum<< "\n x_neighbor: " << x_nieghbor<< "\n k =" << k;

		x_nieghbor = state[i_c + k][j_c];

		if (x_nieghbor == player_num) sum = sum + x_nieghbor;
		if (sum + state[i_c][j_c] == K*player_num)
		{
			if (player_num == 1) outcome = "X wins";
			else if (player_num == -1) outcome = "O wins";
			else thrrow("ERROR IN DETERMINITING OUTCOME");

			return true;
		}

		k++;
	}

//_______________________________Horizontal_____________________________
	sum = 0;
	k = 1;
	x_nieghbor = state[i_c][j_c];
	while ((j_c + k < TTT_SIZE) && (x_nieghbor == player_num))						  // E
	{
		//cout << "\n sum E: " << sum << "\n x_neighbor: " << x_nieghbor << "\n k =" << k;

		x_nieghbor = state[i_c][j_c + k];

		if (x_nieghbor == player_num) sum = sum + x_nieghbor;
		if (sum + state[i_c][j_c] == K*player_num)
		{
			if (player_num == 1) outcome = "X wins";
			else if (player_num == -1) outcome = "O wins";
			else thrrow("ERROR IN DETERMINITING OUTCOME");

			return true;
		}

		k++;
	}
	//cout << "\n";
	k = 1;
	x_nieghbor = state[i_c][j_c];
	while ((j_c - k >= 0) && (x_nieghbor == player_num))							// W
	{
		//cout << "\n sum W: " << sum << "\n x_neighbor: " << x_nieghbor << "\n k =" << k;

		x_nieghbor = state[i_c][j_c - k];

		if (x_nieghbor == player_num) sum = sum + x_nieghbor;
		if (sum + state[i_c][j_c] == K*player_num)
		{
			if (player_num == 1) outcome = "X wins";
			else if (player_num == -1) outcome = "O wins";
			else thrrow("ERROR IN DETERMINITING OUTCOME");

			return true;
		}

		k++;
	}

//____________________________diag+______________________________________
	sum = 0;
	k = 1;
	x_nieghbor = state[i_c][j_c];
	while ((i_c - k >= 0) && (j_c - k >= 0) && (x_nieghbor == player_num))				// NW
	{
		//cout << "\n sum NW:" << sum << "\n x_neighbor: " << x_nieghbor << "\n k =" << k;

		x_nieghbor = state[i_c - k][j_c - k];

		if (x_nieghbor == player_num) sum = sum + x_nieghbor;
		if (sum + state[i_c][j_c] == K*player_num)
		{
			if (player_num == 1) outcome = "X wins";
			else if (player_num == -1) outcome = "O wins";
			else thrrow("ERROR IN DETERMINITING OUTCOME");

			return true;
		}

		k++;
	}
	//cout << "\n";
	k = 1;
	x_nieghbor = state[i_c][j_c];
	while ((i_c + k < TTT_SIZE) && (j_c + k < TTT_SIZE) && (x_nieghbor == player_num))	// SE
	{
		//cout << "\n sum SE: " << sum << "\n x_neighbor: " << x_nieghbor << "\n k =" << k;

		x_nieghbor = state[i_c + k][j_c + k];

		if (x_nieghbor == player_num) sum = sum + x_nieghbor;
		if (sum + state[i_c][j_c] == K*player_num)
		{
			if (player_num == 1) outcome = "X wins";
			else if (player_num == -1) outcome = "O wins";
			else thrrow("ERROR IN DETERMINITING OUTCOME");

			return true;
		}

		k++;
	}

//____________________________diag-______________________________________
	sum = 0;
	k = 1;
	x_nieghbor = state[i_c][j_c];
	while ((i_c - k >= 0) &&  (j_c + k < TTT_SIZE) && (x_nieghbor == player_num))		// NE
	{
		//cout << "\n sum NE: " << sum << "\n x_neighbor: " << x_nieghbor << "\n k =" << k;

		x_nieghbor = state[i_c - k][j_c + k];

		if (x_nieghbor == player_num) sum = sum + x_nieghbor;
		if (sum + state[i_c][j_c] == K*player_num)
		{
			if (player_num == 1) outcome = "X wins";
			else if (player_num == -1) outcome = "O wins";
			else thrrow("ERROR IN DETERMINITING OUTCOME");

			return true;
		}

		k++;
	}
	//cout << "\n";
	k = 1;
	x_nieghbor = state[i_c][j_c];
	while ((i_c + k < TTT_SIZE) && (j_c - k >= 0) && (x_nieghbor == player_num))		// SW
	{
		//cout << "\n sum SW: " << sum << "\n x_neighbor: " << x_nieghbor << "\n k =" << k;

		x_nieghbor = state[i_c + k][j_c - k];

		if (x_nieghbor == player_num) sum = sum + x_nieghbor;
		if (sum + state[i_c][j_c] == K*player_num)
		{
			if (player_num == 1) outcome = "X wins";
			else if (player_num == -1) outcome = "O wins";
			else thrrow("ERROR IN DETERMINITING OUTCOME");

			return true;
		}

		k++;
	}
//_______________________________________________________________________
	//cout << "\n _____no win_______\n \n";

	if (controlSpace.empty())
	{
		outcome = "tie";
		return true;
	}

	return false;
}




Graph TTT_state::state_graphs()
{

}


double TTT_state::evaluation()
{

}




