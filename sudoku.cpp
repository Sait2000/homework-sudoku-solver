#include <iostream>
#include <cstdlib>
#include "sudoku.h"

Sudoku::Sudoku(std::istream& stream)
: answer{}
{
    for (int r = 0; r < BOARD_SIZE; ++r)
    {
        for (int c = 0; c < BOARD_SIZE; ++c)
        {
            int v = stream.get();
            for ( ; ; v = stream.get())
            {
                if (v == std::char_traits<char>::eof())
                {
                    v = EMPTY_CELL;
                    break;
                }
                if (v == '0' || v == '.')
                {
                    v = EMPTY_CELL;
                    break;
                }
                if ('1' <= v && v <= '9')
                {
                    v -= '0';
                    break;
                }
            }

            game_board[r][c] = v;
        }
    }
    if (!solve()) // no solutions
    {
    }
}

void Sudoku::show_game()
{
    std::cout << "+";
    for (int box_c = 0; box_c < BOARD_SIZE; box_c += BOX_WIDTH)
    {
        for (int c = box_c; c < box_c + BOX_WIDTH; ++c)
        {
            std::cout << "--";
        }
        std::cout << "-+";
    }
    std::cout << std::endl;

    for (int box_r = 0; box_r < BOARD_SIZE; box_r += BOX_HEIGHT)
    {
        for (int r = box_r; r < box_r + BOX_HEIGHT; ++r)
        {
            std::cout << "|";
            for (int box_c = 0; box_c < BOARD_SIZE; box_c += BOX_WIDTH)
            {
                for (int c = box_c; c < box_c + BOX_WIDTH; ++c)
                {
                    std::cout << " " << game_board[r][c];
                }
                std::cout << " |";
            }
            std::cout << std::endl;
        }

        std::cout << "+";
        for (int box_c = 0; box_c < BOARD_SIZE; box_c += BOX_WIDTH)
        {
            for (int c = box_c; c < box_c + BOX_WIDTH; ++c)
            {
                std::cout << "--";
            }
            std::cout << "-+";
        }
        std::cout << std::endl;
    }
}

void Sudoku::show_answer()
{
    std::cout << "+";
    for (int box_c = 0; box_c < BOARD_SIZE; box_c += BOX_WIDTH)
    {
        for (int c = box_c; c < box_c + BOX_WIDTH; ++c)
        {
            std::cout << "--";
        }
        std::cout << "-+";
    }
    std::cout << std::endl;

    for (int box_r = 0; box_r < BOARD_SIZE; box_r += BOX_HEIGHT)
    {
        for (int r = box_r; r < box_r + BOX_HEIGHT; ++r)
        {
            std::cout << "|";
            for (int box_c = 0; box_c < BOARD_SIZE; box_c += BOX_WIDTH)
            {
                for (int c = box_c; c < box_c + BOX_WIDTH; ++c)
                {
                    std::cout << " " << answer[r][c];
                }
                std::cout << " |";
            }
            std::cout << std::endl;
        }

        std::cout << "+";
        for (int box_c = 0; box_c < BOARD_SIZE; box_c += BOX_WIDTH)
        {
            for (int c = box_c; c < box_c + BOX_WIDTH; ++c)
            {
                std::cout << "--";
            }
            std::cout << "-+";
        }
        std::cout << std::endl;
    }
}

// bool Sudoku::validate_at(int row, int col, int value)
// {
//     return answer[row][col] == value;
// }

