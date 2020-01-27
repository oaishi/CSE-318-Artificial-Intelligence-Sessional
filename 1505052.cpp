// n puzzle solver
#include<bits/stdc++.h>
using namespace std;
ofstream output1;
int rowsize;
int moves[4][2]= {{-1,0},{1,0},{0,-1},{0,1}}; //up,down,left,right
typedef struct board
{
    vector<int> matrix;
    int g,h;
    board *parent;

    bool operator==(const struct board &board2)
    {
        for (int i = 0; i < rowsize; ++i)
        {
            for (int j = 0; j < rowsize; ++j)
            {
                if((*this).matrix[i + j * rowsize] != board2.matrix[i + j * rowsize])
                    return false;
            }
        }
        return true;
    }

} board;

board start,goal;  //start and finish nodes

class myComparator
{
public:
    int operator() (const board& p1, const board& p2)
    {
        return (p1.g+p1.h) > (p2.g+p2.h);
    }
};

void print(vector<int> matrix)
{
    for (int i = 0; i < rowsize; ++i)
    {
        for (int j = 0; j < rowsize; ++j)
        {
            cout<<matrix[i + j * rowsize]<<" ";
            output1<<matrix[i + j * rowsize]<<" ";
        }
        cout<<endl;
        output1<<endl;
    }
}

pair <int, int> findblank(vector<int> matrix)
{
    pair <int, int> blank ;

    for (int i = 0; i < rowsize; ++i)
    {
        for (int j = 0; j < rowsize; ++j)
        {
            if(matrix[i + j * rowsize]==0)
            {
                blank.first = i; //row
                blank.second = j; //column
                return blank;
            }
        }
    }
}

int inversioncount(vector<int> matrix,int x)
{
    vector<int> matrix2;
    matrix2.resize(rowsize*rowsize);
    int count=0;
    int n=0;
    for (int i = 0; i < rowsize; ++i)
    {
        for (int j = 0; j < rowsize; ++j)
        {
            matrix2[n]=matrix[i + j * rowsize]; //row major matrix
            n++;
        }
    }


    for(int i = 0; i < n; i++)
    {
        for(int j = i+1; j < n; j++)
        {
            if(matrix2[i] > matrix2[j] && matrix2[i] && matrix2[j])
            {
                //cout<<matrix2[i]<<" "<<matrix2[j]<<endl;
                count++;
            }
        }
    }

    if(!(rowsize%2))  //even board size
    {
        count+= x;
        if(!(count%2))  //even invariant
        {
            //cout<<"Not Solvable\n";
            return -1;
        }
    }
    else if(count%2)  //odd board size & odd inversion
    {
        //cout<<"Not Solvable\n";
        return -1;
    }


    return count;
}

int hamming(vector<int> matrix)
{
    int count=0;
    int result=0;
    for (int i = 0; i < rowsize; ++i)
    {
        for (int j = 0; j < rowsize; ++j)
        {
            count++;
            if(matrix[i + j * rowsize]!=0 && matrix[i + j * rowsize]!=count)
            {
                result++;
            }
        }
    }
    return result;
}

int manhattan(vector<int> matrix)
{
    int row,col;
    int result=0;
    for (int i = 0; i < rowsize; ++i)
    {
        for (int j = 0; j < rowsize; ++j)
        {
            if(matrix[i + j * rowsize])
            {
                col = matrix[i + j * rowsize] % rowsize;
                if(col==0)
                    col=rowsize;
                row = (matrix[i + j * rowsize] - col)/rowsize;
                col--;
                result+= abs(i-row)+abs(j-col);
                /*cout<<"Inside manhattan"<<matrix[i + j * rowsize]<<" "<<i<<" "<<j<<" "
                <<row<<" "<<col<<" "<<result<<endl;*/
            }
        }
    }
    return result;
}

int conflict(vector<int> matrix)
{
    int row,col,row1,col1;
    int result=0;
    int conflicting=0;
    for (int i = 0; i < rowsize; ++i)
    {
        for (int j = 0; j < rowsize; ++j)
        {
            if(matrix[i + j * rowsize]!=0)
            {
                col = matrix[i + j * rowsize] % rowsize;
                if(col==0)
                    col=rowsize;
                row = (matrix[i + j * rowsize] - col)/rowsize;
                col--;
                result+= abs(i-row)+abs(j-col);
                // code for conflict resolution
                for (int k = j+1; k < rowsize; ++k)//same row check
                {
                    if(matrix[i + k * rowsize]!=0)
                    {
                        col1 = matrix[i + k * rowsize] % rowsize;
                        if(col1==0)
                            col1=rowsize;
                        row1 = (matrix[i + k * rowsize] - col1)/rowsize;
                        //col1--;
                        if(row1==row && row==i && matrix[i + k * rowsize]<matrix[i + j * rowsize])
                            conflicting++;
                    }
                }
                for (int k = i+1; k < rowsize; ++k)//same column check
                {
                    if(matrix[k + j * rowsize]!=0)
                    {
                        col1 = matrix[k + j * rowsize] % rowsize;
                        if(col1==0)
                            col1=rowsize;
                        col1--;
                        if(col1==col && col==j && matrix[k + j * rowsize]<matrix[i + j * rowsize])
                            conflicting++;
                    }
                }
            }
        }
    }
    return result + (2*conflicting);
}




