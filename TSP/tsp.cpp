#include<bits/stdc++.h>
using namespace std;

typedef struct node
{
    int s;
    int d;
    double dis;
} node;

double cost[100][100];
int visited[100];
int minn=9999;
int minn1;
int minn2;
int mnn1;
int mnn2;
float timeout = 8.0;
vector<int> feasible;
vector<int> feasible1;
vector<int> worstfeas;
vector<int> worstfeas1;
vector<node> saving;
vector<pair<int,int> >vertices;
double mincost;
queue<int> path;
vector<int>path1;
typedef vector<node> savforiter;
typedef vector<pair<int,int> > edge;
typedef pair<int,int> nexthop;
template<typename T>
void remove(std::vector<T> & v, const T & item)
{
    v.erase(std::remove(v.begin(), v.end(), item), v.end());
}
template<typename T>
void pop_front(std::vector<T> &v)
{
    if (v.size() > 0)
    {
        v.erase(v.begin());
    }
}

struct myclass
{
    bool operator() (const node& p1, const node& p2)
    {
        return (p1.dis) > (p2.dis);
    }
} myComparator;


double euclid_dist(double x1,double y1,double x2,double y2)
{
    return sqrt(abs(x1-x2)*abs(x1-x2)+abs(y1-y2)*abs(y1-y2));
}

void clear(queue<int> &q)
{
    queue<int> empty;
    swap(q,empty);
}

double nearest_neighbor_simple(int start,int n)
{
    mincost=0;
    int now=start;
    clear(path);
    path.push(now);
    visited[now]=1;
    int next;

    while(path.size()<n)
    {
        for(int i=0; i<n; i++)
        {
            if(visited[i]==0 && i!=now)
            {
                if(cost[now][i]<=minn)
                {
                    minn=cost[now][i];
                    next=i;
                }
            }
        }
        path.push(next);
        mincost+=cost[now][next];
        visited[next]=1;
        now=next;
        minn=9999;
    }
    mincost+=cost[now][start];
    return mincost;
}

int run_near_simple(int k,int n)
{
    queue<int> best;
    queue<int> worst;
    queue<int> average;
    int cur;
    int bestnode,worstnode,avgnode;
    double bestcase=INFINITY;
    double worstcase=0;
    double avgcase=INFINITY;
    double curdist;

    for(int i=k; i>0; i--)
    {
        cur = rand() % n;

        for(int j=0; j<n; j++)
        {
            visited[j]=0;
        }

        curdist = nearest_neighbor_simple(cur,n);
        if(bestcase>curdist)
        {
            best = path;
            bestnode = cur;
            bestcase = curdist;
        }
        if(worstcase<curdist)
        {
            worst = path;
            worstnode = cur;
            worstcase = curdist;
        }
        double estimatedavg = (worstcase+bestcase)/2;
        if(abs(curdist-estimatedavg)<abs(avgcase-estimatedavg))
        {
            average = path;
            avgnode = cur;
            avgcase = curdist;
        }

    }

    cout<<"the min cost path is: "<<endl;
    feasible.clear();

    while(!best.empty())
    {
        int previous=best.front();
        feasible.push_back(previous);
        cout<<previous<<"->";
        best.pop();
        int next=best.front();
    }
    feasible.push_back(bestnode);
    cout<<bestnode<<endl<<endl;
    cout<<"the min cost is: "<<bestcase<<endl<<endl;

    cout<<"the worst cost path is: "<<endl;
    worstfeas.clear();

    while(!worst.empty())
    {
        int previous=worst.front();
        worstfeas.push_back(previous);
        cout<<previous<<"->";
        worst.pop();
        int next=worst.front();
    }
    worstfeas.push_back(worstnode);
    cout<<worstnode<<endl<<endl;
    cout<<"the worst cost is: "<<worstcase<<endl<<endl;


    cout<<"the average cost path is: "<<endl;
    //worstfeas.clear();

    while(!average.empty())
    {
        int previous=average.front();
        //worstfeas.push_back(previous);
        cout<<previous<<"->";
        average.pop();
        int next=average.front();
    }
    //worstfeas.push_back(worstnode);
    cout<<avgnode<<endl<<endl;
    cout<<"the average cost is: "<<avgcase<<endl<<endl;

    minn1 = bestnode;
    minn2 = worstnode;
    return bestnode;
}


