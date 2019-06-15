#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


//系统时间和计时器
#include <qdatetime.h>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void display(int);

private slots:
    void timerUpdate(void);    //更新当前时间函数
    int on_connectCAN_clicked();    //CAN连接按钮
    int on_Measure_clicked();   //测量按钮

    void on_startMeasure_2_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer = new QTimer(this);
};


#endif // MAINWINDOW_H
