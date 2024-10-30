#include <iostream>
#include <vector>


using namespace std;

int find(vector<string> arr, string elem)
{
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == elem)
            return i;
    }
    return -1;
}

string isValid(string line, string startState, int startRow, vector<string> validStates,
               vector<string> states, vector<vector<string>> tf)
{

    if (line == "_") {
        if (find(validStates, startState) != -1) { return "A"; }
        return "R";
    }
    string currentState = startState;
    int currentRow = startRow, currentColumn;

    for (int i = 0; i < line.size(); i++) {
        currentColumn = find(tf[currentRow], line.substr(i, 1));
        if (currentColumn == -1) {
            return "R";
        }
        currentRow = currentColumn;
        currentState = states[currentRow];
    }

    if (find(validStates, currentState) != -1) {
        return "A";
    }
    return "R";
}

int main()
{
    int q, s, f, n;
    cin >> q >> s >> f >> n;
    vector<string> states(q);
    for (int i = 0; i < q; i++) {
        cin >> states[i];
    }
    vector<string> alph(s);
    for (int i = 0; i < s; i++) {
        cin >> alph[i];
    }
    vector<vector<string>> tf(q, vector<string>(q)); // transition function matrix stateXstate

    for (int i = 0; i < q * q; i++) {
        string s1, s2, op;
        cin >> s1 >> op >> s2;
        tf[find(states, s1)][find(states, s2)] = op;
    }

    string startingState;
    cin >> startingState;
    vector<string> finalStates(f);
    for (int i = 0; i < f; i++) {
        cin >> finalStates[i];
    }

    for (int i = 0; i < n; i++) {
        string line;
        cin >> line;
        cout << isValid(line, startingState, find(states, startingState),
                        finalStates, states, tf) << " ";

    }


//    for(int i = 0; i<q; i++){
//        cout<<endl;
//        for(int j = 0; j<q; j++){
//            cout<<tf[i][j]<<" ";
//        }
//    }

    return 0;
}