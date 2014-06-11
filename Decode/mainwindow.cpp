#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QString arcPath, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    path(arcPath)
{
    ui->setupUi(this);
}

void MainWindow::go()
{
    dec = new Decoder(path);

    connect(dec, SIGNAL(progress(QString,int,int)), this, SLOT(progress(QString,int,int)));
    connect(dec, SIGNAL(error(QString)), this, SLOT(error(QString)) );
    connect(dec, SIGNAL(finished()), this, SLOT(finished()));
    connect(this, SIGNAL(closing()), dec, SLOT(interrupt()));

    dec->Decode();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::progress(QString file, int currentProgress, int totalProgress)
{
    ui->totalProgressBar->setValue(totalProgress);
    ui->currentProgressBar->setValue(currentProgress);
    ui->currentFile->setText(file);
}


void MainWindow::error(QString str)
{
    QMessageBox::critical(this, trUtf8("Error"), str);
    emit closing();
    this->close();
    QApplication::quit();
}

void MainWindow::finished()
{
    QMessageBox::information(this, trUtf8("Done!"), trUtf8("Done!"));
    this->close();
    QApplication::quit();
}
