#include "MainClass.h"
#include <QApplication>
#include <QDir>

const size_t MAX_PATH = 260;

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);
    MainClass mainclass(argc, argv);
    return a.exec();
}
