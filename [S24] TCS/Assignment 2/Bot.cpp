#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

class Transition {
public:
    int from;
    string letter;
    int to;

    Transition(int f, string l, int t) : from(f), letter(l), to(t) {}

    bool operator==(const Transition& other) const {
        return from == other.from && letter == other.letter && to == other.to;
    }

};

void FSAToRegExp(const vector<string>& states, const vector<string>& alphabet, const vector<Transition>& transitions, const int& initial, const vector<int>& accepting) {
    int n = states.size();
    string arr[n+1][n][n];

    // k = 0
    for (const Transition& tr : transitions) {
        if (arr[0][tr.from][tr.to].length() > 0) {
            arr[0][tr.from][tr.to] += "|";
        }
        arr[0][tr.from][tr.to] += tr.letter;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                if (arr[0][i][j].length() > 0) {
                    arr[0][i][i] += "|";
                }
                arr[0][i][i] += "eps";
            } else if (arr[0][i][j].length() == 0) {
                arr[0][i][j] = "{}";
            }
        }
    }

    // k > 0
    for (int k = 1; k < n + 1; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                arr[k][i][j] = "(" + arr[k-1][i][k - 1] + ")(" + arr[k-1][k - 1][k - 1] + ")*(" + arr[k-1][k - 1][j] + ")|(" + arr[k-1][i][j] + ")";
            }
        }
    }

    bool next = false;
    for (const int& fin : accepting) {
        if (next) {
            cout << "|";
        }
        cout << "(" << arr[n][initial][fin] << ")";
        next = true;
    }
}

template <typename T>
bool addUnique(vector<T>& lst, const T& elem) {
    for (int i = 0; i < lst.size(); i++) {
        if (lst[i] == elem) {
            return true;
        }
    }
    lst.push_back(elem);
    return false;
}

bool readType(string& type) {
    getline(cin, type);
    if (type != "type=[non-deterministic]" & type != "type=[deterministic]") {
        cout << "E1: Input file is malformed" << endl;
        return true;
    }
    return false;
}

bool readStates(vector<string>& states) {
    string str;
    getline(cin, str);

    auto start = str.find('[') + 1;
    auto end = str.find(']');

    if (str.substr(0, start) != "states=[" || end != (str.length() - 1)) {
        cout << "E1: Input file is malformed" << endl; // test 33
        return true;
    }
    str = str.substr(start, end - start);

    if (str.size() == 0) {
        cout << "E1: Input file is malformed" << endl; // tests 21 and 31
        return true;
    }

    if (str[str.size() - 1] == ',') {
        cout << "E1: Input file is malformed" << endl; // теста нет
        return true;
    }

    stringstream ss(str);
    string temp;
    while (getline(ss, temp, ',')) {
        if (temp == "") {
            cout << "E1: Input file is malformed" << endl; // теста нет
            return true;
        } else {
            addUnique(states, temp);
        }
    }
    return false;
}

bool readAlphabet(vector<string>& alphabet) {
    string str;
    getline(cin, str);

    auto start = str.find('[') + 1;
    auto end = str.find(']');
    if (str.substr(0, start) != "alphabet=[" || end != (str.length() - 1)) {
        cout << "E1: Input file is malformed" << endl; // теста нет
        return true;
    }
    str = str.substr(start, end - start);

    if (str.size() == 0) {
        cout << "E1: Input file is malformed" << endl; // test 32
        return true;
    }

    if (str[str.size() - 1] == ',') {
        cout << "E1: Input file is malformed" << endl; // test 40
        return true;
    }

    stringstream ss(str);
    string temp;
    while (getline(ss, temp, ',')) {
        if (temp == "") {
            cout << "E1: Input file is malformed" << endl; // теста нет
            return true;
        } else {
            addUnique(alphabet, temp);
        }
    }
    return false;
}

bool readInitial(const vector<string>& states, int& initial) {
    string str;
    getline(cin, str);

    auto start = str.find('[') + 1;
    auto end = str.find(']');
    if (str.substr(0, start) != "initial=[" || end != (str.length() - 1)) {
        cout << "E1: Input file is malformed" << endl; // теста нет
        return true;
    }
    str = str.substr(start, end - start);

    if (str[str.size() - 1] == ',') {
        cout << "E1: Input file is malformed" << endl; // теста нет
        return true;
    }

    if (str.size() == 0) {
        cout << "E2: Initial state is not defined" << endl; // tests 20 and 29
        return true;
    }

    for (int i = 0; i < states.size(); i++) {
        if (states[i] == str) {
            initial = i;
            return false;
        }
    }

    cout << "E4: A state '" << str << "' is not in the set of states" << endl; // test 22
    return true;
}

