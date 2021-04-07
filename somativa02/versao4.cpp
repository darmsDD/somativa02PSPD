#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <map>
#include <algorithm>
#include <stack>
#include <set>
#include <unordered_map>
using namespace std;
#define ff first
#define ss second

using ii = pair<int, int>;
int v, c;
vector<set<int>> clauses;
vector<int> trueInClause;
set<int> falsesClause;
unordered_map<int, int> falsesVariable;
unordered_map<int, bool> value;
void read_input();
void process_full();
void process_flip();
void calculate();

int32_t main()
{
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
    clauses = vector<set<int>>(c);
    trueInClause = vector<int>(c);
    for(int i=0; i<c; ++i){
        int x;
        while (cin >> x, x != 0)
            clauses[i].insert(x);
    }
}

void process_full(){
    for(int i=0; i<v; ++i){
        int x;
        cin >> x;
        value[x] = true;
        value[-x] = false;
    }

    falsesVariable.clear();
    falsesClause.clear();
    for(int i=0; i<c; ++i){
        trueInClause[i] = 0;
        for(auto variable:clauses[i]){
            if(value[variable]){
                trueInClause[i]++;
            }
        }
        if(trueInClause[i] == 0){
            falsesClause.insert(i);
            for(auto variable:clauses[i]) 
                falsesVariable[variable]++; 
        }
    }
}

void process_flip(){
    int x;
    cin >> x;
    value[x] = !value[x];
    value[-x] = !value[-x];

    for(int i=0; i<c; ++i){
        int lastValue = trueInClause[i];

        auto it1 = clauses[i].find(x);
        auto it2 = clauses[i].find(-x);

        if(it1 != clauses[i].end()){
            if(value[x] == false) {
                trueInClause[i]--;
            }
            else{
                trueInClause[i]++;
            }
        }
        else if(it2 != clauses[i].end()){
            if(value[-x] == false) {
                trueInClause[i]--;
            }
            else{
                trueInClause[i]++;
            }
        }
        //valgrind
        if(lastValue!=0 and trueInClause[i] == 0){
            falsesClause.insert(i);
            for(auto variable:clauses[i]) 
                falsesVariable[variable]++; 
        }
        if(lastValue==0 and trueInClause[i] != 0){
            falsesClause.erase(i);
            for(auto variable:clauses[i])
                falsesVariable[variable]--;
        }
    }
}

void calculate()
{
    vector<ii> lits;
    for(auto x:falsesVariable){
        if(x.ss>0) lits.emplace_back(x.ss, x.ff);
    }
    sort(lits.begin(), lits.end(), [](ii a, ii b){
        return a.ff == b.ff ? abs(a.ss) > abs(b.ss) : a.ff > b.ff;
    });
    if(falsesClause.empty()){
        cout << "SAT\n";
    }
    else{
        cout << "[" << falsesClause.size() << " clausulas falsas]"; 
        for(auto x:falsesClause)
            cout << ' ' << x;
        cout << '\n';
        cout << "[lits]";
        for(auto x:lits)
                cout << ' ' << x.ss;
        cout << '\n';
    }
}


