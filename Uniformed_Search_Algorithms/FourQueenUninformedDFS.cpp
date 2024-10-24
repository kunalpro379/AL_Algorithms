#include <iostream>
#include <vector>

using namespace std;

class NQueens
{
private:
    int size;                      // Size of the chessboard (4 for 4-Queens)
    vector<vector<int>> solutions; // To store all possible solutions

    // Function to check if it's safe to place a queen at (row, col)
    bool isSafe(const vector<int> &queens, int row, int col)
    {
        for (int i = 0; i < row; i++)
        {
            // Check column and diagonal conflicts
            if (queens[i] == col ||
                queens[i] - i == col - row ||
                queens[i] + i == col + row)
            {
                return false;
            }
        }
        return true;
    }

    // Recursive DFS function to place queens
    void dfs(vector<int> &queens, int row)
    {
        if (row == size)
        {
            solutions.push_back(queens); // Found a solution
            return;
        }

        for (int col = 0; col < size; col++)
        {
            if (isSafe(queens, row, col))
            {
                queens[row] = col;    // Place the queen
                dfs(queens, row + 1); // Move to the next row
                // Backtrack (no need to explicitly remove the queen)
            }
        }
    }

public:
    NQueens(int n) : size(n) {}

    // Function to solve the N-Queens problem using DFS
    void solve()
    {
        vector<int> queens(size, -1); // Initialize queen positions
        dfs(queens, 0);               // Start DFS from the first row
    }

    // Function to print all solutions
    void printSolutions() const
    {
        for (const auto &solution : solutions)
        {
            for (int col : solution)
            {
                for (int i = 0; i < size; i++)
                {
                    if (i == col)
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
            cout << "------" << endl;
        }
    }
};

int main()
{
    int n = 4; // Size of the board (4 for the 4-Queens problem)
    NQueens nQueens(n);

    cout << "Solving " << n << "-Queens problem using Uninformed DFS...\n";
    nQueens.solve();
    nQueens.printSolutions(); // Print all found solutions

    cout << "Finished." << endl;
    return 0;
}
