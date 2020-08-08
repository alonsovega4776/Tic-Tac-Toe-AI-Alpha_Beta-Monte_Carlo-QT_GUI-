/*
 * main.cpp
 *
 *  Created on: June 22, 2020
 *      Author: Alonso Vega
 */

#include "MainWindow.h"
#include "MCTree.h"
#include "MinMax.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();








/* --------------------------------------------------------------------------------------- TEST: uniform choice on duplicate choices
    vector<int> v = {0,1,4,1,1,11,0,1111,0,1, 1, 5};
    //               0 1 2 3 4 5  6   7  8 9 10 11

   int i;
   LOOP(i, 0, 10) cout << "\n" << MCTree::duplicate_rand_index<int>(v,0) << "\n";
// ----------------------------------------------------------------------------------------- */





/* ------------------------------------------------------------------------- TEST: stack for constructing graph
    std::set<int> Z_3 = {0,1,2,3,4,5,6,7,8};

    FOR_EACH(Z_3.begin(), Z_3.end(), int x)
        cout << x;
    END_FOR_EACH
    cout << "\n";

    std::set<int>::iterator i;

    std::stack<int> my_stack;

    std::set<int>::iterator set_iter;
    for(set_iter = Z_3.begin(); set_iter != Z_3.end(); set_iter++)
        my_stack.push(*set_iter);

    while(!my_stack.empty())
    {
        cout <<  "\n" << my_stack.top();
        my_stack.pop();
    }
// ------------------------------------------------------------------------- */



/* ------------------------------------------------------------------------------------------------------------- TEST: monte carlo search on arbritary state x
    TTT_state x('X');
    int i;
    LOOP(i,0,4) x.transition();
    x.print_board();


    MCTree mytree;
    tuple<int, int> optimal;

    optimal = mytree.UCT_search(x, 1.0/sqrt(2.0), 5); //satisfies Hoeffding Ineqality: 1.0/sqrt(2.0)
    cout << " \n \n \n optimal move : (" << std::get<0>(optimal) << "," << std::get<1>(optimal) << ") \n";

    vector<TNode*> root;
    root.push_back(mytree.get_root());
    mytree.breath_first_print(root);



    // some node in the MC tree
    //TNode* node = mytree.get_root()->get_children()[0]->get_children()[0]->get_children()[2];
    //node->get_parent()->get_state().print_board();
// --------------------------------------------------------------------------------------------------------------*/







/* ------------------------------------------------------------------------- TEST: playing against AI: MCtree seach
    TTT_state x('O');
    TTT_state y;

    tuple<int, int> optimal;



    int i_1, i_2;
    while(!x.get_termina())
    {
        x.print_board();

        cout << "\n enter \n";
        cin >> i_1;
        cin >> i_2;
        cout << "\n ok, got it. \n";

        x.transition(i_1, i_2);

        y = x;

        MCTree* mytree_pt = new MCTree();
        optimal = mytree_pt->UCT_search(y, 0.5, 5000);
        delete mytree_pt;

        x.transition(std::get<0>(optimal), std::get<1>(optimal));
        x.print_board();
    }



    //vector<TNode*> root;
    //root.push_back(mytree.get_root());
    //mytree.breath_first_print(root);


// -------------------------------------------------------------------------*/









/* --------------------------------------------------------------------------------------------------- TEST: testing αβ seach and minimax on some state x_t
    TTT_state x_t('X');

    int i;
    LOOP(i,0,4) x_t.transition();

    x_t.print_board();
    tuple<int, int> optimal = MinMax::αβ_search(x_t);
    cout << " \n \n \noptimal move : (" << std::get<0>(optimal) << "," << std::get<1>(optimal) << ") \n";
//  -----------------------------------------------------------------------------------------------------*/





/*  ---------------------------------------------TEST: playing againt AI (αβ or minimax)
    int i,  i_1, i_2;
    tuple<int, int> optimal;
    while(!x_t.get_termina())
    {
        x_t.print_board();

        cout << "\n enter \n";
        cin >> i_1;
        cin >> i_2;
        cout << "ok \n";

        x_t.transition(i_1, i_2);

        TTT_state x(x_t);

        optimal = MinMax::min_max_search(x);
        x_t.transition(std::get<0>(optimal), std::get<1>(optimal));
        x_t.print_board();

    }
//  ------------------------------------------------*/








 /* ------------------------------------------------------------------------------------------------------------------------TEST: teasting K-in-a-row feature


    TTT_state x('X');

    int i;
    LOOP(i, 0, 20)
    {
        x.transition();
        cout << "\n";
        x.print_board();
        cout << "terminal: " << x.get_termina() << "\n";
        cout << "outcome:" << x.get_outcome() << "\n";
    }


    cout << "\n END!!!!!!";
//  --------------------------------------------------------------------------------------------------------------------------*/




    return a.exec();
}

