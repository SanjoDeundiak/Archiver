#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit closing();
}

void MainWindow::progress(QString file, int currentProgress, int totalProgress)
{
    ui->totalProgressBar->setValue(totalProgress);
    ui->currentProgressBar->setValue(currentProgress);
    ui->currentFile->setText(file);
}
