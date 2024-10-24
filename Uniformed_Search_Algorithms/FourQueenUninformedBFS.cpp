#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Structure to represent the state of the board
struct State
{
    vector<int> queens; // Positions of queens in rows
    int row;            // Current row to place the next queen

    State(vector<int> q, int r) : queens(q), row(r) {}
};

// Function to check if placing a queen at (row, col) is safe
bool isSafe(const vector<int> &queens, int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        // Check for column and diagonal conflicts
        if (queens[i] == col ||
            queens[i] - i == col - row ||
            queens[i] + i == col + row)
        {
            return false;
        }
    }
    return true;
}

// Function to solve the N-Queens problem using Uninformed BFS
void bfs(int n)
{
    queue<State> q;                   // Queue to hold the states
    vector<int> initialQueens(n, -1); // Initialize queen positions
    q.push(State(initialQueens, 0));  // Start with an empty board

    while (!q.empty())
    {
        State currentState = q.front(); // Get the current state
        q.pop();

        // If all queens are placed, print the solution
        if (currentState.row == n)
        {
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    if (currentState.queens[i] == j)
                    {
                        cout << "Q ";
                    }
                    else
                    {
                        cout << ". ";
                    }
                }
                cout << endl;
            }
            cout << "------" << endl; // Separator between solutions
            continue;                 // Go to the next state
        }

        // Generate successors for the current state
        for (int col = 0; col < n; col++)
        {
            if (isSafe(currentState.queens, currentState.row, col))
            {
                vector<int> newQueens = currentState.queens;
                newQueens[currentState.row] = col;              // Place the queen
                q.push(State(newQueens, currentState.row + 1)); // Move to the next row
            }
        }
    }
}

int main()
{
    int n = 4; // Size of the board (4 for the 4-Queens problem)
    cout << "Solving " << n << "-Queens problem using Uninformed BFS...\n";
    bfs(n); // Start the BFS to solve the problem
    cout << "Finished." << endl;
    return 0;
}
