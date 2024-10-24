#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <climits>
#include <algorithm>

class QueensSolver
{
private:
    int n;                  // board size (4 in this case)
    std::vector<int> board; // current board state
    std::random_device rd;
    std::mt19937 gen;

public:
    QueensSolver(int size = 4) : n(size), gen(rd())
    {
        board.resize(n);
    }

    // Initialize board randomly
    void randomizeBoard()
    {
        for (int i = 0; i < n; i++)
        {
            board[i] = rand() % n;
        }
    }

    // Calculate number of conflicts (pairs of queens attacking each other)
    int calculateConflicts()
    {
        int conflicts = 0;
        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                if (board[i] == board[j] || // same row
                    abs(board[i] - board[j]) == abs(i - j))
                { // same diagonal
                    conflicts++;
                }
            }
        }
        return conflicts;
    }

    // Print current board state
    void printBoard()
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (board[j] == i)
                    std::cout << "Q ";
                else
                    std::cout << ". ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    // 1. Hill Climbing
    bool hillClimbing()
    {
        int maxSteps = 1000;
        int steps = 0;

        while (steps < maxSteps)
        {
            int currentConflicts = calculateConflicts();
            if (currentConflicts == 0)
                return true;

            bool improved = false;
            int bestCol = -1;
            int bestRow = -1;
            int minConflicts = currentConflicts;

            // Try moving each queen to each possible position
            for (int col = 0; col < n; col++)
            {
                int originalRow = board[col];
                for (int newRow = 0; newRow < n; newRow++)
                {
                    if (newRow != originalRow)
                    {
                        board[col] = newRow;
                        int newConflicts = calculateConflicts();
                        if (newConflicts < minConflicts)
                        {
                            minConflicts = newConflicts;
                            bestCol = col;
                            bestRow = newRow;
                            improved = true;
                        }
                    }
                }
                board[col] = originalRow;
            }

            if (!improved)
                return false; // Local minimum reached
            board[bestCol] = bestRow;
            steps++;
        }
        return false;
    }

    // 2. Steepest Ascent Hill Climbing
    bool steepestAscentHillClimbing()
    {
        int maxSteps = 1000;
        int steps = 0;

        while (steps < maxSteps)
        {
            int currentConflicts = calculateConflicts();
            if (currentConflicts == 0)
                return true;

            std::vector<std::pair<int, int>> bestMoves;
            int minConflicts = currentConflicts;

            // Find all moves that lead to minimum conflicts
            for (int col = 0; col < n; col++)
            {
                int originalRow = board[col];
                for (int newRow = 0; newRow < n; newRow++)
                {
                    if (newRow != originalRow)
                    {
                        board[col] = newRow;
                        int newConflicts = calculateConflicts();
                        if (newConflicts < minConflicts)
                        {
                            minConflicts = newConflicts;
                            bestMoves.clear();
                            bestMoves.push_back({col, newRow});
                        }
                        else if (newConflicts == minConflicts)
                        {
                            bestMoves.push_back({col, newRow});
                        }
                    }
                }
                board[col] = originalRow;
            }

            if (bestMoves.empty())
                return false; // Local minimum reached

            // Choose random best move
            int moveIndex = rand() % bestMoves.size();
            board[bestMoves[moveIndex].first] = bestMoves[moveIndex].second;
            steps++;
        }
        return false;
    }

    // 3. Simulated Annealing
    bool simulatedAnnealing()
    {
        double temperature = 1.0;
        double coolingRate = 0.995;
        int maxSteps = 1000;
        int steps = 0;

        while (steps < maxSteps && temperature > 0.01)
        {
            int currentConflicts = calculateConflicts();
            if (currentConflicts == 0)
                return true;

            // Generate random neighbor
            int col = rand() % n;
            int originalRow = board[col];
            int newRow = rand() % n;
            board[col] = newRow;

            int newConflicts = calculateConflicts();
            int delta = newConflicts - currentConflicts;

            // Accept if better or with probability based on temperature
            if (delta > 0)
            {
                double probability = exp(-delta / temperature);
                if ((double)rand() / RAND_MAX > probability)
                {
                    board[col] = originalRow; // Revert move
                }
            }

            temperature *= coolingRate;
            steps++;
        }
        return calculateConflicts() == 0;
    }

    // 4. Random-restart Hill Climbing
    bool randomRestartHillClimbing(int maxRestarts = 10)
    {
        for (int i = 0; i < maxRestarts; i++)
        {
            randomizeBoard();
            if (hillClimbing())
                return true;
        }
        return false;
    }
};

int main()
{
    srand(time(0));
    QueensSolver solver;

    // Test all algorithms
    std::cout << "Testing different local search algorithms for 4-Queens:\n\n";

    // 1. Hill Climbing
    solver.randomizeBoard();
    std::cout << "Initial board for Hill Climbing:\n";
    solver.printBoard();
    if (solver.hillClimbing())
    {
        std::cout << "Hill Climbing found solution:\n";
        solver.printBoard();
    }
    else
    {
        std::cout << "Hill Climbing failed to find solution\n\n";
    }

    // 2. Steepest Ascent
    solver.randomizeBoard();
    std::cout << "Initial board for Steepest Ascent:\n";
    solver.printBoard();
    if (solver.steepestAscentHillClimbing())
    {
        std::cout << "Steepest Ascent found solution:\n";
        solver.printBoard();
    }
    else
    {
        std::cout << "Steepest Ascent failed to find solution\n\n";
    }

    // 3. Simulated Annealing
    solver.randomizeBoard();
    std::cout << "Initial board for Simulated Annealing:\n";
    solver.printBoard();
    if (solver.simulatedAnnealing())
    {
        std::cout << "Simulated Annealing found solution:\n";
        solver.printBoard();
    }
    else
    {
        std::cout << "Simulated Annealing failed to find solution\n\n";
    }

    // 4. Random-restart Hill Climbing
    std::cout << "Testing Random-restart Hill Climbing:\n";
    if (solver.randomRestartHillClimbing())
    {
        std::cout << "Random-restart Hill Climbing found solution:\n";
        solver.printBoard();
    }
    else
    {
        std::cout << "Random-restart Hill Climbing failed to find solution\n";
    }

    return 0;
}