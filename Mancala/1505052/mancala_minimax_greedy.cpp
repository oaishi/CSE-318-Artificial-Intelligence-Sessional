#include<bits/stdc++.h>
using namespace std;

#define humanwins -10000
#define cpuwins 10000
#define matchdraw -5000

#define no_move -100000
#define max_depth 2

#define W1 300
#define W2 300
#define W3 300
#define W4 300


int capturedseeds;

class MancalaBoard
{
public:
    typedef vector<int> boardmap;

    MancalaBoard(int pits, int stones) : pits(pits), stones(stones)
    {
        for (int i=0; i<=(2*pits+1); i++)
            field.push_back(stones);

        field[pits] = 0;
        field[2*pits+1] = 0;
    };

    void addField(int index, int add)
    {
        int temp = getField(index);
        temp += add;
        setField(index, temp);
    };

    int getPit(void)
    {
        return pits;
    };
    int getStone(void)
    {
        return stones;
    };

    int getField(int index)
    {
        return field[index];
    };

    boardmap getField(void)
    {
        return field;
    };

    void setField(int index, int value)
    {
        field[index] = value;
    };

    void printBoard(void) ;
    int gameFinish(void) ;

private:
    boardmap field;
    int pits;
    int stones;
};

void MancalaBoard::printBoard(void)
{
    cout << "CPU: ";
    for(int i=(2*pits); i>pits; i--)
    {
        if (i < 10)
            cout <<i<<" |";
        else
            cout <<i<<"|";
    }
    cout<<endl;

    cout << " ----";
    for(int i=pits; i>0; i--)
    {
        cout << "---";
    }
    cout << "---" <<  endl;

    cout << " |  |";

    for(int i=(2*pits); i>pits; i--)
    {
        cout << field[i];
        if (field[i]<10)
            cout << " |";
        else
            cout << "|";
    }
    cout << "  |" << endl;

    cout << " |" << field[(2*pits+1)];

    if (field[2*pits+1] < 10)
        cout << " |";
    else
        cout << "|";

    for(int i=pits; i>1; i--)
        cout << "--+";

    cout << "--|" << field[pits];
    if (field[pits]<10)
        cout << " |";
    else
        cout << "|";

    cout << endl;

    cout << " |  |";
    for(int i=0; i<pits; i++)
    {
        cout << field[i];
        if (field[i]<10)
            cout << " |";
        else
            cout << "|";
    }
    cout << "  |" << endl;

    cout << " ----";
    for(int i=pits; i>0; i--)
        cout << "---";

    cout << "---" << endl;

    cout << "HUM: ";
    for(int i=0; i<pits; i++)
    {
        if (i < 10)
            cout <<i<<" |";
        else
            cout <<i<<"|";
    }
    cout<<endl;
}

int MancalaBoard::gameFinish(void)
{
    if (field[(2*pits+1)] > (stones*pits))
    {
        //cout<<"cpu wins\n";
        return 2;
    }
    if (field[pits] > (stones*pits))
    {
        //cout<<"hooman wins\n";
        return 1;
    }
    int human = 0;
    int cpu = 0;
    int endgame = 0;

    for (int i=0 ; i<pits ; i++)
        human += field[i];

    if (human == 0)
    {
        cpu = 0;
        for (int i=pits+1 ; i<(2*pits+2) ; i++)
            cpu += field[i];

        endgame = 1;
        human = field[pits];
    }

    for (int i=pits+1 ; i<(2*pits+1) ; i++)
        cpu += field[i];

    if (cpu == 0)
    {
        human = 0;
        for (int i=0 ; i<pits+1 ; i++)
            human += field[i];

        endgame = 1;
        cpu = field[(2*pits+1)];
    }

    if ((human>cpu) && (endgame==1)){
        cout<<"hooman wins\n";
        return 1;
    }

    if ((human==cpu) && (endgame==1))
        return 3;

    if ((cpu>human) && (endgame==1)){
        cout<<"cpu wins\n";
        cout<<"human "<<field[pits]<<" - cpu : "<<field[(2*pits+1)]<<endl;
        return 2;
    }


    return 0;
}

