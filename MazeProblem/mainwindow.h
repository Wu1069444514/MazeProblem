#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"maze.h"
#include<QButtonGroup>
#include<QDebug>
#include<QPainter>
#include"astaralgorithm.h"
#include"acoalgorithm.h"
#include<QStack>
#include<QTimer>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int **myMaze;
    int size;
    int rank;
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    bool endReached = false;    //是否找到终点
    bool ProcessFlag = false;   //是否绘制过程
    QList<QPoint> path; //路径
    QList<QPoint> process; //过程
    QList<QPoint> myStack;
    QButtonGroup *sizeGroup;
    QButtonGroup *rankGroup;
    QTimer *timer;
    int time = 0;
    int color = 0;  //A*为0，ACO为1


protected:
    bool paintFlag = false;
    bool eventFilter(QObject *watched, QEvent *event);
    void paintMaze();
    void PaintPath();
    void PaintProcess();
    void GetStartAndDest();
    void RestartProcess();

private slots:
    void on_createMazeButton_clicked();

    void on_AStarButton_clicked();

    void onTime();

    void on_ACOButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
