#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->mazeWidget->installEventFilter(this);
    sizeGroup = new QButtonGroup(this);
    sizeGroup->addButton(ui->tenSize,10);
    sizeGroup->addButton(ui->twentySize,20);
    sizeGroup->addButton(ui->thirtySize,40);

    rankGroup = new QButtonGroup(this);
    rankGroup->addButton(ui->zeroRank,0);
    rankGroup->addButton(ui->sixRank,6);
    rankGroup->addButton(ui->threeRank,3);

    ui->tenSize->setChecked(true);
    ui->threeRank->setChecked(true);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()), this, SLOT(onTime()));

    myMaze = NULL;

}

MainWindow::~MainWindow()
{
    delete ui;
    for (int i = 0; i < size; i++) free(myMaze[i]);
    free(myMaze);
}


//过滤绘图事件
bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->mazeWidget && event->type() == QEvent::Paint&&paintFlag) {
        paintMaze();
        if(endReached)
        {
            if(ProcessFlag)
            {
                PaintProcess();
                ProcessFlag = false;
            }
            else
            {
                PaintPath();
            }
            endReached = false;

        }
    }

    return QWidget::eventFilter(watched, event);
}

// 绘制迷宫
void MainWindow::paintMaze()
{


    QPainter painter(ui->mazeWidget);


    //设置方块大小
    int particleSize;
    switch(size)
    {
    case 10:particleSize  = 60;break;
    case 20:particleSize  = 30;break;
    default:particleSize  = 15;break;
    }

    //绘制方块
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (myMaze[i][j] == ROUTE) {
                painter.setPen(Qt::white);
                painter.setBrush(Qt::white);
                painter.drawRect(i*particleSize, j*particleSize, particleSize, particleSize);
            }
            else {
                painter.setPen(Qt::black);
                painter.setBrush(Qt::black);
                painter.drawRect(i*particleSize, j*particleSize, particleSize, particleSize);
            }
        }
    }

    //绘制起点
    painter.setPen(Qt::gray);
    painter.setBrush(Qt::blue);
    painter.drawEllipse(start_x*particleSize, start_y*particleSize, particleSize, particleSize);

    //绘制终点
    painter.setPen(Qt::gray);
    painter.setBrush(Qt::red);
    painter.drawEllipse(end_x*particleSize, end_y*particleSize, particleSize, particleSize);
}

//绘制路径
void MainWindow::PaintPath()
{

    int particleSize;
    switch(size)
    {
    case 10:particleSize  = 60;break;
    case 20:particleSize  = 30;break;
    default:particleSize  = 15;break;
    }
    QPainter painter(ui->mazeWidget);


    foreach(QPoint q,path)
    {
        if(!color)
        {
            painter.setPen(Qt::green);
            painter.setBrush(Qt::green);
        }
        else
        {
            painter.setPen(Qt::blue);
            painter.setBrush(Qt::blue);
        }

        painter.drawRect(q.x()*particleSize, q.y()*particleSize, particleSize, particleSize);
    }

}


//绘制寻找过程
void MainWindow::PaintProcess()
{

    int particleSize;
    switch(size)
    {
    case 10:particleSize  = 60;break;
    case 20:particleSize  = 30;break;
    default:particleSize  = 15;break;
    }
    QPainter painter(ui->mazeWidget);

    /*foreach(QPoint q,process)
    {
        if(myStack.size() == 5)
        {
            QPoint out = myStack.first();
            myStack.pop_front();
            painter.setPen(Qt::white);
            painter.setBrush(Qt::white);
            painter.drawRect(out.x()*particleSize, out.y()*particleSize, particleSize, particleSize);
        }
        myStack.push_back(q);
        painter.setPen(Qt::gray);
        painter.setBrush(Qt::green);
        painter.drawRect(q.x()*particleSize, q.y()*particleSize, particleSize, particleSize);
    }*/

    //绘制起点
    painter.setPen(Qt:: gray);
    painter.setBrush(Qt::green);
    painter.drawRect(start_x*particleSize, start_y*particleSize, particleSize, particleSize);

    //绘制终点
    painter.setPen(Qt::gray);
    painter.setBrush(Qt::green);
    painter.drawRect(end_x*particleSize, end_y*particleSize, particleSize, particleSize);

    foreach(QPoint p,myStack)
    {
        painter.setPen(Qt::gray);
        painter.setBrush(Qt::green);
        painter.drawRect(p.x()*particleSize, p.y()*particleSize, particleSize, particleSize);
    }



}


//动画控制
void MainWindow::onTime()
{
    if(time<process.size())
    {
        QPoint current = process.at(time);

        if(myStack.size() == 5)
        {
            myStack.pop_front();
        }
        myStack.push_back(current);

        ProcessFlag = true;
        endReached = true;

        update();
        time++;

    }
    else
    {
        endReached = true;
        timer->stop();
        update();
    }

}


//获取起点和终点坐标
void MainWindow::GetStartAndDest()
{
    for (int i = 0; i < size; i++) {
        if(myMaze[i][0] == ROUTE)
        {
            start_x = i;
            start_y = 0;
        }
        if(myMaze[i][size-1] == ROUTE)
        {
            end_x = i;
            end_y = size-1;
        }

        printf("\n");
    }

    for (int j = 0; j < size; j++) {
        if (myMaze[0][j] == ROUTE) {
            start_x = j;
            start_y = 0;
        }
        if(myMaze[size-1][j] == ROUTE)
        {
            end_x = j;
            end_y = size-1;
        }
    }
}

// 点击按钮显示迷宫
void MainWindow::on_createMazeButton_clicked()
{
    RestartProcess();

    size = sizeGroup->checkedId();
    rank = rankGroup->checkedId();
    Maze maze(size,rank);
    maze.GetMaze(myMaze);
    GetStartAndDest();

    paintFlag = true;
    update();

    AStarAlgorithm aStar(myMaze,size,start_x,start_y,end_x,end_y);
    QList<QPoint> path  = aStar.GetPath();
    //qDebug()<<"path:"<<path;


}

//重新开始绘制过程
void MainWindow::RestartProcess()
{
    time = 0;
    myStack.clear();

    if(timer->isActive())
    {
        timer->stop();
        qDebug()<<"stop!";
    }
}


//点击按钮用A*算法求路径
void MainWindow::on_AStarButton_clicked()
{

    if(paintFlag)
    {
        color = 0;
        AStarAlgorithm aStar(myMaze,size,start_x,start_y,end_x,end_y);
        RestartProcess();

        if(ui->showProcessButton->isChecked())
        {
            ProcessFlag = true;
            process =aStar.GetProc();
            int i = 100;
            switch (size) {
            case 10:i = 100;break;
            case 20:i = 40;break;
            default:i = 5;break;

            }
            timer->start(i);
        }


        path  = aStar.GetPath();
        if(!path.empty())
        {
            endReached = true;
        }

        update();
    }

}



void MainWindow::on_ACOButton_clicked()
{

    if(paintFlag)
    {
        color = 1;

        ACOAlgorithm aco(myMaze,size,start_x,start_y,end_x,end_y);
        RestartProcess();

        path  = aco.GetPath();
        if(!path.empty())
        {
            endReached = true;
        }

        update();
    }

}
