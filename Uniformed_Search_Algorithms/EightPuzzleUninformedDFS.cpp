#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <algorithm>

using namespace std;

struct State
{
    vector<vector<int>> board; // 3x3 board representation
    int zeroX, zeroY;          // Position of the empty tile (0)
    string path;               // Path taken to reach this state

    State(vector<vector<int>> b, int x, int y, string p) : board(b), zeroX(x), zeroY(y), path(p) {}

    // Check if this state is the goal state
    bool isGoal() const
    {
        return board == vector<vector<int>>{{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};
    }

    // Print the board
    void print() const
    {
        for (const auto &row : board)
        {
            for (int val : row)
            {
                cout << val << " ";
            }
            cout << endl;
        }
        cout << "Path: " << path << endl;
    }
};

// Function to get all possible moves from the current state
vector<State> getSuccessors(const State &current)
{
    vector<State> successors;
    vector<pair<int, int>> moves = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}; // Down, Up, Right, Left
    for (const auto &move : moves)                                     // Use traditional loop
    {
        int dx = move.first;
        int dy = move.second;

        int newX = current.zeroX + dx;
        int newY = current.zeroY + dy;

        // Check if the new position is within bounds
        if (newX >= 0 && newX < 3 && newY >= 0 && newY < 3)
        {
            // Create a new board configuration
            vector<vector<int>> newBoard = current.board;
            swap(newBoard[current.zeroX][current.zeroY], newBoard[newX][newY]);
            string newPath = current.path + (newPath.empty() ? "" : " ") + to_string(newBoard[newX][newY]);
            successors.emplace_back(newBoard, newX, newY, newPath);
        }
    }
    return successors;
}

// DFS Algorithm
void dfs(const State &initialState)
{
    stack<State> s;
    set<vector<vector<int>>> visited; // To keep track of visited states
    s.push(initialState);
    visited.insert(initialState.board);

    while (!s.empty())
    {
        State current = s.top();
        s.pop();

        // Check if we reached the goal state
        if (current.isGoal())
        {
            current.print();
            return;
        }

        // Generate successors and add them to the stack
        vector<State> successors = getSuccessors(current);
        for (const State &successor : successors)
        {
            if (visited.find(successor.board) == visited.end())
            { // Check if not visited
                visited.insert(successor.board);
                s.push(successor);
            }
        }
    }

    cout << "No solution found." << endl;
}

int main()
{
    // Updated initial configuration of the 8-puzzle
    vector<vector<int>> initialBoard = {
        {1, 2, 3},
        {4, 0, 6},
        {5, 7, 8} // 0 represents the empty space
    };

    // Create the initial state
    State initialState(initialBoard, 1, 1, ""); // zeroX = 1, zeroY = 1 (position of 0)
    cout << "Starting DFS for 8-puzzle...\n";
    dfs(initialState);
    cout << "Finished." << endl;

    return 0;
}
