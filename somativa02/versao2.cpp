#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <map>
#include <algorithm>
#include <stack>
#include <thread>
#include <mutex>
#include <semaphore.h>

#define N_THREADS 4
using namespace std; 

using ii = pair<int, int>;
int v, c;
vector<vector<int>> cl;
vector<int> values;
vector<int> tc_falses[N_THREADS];
map<int, int> tv_falses[N_THREADS];
pthread_t threads[N_THREADS];
int args[N_THREADS];
void read_input();
void process_full();
void process_flip();
void calculate();
void* thread_handler(void* _);

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    read_input();
    string command;
    while(cin >> command){
        if(command == "full"){
            process_full();
        }
        else{
            process_flip();
        }
        calculate();
    }
}

void read_input()
{
    cin >> v >> c;
    values = vector<int>(v+1);
    cl = vector<vector<int>>(c);
    for (auto &vs : cl)
    {
        int x;
        while (cin >> x, x != 0)
            vs.push_back(x);
    }
}

void process_full(){
    for(int i=1; i<=v; ++i){
        int x;
        cin >> x;
        values[abs(x)] = x<0 ? 0 : 1;
    }
}
void process_flip(){
    int x;
    cin >> x;
    if(values[x] == 0){
        values[x] = 1;
    }
    else{
        values[x] = 0;
    }
}

void * thread_handler(void* arg){
    int k = *((int *)arg);
    for(int i=k; i<(int)cl.size(); i+=N_THREADS)
    {
        bool res = false;
        stack<int> q;
        for (auto &x : cl[i])
        {
            if (values[abs(x)] == 0 and x < 0)
                res = true;
            else if (values[abs(x)] != 0 and x > 0)
                res = true;
            else
                q.push(x);
        }
        if (!res)
        {
            tc_falses[k].push_back(i);
            
            while (not q.empty())
            {
                ++tv_falses[k][q.top()];
                q.pop();
            }
        }
    }
    return NULL;
}
void calculate()
{
    vector<int> c_falses;
    map<int, int> v_falses;
    for(int i=0; i<N_THREADS; i++){
        args[i] = i;
        pthread_create(&threads[i],NULL,thread_handler, (void *)&args[i]);
    }
    for(int i=0;i<N_THREADS;i++){
        pthread_join(threads[i],NULL);
    }
    for(int i=0; i<N_THREADS; ++i){
        for(auto &x:tc_falses[i]) c_falses.push_back(x);
        for(auto &x:tv_falses[i]) v_falses[x.first] += x.second;
        tv_falses[i].clear();
        tc_falses[i].clear();
    }
    vector<ii> lits;
    for (auto x : v_falses)
    {
        lits.emplace_back(x.second, x.first);
    }
    sort(lits.begin(), lits.end(), [](ii a, ii b) {
        return a.first == b.first ? abs(a.second) > abs(b.second) : a.first > b.first;
    });
    sort(c_falses.begin(), c_falses.end());
    if (c_falses.empty())
    {
        cout << "SAT\n";
    }
    else
    {
        cout << "[" << c_falses.size() << " clausulas falsas]";
        for (auto &x : c_falses)
            cout << ' ' << x;
        cout << '\n';
        cout << "[lits]";
        for (auto x : lits)
            cout << ' ' << x.second;
        cout << '\n';
    }
}