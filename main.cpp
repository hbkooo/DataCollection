#include "mainwindow.h"
#include <QApplication>
#include <QIcon>
#include <QDebug>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow w;
    QIcon icon(":/image/logo_icon");
    w.setWindowIcon(icon);
    w.setWindowTitle("数据采集系统");
    w.show();

    return a.exec();
}
