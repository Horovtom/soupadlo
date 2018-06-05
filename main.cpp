#include <iostream>
#include <map>
#include <utility>
#include <sstream>
#include <cassert>
#include <queue>

using namespace std;
enum dir {
    UP, LEFT, RIGHT, DOWN
};

class State;

map<string, State*> existingStates;

string shift(string input, dir direction) {
    unsigned int pos = static_cast<int>(input.find('0'));
    unsigned int toPos = pos;
    if (direction == UP) {
        if (pos < 3) return input;
        toPos -= 3;
    } else if (direction == DOWN) {
        if (pos > 5) return input;
        toPos += 3;
    } else if (direction == LEFT) {
        if (pos % 3 == 0) return input;
        toPos -= 1;
    } else {
        if (pos % 3 == 2) return input;
        toPos += 1;
    }
    string output = input;
    output.at(pos) = input.at(toPos);
    output.at(toPos) = '0';
    assert(output.length() == 9);
    return output;
}

string getTable(string the) {
    ostringstream ss("\n");
    for (int i = 0; i < 9; ++i) {
        char ch = the.at(static_cast<unsigned long>(i));
        if (ch != '0')
            ss << ch;
        else cout << " ";
        if (i % 3 == 2) ss << endl;
    }
    ss << endl;
    return ss.str();
}

void printTable(string the) {
    for (int i = 0; i < 9; ++i) {
        char ch = the.at(i);
        if (ch != '0')
            cout << ch;
        else cout << " ";
        if (i % 3 == 2) cout << endl;
    }
    cout << endl;
}

std::queue<State*> toDo;
int maxLevel = -1;
vector<State*> maxLevelNode;
int maxLevelCount = 0;
double cumulDepth = 0;
int howMany = 0;

class State {
public:
    int lowestLevel;
    string current;
    State *leftChild;
    State *upChild;
    State *downChild;
    State *rightChild;
    State* parentState;

    explicit State(string s, int level, State* parent) {
        current = std::move(s);
        lowestLevel = level;
        cumulDepth += lowestLevel;
        if (lowestLevel > maxLevel) {
            maxLevel = lowestLevel;
            maxLevelNode.clear();
            maxLevelNode.emplace_back(this);
            maxLevelCount = 1;
        } else if (lowestLevel == maxLevel) {
            maxLevelCount++;
            maxLevelNode.emplace_back(this);
        }
        parentState = parent;
    }

    void calculateChildren() {
        howMany++;

        string leftShifted = shift(current, LEFT);
        auto it = existingStates.find(leftShifted);
        if (it != existingStates.end()) {
            leftChild = it->second;
        } else {
            leftChild = new State(leftShifted, lowestLevel + 1, this);
            existingStates.insert({leftShifted, leftChild});
            toDo.push(leftChild);
        }

        string rightShifted = shift(current, RIGHT);
        it = existingStates.find(rightShifted);
        if (it != existingStates.end()) {
            rightChild = it->second;
        } else {
            rightChild = new State(rightShifted, lowestLevel + 1, this);
            existingStates.insert({rightShifted, rightChild});
            toDo.push(rightChild);
        }

        string upShifted = shift(current, UP);
        it = existingStates.find(upShifted);
        if (it != existingStates.end()) {
            upChild = it->second;
        } else {
            upChild = new State(upShifted, lowestLevel + 1, this);
            existingStates.insert({upShifted, upChild});
            toDo.push(upChild);
        }

        string downShifted = shift(current, DOWN);
        it = existingStates.find(downShifted);
        if (it != existingStates.end()) {
            downChild = it->second;
        } else {
            downChild = new State(downShifted, lowestLevel + 1, this);
            existingStates.insert({downShifted, downChild});
            toDo.push(downChild);
        }
    }
};


int main() {
    State sorted("123456780", 0, nullptr);
    toDo.push(&sorted);
    while(!toDo.empty()) {
        toDo.front()->calculateChildren();
        toDo.pop();
    }

    cout << "There are: " << howMany << " possible states." << endl;
    cout << "Average depth was: " << cumulDepth / howMany << endl;
    cout << "Max level: " << maxLevel << endl;
    cout << "Max level count: " << maxLevelCount << endl;
    cout << "Max level node: " << endl;
    State* curr = maxLevelNode.at(0);

    printTable(curr->current);

    cout << "Path: "<< endl;
    while(curr != nullptr) {
        printTable(curr->current);
        curr = curr->parentState;
    }

    cout << "Other starting positions: " << endl;
    for (int i = 1; i < maxLevelNode.size(); ++i) {
        printTable(maxLevelNode.at(i)->current);
    }

    return 0;
}