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
    vector<vector<char>> board;
    char currentPlayer; // X or O

    // Constructor
    State(const vector<vector<char>> &b, char p)
    {
        board = b;
        currentPlayer = p;
    }

    // Operator overload for priority queue
    bool operator<(const State &other) const
    {
        return false; // No comparison needed for BFS
    }
};

// Function to print the Tic Tac Toe board
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

// Check if a player has won
bool checkWin(const vector<vector<char>> &board, char player)
{
    for (int i = 0; i < 3; i++)
    {
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) || // Rows
            (board[0][i] == player && board[1][i] == player && board[2][i] == player))   // Columns
        {
            return true;
        }
    }
    // Diagonals
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player))
    {
        return true;
    }
    return false;
}

// Check if the game is over (either win or draw)
bool isGameOver(const vector<vector<char>> &board)
{
    return checkWin(board, PLAYER_X) || checkWin(board, PLAYER_O);
}

// Generate all possible next states (successors)
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
                successors.push_back(State(newBoard, nextPlayer));
            }
        }
    }

    return successors;
}

// Evaluate the current state of the board
int evaluateState(const State &state)
{
    if (checkWin(state.board, PLAYER_X))
        return 1; // X wins
    if (checkWin(state.board, PLAYER_O))
        return -1; // O wins
    return 0;      // No winner
}

// Informed BFS for Tic Tac Toe
void informedBFS()
{
    // Initial empty board with player X starting
    vector<vector<char>> initialBoard(3, vector<char>(3, EMPTY));
    State initialState(initialBoard, PLAYER_X);

    priority_queue<pair<int, State>> pq;
    pq.push({0, initialState});

    while (!pq.empty())
    {
        State currentState = pq.top().second;
        pq.pop();

        // Check if the game is over
        if (isGameOver(currentState.board))
        {
            printBoard(currentState.board);
            cout << "Game Over" << endl;
            return;
        }

        // Generate successors (next possible moves)
        vector<State> successors = generateSuccessors(currentState);
        for (State &successor : successors)
        {
            int heuristicCost = evaluateState(successor);
            pq.push({heuristicCost, successor});
        }
    }
    cout << "No winner" << endl;
}

int main()
{
    cout << "Starting Informed BFS for Tic Tac Toe...\n";
    informedBFS();
    cout << "Finished.\n";

    return 0;
}
