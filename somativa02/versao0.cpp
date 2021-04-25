#include<bits/stdc++.h>

using namespace std;


vector<vector<long long>>clauses;
vector<long long> clausesValue;
map<long long,bool> variablesValue;
map<long long,int> falseClauses;
map<long long,int>falseVariables;
vector<vector<long long>>variablesPosition;

long long n_variaveis,n_clausulas;

int main(){

    
    cin >> n_variaveis >> n_clausulas;

    for(long long i=0;i<n_clausulas;i++){
        clausesValue.push_back(0);
        vector<long long> temp;
        while(true){
            long long variable;
            cin >> variable;
            if(variable==0){
                break;
            }
            
            temp.push_back(variable);
            clausesValue[i]++;
        }
        clauses.push_back(temp);
        
    }
    string comando;
    while(cin >> comando){
        if(comando=="full"){
            full();
        }
        else{
            flip();
        }
    }
    return 0;

}

void full(){

    for(int i=0;i<n_variaveis;i++){
        long long variable;
        cin >> variable;
        variablesValue[variable]= true;
        variablesValue[-variable]= false;
        for(int k=0;k<clausesValue.size();k++){
            auto it = clausesValue[k].find(-variable)
            if(it!=clausesValue.end()){
                int pos = it - clausesValue.begin();
                clausesValue[pos]--;
                if(clausesValue[pos]==0){
                    falseClauses[pos]=1;
                    for(auto )
                }
            }
        }
       

    }

}