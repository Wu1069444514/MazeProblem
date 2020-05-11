#include "acoalgorithm.h"

ACOAlgorithm::ACOAlgorithm(int **pmaze,int psize,int start_x,int star_y,int end_x,int end_y)
{
    size = psize;
    startx = start_x;
    starty = star_y;
    endx = end_x;
    endy = end_y;


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

    //信息素增量数组初始化
    add = (double**)malloc((size) * sizeof(double*));
    for (int i = 0; i < size; i++) {
        add[i] = (double*)calloc((size), sizeof(double));
    }

    //信息素数组初始化
    phe = (double**)malloc((size) * sizeof(double*));
    for (int i = 0; i < size; i++) {
        phe[i] = (double*)calloc((size), sizeof(double));
    }

    for (int i =0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            phe[i][j] = in;
        }
    }

    //初始化蚂蚁数组
    //ant = (Ant*)calloc((M), sizeof(Ant));

    Find();

}

ACOAlgorithm::~ACOAlgorithm()
{
    for (int i = 0; i < size; i++) free(maze[i]);
    free(maze);

    for (int i = 0; i < size; i++) free(add[i]);
    free(add);

    for (int i = 0; i < size; i++) free(phe[i]);
    free(phe);

    //free(ant);
}

ACOAlgorithm::Ant::Ant()
{


}



void ACOAlgorithm::Find()
{

    int iter = iterations;

    //迭代循环
    //qDebug()<<"Start!\n";
    while (iter--)
    {
        //qDebug()<<"iter:"<<iter;
        //初始化蚂蚁状态
        for(int i=0;i<M;i++)
        {

            ant[i].Init(startx,starty,size,maze);

        }

        //M只蚂蚁循环
        for (int i = 0;i<M;i++)
        {
            //qDebug()<<"Ant"<<i<<"!\n";
            //没找到终点则循环
            while (ant[i].position.x()!=endx||ant[i].position.y()!=endy)
            {
                //寻找下一步
                int next = NextStep(ant[i]);
                //找到下一步
                if(next!=-1)
                {
                    //更新坐标并入栈
                    ant[i].position.setX(ant[i].position.x() +offset[next].x);
                    ant[i].position.setY(ant[i].position.y() +offset[next].y);
                    ant[i].stackPath.push(ant[i].position);
                }
                //未找到下一步
                else
                {

                    //回溯
                    ant[i].stackPath.pop();
                    if(ant[i].stackPath.isEmpty())
                    {
                        return;
                        qDebug()<<"Nothing!";
                    }

                    int x = ant[i].position.x();
                    int y = ant[i].position.y();

                    //消除入栈时的障碍
                    ant[i].visited[x][y] = 1;

                    int topx = ant[i].stackPath.top().x();
                    int topy = ant[i].stackPath.top().y();

                    if(x == topx)
                    {
                        if((y - topy)==1)//向右
                        {
                            ant[i].around[topx][topy][0] = 0;//标记该方向已访问
                        }
                        if((y - topy)==-1)//向左
                        {
                            ant[i].around[topx][topy][2] = 0;//标记该方向已访问

                        }
                    }
                    if(y == topy)
                    {

                        if((x - topx)==1)//向下
                        {
                            ant[i].around[topx][topy][1] = 0;//标记该方向已访问

                        }
                         if((y - topy)==-1)//向上
                        {
                             ant[i].around[topx][topy][3] = 0;//标记该方向已访问

                        }
                    }


                    ant[i].position = ant[i].stackPath.top();
                }


            }



        }

        //保存最优路线
        for(int i = 0;i<M;i++)
        {
            int solution = ant[i].stackPath.size();
            if(solution<bestSolution)
            {
                bestSolution = solution;
                path = ant[i].stackPath;
            }
        }

        //初始化信息素增量数组
        for(int i=0;i<size;i++)
        {
            for(int j=0;j<size;j++)
            {
                add[i][j] = 0;
            }
        }

        //本轮迭代中每只蚂蚁在路径上的信息素残留量
        for(int i=0;i<M;i++)
        {
            //Ant-Circle system模型
            double L = ant[i].stackPath.size();
            double d = Q/L;
            while(!ant[i].stackPath.isEmpty())
            {
                QPoint q = ant[i].stackPath.pop();
                add[q.x()][q.y()] += d;
            }
        }

        //更新信息表
        for(int i=0;i<size;i++)
        {
            for(int j=0;j<size;j++)
            {
                phe[i][j] =rho*phe[i][j] + add[i][j];

                //为信息素设置一个下限值和上限值
               if(phe[i][j]<0.0001)
               {
                   phe[i][j] = 0.0001;
               }
               if(phe[i][j]>20)
               {
                   phe[i][j] = 20;
               }

            }
        }



    }

}

//得出下一步的值，返回值为-1，，时表示无解
int ACOAlgorithm::NextStep(Ant ant)
{
    int x = ant.position.x();
    int y = ant.position.y();

    double p_sum = 0;

    for(int i=0;i<4;i++)
    {
        int x1 = x+offset[i].x;
        int y1 = y+offset[i].y;
        if(Valid(x1,y1,i,ant))
        {
            p_sum += pow(phe[x1][y1],alphe) * pow((Q/(double)(ant.stackPath.size())),beta);
        }

    }

    int i = 0;
    //下一步有解
    if(p_sum)
    {
        double drand = (double)(rand())/(RAND_MAX+1);
        double pro = 0;
        for(i = 0;i<4;i++)
        {
          //计算下一个可能的坐标
           int x2 =  x + offset[i].x;
           int y2 =  y + offset[i].y;
           if(Valid(x2,y2,i,ant))
           {
            pro += (pow(phe[x2][y2],alphe) * pow((Q/ant.stackPath.size()),beta))/p_sum;
            if(pro>=drand)
            {
                return i;
            }
           }
        }
    }


    //ant.around[i] = 0;
    return -1;
}



//判断点是否合法
bool ACOAlgorithm::Valid(int x, int y,int off,Ant ant)
{

    //int nextx = x+offset[off].x;
    //int nexty = y+offset[off].y;
    if(x < 0 || x >= size || y < 0 || y >= size)
        return false;
    if(!ant.visited[x][y])
        return false;
    if(!ant.around[x][y][off])
        return false;
    /*if(nextx < 0 || nextx >= size || nexty < 0 || nexty >= size)
        return false;
    if(!maze[nextx][nexty])
        return false;
    */
/*
    foreach(QPoint q,ant.stackPath)
    {
        //if( x== q.x()&&y == q.y() || nextx == q.x() && nexty == q.y())
        if( x== q.x()&&y == q.y())
        {
            return false;
        }
    }*/
    return true;
}

 //获取路径
QList<QPoint> ACOAlgorithm::GetPath()
{
    QList<QPoint> p;
    foreach(QPoint q,path)
    {
        p.append(q);
    }
    return p;
}



