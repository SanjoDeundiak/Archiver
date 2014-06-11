#include "mainclass.h"
#include <QMessageBox>
#include <QApplication>

const size_t MAX_PATH = 260;


MainClass::MainClass(int argc, char *argv[], QObject *parent) :
    QObject(parent)
{
    for (int i = 1; i < argc; i++)
        fileList.push_back(QString::fromLocal8Bit(argv[i]));

    QString dir(QString::fromLocal8Bit(argv[1]));

    dir.remove(dir.lastIndexOf('\\'), dir.length());

    dialog = new Dialog(dir);
    connect(dialog, SIGNAL(go(QString)), this, SLOT(go(QString)));
    dialog->show();
}

void MainClass::error(QString str)
{
    QMessageBox::critical(mainWindow, trUtf8("Error"), str);
    emit mainWindow->closing();
    mainWindow->close();
    QApplication::quit();
}

void MainClass::go(QString str)
{
    delete dialog;
    encoder = new Encoder(fileList, str);
    mainWindow = new MainWindow;
    connect(encoder, SIGNAL(progress(QString,int,int)), mainWindow, SLOT(progress(QString,int,int)));
    connect(encoder, SIGNAL(error(QString)), this, SLOT(error(QString)) );
    connect(encoder, SIGNAL(finished()), this, SLOT(finished()));    
    connect(mainWindow, SIGNAL(closing()), encoder, SLOT(interrupt()));

    mainWindow->show();
    encoder->Encode();
}

void MainClass::finished()
{
    QMessageBox::information(mainWindow, trUtf8("Done!"), trUtf8("Done!"));
    mainWindow->close();
    QApplication::quit();
}
