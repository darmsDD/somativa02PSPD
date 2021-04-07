#include<bits/stdc++.h>

using namespace std;
vector<string> name;
using filesystem::directory_iterator;

ofstream myfile;
double solve(string source,char delimiter){
    string dest="";
    bool minEx=false;
    long long time=0;
    for(int i=0;i<source.size();i++){
        if(source[i]==':'){
            minEx=true;
            //cout << dest << endl;
            //cout << "entrou no minnuto\n";
            time = stoll(dest, nullptr, 10)*60;
            //cout << time << endl;
            dest="";
        }
        else if(source[i]!=delimiter){
            dest+=source[i];
        } else{
            break;
        }
    }
    double time2=0.0;
    if(!dest.empty()){
         time2 = stod(dest);
    }
   
    return time+time2;
}

void imprimeVar(string temp){
    // cout << temp << endl;
    ifstream cin(temp);
    long long variaveis,clausulas;
    cin >> variaveis >> clausulas;
    string s;
    long long i=0;
    bool ver=false;
    while(getline(cin,s)){
        if(s[0]=='f' && ver==false){
            ver=true;
            getline(cin,s);
        } 
        if(ver){
            i++;
        }
        
    }
    cout << "Casos de teste = " << i << " variaveis = " << variaveis << " clausulas = " << clausulas << endl;
    myfile << i << ","<< variaveis << ","<< clausulas <<",";
    return;
}


void getFileNames(){
    string path  = "./pspd-2020.2/input";
   
    for (const auto & file : directory_iterator(path)){
        string a =  file.path();  
        name.push_back(a);
    }
    // for(auto p: name){
    //     cout << p << endl;
    // }

}

string getNomeArquivo(string temp){
    string b="";
    for(int i=temp.size()-1;temp[i]!='/';i--){
        b+=temp[i];
    }
    reverse(b.begin(),b.end());
    // cout << b << endl;

    return b;

}

void openFile(){
    myfile.open ("planilha.csv");
    myfile << "Nome arquivo,Casos de Teste, Variáveis, Cláusulas, Tempo Real, Tempo User, Tempo Sys" << endl;
}


int main(int argc, char *argv[]){
   
    openFile();
    system("mkdir diff");
    getFileNames();
    //compila o arquivo de teste
    if(argc>1){
        string compila="g++ -o prog -std=c++17 -O2 -static -Wall -W ";
        string arquivo(argv[1]);
        compila+=arquivo;
        char compila2[compila.size() + 1];
        strcpy(compila2, compila.c_str());
        system(compila2);
        //cout << compila2 << endl;
    }
    int j=0,k=0;
    char a = '0';
    while(j<name.size()){

        string nomeArquivo =getNomeArquivo(name[j]);
        char nomeArquivo2[nomeArquivo.size() + 1];
        strcpy(nomeArquivo2, nomeArquivo.c_str());
        string arg1 = name[j];
        myfile << nomeArquivo << ",";
        // cout << arg1 << endl;


        
        
        // monta a string para executar o teste com time
        
        string comando = "{ time ./prog < ";
        comando+=arg1;
        comando+=" > out;} 2>time.txt";
        char comando2[comando.size() + 1];
        strcpy(comando2, comando.c_str());
        cout <<"Comando = " << comando2 << endl;
        
        // monta a string pro comando diff
        char arg2[200] = "pspd-2020.2/output/";
        char diff[200] = "diff out ";
        char arquivoDiff[] = " > diff/0";
        if(k>9 && a=='0'){
            a = 'A';
            k=0;
        } else if(k>25){
            a = 'a';
            k=0;
        }
        arquivoDiff[8]=k+ a;
        k++;
        strcat(arg2,nomeArquivo2);
        strcat(diff,arg2);
        strcat(diff,arquivoDiff);
        cout <<"Comando diff = "<< diff << endl;
        
        // joga casos de teste, variaveis e clausulas no csv
        imprimeVar(name[j]);

        double real3=0,user3=0,system3=0;
        for(int i=0;i<5;i++){
            system(comando2);

            // monta e executa a string do arquivo de diff
            if(i==4){
                system(diff);
            }
            

            string line;
            ifstream cin("time.txt");
            string real,user,system,lixo;
            cin >> user >> system >> real;
        

            double user2,real2,system2;
            real2 = solve(real,'e');
            user2 = solve(user,'u');
            system2 = solve(system,'s');
            // cout << fixed << setprecision(2) << "real = " <<real2 << "s ";
            // cout <<"user = " << user2 << "s ";
            // cout <<"system = " <<  system2 <<"s"<<endl;
            real3+=real2;
            user3+=user2;
            system3+=system2;
        }
        real3/=5;
        user3/=5;
        system3/=5;
        cout << fixed << setprecision(2) << "real = " <<real3 <<" user = " << user3 <<" system = " <<  system3 <<"\n\n";
        myfile << real3 << "," << user3 <<"," << system3<< endl;
        j++;
    }
    myfile.close();

    return 0;
}
