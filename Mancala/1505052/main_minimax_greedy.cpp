#include<bits/stdc++.h>
#include "mancala_minimax_greedy.cpp"

using namespace std;

#define def_pit_count 6
#define def_stone_count 4


int main()
{

    int pits, stones, heus , heus1;
    cout << "pits ? 1<p<8" << endl;
    cin >> pits;
    if (pits == 0 || pits > 8 )
    {
        pits = def_pit_count;
    }

    cout << "stones ? 1<s<7" << endl;
    cin >> stones;
    if (stones == 0 || stones > 7)
    {
        stones = def_stone_count;
    }

    cout<<"Heuristics for human? \n";
    cin>>heus;

    cout<<"Heuristics for cpu? \n";
    cin>>heus1;

    //for(int i=0;i<10;i++){

    MancalaBoard board(pits, stones);

    Computer cpu(board);
    cpu.setTurn(false);

    Human human(board);
    human.setTurn(false);

    cout << "Player 1? 1=Human 2=CPU" << endl;
    int begin;
    cin >> begin;
    if (begin == 1)
        human.setTurn(true);
    else
        cpu.setTurn(true);

    board.printBoard();

    while (!board.gameFinish())
    {
        Player::status response;
        if (human.getTurn() == true)
        {

            int pitno;
            human.SelectTurn(pitno, Player::HUMAN, board, Player::MINIMAX,heus);

            cout << "Human sows :" << pitno << endl;

            response = human.ValidSow(board, Player::HUMAN, pitno);
            if (response == Player::STORE)
            {
                cout << "human received another turn!" << endl;
            }
            else
            {
                cpu.setTurn(true);
                human.setTurn(false);
            }

        }
        else if (cpu.getTurn() == true)
        {
            int pitno;

            cpu.SelectTurn(pitno, Player::CPU, board, Player::MINIMAX,heus1);

            cout << "CPU sows :" << pitno << endl;

            response = cpu.ValidSow(board, Player::CPU, pitno);

            if (response == Player::STORE)
            {
                cout << "CPU received another turn!" << endl;
            }
            else
            {
                human.setTurn(true);
                cpu.setTurn(false);
            }
        }

        board.printBoard();
    }

    int matchresult = board.gameFinish();

    if(matchresult==1)
        cout<<"Winner : Human \n";
    else if(matchresult==2)
        cout<<"Winner : CPU \n";
    else
        cout<<"Draw \n";

    //}

    while(1)
        cin.get();

    return 0;
}
