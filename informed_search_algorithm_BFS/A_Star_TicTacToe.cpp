#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

const char EMPTY = '_';
const char PLAYER_X = 'X';
const char PLAYER_O = 'O';

struct State
{
    vector<vector<char>> board; // Current board state
    char currentPlayer;         // X or O
    int g;                      // Path cost (number of moves)
    int h;                      // Heuristic cost

    State(vector<vector<char>> b, char p, int g, int h)
    {
        this->board = b;
        this->currentPlayer = p;
        this->g = g;
        this->h = h;
    }

    // Comparison operator for priority queue
    bool operator<(const State &other) const
    {
        return (g + h) > other.g + other.h; // Min-heap based on f(n) = g + h
    }
};

// Function to print the board
void printBoard(const vector<vector<char>> &board)
{
    for (const auto &row : board)
    {
        for (char cell : row)
        {
            cout << cell << "|";
        }
        cout << endl;
        cout << "-----\n";
    }
}

// Function to check if a player has won
bool checkWin(const vector<vector<char>> &board, char player)
{
    for (int i = 0; i < 3; i++)
    {
        if (((board[i][0] == player && board[i][1] == player && board[i][2] == player) || // Rows
             (board[0][i] == player && board[1][i] == player && board[2][i] == player)))  // Columns
        {
            return true;
        }
    }
    // Check diagonals
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player))
    {
        return true;
    }
    return false;
}

// Function to check if the game is over
bool isGameOver(const vector<vector<char>> &board)
{
    return checkWin(board, PLAYER_X) || checkWin(board, PLAYER_O);
}

// Heuristic function (number of potential wins for the current player)
int calculateHeuristic(const vector<vector<char>> &board, char player)
{
    int score = 0;

    // Rows and columns
    for (int i = 0; i < 3; i++)
    {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == EMPTY)
            score++;
        if (board[i][0] == player && board[i][2] == player && board[i][1] == EMPTY)
            score++;
        if (board[i][1] == player && board[i][2] == player && board[i][0] == EMPTY)
            score++;

        if (board[0][i] == player && board[1][i] == player && board[2][i] == EMPTY)
            score++;
        if (board[0][i] == player && board[2][i] == player && board[1][i] == EMPTY)
            score++;
        if (board[1][i] == player && board[2][i] == player && board[0][i] == EMPTY)
            score++;
    }

    // Diagonals
    if (board[0][0] == player && board[1][1] == player && board[2][2] == EMPTY)
        score++;
    if (board[0][0] == player && board[2][2] == player && board[1][1] == EMPTY)
        score++;
    if (board[1][1] == player && board[2][2] == player && board[0][0] == EMPTY)
        score++;

    if (board[0][2] == player && board[1][1] == player && board[2][0] == EMPTY)
        score++;
    if (board[0][2] == player && board[2][0] == player && board[1][1] == EMPTY)
        score++;
    if (board[1][1] == player && board[2][0] == player && board[0][2] == EMPTY)
        score++;

    return score;
}

// Generate successor states for the current state
vector<State> generateSuccessors(const State &currentState)
{
    vector<State> successors;
    char nextPlayer = (currentState.currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (currentState.board[i][j] == EMPTY)
            {
                vector<vector<char>> newBoard = currentState.board;
                newBoard[i][j] = currentState.currentPlayer;
                int h = calculateHeuristic(newBoard, nextPlayer);
                successors.emplace_back(newBoard, nextPlayer, currentState.g + 1, h);
            }
        }
    }
    return successors;
}

// A* search algorithm for Tic-Tac-Toe
void aStarSearch()
{
    priority_queue<State> pq;                                          // Min-heap
    vector<vector<char>> initialBoard(3, vector<char>(3, EMPTY));      // Initial empty board
    int initialHeuristic = calculateHeuristic(initialBoard, PLAYER_O); // Heuristic for O's turn
    pq.emplace(initialBoard, PLAYER_X, 0, initialHeuristic);           // Start with X

    while (!pq.empty())
    {
        State currentState = pq.top();
        pq.pop();

        // Print the current board state
        cout << "Current Board:\n";
        printBoard(currentState.board);
        cout << "g: " << currentState.g << ", h: " << currentState.h << endl;

        // Check if the game is over
        if (isGameOver(currentState.board))
        {
            cout << "Game Over. " << currentState.currentPlayer << " wins!" << endl;
            return;
        }

        // Generate successors and add them to the priority queue
        vector<State> successors = generateSuccessors(currentState);
        for (const State &successor : successors)
        {
            pq.push(successor);
        }
    }
    cout << "No more moves available. Game is a draw." << endl; // If the queue is exhausted
}

int main()
{
    cout << "A* Search for Tic-Tac-Toe:\n";
    aStarSearch();
    return 0;
}
