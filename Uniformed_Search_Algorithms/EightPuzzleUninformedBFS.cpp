#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <string>

using namespace std;

struct State
{
    vector<vector<int>> board;
    int zeroX, zeroY;
    string path;

    State(vector<vector<int>> b, int x, int y, string p)
        : board(b), zeroX(x), zeroY(y), path(p) {}

    bool isGoal() const
    {
        return board == vector<vector<int>>{{1, 2, 3},
                                            {4, 5, 6},
                                            {7, 8, 0}};
    }

    void print() const
    {
        cout << "\nFinal state:\n";
        for (const auto &row : board)
        {
            for (int val : row)
            {
                cout << val << " ";
            }
            cout << endl;
        }
        cout << "\nSolution path: " << path;
        cout << "\nNumber of moves: " << (path.empty() ? 0 : count(path.begin(), path.end(), ' ') + 1) << endl;
    }
};

string boardToString(const vector<vector<int>> &board)
{
    string result;
    for (const auto &row : board)
    {
        for (int val : row)
        {
            result += to_string(val);
        }
    }
    return result;
}

vector<State> getSuccessors(const State &current)
{
    vector<State> successors;
    vector<pair<int, int>> moves = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}; // Down, Up, Right, Left
    vector<string> moveNames = {"D", "U", "R", "L"};

    for (int i = 0; i < moves.size(); i++)
    {
        int newX = current.zeroX + moves[i].first;
        int newY = current.zeroY + moves[i].second;

        if (newX >= 0 && newX < 3 && newY >= 0 && newY < 3)
        {
            vector<vector<int>> newBoard = current.board;
            swap(newBoard[current.zeroX][current.zeroY],
                 newBoard[newX][newY]);

            string newPath = current.path;
            if (!newPath.empty())
                newPath += " ";
            newPath += moveNames[i];

            successors.push_back(State(newBoard, newX, newY, newPath));
        }
    }
    return successors;
}

bool bfs(const State &initialState)
{
    queue<State> q;
    set<string> visited;

    q.push(initialState);
    visited.insert(boardToString(initialState.board));

    int statesExplored = 0;

    while (!q.empty())
    {
        State current = q.front();
        q.pop();
        statesExplored++;

        // Debug print current state
        cout << "Exploring state:\n";
        for (const auto &row : current.board)
        {
            for (int val : row)
            {
                cout << val << " ";
            }
            cout << endl;
        }
        cout << "Path so far: " << current.path << "\n\n";

        if (current.isGoal())
        {
            current.print();
            cout << "States explored: " << statesExplored << endl;
            return true;
        }

        vector<State> successors = getSuccessors(current);
        for (const State &successor : successors)
        {
            string boardStr = boardToString(successor.board);
            if (visited.find(boardStr) == visited.end())
            {
                visited.insert(boardStr);
                q.push(successor);
            }
        }
    }

    return false;
}

int main()
{
    vector<vector<int>> initialBoard = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 0, 8}};

    // Empty tile is at (2, 1)
    State initialState(initialBoard, 2, 1, "");

    cout << "Initial state:\n";
    for (const auto &row : initialBoard)
    {
        for (int val : row)
        {
            cout << val << " ";
        }
        cout << endl;
    }
    cout << "\nSearching for solution using BFS...\n\n";

    if (!bfs(initialState))
    {
        cout << "No solution found." << endl;
    }

    return 0;
}