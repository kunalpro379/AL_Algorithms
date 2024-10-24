#include <iostream>
#include <vector>
#include <queue>
#include <set>
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
    for (size_t i = 0; i < moves.size(); ++i)
    {
        int dx = moves[i].first;
        int dy = moves[i].second;

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

// BFS Algorithm
void bfs(const State &initialState)
{
    queue<State> q;
    set<vector<vector<int>>> visited; // To keep track of visited states
    q.push(initialState);
    visited.insert(initialState.board);

    while (!q.empty())
    {
        State current = q.front();
        q.pop();

        // Check if we reached the goal state
        if (current.isGoal())
        {
            current.print();
            return;
        }

        // Generate successors and add them to the queue
        vector<State> successors = getSuccessors(current);
        for (const State &successor : successors)
        {
            if (visited.find(successor.board) == visited.end())
            { // Check if not visited
                visited.insert(successor.board);
                q.push(successor);
            }
        }
    }

    cout << "No solution found." << endl;
}

int main()
{
    // Initial configuration of the 8-puzzle
    vector<vector<int>> initialBoard = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 0, 8} // 0 represents the empty space
    };

    // Create the initial state
    State initialState(initialBoard, 2, 1, ""); // zeroX = 2, zeroY = 1
    cout << "Starting BFS for 8-puzzle...\n";
    bfs(initialState);
    cout << "Finished." << endl;

    return 0;
}
