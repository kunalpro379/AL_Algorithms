#include <iostream>
#include <queue>
#include <cmath>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

// States
struct State
{
    vector<int> queens; // positions of queens in rows
    int heuristicCost;

    // Constructor
    State(vector<int> q, int c)
    {
        this->queens = q;
        this->heuristicCost = c;
    }

    // Operator overload for priority_queue (min-heap behavior)
    bool operator<(const State &other) const
    {
        return this->heuristicCost > other.heuristicCost; // Min-heap
    }
};

// Calculate heuristic cost based on conflicts
int calculateHeuristicCost(const vector<int> &queens)
{
    int conflicts = 0;
    int n = queens.size();
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            // Same column or diagonal
            if (queens[i] == queens[j] || abs(queens[i] - queens[j]) == abs(i - j)) // heuristic functions
                conflicts++;
        }
    }
    return conflicts;
}

// Move queens in every configuration and calculate heuristic cost
vector<State> generateSuccessors(const State &currentState)
{
    vector<State> successors;
    int n = currentState.queens.size();
    for (int col = 0; col < n; col++)
    {
        for (int row = 0; row < n; row++)
        {
            if (currentState.queens[col] != row)
            { // Change position
                vector<int> newQueens = currentState.queens;
                newQueens[col] = row;
                int newHeuristicCost = calculateHeuristicCost(newQueens);
                successors.push_back(State(newQueens, newHeuristicCost));
            }
        }
    }
    return successors; // Return generated successors
}

// Informed BFS for N-Queens problem
void informedBFS(int n)
{
    priority_queue<State> pq;
    vector<int> initialQueens(n, 0); // All queens at row 0
    pq.push(State(initialQueens, calculateHeuristicCost(initialQueens)));

    while (!pq.empty())
    {
        State currentState = pq.top();
        pq.pop();

        // Check if goal state
        if (currentState.heuristicCost == 0)
        {
            cout << "Goal state found:" << endl;
            for (int i = 0; i < n; i++)
                cout << currentState.queens[i] << " "; // Output the solution
            cout << endl;
            return;
        }

        // Generate successors
        vector<State> successors = generateSuccessors(currentState);
        for (State &successor : successors)
            pq.push(successor); // Push successor states into the priority queue
    }
    cout << "No solution found." << endl;
}

int main()
{
    int n = 8; // Example for 8-Queens
    cout << "Solving " << n << "-Queens problem using informed BFS..." << endl;
    informedBFS(n);
    cout << "Finished..." << endl;
    return 0;
}
