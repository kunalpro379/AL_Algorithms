#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>

class PuzzleSolver
{
private:
    std::vector<int> current_state;
    std::vector<int> goal_state;
    const int SIZE = 4; // For 4-puzzle (2x2 grid + empty tile)

public:
    PuzzleSolver()
    {
        // Initialize goal state (0 represents empty tile)
        goal_state = {1, 2, 3, 0};

        // Initialize current state
        current_state = goal_state;
        randomizeState();
    }

    // Get possible moves for the empty tile
    std::vector<int> getPossibleMoves()
    {
        std::vector<int> moves;
        int empty_pos = std::find(current_state.begin(), current_state.end(), 0) - current_state.begin();

        // For a 2x2 grid:
        // If not in leftmost column, can move left
        if (empty_pos % 2 != 0)
            moves.push_back(empty_pos - 1);

        // If not in rightmost column, can move right
        if (empty_pos % 2 != 1)
            moves.push_back(empty_pos + 1);

        // If not in top row, can move up
        if (empty_pos >= 2)
            moves.push_back(empty_pos - 2);

        // If not in bottom row, can move down
        if (empty_pos < 2)
            moves.push_back(empty_pos + 2);

        return moves;
    }

    // Generate neighbor state by moving tile
    std::vector<int> getNeighbor(int move_pos)
    {
        std::vector<int> neighbor = current_state;
        int empty_pos = std::find(neighbor.begin(), neighbor.end(), 0) - neighbor.begin();
        std::swap(neighbor[empty_pos], neighbor[move_pos]);
        return neighbor;
    }

    // Calculate Manhattan distance heuristic
    int calculateManhattanDistance(const std::vector<int> &state)
    {
        int distance = 0;
        for (int i = 0; i < SIZE; i++)
        {
            if (state[i] != 0)
            { // Don't calculate for empty tile
                int current_row = i / 2;
                int current_col = i % 2;

                // Find goal position for current number
                int goal_pos = std::find(goal_state.begin(), goal_state.end(), state[i]) - goal_state.begin();
                int goal_row = goal_pos / 2;
                int goal_col = goal_pos % 2;

                // Add Manhattan distance for this tile
                distance += abs(current_row - goal_row) + abs(current_col - goal_col);
            }
        }
        return distance;
    }

    // Randomize the current state
    void randomizeState()
    {
        current_state = goal_state;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(current_state.begin(), current_state.end(), gen);
    }

    // Print the puzzle state
    void printState()
    {
        std::cout << "\n-------------\n";
        for (int i = 0; i < SIZE; i++)
        {
            if (i % 2 == 0)
                std::cout << "| ";
            if (current_state[i] == 0)
                std::cout << "_ | ";
            else
                std::cout << current_state[i] << " | ";
            if (i % 2 == 1)
                std::cout << "\n-------------\n";
        }
    }

    // Hill Climbing algorithm
    bool hillClimbing(int maxSteps = 1000)
    {
        int steps = 0;
        int current_value = calculateManhattanDistance(current_state);

        std::cout << "Initial state with h = " << current_value << ":\n";
        printState();

        while (steps < maxSteps)
        {
            if (current_value == 0)
            {
                std::cout << "\nSolution found in " << steps << " steps!\n";
                return true;
            }

            // Get all possible moves
            std::vector<int> moves = getPossibleMoves();
            bool improved = false;
            int best_value = current_value;
            std::vector<int> best_state = current_state;

            // Try all possible moves and find the best one
            for (int move : moves)
            {
                std::vector<int> neighbor = getNeighbor(move);
                int neighbor_value = calculateManhattanDistance(neighbor);

                if (neighbor_value < best_value)
                {
                    best_value = neighbor_value;
                    best_state = neighbor;
                    improved = true;
                }
            }

            // If no better state found, we're stuck in local minimum
            if (!improved)
            {
                std::cout << "\nStuck in local minimum with h = " << current_value << "\n";
                return false;
            }

            // Move to the best neighbor
            current_state = best_state;
            current_value = best_value;
            steps++;

            // Print progress every 100 steps
            if (steps % 100 == 0)
            {
                std::cout << "\nStep " << steps << " with h = " << current_value << ":\n";
                printState();
            }
        }

        std::cout << "\nFailed to find solution in " << maxSteps << " steps.\n";
        return false;
    }
};

int main()
{
    PuzzleSolver solver;

    std::cout << "4-Puzzle Solver using Hill Climbing\n";
    std::cout << "Goal State:\n";
    std::vector<int> goal = {1, 2, 3, 0};
    for (int i = 0; i < 4; i++)
    {
        if (i % 2 == 0)
            std::cout << "\n-------------\n| ";
        if (goal[i] == 0)
            std::cout << "_ | ";
        else
            std::cout << goal[i] << " | ";
    }
    std::cout << "\n-------------\n";

    solver.hillClimbing();

    std::cout << "\nFinal state:\n";
    solver.printState();

    return 0;
}