void nearest_neighbor(int start,int n)
{
    for(int i=0; i<n; i++)
    {
        visited[i]=0;
    }
    mincost=0;
    int now=start;
    clear(path);
    path.push(now);
    visited[now]=1;
    int next,next1,next2;
    vector<pair<int,int> > neighbour;
    pair<int,int> np;
    int randcount = 5;
    int r;

    while(path.size()<n)
    {
        neighbour.clear();
        for(int i=0; i<n; i++)
        {
            if(visited[i]==0 && i!=now)
            {
                neighbour.push_back(make_pair(i, cost[now][i]));
            }
        }

        sort(neighbour.begin(), neighbour.end(), [](auto &left, auto &right)
        {
            return left.second < right.second;
        });
        if(randcount>neighbour.size())
        {
            randcount = neighbour.size();
        }
        r = rand() % randcount;
        np = neighbour.at(r);
        path.push(np.first);
        visited[np.first]=1;
        now=np.first;

    }
    cout<<"the min cost path is: "<<endl;

    while(!path.empty())
    {
        int previous=path.front();
        feasible.push_back(previous);
        cout<<previous<<"->";
        path.pop();
        int next=path.front();
        mincost+=cost[previous][next];
    }
    feasible.push_back(start);
    cout<<start<<endl<<endl;
    cout<<"the min cost is: "<<mincost<<endl<<endl;
}

