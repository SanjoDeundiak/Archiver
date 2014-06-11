#ifndef MAINCLASS_H
#define MAINCLASS_H

#include <QObject>

class MainClass : public QObject
{
    Q_OBJECT
public:
    explicit MainClass(int argc, char *argv[], QObject *parent = 0);

signals:
    void go(QString);

public slots:

};

#endif // MAINCLASS_H
