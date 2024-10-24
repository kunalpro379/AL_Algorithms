#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;

struct State
{
    vector<int> queens; // queens[i] represents the row of the queen in column i
    int heuristicCost;  // Heuristic cost: number of pairs of queens that attack each other
    int gCost;          // Path cost: number of steps taken
    int fCost;          // f(n) = g(n) + h(n)

    State(vector<int> q, int g, int h)
    {
        this->queens = q;
        this->gCost = g;
        this->heuristicCost = h;
        this->fCost = g + h;
    }
};

// Comparator for the priority queue
struct CompareState
{
    bool operator()(const State &a, const State &b)
    {
        return a.fCost > b.fCost; // min-heap based on fCost
    }
};

// Calculate the heuristic cost (number of attacking pairs)
int calculateHeuristicCost(const vector<int> &queens)
{
    int n = queens.size();
    int attacks = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            // Check if queens[i] and queens[j] attack each other
            if (queens[i] == queens[j] || abs(queens[i] - queens[j]) == abs(i - j))
            {
                attacks++;
            }
        }
    }
    return attacks;
}

// Generate successor states by placing a queen in the next column
vector<State> generateSuccessors(const State &currentState)
{
    vector<State> successors;
    int n = currentState.queens.size();
    int column = currentState.gCost; // Next column to place a queen

    for (int row = 0; row < n; row++)
    {
        vector<int> newQueens = currentState.queens;
        newQueens[column] = row; // Place a queen in the new position

        int newHCost = calculateHeuristicCost(newQueens);
        successors.emplace_back(newQueens, column + 1, newHCost);
    }
    return successors;
}

// A* Search algorithm for the N-Queens problem
void aStarSearchForNQueens(int n)
{
    priority_queue<State, vector<State>, CompareState> pq; // Min-heap
    vector<int> initialQueens(n, -1);                      // Start with -1 (no queens placed)
    int initialHCost = calculateHeuristicCost(initialQueens);

    pq.emplace(initialQueens, 0, initialHCost); // Push initial state into the priority queue

    while (!pq.empty())
    {
        State currentState = pq.top(); // Get the state with the lowest fCost
        pq.pop();

        // If all queens are placed, print the solution
        if (currentState.gCost == n)
        {
            cout << "Solution found: ";
            for (int i = 0; i < n; i++)
            {
                cout << "(" << i << ", " << currentState.queens[i] << ") ";
            }
            cout << endl;
            return;
        }

        // Generate and process successors
        vector<State> successors = generateSuccessors(currentState);
        for (const State &successor : successors)
        {
            pq.push(successor); // Push successor states into the priority queue
        }
    }

    cout << "No solution found." << endl; // If the queue is exhausted
}

int main()
{
    int n = 4; // Change n to solve for a different number of queens
    cout << "A* Search for " << n << "-Queens:" << endl;
    aStarSearchForNQueens(n);
    return 0;
}
