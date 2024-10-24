#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <algorithm>
#include <string>

using namespace std;

struct State
{
    vector<vector<int>> board; // 3x3 board representation
    int zeroX, zeroY;          // Position of the empty tile (0)
    string path;               // Path taken to reach this state

    State(vector<vector<int>> b, int x, int y, string p)
        : board(b), zeroX(x), zeroY(y), path(p) {}

    // Check if this state is the goal state
    bool isGoal() const
    {
        return board == vector<vector<int>>{{1, 2, 3},
                                            {4, 5, 6},
                                            {7, 8, 0}};
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

// Function to convert board to string for visited set
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

// Function to get move direction as string
string getMoveDirection(int dx, int dy)
{
    if (dx == 1)
        return "D";
    if (dx == -1)
        return "U";
    if (dy == 1)
        return "R";
    return "L";
}

// Function to get all possible moves from the current state
vector<State> getSuccessors(const State &current)
{
    vector<State> successors;
    // Down, Up, Right, Left
    vector<pair<int, int>> moves = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    for (const auto &move : moves)
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
            swap(newBoard[current.zeroX][current.zeroY],
                 newBoard[newX][newY]);

            // Add move direction to path
            string newPath = current.path;
            if (!newPath.empty())
                newPath += " ";
            newPath += getMoveDirection(dx, dy);

            successors.emplace_back(newBoard, newX, newY, newPath);
        }
    }
    return successors;
}

// DFS Algorithm
bool dfs(const State &initialState)
{
    stack<State> s;
    set<string> visited; // To keep track of visited states using string representation

    s.push(initialState);
    visited.insert(boardToString(initialState.board));

    while (!s.empty())
    {
        State current = s.top();
        s.pop();

        // Check if we reached the goal state
        if (current.isGoal())
        {
            cout << "Solution found!\n";
            current.print();
            return true;
        }

        // Generate successors and add them to the stack
        vector<State> successors = getSuccessors(current);
        for (const State &successor : successors)
        {
            string boardStr = boardToString(successor.board);
            if (visited.find(boardStr) == visited.end())
            {
                visited.insert(boardStr);
                s.push(successor);
            }
        }
    }

    return false;
}

int main()
{
    // Initial configuration of the 8-puzzle
    vector<vector<int>> initialBoard = {
        {1, 2, 3},
        {4, 5, 6},
        {0, 7, 8} // 0 represents the empty space
    };

    // Create the initial state
    State initialState(initialBoard, 2, 0, "");

    cout << "Initial state:\n";
    initialState.print();
    cout << "\nSearching for solution...\n\n";

    if (!dfs(initialState))
    {
        cout << "No solution found.\n";
    }

    return 0;
}