bool readAccepting(const vector<string>& states, vector<int>& accepting) {
    string str;
    getline(cin, str);

    auto start = str.find('[') + 1;
    auto end = str.find(']');
    if (str.substr(0, start) != "accepting=[" || end != (str.length() - 1)) {
        cout << "E1: Input file is malformed" << endl; // test 19
        return true;
    }
    str = str.substr(start, end - start);

    if (str.size() == 0) {
        cout << "E3: Set of accepting states is empty" << endl; // test 5 and 28
        return true;
    }

    if (str[str.size() - 1] == ',') {
        cout << "E1: Input file is malformed" << endl; // нет тестов
        return true;
    }

    stringstream ss(str);
    string temp;
    while (getline(ss, temp, ',')) {
        if (temp == "") {
            cout << "E1: Input file is malformed" << endl; // нет тестов
            return true;
        }
        bool found = false;
        for (int i = 0; i < states.size(); i++) {
            if (states[i] == temp) {
                addUnique(accepting, i);
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "E4: A state '" << temp << "' is not in the set of states" << endl; // нет тестов
            return true;
        }
    }
    return false;
}

bool readTransitions(const vector<string>& states, const vector<string>& alphabet, vector<Transition>& tr) {
    string str;
    getline(cin, str);

    auto start = str.find('[') + 1;
    auto end = str.find(']');
    if (str.substr(0, start) != "transitions=[" || end != (str.length() - 1)) {
        cout << "E1: Input file is malformed" << endl; // нет тестов
        return true;
    }
    str = str.substr(start, end - start);

    if (str.size() == 0) {
        return false;
    }

    if (str[str.size() - 1] == ',') {
        cout << "E1: Input file is malformed" << endl; // нет тестов
        return true;
    }

    stringstream ss1(str);
    string temp1;
    while (getline(ss1, temp1, ',')) {
        stringstream ss(temp1);
        string temp;

        int from = -1;
        getline(ss, temp, '>');
        if (temp == "") {
            cout << "E1: Input file is malformed" << endl; // нет тестов
            return true;
        }
        for (int i = 0; i < states.size(); i++) {
            if (states[i] == temp) {
                from = i;
                break;
            }
        }
        if (from == -1) {
            cout << "E4: A state '" << temp << "' is not in the set of states" << endl; // нет тестов
            return true;
        }

        getline(ss, temp, '>');
        string letter = temp;
        if (temp == "") {
            cout << "E1: Input file is malformed" << endl; // test 37
            return true;
        }
        bool found = false;
        for (int i = 0; i < alphabet.size(); i++) {
            if (alphabet[i] == temp) {
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "E5: A transition '" << temp << "' is not represented in the alphabet" << endl; // test 18 and 30
            return true;
        }

        int to = -1;
        getline(ss, temp, '>');
        if (temp == "") {
            cout << "E1: Input file is malformed" << endl; // нет тестов
            return true;
        }
        for (int i = 0; i < states.size(); i++) {
            if (states[i] == temp) {
                to = i;
                break;
            }
        }
        if (to == -1) {
            cout << "E4: A state '" << temp << "' is not in the set of states" << endl; // нет тестов
            return true;
        }

        if (addUnique(tr, Transition(from, letter, to))) {
            cout << "E1: Input file is malformed" << endl; // test 27
            return true;
        }
    }

    return false;
}

bool checkDisjoint(const vector<string>& states, const vector<Transition>& transitions, const int& initial) {
    set<int> canVisit;
    canVisit.insert(initial);

    int num = canVisit.size();
    while (true) {
        for (const Transition& t : transitions) {
            if (t.from == initial) {
                canVisit.insert(t.to);
            } else if (canVisit.find(t.from) != canVisit.end()) {
                canVisit.insert(t.to);
            }
        }
        if (canVisit.size() == num) {
            break;
        }
        num = canVisit.size();
    }

    if (canVisit.size() < states.size()) {
        cout << "E6: Some states are disjoint" << endl; // test 1
        return true;
    }
    return false;
}

bool checkDet(const vector<string>& states, const vector<Transition>& transitions) {
    map<int, set<string>> ways;
    for (const Transition& t : transitions) {
        if (ways[t.from].find(t.letter) != ways[t.from].end()) {
            cout << "E7: FSA is non-deterministic" << endl; // test 6
            return true;
        }
        ways[t.from].insert(t.letter);
    }
    return false;
}

int main() {

    string type;
    if (readType(type)) return 0;

    vector<string> states;
    if (readStates(states)) return 0;

    vector<string> alphabet;
    if (readAlphabet(alphabet)) return 0;

    int initial;
    if (readInitial(states, initial)) return 0;

    vector<int> accepting;
    if (readAccepting(states, accepting)) return 0;
    sort(accepting.begin(), accepting.end());

    vector<Transition> transitions;
    if (readTransitions(states, alphabet, transitions)) return 0;

    if (checkDisjoint(states, transitions, initial)) return 0;
    if (type == "type=[deterministic]" && checkDet(states, transitions)) return 0;

    FSAToRegExp(states, alphabet, transitions, initial, accepting);
}
