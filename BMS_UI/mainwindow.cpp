#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ControlCAN.h"
#include "can_init.h"
#include "workthread.h"

//线程类实例化
CAN_rec_thread CANrecThread;
//CAN_sen_thread CANsenThread;
//display_thread DisplayThread;

bool Measure_OK = FALSE;     //测量线程开始/结束判断
bool ConReg_check = FALSE;

QString GPIO[2];
QString REFON[2];
QString SWTRD[2];
QString DCTO[2];
QString ADCOPT[2];
QString VUV[2];
QString VOV[2];
QString DCC[2];

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Measure->setDisabled(true);     //不允许测量
    ui->openCheck1->display("00000000");    //初始化显示
    ui->openCheck2->display("00000000");

    //信号和槽函数连接
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));      //时间更新槽
    connect(&CANrecThread,SIGNAL(display_signal(int)),this,SLOT(display(int)));       //信号触发显示函数
    timer->start(1000);         //开始计时
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*******时间采集部分*******/
void MainWindow::timerUpdate(void)
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    ui->CurrentTime->setText(str);
}

/*********显示部分**********/
void MainWindow::display(int a)
{
    switch(a)
    {
    case 0:

        break;
    case 1:
        ui->currentStatus->setText(tr("接收数据出错（1）"));
        break;
    case 2:
        ui->currentStatus->setText(tr("接收数据出错（2）"));
        break;
    case 3:
        ui->currentStatus->setText(tr("正在测量..."));
        ui->lcdVol1_1->display(Display_Vol[0]);        //显示数据
        ui->lcdVol1_2->display(Display_Vol[1]);
        ui->lcdVol1_3->display(Display_Vol[2]);
        ui->lcdVol1_4->display(Display_Vol[3]);
        ui->lcdVol1_5->display(Display_Vol[4]);
        ui->lcdVol1_6->display(Display_Vol[5]);
        ui->lcdVol1_7->display(Display_Vol[6]);
        ui->lcdVol1_8->display(Display_Vol[7]);

        ui->lcdVol2_1->display(Display_Vol[8]);        //显示数据
        ui->lcdVol2_2->display(Display_Vol[9]);
        ui->lcdVol2_3->display(Display_Vol[10]);
        ui->lcdVol2_4->display(Display_Vol[11]);
        ui->lcdVol2_5->display(Display_Vol[12]);
        ui->lcdVol2_6->display(Display_Vol[13]);
        ui->lcdVol2_7->display(Display_Vol[14]);
        ui->lcdVol2_8->display(Display_Vol[15]);


        ui->openCheck1->display(Display_Open[0]);
        ui->openCheck2->display(Display_Open[1]);

        ui->lcdTemp1->display(Display_Temp[0]);
        ui->lcdTemp2->display(Display_Temp[1]);
        break;

    case 4:
        ui->GPIO1->setText(GPIO[0]);
        ui->REFON1->setText(REFON[0]);
        ui->SWTRD1->setText(SWTRD[0]);
        ui->DCTO1->setText(DCTO[0]);
        ui->ADCOPT1->setText(ADCOPT[0]);
        ui->VUV1->setText(VUV[0]);
        ui->VOV1->setText(VOV[0]);
        ui->DCC1->setText(DCC[0]);

        ui->GPIO2->setText(GPIO[1]);
        ui->REFON2->setText(REFON[1]);
        ui->SWTRD2->setText(SWTRD[1]);
        ui->DCTO2->setText(DCTO[1]);
        ui->ADCOPT2->setText(ADCOPT[1]);
        ui->VUV2->setText(VUV[1]);
        ui->VOV2->setText(VOV[1]);
        ui->DCC2->setText(DCC[1]);

        ui->currentStatus->setText(tr("控制寄存器信息返回成功！"));
        Measure_OK = TRUE;
        ConReg_check = FALSE;
        CANrecThread.start();       //开始接收线程
        break;
    default:
        break;
    }
    DisplayFlag = 0;
}

