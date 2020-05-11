#ifndef ASTARALGORITHM_H
#define ASTARALGORITHM_H
#include<cmath>
#include<QList>
#include<QDebug>


class AStarAlgorithm
{
public:
    AStarAlgorithm(int **pmaze,int psize,int start_x,int star_y,int end_x,int end_y);
    ~AStarAlgorithm();
    QList<QPoint> GetPath();    //获取路径
    QList<QPoint> GetProc();    //获取过程

    int**maze;  //迷宫地图
    int startx,starty;  //起点
    int endx,endy;  //终点
    int size;   //规模


protected:

    //点类
     class Position
    {
    public:
        //Position();
        Position(int px,int py, int pendx,int pendy,Position *plastPosition)
        {
            this->x =px;
            this->y = py;
            this->lastPosition = plastPosition;
            if(lastPosition!=NULL)
            {
                pastDistance = lastPosition->pastDistance+1;
                lastPoint.setX(plastPosition->x);
                lastPoint.setY(plastPosition->y);
            }
            predictDistance = std::abs(pendx-x)+std::abs(pendy-y);


        }
        int Distance(){
            return pastDistance+predictDistance;
        }
        //当前点的坐标
        int x;
        int y;

        //已走过距离
        int pastDistance = 0;
        //到终点的估计距离
        int predictDistance =0;
        //上一个点
        Position *lastPosition;
        //上一个点坐标
        QPoint lastPoint;
    };


    QList<Position> open;   //open表
    QList<Position> closed; //closed表

    QList<QPoint> path; //记录路径
    QList<QPoint> process; //记录过程

    QPoint **backtrack; //回溯矩阵，用来回溯上一个节点坐标

    bool endReached = false;    //是否找到终点

    void Find();    //找到路径
    bool Valid(int x, int y);   //判断点是否合法
    void AddToOpenList(Position p); //将合适的点加入open表中
};

#endif // ASTARALGORITHM_H