bool Sudoku::is_complete()
{
    for (int r = 0; r < BOARD_SIZE; ++r)
    {
        for (int c = 0; c < BOARD_SIZE; ++c)
        {
            if (game_board[r][c] != answer[r][c])
            {
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::solve()
{
    return SudokuSolver::solve(game_board, answer);
}

void Sudoku::write_game_board(int row, int col, int value)
{
    game_board[row][col] = value;
}

void Sudoku::erase_game_board(int row, int col)
{
    game_board[row][col] = EMPTY_CELL;
}

void Sudoku::hint()
{
    int row = std::rand() % BOARD_SIZE;
    int col = std::rand() % BOARD_SIZE;

    std::cout << "Correct answer at ("
        << row << ", "
        << col
    << ") is " << answer[row][col] << "." << std::endl;

    if (answer[row][col] == game_board[row][col])
    {
        std::cout << "Your answer is correct." << std::endl;
    }
}

SudokuSolver::SudokuSolver(int const in_board[BOARD_SIZE][BOARD_SIZE])
: solution_found(false)
, eliminated{}
, num_eliminated{}
{
    for (int r = 0; r < BOARD_SIZE; ++r)
    {
        for (int c = 0; c < BOARD_SIZE; ++c)
        {
            int v = in_board[r][c];
            if (v != EMPTY_CELL)
            {
                if (!assign({r, c}, v - 1))
                {
                    return;
                }
            }
        }
    }

    bool should_backtrack = false;
    for ( ; ; )
    {
        // assining target & value
        Position p;
        int v;

        if (should_backtrack)
        {
            if (call_stack.empty())
            {
                return;
            }
            CallStackRecord rec_c = backtrack();
            p = rec_c.position;
            for (v = rec_c.value + 1; v < BOARD_SIZE; ++v)
            {
                if (!eliminated[p.row][p.col][v])
                {
                    break;
                }
            }
            if (v == BOARD_SIZE)
            {
                // should_backtrack = true;
                continue;
            }
        }
        else
        {
            v = 0;
            bool found_target = false;
            int minimum_size = BOARD_SIZE + 1;
            for (int r = 0; r < BOARD_SIZE; ++r)
            {
                for (int c = 0; c < BOARD_SIZE; ++c)
                {
                    if (num_eliminated[r][c] < BOARD_SIZE - 1)
                    {
                        int size = BOARD_SIZE - num_eliminated[r][c];
                        if (size < minimum_size)
                        {
                            found_target = true;
                            p = Position {r, c};
                            minimum_size = size;
                        }
                        break;
                    }
                }
            }
            if (!found_target)
            {
                solution_found = true;
                return;
            }
        }

        call_stack.push_back({p, v, history.size()});
        should_backtrack = false;
        if (!assign(p, v))
        {
            should_backtrack = true;
        }
    }
}

bool SudokuSolver::assign(Position p, int v)
{
    for (int v_el = 0; v_el < BOARD_SIZE; ++v_el)
    {
        if (v_el != v)
        {
            if (!eliminate(p, v_el))
            {
                elimination_stack.clear();
                return false;
            }
        }
    }

    while (!elimination_stack.empty())
    {
        HistoryRecord rec = elimination_stack.back();
        elimination_stack.pop_back();
        Position p = rec.position;
        int v = rec.value;
        int r = p.row;
        int c = p.col;
        for (int house_type = 0; house_type < 3; ++house_type)
        {
            // house size
            int lr =    house_type == 0 ?  BOARD_SIZE
                   :    house_type == 1 ?  BOX_WIDTH
                   : /* house_type == 2 */ 1;
            int lc = BOARD_SIZE / lr;

            // house position
            int br = r / lr * lr;
            int bc = c / lc * lc;

            for (int dr = 0; dr < lr; ++dr)
            {
                for (int dc = 0; dc < lc; ++dc)
                {
                    int nr = br + dr;
                    int nc = bc + dc;
                    if (r != nr || c != nc)
                    {
                        if (!eliminate({nr, nc}, v))
                        {
                            elimination_stack.clear();
                            return false;
                        }
                    }
                }
            }
        }
    }

    return true;
}

bool SudokuSolver::eliminate(Position p, int v)
{
    if (!eliminated[p.row][p.col][v])
    {
        eliminated[p.row][p.col][v] = true;
        ++num_eliminated[p.row][p.col];
        history.push_back({p, v});
        if (num_eliminated[p.row][p.col] == BOARD_SIZE - 1)
        {
            for (int v_p = 0; v_p < BOARD_SIZE; ++v_p)
            {
                if (!eliminated[p.row][p.col][v_p])
                {
                    elimination_stack.push_back({p, v_p});
                }
            }
        }
        else if (num_eliminated[p.row][p.col] == BOARD_SIZE)
        {
            return false;
        }
    }
    return true;
}

CallStackRecord SudokuSolver::backtrack()
{
    CallStackRecord rec_c = call_stack.back();
    call_stack.pop_back();
    while (history.size() > rec_c.history_length)
    {
        HistoryRecord rec_h = history.back();
        history.pop_back();
        int r = rec_h.position.row;
        int c = rec_h.position.col;
        eliminated[r][c][rec_h.value] = false;
        --num_eliminated[r][c];
    }
    return rec_c;
}

bool SudokuSolver::solve(int const in_board[BOARD_SIZE][BOARD_SIZE], int out_board[BOARD_SIZE][BOARD_SIZE])
{
    SudokuSolver solver = in_board;

    if (!solver.solution_found)
    {
        return false;
    }

    for (int r = 0; r < BOARD_SIZE; ++r)
    {
        for (int c = 0; c < BOARD_SIZE; ++c)
        {
            for (int v = 0; v < BOARD_SIZE; ++v)
            {
                if (!solver.eliminated[r][c][v])
                {
                    out_board[r][c] = v + 1;
                }
            }
        }
    }

    return true;
}