bool isnotcycle(int x,int y)
{
    for(edge::iterator it = vertices.begin(); it != vertices.end(); ++it)
    {
        if((it->first)==x)
        {
            for(edge::iterator it1 = vertices.begin(); it1 != vertices.end(); ++it1)
            {
                if((it1->second)==y)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

bool isnotcycle2(int x)
{
    for(edge::iterator it = vertices.begin(); it != vertices.end(); ++it)
    {
        if((it->first)==x)
        {
            return false;
        }
    }
    return true;
}

double savings(int start,int n)
{
    mincost=0;
    int now=start;
    clear(path);
    path.push(now);
    visited[now]=1;
    int next;
    node newnode;
    int n1,n2,cur;
    vector<int> unvisited;
    vector<int> unsink;
    saving.clear();
    vertices.clear();

    for(int i=0 ; i<n ; i++)
    {
        if(i!=start)
        {
            unvisited.push_back(i);
            unsink.push_back(i);
        }
        for(int j=0; j <n ; j++)
        {
            if(i!=j && i!=start && j!=start)
            {
                newnode = {i, j, cost[start][i]+cost[start][j]-cost[i][j]};
                saving.push_back(newnode);
            }
        }
    }

    sort (saving.begin(), saving.end(), myComparator);
    const clock_t begin_time = clock();
    while(unvisited.size() >1)  //tour creation
    {
        //cout<<"inside";
        if((float( clock () - begin_time ) /  CLOCKS_PER_SEC)>=timeout)
        {
            return INFINITY;
        }
        newnode = saving.front();
        pop_front(saving);
        if(visited[newnode.d]==0 && isnotcycle(newnode.d,newnode.s) && isnotcycle2(newnode.s)) //there should be no partial tour with source = d
        {
            visited[newnode.d]=1;
            vertices.push_back(make_pair(newnode.s, newnode.d));
            mincost+=cost[newnode.s][newnode.d];
            remove(unvisited,newnode.d);
            remove(unsink,newnode.s);

        }
    }

    n1 = unvisited.at(0);
    n2 = unsink.at(0);

    vertices.push_back(make_pair(n2, now));
    vertices.push_back(make_pair(now, n1));
    mincost+=cost[n2][now];
    mincost+=cost[now][n1];
    path.push(n1);
    cur = n1;

    vertices.pop_back();
    //edges to queue
    while(vertices.size()>0)
    {
        for(edge::iterator it = vertices.begin(); it != vertices.end(); ++it)
        {
            if((it->first)==cur)  // find the next node to visit
            {
                cur = it->second;
                //cout<<"->"<< cur;
                path.push(cur);
                vertices.erase(it);
                break;
            }
        }
    }

    return mincost;

}


int run_saving_simple(int k,int n)
{
    queue<int> best;
    queue<int> worst;
    queue<int> average;
    int cur;
    int bestnode,worstnode,avgnode;
    double bestcase=INFINITY;
    double worstcase=0;
    double avgcase=INFINITY;
    double curdist;

    for(int i=k; i>0; i--)
    {
        cur = rand() % n;
        cout<<cur<<"\n";
        for(int j=0; j<n; j++)
        {
            visited[j]=0;
        }

        //cout<<"before sav";
        curdist = savings(cur,n);
        //cout<<"after sav";
        if(bestcase>curdist)
        {
            best = path;
            //swap(path,best);
            bestnode = cur;
            bestcase = curdist;
        }
        if(worstcase<curdist && curdist!=INFINITY)
        {
            worst = path;
            //swap(path,worst);
            worstnode = cur;
            worstcase = curdist;
        }

        double estimatedavg = (worstcase+bestcase)/2;
        if(abs(curdist-estimatedavg)<abs(avgcase-estimatedavg) && curdist!=INFINITY)
        {
            average = path;
            avgnode = cur;
            avgcase = curdist;
        }

    }

    cur = minn1;
    //cout<<cur<<"\n";
    for(int j=0; j<n; j++)
    {
        visited[j]=0;
    }

    //cout<<"before sav";
    curdist = savings(cur,n);
    //cout<<"after sav";
    if(bestcase>curdist)
    {
        best = path;
        //swap(path,best);
        bestnode = cur;
        bestcase = curdist;
    }
    if(worstcase<curdist && curdist!=INFINITY)
    {
        worst = path;
        //swap(path,worst);
        worstnode = cur;
        worstcase = curdist;
    }

    cur = minn2;
    //cout<<cur<<"\n";
    for(int j=0; j<n; j++)
    {
        visited[j]=0;
    }

    //cout<<"before sav";
    curdist = savings(cur,n);
    //cout<<"after sav";
    if(bestcase>curdist)
    {
        best = path;
        //swap(path,best);
        bestnode = cur;
        bestcase = curdist;
    }
    if(worstcase<curdist && curdist!=INFINITY)
    {
        worst = path;
        //swap(path,worst);
        worstnode = cur;
        worstcase = curdist;
    }

    if(bestcase!=INFINITY){

    cout<<"the min cost path is: "<<endl;

    feasible1.clear();
    //mincost=0;
    while(!best.empty())
    {
        int previous=best.front();
        feasible1.push_back(previous);
        cout<<previous<<"->";
        best.pop();
        int next=best.front();
        //mincost+=cost[previous][next];
    }
    //cout<<bestnode<<endl<<endl;
    cout<<"\nthe min cost is: "<<bestcase<<endl<<endl;
    }
    else
        return 0;

    if(worstcase!=0){
    cout<<"the worst cost path is: "<<endl;

    worstfeas1.clear();

    //double worstcost=0;
    while(!worst.empty())
    {
        int previous=worst.front();
        worstfeas1.push_back(previous);
        cout<<previous<<"->";
        worst.pop();
        int next=worst.front();
        //worstcost+=cost[previous][next];
    }
    //cout<<worstnode<<endl<<endl;
    cout<<"\nthe worst cost is: "<<worstcase<<endl<<endl;
    }
    else
        return -1;

    if(avgcase!= INFINITY && avgcase!=bestcase)
    {
        cout<<"the average cost path is: "<<endl;
    //worstfeas.clear();

    while(!average.empty())
    {
        int previous=average.front();
        //worstfeas.push_back(previous);
        cout<<previous<<"->";
        average.pop();
        int next=average.front();
    }
    //worstfeas.push_back(worstnode);
    cout<<avgnode<<endl<<endl;
    cout<<"the average cost is: "<<avgcase<<endl<<endl;
    }
    mnn1 = bestnode;
    mnn2 = worstnode;
    return 1;
}

double savings_random(int start,int n)
{
    mincost=0;
    int now=start;
    clear(path);
    path.push(now);
    visited[now]=1;
    int next;
    node newnode;
    int n1,n2,cur;
    vector<int> unvisited;
    vector<int> unsink;
    saving.clear();
    vertices.clear();

    for(int i=0 ; i<n ; i++)
    {
        if(i!=start)
        {
            unvisited.push_back(i);
            unsink.push_back(i);
        }
        for(int j=0; j <n ; j++)
        {
            if(i!=j && i!=start && j!=start)
            {
                newnode = {i, j, cost[start][i]+cost[start][j]-cost[i][j]};
                saving.push_back(newnode);
            }
        }
    }

    sort (saving.begin(), saving.end(), myComparator);
    int c = rand() % saving.size();
    newnode = saving.at(c);
    saving.erase(saving.begin() + c);
        //pop_front(saving);
        if(visited[newnode.d]==0 && isnotcycle(newnode.d,newnode.s) && isnotcycle2(newnode.s)) //there should be no partial tour with source = d
        {
            visited[newnode.d]=1;
            vertices.push_back(make_pair(newnode.s, newnode.d));
            mincost+=cost[newnode.s][newnode.d];
            remove(unvisited,newnode.d);
            remove(unsink,newnode.s);
        }


    const clock_t begin_time = clock();
    while(unvisited.size() >1)  //tour creation
    {
        if((float( clock () - begin_time ) /  CLOCKS_PER_SEC)>=timeout)
        {
            return INFINITY;
        }
        newnode = saving.front();
        pop_front(saving);
        if(visited[newnode.d]==0 && isnotcycle(newnode.d,newnode.s) && isnotcycle2(newnode.s)) //there should be no partial tour with source = d
        {
            visited[newnode.d]=1;
            vertices.push_back(make_pair(newnode.s, newnode.d));
            mincost+=cost[newnode.s][newnode.d];
            remove(unvisited,newnode.d);
            remove(unsink,newnode.s);
        }
    }

    n1 = unvisited.at(0);
    n2 = unsink.at(0);

    vertices.push_back(make_pair(n2, now));
    vertices.push_back(make_pair(now, n1));
    mincost+=cost[n2][now];
    mincost+=cost[now][n1];
    path.push(n1);
    cur = n1;

    vertices.pop_back();
    //edges to queue
    while(vertices.size()>0)
    {
        for(edge::iterator it = vertices.begin(); it != vertices.end(); ++it)
        {
            if((it->first)==cur)  // find the next node to visit
            {
                cur = it->second;
                //cout<<"->"<< cur;
                path.push(cur);
                vertices.erase(it);
                break;
            }
        }
    }

    return mincost;

}


double totalDistance(vector<int>path)
{
    vector<int>::iterator it;
    int prev,later,cnt=0;
    double dist=0;
    it=path.begin();
    while(it<path.end())
    {
        cnt++;
        prev=*it;
        it++;
        later=*it;
        if(cnt!=path.size())
        {
            dist+=cost[prev][later];
        }
    }
    return dist;
}

vector<int> twoOptSwap(vector<int>path,int p,int k)
{
    vector<int>new_path;
    for(int i=0; i<=p-1; i++)
    {
        new_path.push_back(path[i]);
    }
    for(int j=k; j>=p; j--)
    {
        new_path.push_back(path[j]);
    }
    for(int i=k+1; i<path.size(); i++)
    {
        new_path.push_back(path[i]);
    }
    return new_path;
}

void twoOpt(int option , int n)
{
    for(int i=0; i<n; i++)
    {
        visited[i]=0;
    }
    path1.clear();
    vector<int>new_path;
    vector<int>new_path_worst;
    double new_dist=0;
    double best_dist;
    double worst_dist;

    int improvement=0;

    //nearest_neighbor(0,n);

    if(option==1)
    path1=feasible;
    else if(option==2)
    path1=worstfeas;
    else if(option==3)
    path1=feasible1;
    else if(option==4)
    path1=worstfeas1;

    new_path_worst=path1;

    while(improvement<20)
    {
        best_dist=totalDistance(path1);
        worst_dist=totalDistance(path1);
        for(int i=1; i<n-1; i++)
        {
            for(int j=i+1; j<n; j++)
            {
                new_path=twoOptSwap(path1,i,j);
                new_dist=totalDistance(new_path);
                if(new_dist<best_dist)
                {
                    improvement=0;
                    path1=new_path;
                    best_dist=new_dist;
                }
                else if(new_dist>worst_dist)
                {
                    new_path_worst=new_path;
                    worst_dist=new_dist;
                }
            }
        }
        improvement++;
    }
    //cout<<"the two opt: "<<endl;
    vector<int>::iterator it;
    cout<<"the min cost path is : "<<endl;

    int prev,later,cnt=0;
    mincost=0;
    it=path1.begin();
    while(it<path1.end())
    {
        cnt++;
        cout<<*it;
        prev=*it;
        it++;
        later=*it;
        if(cnt!=path1.size())
        {
            cout<<"->";
            mincost+=cost[prev][later];
        }
    }
    cout<<endl;
    cout<<"the mincost is: "<<mincost<<endl;


    cout<<"the worst cost path is : "<<endl;

    prev,later,cnt=0;
    mincost=0;
    it=new_path_worst.begin();
    while(it<new_path_worst.end())
    {
        cnt++;
        cout<<*it;
        prev=*it;
        it++;
        later=*it;
        if(cnt!=new_path_worst.size())
        {
            cout<<"->";
            mincost+=cost[prev][later];
        }
    }
    cout<<endl;
    cout<<"the worst cost is: "<<mincost<<endl;

    return;
}

double cheapest_insertion(int start,int n)
{
    mincost=0;
    int next,from,to;
    path1.clear();
    path1.push_back(start);
    visited[start]=1;
    minn=9999;
    for(int i=0; i<n; i++)
    {
        if(visited[i]==0 && i!=start)
        {
            if(cost[start][i]<=minn)
            {
                minn=cost[start][i];
                next=i;
            }
        }
    }
    path1.push_back(next);
    visited[next]=1;
    path1.push_back(start);

    minn=9999;
    for(int i=0; i<n; i++)
    {
        if(visited[i]==0)
        {
            for(int j=0; j+1<path1.size(); j++)
            {
                if(cost[path1[j]][path1[i]]+cost[path1[i]][path1[j+1]]-cost[path1[j]][path1[j+1]]<=minn)
                {
                    minn=cost[path1[j]][path1[i]]+cost[path1[i]][path1[j+1]]-cost[path1[j]][path1[j+1]];
                    from=path1[j];
                    to=path1[j+1];
                }
            }
            vector<int>::iterator it;
            it=path1.begin();
            while(*it!=from)
            {
                it++;
            }
            it++;
            path1.insert(it,i);
            visited[i]=1;
            minn=9999;
        }
    }

    //mincost=0;
    vector<int>::iterator it;
    //cout<<"the min cost path is : "<<endl;

    int prev,later,cnt=0;
    it=path1.begin();
    while(it<path1.end())
    {
        cnt++;
        //cout<<*it;
        prev=*it;
        it++;
        later=*it;
        if(cnt!=path1.size())
        {
            //cout<<"->";
            mincost+=cost[prev][later];
        }
    }

    return mincost;

}

int run_close_simple(int k,int n)
{
    vector<int> best;
    vector<int> worst;
    vector<int> average;
    int cur;
    int bestnode,worstnode,avgnode;
    double bestcase=INFINITY;
    double worstcase=0;
    double avgcase = INFINITY;
    double curdist;

    for(int i=k; i>0; i--)
    {
        cur = rand() % n;

        for(int j=0; j<n; j++)
        {
            visited[j]=0;
        }

        curdist = cheapest_insertion(cur,n);
        if(bestcase>curdist)
        {
            best = path1;
            bestnode = cur;
            bestcase = curdist;
        }
        if(worstcase<curdist)
        {
            worst = path1;
            worstnode = cur;
            worstcase = curdist;
        }
        double estimatedavg = (worstcase+bestcase)/2;
        if(abs(curdist-estimatedavg)<abs(avgcase-estimatedavg))
        {
            average = path1;
            avgnode = cur;
            avgcase = curdist;
        }
    }

    cout<<"the min cost path is : "<<endl;
    vector<int>::iterator it;
    int prev,later,cnt=0;
    it=best.begin();
    while(it<best.end())
    {
        cnt++;
        cout<<*it;
        prev=*it;
        it++;
        later=*it;
        if(cnt!=best.size())
        {
            cout<<"->";
        }
    }

    cout<<"\nthe min cost is: "<<bestcase<<endl<<endl;

    cout<<"the worst cost path is: "<<endl;

    prev,later,cnt=0;
    it=worst.begin();
    while(it<worst.end())
    {
        cnt++;
        cout<<*it;
        prev=*it;
        it++;
        later=*it;
        if(cnt!=worst.size())
        {
            cout<<"->";
        }
    }

    cout<<"\nthe worst cost is: "<<worstcase<<endl<<endl;


    cout<<"the average cost path is: "<<endl;

    prev,later,cnt=0;
    it=average.begin();
    while(it<average.end())
    {
        cnt++;
        cout<<*it;
        prev=*it;
        it++;
        later=*it;
        if(cnt!=average.size())
        {
            cout<<"->";
        }
    }

    cout<<"\nthe average cost is: "<<avgcase<<endl<<endl;

   // minn1 = bestnode;
   // minn2 = worstnode;
    return bestnode;
}

int main()
{
    ifstream fileread("input.txt");
    int n;
    double x,y;
    fileread>>n;
    vector<pair<double,double> >cord;
    for(int i=0; i<n; i++)
    {
        fileread>>x>>x>>y;
        cord.push_back(make_pair(x,y));
    }
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
            if(i==j)
            {
                cost[i][j]=0;
            }
            else
            {
                cost[i][j]=euclid_dist(cord[i].first,cord[i].second,cord[j].first,cord[j].second);
            }
        }
    }

    cout<<"The cheapest insertion: "<<endl;
    run_close_simple(5,n);

    cout<<"The nearest neighbor statistics: "<<endl;
    int best = run_near_simple(5,n);

    int flag = 0;

    cout<<"Savings Heuristics statistics: "<<endl;
    flag = run_saving_simple(5,n);

    cout<<"The nearest neighbor statistics for best node - "<< best << ": "<<endl;
    nearest_neighbor(best,n);

    /*if(flag != 0){
    cout<<"The savings statistics for best node - "<< mnn1 << ": "<<endl;
    savings_random(mnn1,n);
    }*/

    cout<<"\nRunning two opt for best node -"<<best <<": from nearest neighbour" <<endl;
    twoOpt(1,n);

    cout<<"\nRunning two opt for worst node -"<<minn2 <<": from nearest neighbour" <<endl;
    twoOpt(2,n);

    if(flag){
    cout<<"\nRunning two opt for best node -"<<best <<": from savings neighbour" <<endl;
    twoOpt(3,n);

    cout<<"\nRunning two opt for worst node -"<<minn2 <<": from savings neighbour" <<endl;
    twoOpt(4,n);}

    return 0;
}

