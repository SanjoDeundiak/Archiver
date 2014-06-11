#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "Decoder.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString arcPath, QWidget *parent = 0);
    ~MainWindow();

private:
    Decoder *dec;
    Ui::MainWindow *ui;
    bool flag = false;
    QString path;

signals:
    void closing();
public:
    Q_INVOKABLE void go();

public slots:    
    void progress(QString, int, int);
    void error(QString str);
    void finished();
};

#endif // MAINWINDOW_H
