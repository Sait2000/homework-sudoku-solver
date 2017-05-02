#include <istream>
#include <vector>

#define BOX_HEIGHT 3
#define BOX_WIDTH 3
#define BOARD_SIZE (BOX_HEIGHT * BOX_WIDTH)

#define EMPTY_CELL 0

class Sudoku
{
private:
    int game_board[BOARD_SIZE][BOARD_SIZE];
    int answer[BOARD_SIZE][BOARD_SIZE];
public:
    Sudoku(std::istream& stream);
    void show_game();
    void show_answer();
    // bool validate_at(int row, int col, int value);
    bool is_complete();
    bool solve();
    void write_game_board(int row, int col, int value);
    void erase_game_board(int row, int col);
    void hint();
};

class SudokuSolver
{
private:
    struct Position
    {
        int row;
        int col;
    };

    struct HistoryRecord
    {
        Position position;
        int value;
    };

    struct CallStackRecord
    {
        Position position;
        int value;
        std::vector<HistoryRecord>::size_type history_length;
    };

    // false: not eliminated
    // true: eliminated
    // !!! [0..BOARD_SIZE) !!!
    bool eliminated[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE];
    int num_eliminated[BOARD_SIZE][BOARD_SIZE];

    std::vector<CallStackRecord> call_stack; // backtracking information
    std::vector<HistoryRecord> history; // elimination history
    std::vector<HistoryRecord> elimination_stack; // list of naked single

    bool solution_found;

    SudokuSolver(int const in_board[BOARD_SIZE][BOARD_SIZE]);

    bool assign(Position, int);
    bool eliminate(Position, int);
    CallStackRecord backtrack();
public:
    static bool solve(int const in_board[BOARD_SIZE][BOARD_SIZE], int out_board[BOARD_SIZE][BOARD_SIZE]);
};
