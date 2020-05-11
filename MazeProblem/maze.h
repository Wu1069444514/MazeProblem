#ifndef MAZE_H
#define MAZE_H
#include<stdio.h>
#include<Windows.h>
#include<time.h>
#include<math.h>

#define WALL  0
#define ROUTE 1

class Maze
{

public:
    Maze(int x,int rank = 0);
    ~Maze();

    //复制迷宫
    void GetMaze(int** &maze);

protected:
    //迷宫初始化
    void InitMaze();

    //创建迷宫
    void CreateMaze(int** maze, int x, int y);

private:
    int Rank;       //迷宫复杂度参数，值越小迷宫越复杂，最小值为0
    int size;
    int** mazeStart;
};


#endif // MAZE_H
