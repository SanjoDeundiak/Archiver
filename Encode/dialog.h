#ifndef DIALOG_H
#define DIALOG_H

#include <QWidget>

namespace Ui {
class Dialog;
}

class Dialog : public QWidget
{
    Q_OBJECT

public:
    explicit Dialog(QString dir, QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

signals:
    void go(QString);

private:
    QString dir;
    Ui::Dialog *ui;
};

#endif // DIALOG_H
