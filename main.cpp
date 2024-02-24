#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

class State {
private:
    bool final;
    std::map<char, vector<State *>> map;
public:
    explicit State(bool final = false) : final(final) {
    }

    void addMap(char c, State *state) {
        this->map[c].push_back(state);
    }

    bool has(char c) {
        return this->map.contains(c);
    }

    vector<State *> get(char c) {
        return this->map[c];
    }

    bool isFinal() const {
        return this->final;
    }
};

template<class T>
int getIndexFromVector(vector<T> v, T item) {
    const auto it = find(v.begin(), v.end(), item);

    if (it != v.end()) {
        return it - v.begin();
    } else {
        return -1;
    }
}

int main() {
    // INPUT DATA
//    int numberOfStates, numberOfSymbols;
//
//    cout << "Enter number of statesNdka = ";
//    cin >> numberOfStates;
//
//    cout << "Enter number of symbols = ";
//    cin >> numberOfSymbols;
//
//    vector<State *> statesNdka(numberOfStates);
//    vector<char> symbols(numberOfSymbols);
//
//    for (int i = 0; i < numberOfSymbols; i++) {
//        cout << "Enter char[" << i << "] = ";
//        cin >> symbols[i];
//    }
//
//    for (int i = 0; i < numberOfStates; i++) {
//        bool isFinal;
//        cout << "q" << i << " is final = ";
//        cin >> isFinal;
//        statesNdka[i] = new State(isFinal);
//    }
//
//    for (int i = 0; i < numberOfStates; i++) {
//        cout << "Enter states mapping (-1 for break)" << endl;
//        for (int j = 0; j < symbols.size(); j++) {
//            do {
//                int targetState;
//                cout << "q" << i << "(" << symbols[j] << ", ?) = q";
//                cin >> targetState;
//
//                if (targetState == -1) {
//                    break;
//                }
//                statesNdka[i]->addMap(symbols[j], statesNdka[targetState]);
//            } while (true);
//        }
//    }

    vector<State *> statesNdka = {
            new State(false),
            new State(true)
    };
    vector<char> symbols = {
            'H',
            'T'
    };

    statesNdka[0]->addMap('H', statesNdka[0]);
    statesNdka[0]->addMap('T', statesNdka[0]);
    statesNdka[0]->addMap('T', statesNdka[1]);



    ////////// ALGORITHM

    map<string, State *> newStates;
    map<string, map<char, string>> newStatesMap;

    queue<string> q;
    q.emplace("0");
    for (; !q.empty(); q.pop()) {
        const auto item = q.front();
        bool newStateIsFinal = false;

        for (int c = 0; c < symbols.size(); c++) {
            string newState;

            for (int i = 0; i < item.size(); i++) {
                const auto indexInt = item[i] - '0';

                if (statesNdka[indexInt]->isFinal()) {
                    newStateIsFinal = true;
                }

                if (!statesNdka[indexInt]->has(symbols[c])) {
                    continue;
                }
                auto states = statesNdka[indexInt]->get(symbols[c]);

                for (auto z = 0; z < states.size(); z++) {
                    const auto index = to_string(getIndexFromVector(statesNdka, states[z]))[0];

                    if (find(newState.begin(), newState.end(), index) == newState.end()) {
                        newState += index;
                    }
                }
            }

            if (newState.empty()) {
                continue;
            }

            sort(newState.begin(), newState.end());

            if (!newStates.contains(newState)) {
                q.emplace(newState);
            }

            newStatesMap[item][symbols[c]] = newState;
        }

        newStates[item] = new State(newStateIsFinal);
    }

    cout << "==> DKA MAPPING <== " << newStates.size() << endl;
    for (const auto &[stateIndex, state]: newStates) {
        auto map = newStatesMap[stateIndex];


        for (const auto &[symbol, targetStateIndex]: map) {
            cout << "q" << stateIndex << "(" << symbol << ", q" << targetStateIndex << ")" << endl;
        }
    }

    cout << "==> FINAL STATES <==" << endl;
    for (const auto &[stateIndex, state]: newStates) {
        if (state->isFinal()) {
            cout << "q" << stateIndex << endl;
        }
    }

    cout << endl;
    cout << "digraph {" << endl;
    cout << "rankdir=\"LR\"" << endl;

    cout << "{" << endl;
    for (const auto &[stateIndex, state]: newStates) {
        cout << "q" << stateIndex << " [shape=\"" << ((state->isFinal() ? "doublecircle" : "circle")) << "\"]"
             << endl;

    }
    cout << "}" << endl;

    for (const auto &[stateIndex, state]: newStates) {
        auto map = newStatesMap[stateIndex];

        for (const auto &[symbol, targetStateIndex]: map) {
            cout << "q" << stateIndex << " -> " << "q" << targetStateIndex << "[label=\"" << symbol << "\"]" << endl;
        }
    }

    cout << "}";


    return 0;
}
