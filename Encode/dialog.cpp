#include "dialog.h"
#include "ui_dialog.h"
#include <QFileDialog>

Dialog::Dialog(QString _dir, QWidget *parent) :
    QWidget(parent),
    dir(_dir),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowFlags( ( (windowFlags() | Qt::CustomizeWindowHint)
                           & ~Qt::WindowMinimizeButtonHint & ~Qt::WindowMaximizeButtonHint) );
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    QString name = ui->lineEdit->text();
    if (!name.endsWith(".dcv", Qt::CaseInsensitive))
        name.append(".dcv");
    emit go(name);
}

void Dialog::on_pushButton_2_clicked()
{
    ui->lineEdit->setText(QFileDialog::getSaveFileName(this, tr("Save File"), dir, tr("Files (*.dcv)")));
}
