#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <unordered_set>
#include <string>

using namespace std;

struct State
{
    vector<vector<int>> board; // Current board state
    int g;                     // Cost to reach this state (number of moves)
    int h;                     // Heuristic cost (Manhattan distance)
    pair<int, int> zeroPos;    // Position of the zero (blank space)
    string moveSequence;       // Sequence of moves to reach this state

    // Constructor
    State(vector<vector<int>> b, int g, int h, pair<int, int> pos, string moves)
        : board(b), g(g), h(h), zeroPos(pos), moveSequence(moves) {}

    // Calculate f(n) = g(n) + h(n)
    int f() const
    {
        return g + h;
    }

    // Generate a string representation for the board to use in the unordered_set
    string getBoardString() const
    {
        string result;
        for (const auto &row : board)
        {
            for (int num : row)
            {
                result += to_string(num) + ",";
            }
        }
        return result;
    }

    // Check if two states are equal
    bool operator==(const State &other) const
    {
        return board == other.board;
    }

    // Comparison operator for priority_queue
    bool operator>(const State &other) const
    {
        return f() > other.f(); // We want the state with the smallest f value to come first
    }
};

// Hash function for State to use in unordered_set
namespace std
{
    template <>
    struct hash<State>
    {
        size_t operator()(const State &s) const
        {
            return hash<string>()(s.getBoardString());
        }
    };
}

// Calculate the Manhattan distance
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
                // Find the goal position of each tile
                int value = board[i][j];
                int goalRow = (value - 1) / 3;
                int goalCol = (value - 1) % 3;
                distance += abs(i - goalRow) + abs(j - goalCol);
            }
        }
    }
    return distance;
}

// Generate successors (next states)
vector<State> generateSuccessors(const State &currentState)
{
    vector<State> successors;
    int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}; // Right, Down, Left, Up
    char moveNames[4] = {'R', 'D', 'L', 'U'};                  // Move representations

    for (int i = 0; i < 4; i++)
    {
        int newRow = currentState.zeroPos.first + directions[i][0];
        int newCol = currentState.zeroPos.second + directions[i][1];

        if (newRow >= 0 && newRow < 3 && newCol >= 0 && newCol < 3)
        {
            vector<vector<int>> newBoard = currentState.board;
            swap(newBoard[currentState.zeroPos.first][currentState.zeroPos.second],
                 newBoard[newRow][newCol]);
            int newH = calculateManhattanDistance(newBoard);
            string newMoveSequence = currentState.moveSequence + moveNames[i]; // Append move
            successors.emplace_back(newBoard, currentState.g + 1, newH, make_pair(newRow, newCol), newMoveSequence);
        }
    }
    return successors;
}

// A* search for the 8-puzzle problem
void aStarSearch(const vector<vector<int>> &initialBoard)
{
    priority_queue<State, vector<State>, greater<State>> pq; // Min-heap based on f(n)
    unordered_set<State> visited;                            // To track visited states

    // Find the initial position of the zero
    pair<int, int> zeroPos;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (initialBoard[i][j] == 0)
            {
                zeroPos = {i, j};
            }
        }
    }

    // Create the initial state
    int h = calculateManhattanDistance(initialBoard);
    State initialState(initialBoard, 0, h, zeroPos, "");
    pq.push(initialState);
    visited.insert(initialState);

    // Goal state
    vector<vector<int>> goalBoard = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};

    while (!pq.empty())
    {
        State currentState = pq.top();
        pq.pop();

        // Check if we have reached the goal state
        if (currentState.board == goalBoard)
        {
            cout << "Solution found in " << currentState.g << " moves: " << currentState.moveSequence << endl;
            return;
        }

        // Generate successors
        vector<State> successors = generateSuccessors(currentState);
        for (const State &successor : successors)
        {
            if (visited.find(successor) == visited.end())
            {
                pq.push(successor);
                visited.insert(successor);
            }
        }
    }

    cout << "No solution found." << endl;
}

int main()
{
    vector<vector<int>> initialBoard = {
        {1, 2, 3},
        {0, 4, 6},
        {7, 5, 8}};

    cout << "A* Search for the 8-Puzzle:\n";
    aStarSearch(initialBoard);
    return 0;
}