class Player
{

public:
    enum Side { HUMAN, CPU };
    Player(MancalaBoard board, Side side);
    virtual bool getTurn() const = 0;
    enum status { TRYAGAIN, STORE, EMPTY, PIT };
    status ValidSow(MancalaBoard& board, Side side, int frompit);
    virtual void setTurn(bool turn) = 0;
    void CaptureSeeds(MancalaBoard& board, Side side, int lastpit);
    enum Algo { GREEDY, MINIMAX };
    virtual int SelectTurn(int& bestMove, Side side, MancalaBoard board, Algo tactic, int heus) = 0;

private:
    MancalaBoard board;
    Side side;

};

Player::Player(MancalaBoard board, Side side) : side(side), board(board) {}

Player::status Player::ValidSow(MancalaBoard& board, Side side, int frompit)
{
    if ((side == HUMAN) && (frompit > board.getPit()))
    {
        return TRYAGAIN;
    }
    if ((side == CPU) && (frompit < (board.getPit())+1))
    {
        return TRYAGAIN;
    }
    if (frompit < 0)
    {
        return TRYAGAIN;
    }
    if ((frompit == board.getPit()) || (frompit == (2*board.getPit()+1)))
    {
        return TRYAGAIN;
    }
    if (board.getField(frompit) == 0)
    {
        return TRYAGAIN;
    }

    int pit = frompit;
    int previouspit = frompit;
    int sewed = board.getField(frompit);
    board.setField(frompit, 0);


    while (sewed > 0)
    {
        if(pit != -1)
        {
            previouspit = pit;
            pit++;
        }
        if (pit == -1)
        {
            previouspit = (2*board.getPit()+1);
            pit = 0;
        }


        if (side == CPU)
        {
            if (pit == (2*board.getPit()+1))
            {
                board.addField(pit, 1);
                sewed--;
                pit = -1;
            }
            else
            {
                board.addField(pit, 1);
                sewed--;
            }
        }
        else if (side == HUMAN)
        {
            if (pit == (2*board.getPit()+1))
            {
                pit = -1;
            }
            else
            {
                board.addField(pit, 1);
                sewed--;
            }
        }
    }

    if (pit == -1)
        pit = (2*board.getPit()+1);

    if ( ( (pit == board.getPit()) && (side == HUMAN) ) || ((pit == (2*board.getPit()+1)) && (side == CPU)) )
        return STORE;

    if ((board.getField(pit) == 1) && !((pit == board.getPit()) || (pit == (2*board.getPit()+1))) )
    {
        CaptureSeeds(board, side, pit);
        return EMPTY;
    }



    return PIT;
};

void Player::CaptureSeeds(MancalaBoard& board, Side side, int lastpit)
{
    int stealpit;
    if (side == CPU)
    {
        if (lastpit > board.getPit())
        {
            stealpit = lastpit - board.getPit();
            stealpit = board.getPit() - stealpit;

            capturedseeds = board.getField(stealpit);

            board.addField((2*board.getPit()+1), board.getField(stealpit));
            board.addField((2*board.getPit()+1), 1);
            board.setField(lastpit, 0);
            board.setField(stealpit, 0);
        }
    }
    else if (side == HUMAN)
    {
        if (lastpit < board.getPit())
        {
            stealpit = board.getPit() - lastpit;
            stealpit = board.getPit() + stealpit;

            capturedseeds = board.getField(stealpit);

            board.addField(board.getPit(), board.getField(stealpit));
            board.addField(board.getPit(), 1);
            board.setField(lastpit, 0);
            board.setField(stealpit, 0);
        }
    }
}

class Human: public Player
{

private:
    int depth;
    bool turn;
    int step;

public:
    Human(MancalaBoard& board);
    int SelectTurn(int& bestMove, Side side, MancalaBoard board, Algo tactic, int heus);
    bool getTurn() const
    {
        return turn;
    }
    void setTurn(bool t)
    {
        if (t==true)
            turn=true;

        if (t==false)
            turn=false;

        return;
    }

};