/*********打开/关闭CAN接口按键***********/
int MainWindow::on_connectCAN_clicked()
{
    int flag = 0;

    if(ui->connectCAN->text()==tr("打开接口"))
    {
        flag = VCI_OpenDevice(VCI_USBCAN1,0,0);     //打开设备
        if(flag != STATUS_OK)
        {
            ui->currentStatus->setText(tr("打开接口失败"));
            return FALSE;
        }

        ui->currentStatus->setText(tr("初始化接口..."));

        flag = VCI_InitCAN(VCI_USBCAN1,0,0,&CANinitData);       //初始化设备
        if(flag != STATUS_OK)
        {
            ui->currentStatus->setText(tr("初始化接口失败"));
            return FALSE;
        }

        ui->Measure->setEnabled(true);      //允许测量
        ui->connectCAN->setText(tr("关闭接口"));
        ui->currentStatus->setText(tr("接口已打开！"));
        return 0;
    }

    if(ui->connectCAN->text()==tr("关闭接口"))
    {
        flag = VCI_CloseDevice(VCI_USBCAN1,0);      //关闭CAN接口
        if(flag != STATUS_OK)
        {
            ui->currentStatus->setText(tr("关闭接口失败"));
            return FALSE;
        }

        ui->Measure->setDisabled(true);
        ui->connectCAN->setText(tr("打开接口"));
        ui->currentStatus->setText(tr("接口已关闭！"));
        return 0;
    }
    return 0;
}

/**********开始测量按键******************/
int MainWindow::on_Measure_clicked()
{
    int flag;

    if(ui->Measure->text()==tr("开始测量"))
     {
        ui->connectCAN->setDisabled(true);
        ui->Measure->setText(tr("停止测量"));
        ui->currentStatus->setText(tr("正在测量..."));

        flag = VCI_StartCAN(VCI_USBCAN1,0,0);
        if(flag != STATUS_OK)
        {
            ui->currentStatus->setText(tr("通信失败！"));
            return FALSE;
        }

        flag = VCI_ClearBuffer(VCI_USBCAN1,0,0);
        if(flag != STATUS_OK)
        {
            ui->currentStatus->setText(tr("清除缓存失败！"));
            return FALSE;
        }

        flag = VCI_Transmit(VCI_USBCAN1,0,0,&StartMonitor,1);       //发送开始测量命令
        if(flag != 1)
        {
            ui->currentStatus->setText(tr("发送测量命令失败！"));
            return FALSE;
        }

        Measure_OK = TRUE;
        ConReg_check = FALSE;
        CANrecThread.start();       //开始接收线程

        return 0;
     }

    if(ui->Measure->text()==tr("停止测量"))
    {
        ui->connectCAN->setEnabled(true);
        ui->Measure->setText(tr("开始测量"));

        Measure_OK = FALSE;             //中断线程采集循环
        ConReg_check = FALSE;

        while(CANrecThread.isRunning());    //如果线程正在运行，则等待结束

        flag = VCI_Transmit(VCI_USBCAN1,0,0,&Break,1);       //发送停止测量命令
        if(flag != 1)
        {
            ui->currentStatus->setText(tr("发送停止测量命令失败！"));
            return FALSE;
        }

        ui->currentStatus->setText(tr("停止测量!"));

        ui->lcdVol1_1->display(0);        //数据归零
        ui->lcdVol1_2->display(0);
        ui->lcdVol1_3->display(0);
        ui->lcdVol1_4->display(0);
        ui->lcdVol1_5->display(0);
        ui->lcdVol1_6->display(0);
        ui->lcdVol1_7->display(0);
        ui->lcdVol1_8->display(0);

        ui->lcdVol2_1->display(0);        //数据归零
        ui->lcdVol2_2->display(0);
        ui->lcdVol2_3->display(0);
        ui->lcdVol2_4->display(0);
        ui->lcdVol2_5->display(0);
        ui->lcdVol2_6->display(0);
        ui->lcdVol2_7->display(0);
        ui->lcdVol2_8->display(0);

        ui->openCheck1->display("00000000");
        ui->openCheck2->display("00000000");

        ui->lcdTemp1->display(0);
        ui->lcdTemp2->display(0);

        return 0;
    }
    return 0;
}


void MainWindow::on_startMeasure_2_clicked()
{
    ConReg_check = FALSE;
    Measure_OK = FALSE;
    while(CANrecThread.isRunning());    //如果线程正在运行，则等待结束
    ui->currentStatus->setText(tr("正在返回控制寄存器信息...请等待"));

    ConReg_check = TRUE;        //读寄存器部分开启
    CANrecThread.start();       //开始接收线程

}
