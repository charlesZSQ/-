#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QThread>

class CAN_rec_thread: public QThread
{
    Q_OBJECT
public:
    CAN_rec_thread();
signals:
    void display_signal(int);
protected:
    void run();
};

/*class CAN_sen_thread: public QThread
{
    Q_OBJECT
public:
    CAN_sen_thread();
protected:
    void run();
};

class display_thread: public QThread
{
    Q_OBJECT
public:
    display_thread();
protected:
    void run();
};*/


#endif // WORKTHREAD_H