Human::Human(MancalaBoard& board) : Player(board, HUMAN), turn(false) {}

int stones_on_cpu_side(MancalaBoard dummyboard)
{
    int sum = 0;
    for (int i=(dummyboard.getPit()+1); i<(2*dummyboard.getPit()+1); i++)
        sum+= dummyboard.getField(i);

    return sum;
}

int stones_on_human_side(MancalaBoard dummyboard)
{
    int sum = 0;
    for (int i=0; i<dummyboard.getPit(); i++)
        sum+= dummyboard.getField(i);

    return sum;

}

int Human::SelectTurn(int& bestMove, Side side, MancalaBoard board, Algo tactic, int heus)
{
    int* a = NULL;
    a = new int[board.getPit()];
    MancalaBoard dummyboard = board;
    int dummy;
    depth++;

    if (tactic == GREEDY)
    {
        int score = board.getField(board.getPit());
        int humanpits = 0;
        for (int i=0; i<board.getPit(); i++)
        {
            int movevalue = score;
            if (board.getField(i) != 0)
            {
                status reply = ValidSow(dummyboard, side, i);
                movevalue = dummyboard.getField(board.getPit()) - score;
                if(heus>1)
                    movevalue = W1 * movevalue + W2 * (stones_on_human_side(dummyboard) - stones_on_cpu_side(dummyboard));
                if (reply == STORE)
                {
                    if(heus>2)
                        a[humanpits] = movevalue + W3 * SelectTurn(dummy, side, dummyboard, GREEDY,heus);
                    else
                        a[humanpits] = movevalue ;//+ W3 * SelectTurn(dummy, side, dummyboard, GREEDY,heus);
                }
                else if (reply == EMPTY && heus>3)
                {
                    a[humanpits] = movevalue + W4 * capturedseeds;
                }
                else
                    a[humanpits] = movevalue;

            }
            else
                a[humanpits] = no_move;

            humanpits++;
            dummyboard = board;
        }


    }
    else if (tactic == MINIMAX)
    {
        if (side == CPU) // HUMAN
        {
            int score = board.getField(2*board.getPit()+1);
            for (int i=(board.getPit()+1); i<(2*board.getPit()+1); i++)
            {
                int movevalue = score;
                if (board.getField(i) != 0)
                {
                    status reply = ValidSow(dummyboard, side, i);
                    movevalue =  dummyboard.getField(2*board.getPit()+1) - score;

                    if(heus>1)
                        movevalue = W1 * movevalue + W2 * (stones_on_cpu_side(dummyboard) - stones_on_human_side(dummyboard));

                    if (reply == STORE)
                    {
                        depth--;
                        step++;
                        //a[i] = movevalue + SelectTurn(dummy, CPU, dummyboard, ALPHABETA,heus);
                        if(heus>2)
                            a[i] = movevalue + W3 * SelectTurn(dummy, CPU, dummyboard, MINIMAX,heus);
                        else
                          //  a[humanpits] = movevalue ;
                            a[i] = movevalue + SelectTurn(dummy, CPU, dummyboard, MINIMAX,heus); // recursive call to chooseMove

                        step--;
                        depth++;
                    }
                    else
                    {
                        if (depth <= max_depth)
                            a[i] = movevalue - SelectTurn(dummy, HUMAN, dummyboard, MINIMAX,heus); // recursive call to chooseMove
                        else if (reply == EMPTY && heus>3)
                        {
                            a[i] = movevalue + W4 * capturedseeds;
                        }
                        else
                            a[i] = movevalue;
                    }
                }
                else
                    a[i] = no_move;

                dummyboard = board;
            }
        }
        else if (side == HUMAN) // COMPUTER
        {
            int score = board.getField(board.getPit());
            int humanpits = 0;
            for (int i=0; i<board.getPit(); i++)
            {
                int movevalue = score;
                if (board.getField(i) != 0)
                {
                    status reply = ValidSow(dummyboard, side, i);
                    movevalue = dummyboard.getField(board.getPit()) - score;

                    if(heus>1)
                        movevalue = W1 * movevalue + W2 * (stones_on_human_side(dummyboard) - stones_on_cpu_side(dummyboard));

                    if (reply == STORE)
                    {
                        depth--;
                        step++;
                        if(heus>2)
                            a[humanpits] = movevalue + W3 * SelectTurn(dummy, side, dummyboard, MINIMAX,heus);
                        else
                          //  a[humanpits] = movevalue ;
                            a[humanpits] = movevalue + SelectTurn(dummy, HUMAN, dummyboard, MINIMAX,heus); // recursive call to chooseMove
                        step--;
                        depth++;

                    }

                    else
                    {
                        if (depth <= max_depth)
                            a[humanpits] = movevalue - SelectTurn(dummy, CPU, dummyboard, MINIMAX, heus); // recursive call to chooseMove
                        else if (reply == EMPTY && heus>3)
                        {
                            a[humanpits] = movevalue + W4 * capturedseeds;
                        }
                        else
                            a[humanpits] = movevalue;

                    }
                }
                else
                    a[humanpits] = no_move;

                humanpits++;
                dummyboard = board;
            }
        }
    }

    int max = INT_MIN;

    //cout<<"human ";

    for (int i=0; i<board.getPit(); i++)
    {
        if (a[i] > max)
        {
            max = a[i];
            // cout<<i<<" ";
            bestMove = i;
        }
    }
    // cout<<endl;

    if (max == no_move)
    {
        if (side == CPU)
        {
            if (dummyboard.getField(2*board.getPit()+1) > (board.getPit() * board.getStone()))
                max = cpuwins;
            else if (dummyboard.getField(2*board.getPit()+1) == (board.getPit() * board.getStone()))
                max = matchdraw;
            else
                max = humanwins;
        }
        else if (side == HUMAN)
        {
            if (dummyboard.getField(board.getPit()) > (board.getPit() * board.getStone()))
                max = -humanwins;
            else if (dummyboard.getField(board.getPit()) == (board.getPit() * board.getStone()))
                max = -matchdraw;
            else
                max = -cpuwins;
        }
    }

    depth--;
    delete [] a;
    return max;
}

