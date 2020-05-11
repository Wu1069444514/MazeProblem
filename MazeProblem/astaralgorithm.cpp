#include "astaralgorithm.h"

AStarAlgorithm::AStarAlgorithm(int **pmaze,int psize,int start_x,int star_y,int end_x,int end_y)
{
    size = psize;
    startx = start_x;
    starty = star_y;
    endx = end_x;
    endy = end_y;

    backtrack = (QPoint**)malloc((size) * sizeof(QPoint*));
    for (int i = 0; i < size; i++) {
        backtrack[i] = (QPoint*)calloc((size), sizeof(QPoint));
    }

    backtrack[start_x][starty].setX(-1);
    backtrack[start_x][starty].setY(-1);

    //获取迷宫地图
    maze = (int**)malloc((size) * sizeof(int*));
    for (int i = 0; i < size; i++) {
        maze[i] = (int*)calloc((size), sizeof(int));
    }

    for (int i =0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            maze[i][j] = pmaze[i][j];
        }
    }

    Find();

}

AStarAlgorithm::~AStarAlgorithm()
{
    for (int i = 0; i < size; i++) free(maze[i]);
    free(maze);

    for (int i = 0; i < size; i++) free(backtrack[i]);
    free(backtrack);
}


//寻找路径
void AStarAlgorithm::Find()
{
    Position start(startx,starty,endx,endy,NULL);
    Position end(endx,endx,endx,endy,NULL);
    open.append(start);

    while(true)
    {

        if(open.empty())
        {
            endReached = false;
            break;
        }

        Position choosed = open.first();
        open.removeFirst();
        closed.append(choosed);
        process.append(QPoint(choosed.x,choosed.y));

        if(choosed.x == endx&&choosed.y == endy)
        {
            end = choosed;
            endReached = true;
            break;
        }

        if(Valid(choosed.x-1, choosed.y)) {
            AddToOpenList( Position(choosed.x-1, choosed.y,endx,endy, &choosed));
        }

        if(Valid(choosed.x+1, choosed.y)) {
            AddToOpenList(Position(choosed.x+1, choosed.y,endx,endy, &choosed));
        }
        if(Valid(choosed.x, choosed.y-1)) {
            AddToOpenList(Position(choosed.x, choosed.y-1, endx,endy,&choosed));
        }
        if(Valid(choosed.x, choosed.y+1)) {
            AddToOpenList(Position(choosed.x, choosed.y+1,endx,endy, &choosed));
        }



    }

    //获得路径
    QPoint temp(end.x,end.y);
    while(temp.x()!=-1)
    {
        path.push_front(temp);
        temp = backtrack[temp.x()][temp.y()];
    }


}


//判断点是否合法（是否越界或是否已经在closed表中）
bool AStarAlgorithm::Valid(int x, int y) {
    if(x < 0 || x >= size || y < 0 || y >= size)
        return false;
    if(!maze[x][y])
        return false;
    foreach(Position iter,closed)
    {
        if(x == iter.x && y == iter.y)
        {
            return false;
        }
    }

    return true;
}


//将满足条件的点添加进open表
void AStarAlgorithm::AddToOpenList(Position p)
{
    int n =0;
    int i = 0;

    //设置上一个点的坐标
    backtrack[p.x][p.y].setX(p.lastPoint.x());
    backtrack[p.x][p.y].setY(p.lastPoint.y());


    //若该点已经在open表中，与原先的点比较F值大小
    foreach(Position iter1,open)
    {
        if(p.x == iter1.x && p.y == iter1.y)
        {
            if(p.Distance()<=iter1.Distance())
            {
                open.removeAt(n);
            }
            else
            {
                return;
            }
        }
    }

    //按从小到大顺序插入open表
     foreach(Position iter2,open)
     {
         if(p.Distance()<=iter2.Distance())
         {
             open.insert(i,p);
             return;
         }
         i++;
     }
     open.push_back(p);

}


//获取路径
QList<QPoint>AStarAlgorithm:: GetPath()
{
    return path;
}

//获取过程
QList<QPoint>AStarAlgorithm:: GetProc()
{
    /*foreach(Position p,closed)
    {
        QPoint q(p.x,p.y);
        process.append(q);
    }*/
    return process;
}



