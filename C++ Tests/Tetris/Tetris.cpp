#include <algorithm>
#include <chrono>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <thread>
#include <vector>

using namespace std;
// using namespace std::chrono_literals;

bool gameOver = false;

const int canvasWidth = 300;
const int canvasHeight = 600;
const int grid = 32;

map<char, vector<vector<int>>> tetrominos = {
    {'I', {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
    {'J', {{1, 0, 0}, {1, 1, 1}, {0, 0, 0}}},
    {'L', {{0, 0, 1}, {1, 1, 1}, {0, 0, 0}}},
    {'O', {{1, 1}, {1, 1}}},
    {'S', {{0, 1, 1}, {1, 1, 0}, {0, 0, 0}}},
    {'Z', {{1, 1, 0}, {0, 1, 1}, {0, 0, 0}}},
    {'T', {{0, 1, 0}, {1, 1, 1}, {0, 0, 0}}}};

map<char, string> colors = {
    {'I', "cyan"},
    {'O', "yellow"},
    {'T', "purple"},
    {'S', "green"},
    {'Z', "red"},
    {'J', "blue"},
    {'L', "orange"}
};

vector<vector<int>> playfield(20, vector<int>(10, 0));
random_device rd;
mt19937 gen(rd());
vector<char> tetrominoSequence;

void generateSequence() {
  vector<char> sequence = {'I', 'O', 'T', 'S', 'Z', 'J', 'L'};
  while (!sequence.empty()) {
    uniform_int_distribution<> distr(0, sequence.size() - 1);
    int randIndex = distr(gen);
    char name = sequence[randIndex];
    tetrominoSequence.push_back(name);
    sequence.erase(sequence.begin() + randIndex);
  }
}

struct Tetromino {
  char name;
  vector<vector<int>> matrix;
  int row, col;
} tetromino;

vector<vector<int>> rotate(const vector<vector<int>> &matrix) {
  int N = matrix.size() - 1;
  vector<vector<int>> result(matrix.size(), vector<int>(matrix.size()));
  for (int i = 0; i < matrix.size(); ++i) {
    for (int j = 0; j < matrix[i].size(); ++j) {
      result[j][N - i] = matrix[i][j];
    }
  }
  return result;
}

bool isValidMove(const vector<vector<int>> &matrix, int cellRow, int cellCol) {
  for (int row = 0; row < matrix.size(); ++row) {
    for (int col = 0; col < matrix[row].size(); ++col) {
      if (matrix[row][col] &&
          (cellCol + col < 0 || cellCol + col >= playfield[0].size() ||
           cellRow + row >= playfield.size() ||
           playfield[cellRow + row][cellCol + col])) {
        return false;
      }
    }
  }
  return true;
}

Tetromino getNextTetromino() {
  if (tetrominoSequence.empty()) {
    generateSequence();
  }
  char name = tetrominoSequence.back();
  tetrominoSequence.pop_back();
  vector<vector<int>> matrix = tetrominos[name];
  int col = playfield[0].size() / 2 - matrix[0].size() / 2;
  int row = (name == 'I') ? -1 : -2;

  return {name, matrix, row, col};
}

void showGameOver() {
  gameOver = true;
  cout << "\n\n";
  cout << "GAME OVER!" << endl;
  // Add your additional game over screen rendering here
  // For example, you can print additional messages, ASCII art, or other
  // graphics
  // ... (your existing code)
}

void placeTetromino() {
  for (int row = 0; row < tetromino.matrix.size(); ++row) {
    for (int col = 0; col < tetromino.matrix[row].size(); ++col) {
      if (tetromino.matrix[row][col]) {
        if (tetromino.row + row < 0) {
          showGameOver();
          return;
        }
        playfield[tetromino.row + row][tetromino.col + col] = tetromino.name;
      }
    }
  }
  for (int row = playfield.size() - 1; row >= 0;) {
    if (all_of(playfield[row].begin(), playfield[row].end(),
        [](int cell) { return cell != 0; })) {
      for (int r = row; r >= 0; --r) {
        for (int c = 0; c < playfield[r].size(); ++c) {
          playfield[r][c] = playfield[r - 1][c];
        }
      }
    } else {
      --row;
    }
  }
  tetromino = getNextTetromino();
}

// Function to check if the tetromino can move down
bool canMoveDown(const Tetromino& tetromino) {
    for (int row = 0; row < tetromino.matrix.size(); ++row) {
        for (int col = 0; col < tetromino.matrix[row].size(); ++col) {
            if (tetromino.matrix[row][col]) {
                int nextRow = tetromino.row + row + 1;
                if (nextRow >= playfield.size() || playfield[nextRow][tetromino.col + col] != 0) {
                    return false;
                }
            }
        }
    }
    return true;
}

// Function to clear completed lines
void clearCompletedLines() {
    for (int row = playfield.size() - 1; row >= 0; ) {
        if (all_of(playfield[row].begin(), playfield[row].end(), [](int cell) { return cell != 0; })) {
            playfield.erase(playfield.begin() + row);
            playfield.insert(playfield.begin(), vector<int>(playfield[0].size(), 0));
        } else {
            --row;
        }
    }
}


int main() {
    // Initialize your game variables
    tetrominoSequence.clear();
    generateSequence();
    Tetromino tetromino = getNextTetromino(); // Declare tetromino here

    // Your game loop
    while (!gameOver) {
        // Process user input
        // ... (handle keyboard events)
        char userInput;
        cin >> userInput;

        // Handle user input
        if (userInput == 'q' || userInput == 'Q') {
            // Quit the game or perform some other action
            break; // Exit the loop
        } else if (userInput == 'left') {
            // Move tetromino left
        } else if (userInput == 'right') {
            // Move tetromino right
        } else if (userInput == 'down') {
            // Move tetromino down
        } else if (userInput == 'rotate') {
            // Rotate the tetromino
        }

        // Update game logic
        // ... (update tetromino position and check for collisions)
        
        if (canMoveDown(tetromino)) {
            tetromino.row++; // Move the tetromino down
        } else {
            placeTetromino(); // Place the tetromino on the playfield
            clearCompletedLines(); // Clear any completed lines
            tetromino = getNextTetromino(); // Get the next tetromino
            if (!canMoveDown(tetromino)) {
                // If the new tetromino can't be placed, the game is over
                showGameOver();
                break;
            }
        }

        // Render the game
        // ... (clear the screen, draw playfield and tetromino)
        // Example:
        // Clear the screen
        system("clear"); // Use "cls" for Windows

        // Render the game
        for (int row = 0; row < playfield.size(); ++row) {
            for (int col = 0; col < playfield[row].size(); ++col) {
                if (playfield[row][col] != 0) {
                    cout << colors[playfield[row][col]]; // Print tetromino color
                } else {
                    cout << "."; // Empty cell
                }
            }
            cout << endl; // Move to the next row
        }

        // Implement a delay to control frame rate
        this_thread::sleep_for(100ms); // Adjust as needed
    }

    return 0;
}
