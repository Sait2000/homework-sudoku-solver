# Naive C++ Sudoku ~~Solver~~ Game (HOMEWORK)
README를 쓰려고 하니까 이게 solver가 아니고 게임이었다는 게 생각났다...

## 빌드
```
g++ -std=c++11 main.cpp sudoku.cpp -Wall -Werror -Wextra -o main
```

## 사용법
solver로 사용하기 (.sdk 파일 지원)
```
cp [inputfile_path] Text.txt
printf "1\n5\n" | ./main | tail -n14 | head -n13
```

## 구조
코드:

* `main.cpp`: main()이나 뭐 그런 거
* `sudoku.cpp`: class Sudoku(^^;), class SudokuSolver
* `sudoku.py`: SudokuSolver의 prototype 겸 pseudo code

샘플:

* `Text.txt`: 실행파일이 읽는 파일
* `sample/`: 돌려봤던 스도쿠 문제들

## 알고리즘
[Peter Norvig의 글](http://norvig.com/sudoku.html)에서 봤던 알고리즘을 떠올리면서 만들었다. Peter Norvig의 solver는 [Naked Single](http://sudopedia.enjoysudoku.com/Naked_Single.html)하고 [Hidden Single](http://sudopedia.enjoysudoku.com/Hidden_Single.html)을 사용하는데 이 repo의 solver는 Naked Single만 사용한다.
