#include <iostream>
#include <list>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
int boatcap;
struct state
{
    int left_missionary; //left mis count
    int left_cannibal; //left can count
    int right_missionary; //right mis count
    int right_cannibal; //right can count
    bool left_boat; //boat left check
    bool right_boat; //boat right check
    struct state *parent;

    bool operator==(const struct state &state2)
    {
        return ((*this).left_missionary == state2.left_missionary &&
                (*this).left_cannibal == state2.left_cannibal &&
                (*this).right_missionary == state2.right_missionary &&
                (*this).right_cannibal == state2.right_cannibal &&
                (*this).left_boat == state2.left_boat &&
                (*this).right_boat == state2.right_boat);
    }

    struct state operator+(const struct state &state2)
    {
        struct state result_state;
        result_state.left_missionary = (*this).left_missionary + state2.left_missionary;
        result_state.left_cannibal = (*this).left_cannibal + state2.left_cannibal;
        result_state.right_missionary = (*this).right_missionary + state2.right_missionary;
        result_state.right_cannibal = (*this).right_cannibal + state2.right_cannibal;
        result_state.left_boat = !(*this).left_boat;
        result_state.right_boat = !(*this).right_boat;
        result_state.parent = this;

        return result_state;
    }
};

int expand , explore ;
float timeout = 0.0;
bool check_valid_state(struct state newstate)
{
    /* Check for positive; missionaries > cannibals */
    if (newstate.left_missionary < 0 ||newstate.left_cannibal < 0 || newstate.right_missionary < 0 || newstate.right_cannibal < 0)
        return false;

    if ((newstate.left_missionary != 0 && newstate.left_missionary < newstate.left_cannibal) ||
            (newstate.right_missionary != 0 && newstate.right_missionary < newstate.right_cannibal))
        return false;

    return true;
}

void print_states_of_solution(struct state nprint, ofstream *out)
{
    if(nprint.left_boat)
    {
        cout << "Boat on destination side"<<endl;
        *out << "Boat on destination side"<<endl;
    }
    else
    {
        cout << "Boat on wrong side"<<endl;
        *out << "Boat on wrong side"<<endl;
    }

    cout << "Destination Bank: Mis - " << nprint.left_missionary <<  " Can - "<<nprint.left_cannibal << "\n";
    cout << "wrong Bank: Mis - " << nprint.right_missionary << " Can - " << nprint.right_cannibal << "\n\n";

    *out << "Destination Bank: Mis - " << nprint.left_missionary <<  " Can - "<<nprint.left_cannibal << "\n";
    *out << "wrong Bank: Mis - " << nprint.right_missionary << " Can - " << nprint.right_cannibal << "\n\n";
}

/* Build the list of successors */
list<struct state> find_succesors(struct state *exstate)
{
    list<struct state> successor;
    struct state s;
    int i,j;

    for (i = 0; i <= boatcap; i++)
    {
        for (j = 0; j <= boatcap; j++)
        {
            if(i==0 && j==0)
            {
                continue;
            }
            else if((i+j)<=boatcap )
            {
                const struct state actionleft = {-i, -j, i, j, NULL, NULL, NULL};
                const struct state actionright = {i, j, -i, -j, NULL, NULL, NULL};
                s = *exstate + (exstate->left_boat ? actionleft : actionright);
                if (check_valid_state(s))
                {
                    successor.push_front(s);
                }
            }
        }
    }

    return successor;
}


/* BFS & DFS */
list<struct state> search_algorithm(struct state start, struct state goal, bool mode)
{
    expand = 0;
    explore = 0;
    list<struct state> closed_list;
    list<struct state> open_list; //open list
    list<struct state> solution;
    list<struct state> expanded_nodes;
    struct state current_state;
    struct state current_state1;
    const clock_t begin_time = clock();

    open_list.push_front(start);
    explore++;
    while (!open_list.empty())
    {
        if((float( clock () - begin_time ) /  CLOCKS_PER_SEC)>=timeout){
            cout<<"\n\n\nTimeout!\n\n\n";
            open_list.clear();
            return open_list;
        }
        if (mode == true) //DFS
        {
            current_state = open_list.front();
            open_list.pop_front();
            //expand++;
        }
        else //BFS
        {
            current_state = open_list.back();
            open_list.pop_back();
            //expand++;
        }

        if (current_state == goal)
        {
            while (!(current_state == start))
            {
                solution.push_front(current_state);
                current_state = *current_state.parent;
            }
            solution.push_front(start);
            cout << "time exceeded : " <<float( clock () - begin_time ) /  CLOCKS_PER_SEC << " s" << endl;
            return solution;
        }

        /*  state from solution is not in closed_list */
        if (find(closed_list.begin(), closed_list.end(), current_state) == closed_list.end())
        {
            closed_list.push_front(current_state);
            explore++;
            struct state *storestate = new state;
            *storestate = current_state;
            expanded_nodes = find_succesors(storestate);
            while (!expanded_nodes.empty()){
                current_state1 = expanded_nodes.front();
                expanded_nodes.pop_front();
                if ((find(open_list.begin(), open_list.end(), current_state1) == open_list.end()) &&
                    (find(closed_list.begin(), closed_list.end(), current_state1) == closed_list.end())){
                    open_list.insert(open_list.begin(),current_state1);
                    expand++;
                }
            }
        }
    }
    cout << "time exceeded : " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << " s"<<endl;
    return open_list;
}

int main( )
{
    list<struct state> solution;
    struct state start = {};
    struct state goal = {};
    ofstream output("result.txt");
    bool mode;

    cout<<"Enter Missionaries Count"<<endl;
    cin>>start.right_missionary;
    goal.left_missionary = start.right_missionary;

    cout<<"Enter Cannibals Count"<<endl;
    cin>>start.right_cannibal;
    goal.left_cannibal = start.right_cannibal;

    cout<<"Enter Boat Capacity"<<endl;
    cin>>boatcap;

    cout<<"Enter Timeout"<<endl;
    cin>>timeout;

    start.left_missionary = 0;
    start.left_cannibal = 0;
    start.left_boat = false;
    start.right_boat = true;

    goal.left_boat = true;
    goal.right_missionary = 0;
    goal.right_cannibal = 0;
    goal.right_boat = false;

    printf("Running BFS...\n");
    output << "Running BFS...\n";

    solution = search_algorithm(start, goal, false);

    if(solution.empty())
    {
        output << "No Solution"<<endl;
        cout   << "No Solution" << endl;
    }

    while(!solution.empty())
    {
        print_states_of_solution(solution.front(), &output);
        solution.pop_front();
    }

    cout<<"Expanded : " << expand << " Explored : " << explore <<endl;
    printf("Finished\n");
    output << "Finished\n";

    printf("\n\n\nRunning DFS...\n");
    output << "\n\n\nRunning DFS...\n";

    solution.clear();
    solution = search_algorithm(start, goal, true);

    if(solution.empty())
    {
        output << "No Solution"<<endl;
        cout   << "No Solution" << endl;
    }

    while(!solution.empty())
    {
        print_states_of_solution(solution.front(), &output);
        solution.pop_front();
    }
    cout<<"Expanded : " << expand << " Explored : " << explore <<endl;
    printf("Finished\n");
    output << "Finished\n";

    output.close();

    return 0;
}
