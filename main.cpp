#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include "sudoku.h"

std::pair<bool, int> read_int()
{
    std::string line;

    std::cin.clear();
    std::getline(std::cin, line);

    int res;
    std::istringstream line_stream(line);

    line_stream >> res;

    if (!line_stream.fail())
    {
        line_stream >> std::ws;
        if (line_stream.eof())
        {
            return {true, res};
        }
    }

    return {false, 0};
}

template <typename T>
struct MenuItem
{
    int const key;
    std::string const label;
    T const value;
};

template <typename T>
T get_selection(std::vector<MenuItem<T> > v)
{
    for ( ; ; )
    {
        for (auto it = v.begin(); it != v.end(); ++it)
        {
            std::cout << it->key << ": " << it->label << std::endl;
        }

        auto input = read_int();
        if (input.first)
        {
            int selected = input.second;
            for (auto it = v.begin(); it != v.end(); ++it)
            {
                if (it->key == selected)
                {
                    return it->value;
                }
            }
        }

        std::cout << std::endl;
        std::cout << "Invalid input" << std::endl;
        std::cout << std::endl;
    }
}

bool handler_write(Sudoku& sudoku_instance);
bool handler_erase(Sudoku& sudoku_instance);
bool handler_give_hint(Sudoku& sudoku_instance);
bool handler_check_complete(Sudoku& sudoku_instance);
bool handler_show_solution(Sudoku& sudoku_instance);
bool handler_quit(Sudoku& sudoku_instance);

int main()
{
    std::cout << (
        "+------------------+\n"
        "| Sudoku Game Menu |\n"
        "+------------------+\n"
    ) << std::endl;

    std::vector<MenuItem<bool> > const intro_options = {
        {1, "Game Start", true},
        {0, "Quit", false},
    };

    std::vector<MenuItem<bool (*)(Sudoku&)> > const loop_options = {
        {1, "Write", handler_write},
        {2, "Erase", handler_erase},
        {3, "Hint", handler_give_hint},
        {4, "Complete?", handler_check_complete},
        {5, "Solution", handler_show_solution},
        {0, "Quit", handler_quit},
    };

    bool game_start = get_selection(intro_options);

    if (game_start)
    {
        std::ifstream sudoku_file("Text.txt");
        Sudoku sudoku_instance = sudoku_file;
        sudoku_file.close();

        bool continue_loop = true;
        while (continue_loop)
        {
            std::cout << std::endl;
            sudoku_instance.show_game();
            auto handler = get_selection(loop_options);
            std::cout << std::endl;
            continue_loop = handler(sudoku_instance);
        }
    }

    return 0;
}

bool handler_write(Sudoku& sudoku_instance)
{
    int row;
    int col;
    int value;

    bool valid_input = true;

    if (valid_input)
    {
        std::cout << "Row(0-based): ";
        auto row_input = read_int();
        if (row_input.first)
        {
            row = row_input.second;
        }
        else
        {
            valid_input = false;
        }
    }

    if (valid_input)
    {
        std::cout << "Column(0-based): ";
        auto col_input = read_int();
        if (col_input.first)
        {
            col = col_input.second;
        }
        else
        {
            valid_input = false;
        }
    }

    if (valid_input)
    {
        std::cout << "Value: ";
        auto value_input = read_int();
        if (value_input.first)
        {
            value = value_input.second;
        }
        else
        {
            valid_input = false;
        }
    }

    if (valid_input)
    {
        sudoku_instance.write_game_board(row, col, value);
    }
    else
    {
        std::cout << "Invalid input" << std::endl;
    }

    return true;
}

bool handler_erase(Sudoku& sudoku_instance)
{
    int row;
    int col;

    bool valid_input = true;

    if (valid_input)
    {
        std::cout << "Row(0-based): ";
        auto row_input = read_int();
        if (row_input.first)
        {
            row = row_input.second;
        }
        else
        {
            valid_input = false;
        }
    }

    if (valid_input)
    {
        std::cout << "Column(0-based): ";
        auto col_input = read_int();
        if (col_input.first)
        {
            col = col_input.second;
        }
        else
        {
            valid_input = false;
        }
    }

    if (valid_input)
    {
        sudoku_instance.erase_game_board(row, col);
    }
    else
    {
        std::cout << "Invalid input" << std::endl;
    }

    return true;
}

bool handler_give_hint(Sudoku& sudoku_instance)
{
    sudoku_instance.hint();
    return true;
}

bool handler_check_complete(Sudoku& sudoku_instance)
{
    std::cout << (
        sudoku_instance.is_complete()
        ? "Yes."
        : "No."
    ) << std::endl;

    return true;
}

bool handler_show_solution(Sudoku& sudoku_instance)
{
    sudoku_instance.show_answer();
    std::cout << "BYE BYE" << std::endl;

    return false;
}

bool handler_quit(Sudoku&)
{
    return false;
}
