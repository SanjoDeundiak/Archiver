#include "mainwindow.h"
#include <QApplication>
#include "mainwindow.h"
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);    
    MainWindow w(QString::fromLocal8Bit(argv[1]));
    w.show();

    QMetaObject::invokeMethod(&w, "go", Qt::QueuedConnection);

    return a.exec();
}
