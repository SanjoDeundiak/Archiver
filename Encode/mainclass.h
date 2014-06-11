#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <QObject>
#include "mainwindow.h"
#include "dialog.h"
#include <QApplication>
#include "Encoder.h"


class MainClass : public QObject
{
    Q_OBJECT

private:
    MainWindow *mainWindow;
    Dialog *dialog;
    Encoder *encoder;

    vector<QString> fileList;

public:
    explicit MainClass(int argc, char *argv[], QObject *parent = 0);
    ~MainClass()
    {
        delete mainWindow;
        delete encoder;
    }

signals:

public slots:
    void go(QString str);
    void error(QString str);
    void finished();

};

#endif // MAINCLASS_H