bool validity(int x,int y)
{
    if(x>=0 && x<rowsize && y>=0 && y<rowsize)
        return true;
    return false;
}

//find visible nodes from these sources
list<board> find_succesors(board *current)
{
    list<board> successor;
    board s;
    int row,col;
    pair <int, int> blank = findblank(current->matrix);
    for(int i=0; i<4; i++)
    {
        row = blank.first + moves[i][0];
        col = blank.second + moves[i][1];
        if(validity(row,col))  //valid node check , now make a move
        {
            s.matrix = current->matrix; //copy the parent vector
            swap(s.matrix[blank.first + blank.second * rowsize],s.matrix[row + col * rowsize]);
            s.parent = current; //parent set
            s.g = current->g + 1; //distance edit
            //if((inversioncount(s.matrix,row))!=-1)  //can lead to solution
            //{

            successor.push_front(s);
            //}
        }
    }

    return successor;
}

void search_algorithm(int mode)
{
    int expand, explore;
    list<board> closed_list;
    list<board> expanded_nodes;
    board current, childnode;
    priority_queue <board, vector<board>, myComparator > open_list;

    start.g=0;
    open_list.push(start);
    while (open_list.empty() == false)
    {
        current = open_list.top();
        open_list.pop();

        if (current == goal) //print solution
        {
            list<board> solution;
            cout<<"Solution found\n";
            expand = closed_list.size();
            cout<<"Expansion :"<<expand<<endl;
            explore = expand + open_list.size();
            cout<<"Exploration :"<<explore<<endl;
            output1<<"Solution found\n";
            output1<<"Expansion :"<<expand<<endl;
            output1<<"Exploration :"<<explore<<endl;
            int n=0;
            while (!(current == start))  //inverse path
            {
                solution.push_front(current);
                current = *current.parent;
            }
            solution.push_front(start);

            while(!solution.empty())
            {
                cout<<n<<"."<<endl;
                output1<<n<<"."<<endl;
                print(solution.front().matrix);
                solution.pop_front();
                n++;
            }
            return;
        }

        //not found in the list
        if (find(closed_list.begin(), closed_list.end(), current) == closed_list.end())
        {
            closed_list.push_front(current);
            board *storestate = new board;
            *storestate = current;
            expanded_nodes = find_succesors(storestate);
            while (!expanded_nodes.empty())
            {
                childnode = expanded_nodes.front();
                expanded_nodes.pop_front();
                if ((find(closed_list.begin(), closed_list.end(), childnode) == closed_list.end()))
                {
                    //new node
                    if(mode==1)
                        childnode.h = hamming(childnode.matrix);
                    else if(mode==2)
                        childnode.h = manhattan(childnode.matrix);
                    else
                        childnode.h = conflict(childnode.matrix);
                    //cout<<childnode.h+childnode.g<<" before pushing\n";
                    open_list.push(childnode);
                }
            }
        }

    }
}



int main()
{
    ifstream output("start.txt");
    cout<<"Enter puzzle size:";
    cin>>rowsize;
    start.matrix.resize(rowsize*rowsize);
    goal.matrix.resize(rowsize*rowsize);

    int n=0;
    for (int i = 0; i < rowsize; ++i)
    {
        for (int j = 0; j < rowsize; ++j)
        {
            n++;
            //cin>>start.matrix[i + j * rowsize];
            output>>start.matrix[i + j * rowsize];
            goal.matrix[i + j * rowsize] = n;
        }
    }
    output.close();
    goal.matrix[(rowsize-1) + (rowsize-1) * rowsize] = 0;

    print(start.matrix);

    int j = inversioncount(start.matrix, findblank(start.matrix).first);
    if(j==-1)
    {
        cout<<"Not solvable\n";
        return -1;
    }

    output1.open("resultham.txt");

    cout<<"Hamming Heauristics.\n\n\n";
    output1<<"Hamming Heauristics.\n\n\n";
    search_algorithm(1);

    output1.close();

    output1.open("resultman.txt");
    cout<<"\n\n\n Manhattan Heauristics.\n\n\n";
    output1<<"Manhattan Heauristics.\n\n\n";
    search_algorithm(2);

    output1.close();

    output1.open("resultlin.txt");
    cout<<"\n\n Linear conflict Heauristics.\n\n";
    output1<<"Linear conflict Heauristics.\n\n";
    search_algorithm(3);
    output1.close();
    return 0;
}
