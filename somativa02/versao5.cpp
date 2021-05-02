#include <bits/stdc++.h>
#include <semaphore.h>
#define N_THREADS 4

using namespace std;

long long n_variables, n_clauses;
const long int MAX{1100000};

sem_t semaforo;

map<long int, vector<long int>> variable_in_clauses;
vector<vector<long int>> clauses;
bitset<MAX> variable;
vector<long int> clause_value;
map<long int, long int> false_variables;
set<long int> false_clauses;

void full();
void resultado();
void flip();

struct comp
{
    bool operator()(const pair<long int, long int> &a, const pair<long int, long int> &b) const
    {
        if (a.second != b.second)
        {
            return a.second > b.second;
        }
        return abs(a.first) > abs(b.first);
    }
};

int main()
{
    sem_init(&semaforo, 0, 1);
    cin >> n_variables >> n_clauses;

    clause_value.assign(n_clauses + 1, 0);
    for (long int i = 0; i < n_clauses; i++)
    {
        long int t;
        vector<long int> temp_clause;
        while (cin >> t, t != 0)
        {
            temp_clause.push_back(t);
            variable_in_clauses[t].push_back(i);
        }
        clauses.push_back(temp_clause);
    }

    string s;
    while (cin >> s)
    {
        if (s == "full")
        {
            variable.reset();
            fill(clause_value.begin(), clause_value.end(), 0);
            false_variables.clear();
            false_clauses.clear();
            full();
        }
        else if (s == "flip")
        {
            flip();
        }
        resultado();
    }

    return 0;
}

void *full_thread_handler(void *arg)
{
    int index = *((int *)arg);
    int blk = (int)clauses.size() / N_THREADS;
    int l = index * blk;
    int r = (index == N_THREADS - 1) ? clauses.size() : l + blk;

    for (int i = l; i < r; i++)
    {
        for (size_t j = 0; j < clauses[i].size(); j++)
        {
            long int t = clauses[i][j];
            if ((t < 0 && variable[abs(t)] == 0) || (t > 0 && variable[t] == 1))
            {
                clause_value[i] += 1;
            }
        }
        if (clause_value[i] == 0)
        {
            sem_wait(&semaforo);
            false_clauses.insert(i);
            for (size_t j = 0; j < clauses[i].size(); j++)
            {
                long int t = clauses[i][j];
                false_variables[t]++;
            }
            sem_post(&semaforo);
        }
    }
    return NULL;
}

void full()
{

    for (long int i = 0; i < n_variables; i++)
    {
        long int t;
        cin >> t;
        if (t < 0)
        {
            t = abs(t);
            variable[t] = 0;
        }
        else
        {
            variable[t] = 1;
        }
    }

    pthread_t threads[N_THREADS];
    int args[N_THREADS];

    for (int i = 0; i < N_THREADS; i++)
    {
        args[i] = i;
        pthread_create(&threads[i], NULL, full_thread_handler, (void *)&args[i]);
    }
    for (int i = 0; i < N_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
}

void clauses_new_values(vector<long int> &variable_in_clause, bool positive)
{
    long int k = positive ? 1 : -1;
    for (auto &p : variable_in_clause)
    {

        if (clause_value[p] == 0 && k == 1)
        {
            for (size_t i = 0; i < clauses[p].size(); i++)
            {
                long int u = clauses[p][i];
                false_variables[u]--;
                if (false_variables[u] <= 0)
                {
                    false_variables.erase(u);
                }
            }
            false_clauses.erase(p);
        }
        else if (clause_value[p] == 1 && k == -1)
        {
            for (size_t i = 0; i < clauses[p].size(); i++)
            {
                long int u = clauses[p][i];
                false_variables[u]++;
            }
            false_clauses.insert(p);
        }

        clause_value[p] += k;
    }
}

void flip()
{
    long int t;
    cin >> t;
    long int k = abs(t);
    variable[k] = variable[k] ? 0 : 1;

    if (variable[k] == 0)
    {
        clauses_new_values(variable_in_clauses[k], false);
        clauses_new_values(variable_in_clauses[-k], true);
    }
    else
    {
        clauses_new_values(variable_in_clauses[-k], false);
        clauses_new_values(variable_in_clauses[k], true);
    }
}

void resultado()
{

    if (false_clauses.empty())
    {
        cout << "SAT" << endl;
        return;
    }

    cout << "[" << false_clauses.size() << " clausulas falsas]";
    for (auto const &p : false_clauses)
    {
        cout << ' ' << p;
    }
    cout << endl;
    set<pair<long int, long int>, comp> false_variables2(false_variables.begin(), false_variables.end());
    cout << "[lits]";
    for (auto const &p : false_variables2)
    {
        cout << ' ' << p.first;
    }
    cout << endl;
}