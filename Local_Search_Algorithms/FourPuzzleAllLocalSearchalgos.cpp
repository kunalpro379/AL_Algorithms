#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <unordered_set>
#include <string>

using namespace std;

struct State
{
    vector<vector<int>> board;
    int gCost;               // Path cost (number of moves)
    int hCost;               // Heuristic (Manhattan distance)
    int fCost;               // f(n) = g(n) + h(n)
    pair<int, int> blankPos; // Position of blank (0)
    State *parent;           // Pointer to the parent state

    State(vector<vector<int>> b, int g, int h, pair<int, int> pos, State *p = nullptr)
        : board(b), gCost(g), hCost(h), blankPos(pos), parent(p)
    {
        fCost = g + h;
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

// Calculate Manhattan distance for the current board
int calculateManhattanDistance(const vector<vector<int>> &board)
{
    int distance = 0;
    int goal[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}}; // Goal state

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] != 0)
            {
                int value = board[i][j];
                int goalRow = (value - 1) / 3;
                int goalCol = (value - 1) % 3;
                distance += abs(i - goalRow) + abs(j - goalCol);
            }
        }
    }
    return distance;
}

// Check if the current state is the goal state
bool isGoalState(const vector<vector<int>> &board)
{
    int goal[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}}; // Goal state
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] != goal[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

// Generate successor states
vector<State> generateSuccessors(State currentState)
{
    vector<State> successors;
    int row = currentState.blankPos.first;
    int col = currentState.blankPos.second;

    // Possible moves: up, down, left, right
    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (const auto &direction : directions)
    {
        int dRow = direction.first;
        int dCol = direction.second;

        int newRow = row + dRow;
        int newCol = col + dCol;

        // Check if the new position is valid
        if (newRow >= 0 && newRow < 3 && newCol >= 0 && newCol < 3)
        {
            // Create a new board state
            vector<vector<int>> newBoard = currentState.board;
            swap(newBoard[row][col], newBoard[newRow][newCol]); // Move the blank tile

            // Calculate the new heuristic cost
            int newHCost = calculateManhattanDistance(newBoard);
            successors.emplace_back(newBoard, currentState.gCost + 1, newHCost, make_pair(newRow, newCol), new State(currentState)); // Set parent
        }
    }
    return successors;
}

// Function to print the path from the initial state to the goal state
void printSolution(State *state)
{
    vector<vector<vector<int>>> path; // To store the path of boards
    while (state != nullptr)
    {
        path.push_back(state->board);
        state = state->parent;
    }

    // Print the path in reverse order
    for (auto it = path.rbegin(); it != path.rend(); ++it)
    {
        for (const auto &row : *it)
        {
            for (int num : row)
            {
                cout << num << " ";
            }
            cout << endl;
        }
        cout << "-----" << endl; // Separator between states
    }
}

// A* Search algorithm for the 8-Puzzle problem
void aStarSearchFor8Puzzle(const vector<vector<int>> &initialBoard)
{
    priority_queue<State, vector<State>, CompareState> pq; // Min-heap
    unordered_set<string> visited;                         // To track visited states

    pair<int, int> blankPos;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (initialBoard[i][j] == 0)
            {
                blankPos = {i, j}; // Find the position of the blank
            }
        }
    }

    int initialHCost = calculateManhattanDistance(initialBoard);
    pq.emplace(initialBoard, 0, initialHCost, blankPos); // Push initial state into the priority queue

    while (!pq.empty())
    {
        State currentState = pq.top(); // Get the state with the lowest fCost
        pq.pop();

        // Check if the current state is the goal state
        if (isGoalState(currentState.board))
        {
            cout << "Solution found!" << endl;
            printSolution(new State(currentState)); // Print the solution path
            return;
        }

        // Serialize the board state for visited checking
        string stateString;
        for (const auto &row : currentState.board)
        {
            for (int num : row)
            {
                stateString += to_string(num) + ",";
            }
        }

        // Skip if we've already visited this state
        if (visited.find(stateString) != visited.end())
        {
            continue;
        }
        visited.insert(stateString);

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
    vector<vector<int>> initialBoard = {
        {1, 2, 3},
        {4, 0, 6},
        {7, 5, 8}};

    cout << "A* Search for 8-Puzzle:" << endl;
    aStarSearchFor8Puzzle(initialBoard);
    return 0;
}
