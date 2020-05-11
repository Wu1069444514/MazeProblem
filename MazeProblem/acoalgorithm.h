#ifndef ACOALGORITHM_H
#define ACOALGORITHM_H
#include<QList>
#include<QStack>
#include<QDebug>
#include<cmath>
#include<QVector>
class ACOAlgorithm
{
public:
    ACOAlgorithm(int **pmaze,int psize,int start_x,int star_y,int end_x,int end_y);
    ~ACOAlgorithm();
    QList<QPoint> GetPath();    //获取路径

    //迷宫参数
    int**maze;  //迷宫地图
    int startx,starty;  //起点
    int endx,endy;  //终点
    int size;   //规模

    int M = 5; //每一轮中蚂蚁个数
    int iterations = 5;    //迭代次数
    int in = 1; //信息素初始值

    double **add;   //每一段的信息素增量数组
    double **phe;   //每一段路径上的信息素数组
    //double MAX =

    //主要参数
    double alphe = 1;   //信息素重要程度
    double beta = 1;    //启发因子的重要程度
    double rho = 0.7;   //信息素蒸发系数
    double Q = 1;      //信息素增加强度系数


    //偏移量
    struct offset
    {
        int x;
        int y;
    }offset[4]={{1,0},{-1,0},{0,1},{0,-1}};


protected:
    class Ant
    {
    public:
        Ant();

        QStack<QPoint> stackPath;   //储存路径堆栈
        QPoint position;    //当前位置坐标
        int ***around;
        int **visited;

        QVector<QPoint> test;

        void Init(int startx,int starty,int size,int **maze)
        {
            around = (int***)malloc((size) * sizeof(int**));
            for (int i = 0; i < size; i++)
            {
                around[i] = (int**)malloc((size) * sizeof(int*));
            }
            for(int i = 0;i<size;i++)
            {
                for(int j=0;j<size;j++)
                {
                    around[i][j] = (int*)malloc((4)* sizeof(int));
                }
            }


            visited = (int**)malloc((size) * sizeof(int*));
            for (int i = 0; i < size; i++) {
                visited[i] = (int*)calloc((size), sizeof(int));
            }

            //初始化周围点矩阵
            for(int i=0;i<size;i++)
            {
                for(int j=0;j<size;j++)
                {
                    for(int n=0;n<4;n++)
                    {
                        around[i][j][n]=1;
                    }
                }
            }

            //记录是否访问过
            for(int i=0;i<size;i++)
            {
                for(int j=0;j<size;j++)
                {
                    visited[i][j] = maze[i][j];
                }
            }

            //起点设为障碍点
            visited[startx][starty] = 0;

            //qDebug()<<"around"<<around[5][5][0];

            position = QPoint(startx,starty);
            stackPath.clear();

            stackPath.append(position);


        }
    };

    void Find();
    int NextStep(Ant ant);  //得出下一步位置
    bool Valid(int x,int y,int off,Ant ant);   //判断点是否合法

    double bestSolution = 0x7fffffff;  //最短距离
    QStack<QPoint> path; //最优路径
    QList<QPoint> process; //记录过程


    Ant ant[5];

};

#endif // ACOALGORITHM_H
