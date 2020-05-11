#include "Maze.h"

Maze::Maze(int x,int rank)
{
    size = x+2;
    Rank = rank;
    InitMaze();
}

Maze::~Maze()
{
    for (int i = 0; i < size; i++) free(mazeStart[i]);
    free(mazeStart);
}

void Maze::CreateMaze(int **maze,int x ,int y)
{

    maze[x][y] = ROUTE;

    //确保四个方向随机
    int direction[4][2] = { { 1,0 },{ -1,0 },{ 0,1 },{ 0,-1 } };
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        int temp = direction[0][0];
        direction[0][0] = direction[r][0];
        direction[r][0] = temp;

        temp = direction[0][1];
        direction[0][1] = direction[r][1];
        direction[r][1] = temp;
    }

    //向四个方向开挖
    for (int i = 0; i < 4; i++) {
        int dx = x;
        int dy = y;

        //控制挖的距离，由Rank来调整大小
        int range = 1 + (Rank == 0 ? 0 : rand() % Rank);
        while (range > 0) {
            dx += direction[i][0];
            dy += direction[i][1];

            //排除掉回头路
            if (maze[dx][dy] == ROUTE) {
                break;
            }

            //判断是否挖穿路径
            int count = 0;
            for (int j = dx - 1; j < dx + 2; j++) {
                for (int k = dy - 1; k < dy + 2; k++) {
                    //abs(j - dx) + abs(k - dy) == 1 确保只判断九宫格的四个特定位置
                    if (abs(j - dx) + abs(k - dy) == 1 && maze[j][k] == ROUTE) {
                        count++;
                    }
                }
            }

            if (count > 1) {
                break;
            }

            //确保不会挖穿时，前进
            --range;
            maze[dx][dy] = ROUTE;
        }

        //没有挖穿危险，以此为节点递归
        if (range <= 0) {
            CreateMaze(maze, dx, dy);
        }
    }

}

void Maze::InitMaze()
{
    srand((unsigned)time(NULL));

    mazeStart = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        mazeStart[i] = (int*)calloc(size, sizeof(int));
    }

    //最外围层设为路径的原因，为了防止挖路时挖出边界，同时为了保护迷宫主体外的一圈墙体被挖穿
    for (int i = 0; i < size; i++) {
        mazeStart[i][0] = ROUTE;
        mazeStart[0][i] = ROUTE;
        mazeStart[i][size - 1] = ROUTE;
        mazeStart[size - 1][i] = ROUTE;
    }

    CreateMaze(mazeStart, 2, 2);

    //画迷宫的入口和出口
    mazeStart[2][1] = ROUTE;	//入口

    //由于算法随机性，出口有一定概率不在（L-3,L-2）处，此时需要寻找出口
    for (int i = size - 3; i >= 0; i--) {
        if (mazeStart[i][size - 3] == ROUTE) {
            mazeStart[i][size - 2] = ROUTE;
            break;
        }
    }
    //mazeStart[2][1] = ROUTE;

}

void Maze::GetMaze(int** &maze)
{
    maze = (int**)malloc((size-2) * sizeof(int*));
    for (int i = 0; i < size-2; i++) {
        maze[i] = (int*)calloc((size-2), sizeof(int));
    }

    int m = 0, n = 0;
    for (int i = 1; i < size-1; i++) {
        for (int j = 1; j < size-1; j++) {
            maze[m][n] = mazeStart[i][j];
            n++;
        }
        n = 0;
        m++;
    }

}


