#include "mainwindow.h"

#include <QApplication>
int main(int argc, char *argv[])
{
/*    int size;
    cout << "请输入迷宫大小：";
    cin >> size;

    Maze MyMaze(size);

    int** My = MyMaze.GetMaze();

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (My[i][j] == ROUTE) {
                printf("  ");
            }
            else {
                printf("国");
            }
        }
        printf("\n");
    }
*/
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
