#include <iostream>
#include <deque>
#include <cstring>

using namespace std;

#define MAXN 200005

// Represents the number of people of that specific type in the party
typedef pair<int, long long int> pil;

typedef struct operation {
    string direction;
    long long int people;
    int type;
    long long int persuasion;
}Operation;

long long int result[MAXN] = {};

// inline void printResult(int n) {
//     for(int i = 0; i < n; ++i)
//         cout << result[i] << " ";
//    cout << endl;
// }

void printDeque(deque<pil> &party) {
    for(auto i : party)
        cout << "(" << i.first + 1 << ", " << i.second << ")" << " ";
    cout << endl;
}

void insertFront(deque<pil> &party, long long int &people, int &type, long long int &persuasion, int factor) {
    // The new group that will be appended
    pil newGroup = make_pair(type, people + persuasion);

    while(persuasion > 0) {
        // Substract from current last group
        if(party.front().second > persuasion) {
            party.front().second -= persuasion;
            result[party.front().first] -= persuasion * factor;

            persuasion = 0;
        }
        // Subtract persuasion and pop last group
        else {
            persuasion -= party.front().second;

            result[party.front().first] -= party.front().second * factor;

            party.pop_front();
        }
    }

    // Add new group
    party.push_front(newGroup);
}

void insertBack(deque<pil> &party, long long int &people, int &type, long long int &persuasion, int factor) {
    // The new group that will be appended
    pil newGroup = make_pair(type, people + persuasion);

    while(persuasion > 0) {
        // Substract from current last group
        if(party.back().second > persuasion) {
            party.back().second -= persuasion;
            result[party.back().first] -= persuasion * factor;

            persuasion = 0;
        }
        // Subtract persuasion and pop last group
        else {
            persuasion -= party.back().second;

            result[party.back().first] -= party.back().second * factor;

            party.pop_back();
        }
    }

    // Add new group
    party.push_back(newGroup);
}


int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // Current situation of the party
    deque<pil> party;
    Operation operations[MAXN];

    // Number of types
    int N;
    // Number of groups
    int Q;

    cin >> N >> Q;

    string direction;
    int type;
    long long int people, persuasion;

    // Read in different groups
    for(int i = 0; i < Q; ++i) {

        cin >> direction >> people >> type >> persuasion;

        result[type - 1] += (people + persuasion) * (Q - i);

        operations[i].direction = direction;
        operations[i].people = people;
        operations[i].type = type - 1;
        operations[i].persuasion = persuasion;
    }

    // Read in different groups
    for(int i = 0; i < Q; ++i) {

        if(operations[i].direction == "front")
            insertFront(party, operations[i].people, operations[i].type, operations[i].persuasion, Q - i);
        else if(operations[i].direction == "back") {
            insertBack(party, operations[i].people, operations[i].type, operations[i].persuasion, Q - i);
        }
        else {
            cout << "Unknown direction" << endl;
            exit(-1);
        }

        #ifdef DEBUG
        printDeque(party);
        printResult(N);
        #endif
    }

    // Print result
    for(int i = 0; i < N; ++i)
        cout << result[i] << " ";
    cout << endl;


    return 0;

}

