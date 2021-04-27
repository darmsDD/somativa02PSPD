#include<bits/stdc++.h>

using namespace std;


long long n_variables,n_clauses;
const int MAX {1100000};

void full(vector<vector<int>> & clauses,bitset<MAX> & variable, vector<int> & clause_value, map<int,int> &false_variables,set<int>&false_clauses);
void resultado(map<int,int> &false_variables,set<int>&false_clauses);
void flip(map<int,vector<int>> & variable_in_clauses,bitset<MAX> &variable,map<int,int> &false_variables,set<int>&false_clauses,vector<int> & clause_value);

struct comp {

    template<typename T>
    bool operator() ( const T &a,  const T &b) const{
        if (a.second != b.second) {
            return a.second > b.second;
        }
        return abs(a.first) > abs(b.first);
    }
};




int main(){

    
    cin >> n_variables >> n_clauses;
   
    map<int,vector<int>>variable_in_clauses;
    vector<vector<int>>clauses;
    bitset<MAX> variable;
    for(int i=0;i<n_clauses;i++){
        int t;
        vector<int>temp_clause;
        while(cin >> t,t!=0){
            temp_clause.push_back(t);
            variable_in_clauses[t].push_back(i);
        }
        clauses.push_back(temp_clause);
    }

    vector<int>clause_value(n_clauses+1,0);
    map<int,int> false_variables;
    set<int>false_clauses;
    string s;
    while(cin >> s){
        if(s=="full"){
            full(clauses,variable,clause_value,false_variables,false_clauses); 
        } else if(s=="flip"){
            flip(variable_in_clauses,variable,false_variables,false_clauses,clause_value);
        }
        resultado(false_variables,false_clauses);
    }


    return 0;

}

void full(vector<vector<int>> & clauses,bitset<MAX> & variable, vector<int> & clause_value,map<int,int> &false_variables,set<int>&false_clauses){

    for(int i=0;i<n_variables;i++){
        int t;
        cin >> t;
        if(t<0){
            t = abs(t);
            variable[t]=0;
        } else{
            variable[t]=1;
        }
    }

    for(int i=0;i<clauses.size();i++){
        for(int j=0;j<clauses[i].size();j++){
            int t = clauses[i][j];
            if((t<0 && variable[abs(t)]==0) ||(t>0 && variable[t]==1) ){
                clause_value[i]+=1;
            }
        }
        if(clause_value[i]==0){
            false_clauses.insert(i);
            for(int j=0;j<clauses[i].size();j++){
                int t = clauses[i][j];
                false_variables[t]++;
            }
           
        }
    }
    

}

void clauses_new_values(vector<int> & variable_in_clause,bool positive,vector<int> & clause_value,int t,map<int,int> &false_variables,set<int>&false_clauses){
    int k = positive?1:-1;
    for(auto & p: variable_in_clause){
        if(clause_value[p]==0 && k==1){
            false_variables[t]--;
            false_clauses.erase(p);
        } else if(clause_value[p]==1 && k==-1){
            false_variables[t]++;
            false_clauses.insert(p);
        }
        
        clause_value[p]+=k;

    }
    if(false_variables[t]==0){
        false_variables.erase(t);
    }

}



void flip (map<int,vector<int>> & variable_in_clauses, bitset<MAX> &variable,map<int,int> &false_variables,set<int>&false_clauses,vector<int> & clause_value){
    int t;
    cin >> t;
    int k = abs(t);
    variable[k]= variable[k]?0:1;
    
    if(variable[k] == 0){
        clauses_new_values(variable_in_clauses[k],false,clause_value,k,false_variables,false_clauses);
        clauses_new_values(variable_in_clauses[-k],true,clause_value,-k,false_variables,false_clauses);
    } else{
        clauses_new_values(variable_in_clauses[-k],false,clause_value,-k,false_variables,false_clauses);
        clauses_new_values(variable_in_clauses[k],true,clause_value,k,false_variables,false_clauses);
    }
    
}



void resultado(map<int,int> &false_variables,set<int>&false_clauses){

    if(false_clauses.empty()){
        cout << "SAT" << endl;
        return;
    }

    cout << "["<<false_clauses.size()<<" clausulas falsas]";
    for(auto const &p: false_clauses){
        cout <<' ' << p;
    }
    cout << endl;
    set<pair<int,int>,comp> false_variables2(false_variables.begin(),false_variables.end());
    cout << "[lits]";
    for(auto const &p: false_variables2){
        cout << ' ' << p.first;
    }
    cout << endl;

}