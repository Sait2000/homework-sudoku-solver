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