class Computer: public Player
{
public:
    Computer(MancalaBoard& board);
    int SelectTurn(int& bestMove, Side side, MancalaBoard board, Algo tactic, int heus);
    bool getTurn() const
    {
        return turn;
    }

    void setTurn(bool t)
    {
        if (t==true)
            turn=true;
        if (t==false)
            turn=false;

        return;
    }

private:
    bool turn;
    int depth;
    int step;
};

Computer::Computer(MancalaBoard& board) : Player(board, CPU), turn(false), depth(0), step(0) {}

int Computer::SelectTurn(int& bestMove, Side side, MancalaBoard board, Algo tactic, int heus)
{
    depth++;
    int* a = NULL;
    a = new int[board.getPit()];
    MancalaBoard boardcopy = board;
    int dummy;

    if (tactic == GREEDY)
    {
        int score = board.getField(2*board.getPit()+1);
        int computerpits = 0;
        for (int i=board.getPit()+1; i<(2*board.getPit()+1); i++)
        {
            int movevalue = score;
            if (board.getField(i) != 0)
            {
                status reply = ValidSow(boardcopy, side, i);
                movevalue = boardcopy.getField(2*board.getPit()+1) - score;
                if(heus>1)
                        movevalue = W1 * movevalue + W2 * (stones_on_cpu_side(boardcopy) - stones_on_human_side(boardcopy));



                if (reply == STORE)
                {
                    if(heus>2)
                            a[computerpits] = movevalue + W3 * SelectTurn(dummy, side, boardcopy, GREEDY,heus);
                        else
                            a[computerpits] = movevalue ;
                }
                else if (reply == EMPTY && heus>3)
                {
                    a[computerpits] = movevalue + W4 * capturedseeds;
                }
                else
                    a[computerpits] = movevalue;
            }
            else
                a[computerpits] = no_move;

            computerpits++;
            boardcopy = board;
        }


    }
    else if (tactic == MINIMAX)
    {
        if (side == HUMAN)
        {
            int score = board.getField(board.getPit());
            for (int i=0; i<board.getPit(); i++)
            {
                int movevalue = score;
                if (board.getField(i) != 0)
                {
                    status reply = ValidSow(boardcopy, side, i);
                    movevalue =  boardcopy.getField(board.getPit()) - score;

                    if(heus>1)
                        movevalue = W1 * movevalue + W2 * (stones_on_human_side(boardcopy) - stones_on_cpu_side(boardcopy));


                    if (reply == STORE)
                    {
                        depth--;
                        step++;
                        if(heus>2)
                            a[i] = movevalue + W3 * SelectTurn(dummy, HUMAN, boardcopy, MINIMAX,heus);
                        else
                          //  a[humanpits] = movevalue ;
                            a[i] = movevalue + SelectTurn(dummy, HUMAN, boardcopy, MINIMAX,heus); // recursive call to chooseMove

                        step--;
                        depth++;
                    }
                    else
                    {
                        if (depth <= max_depth)
                            a[i] = movevalue - SelectTurn(dummy, CPU, boardcopy, MINIMAX, heus); // recursive call to chooseMove
                        else if (reply == EMPTY && heus>3)
                        {
                            a[i] = movevalue + W4 * capturedseeds;
                        }
                        else
                            a[i] = movevalue;

                    }
                }
                else
                    a[i] = no_move;

                boardcopy = board;
            }
        }
        else if (side == CPU)
        {
            int score = board.getField(2*board.getPit()+1);
            int computerpits = 0;
            for (int i=(board.getPit()+1); i<(2*board.getPit()+1); i++)
            {
                int movevalue = score;
                if (board.getField(i) != 0)
                {
                    status reply = ValidSow(boardcopy, side, i);
                    movevalue = boardcopy.getField(2*board.getPit()+1) - score;

                    if(heus>1)
                        movevalue = W1 * movevalue + W2 * (stones_on_cpu_side(boardcopy) - stones_on_human_side(boardcopy));


                    if (reply == STORE)
                    {
                        depth--;
                        step++;
                        if(heus>2)
                            a[computerpits] = movevalue + W3 * SelectTurn(dummy, CPU, boardcopy, MINIMAX, heus); // recursive call to chooseMove
                        else
                            a[computerpits] = movevalue + SelectTurn(dummy, CPU, boardcopy, MINIMAX, heus); ;
                        step--;
                        depth++;
                    }
                    else
                    {
                        if (depth <= max_depth)
                            a[computerpits] = movevalue - SelectTurn(dummy, HUMAN, boardcopy, MINIMAX, heus); // recursive call to chooseMove
                        else if (reply == EMPTY && heus>3)
                        {
                            a[computerpits] = movevalue + W4 * capturedseeds;
                        }
                        else
                            a[computerpits] = movevalue;

                    }
                }
                else
                    a[computerpits] = no_move;

                computerpits++;
                boardcopy = board;
            }
        }
    }

    int max = INT_MIN;
    for (int i=0; i<board.getPit(); i++)
    {
        if (a[i] > max)
        {
            max = a[i];
            bestMove = i+board.getPit()+1;
        }
    }

    if (max == no_move)
    {
        if (side == CPU)
        {
            if (boardcopy.getField(2*board.getPit()+1) > (board.getPit() * board.getStone()))
                max = cpuwins;
            else if (boardcopy.getField(2*board.getPit()+1) == (board.getPit() * board.getStone()))
                max = matchdraw;
            else
                max = humanwins;
        }
        else if (side == HUMAN)
        {
            if (boardcopy.getField(board.getPit()) > (board.getPit() * board.getStone()))
                max = -humanwins;
            else if (boardcopy.getField(board.getPit()) == (board.getPit() * board.getStone()))
                max = -matchdraw;
            else
                max = -cpuwins;
        }
    }

    depth--;
    delete [] a;
    return max;
}
