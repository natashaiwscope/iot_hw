#include <QMainWindow>
#include <QtGui>
#include <QColorDialog>
#include <qapplication.h>
#include <qmainwindow.h>
#include <QtNetwork>
#include <qtoolbar.h>
#include <qcombobox.h>
#include <qwwled.h>

#include "images/connect.xpm"
#include "images/disconnect.xpm"
#include "images/sd.xpm"
#include "images/run.xpm"
#include "images/startscript.xpm"
#include "images/boot.xpm"
#include "images/script_red.xpm"
#include "images/script_grn.xpm"
#include "images/script_yel.xpm"
#include "images/oscilloscope.xpm"

#ifdef LINUX_WAY
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <sys/socket.h>
#include <csignal>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#else
#include <windows.h>
#endif




#include "heartbeat.h"
#include "adio_pin.h"
#include "ci2c_io.h"
#include "rgbi2c.h"

#include "ext_udp.h"

extern "C" void vRegisterCLICommands( void );

//unsigned char  uart_buffer[1024];
//unsigned short uart_buffer_len=0;

HeartBeat::HeartBeat()
{
    printf("starting listerner \n");
    tBar        = new QToolBar();
    adio_PIN    = new ADIO_PIN(this);
    i2cIO       = new CI2C_IO(this);
    rgbIO       = new RGBI2C(this);
    hum         = new HUMIDITY(this);

#ifdef LINUX_WAY
    ext_msgPump = new ExternSig();
#endif

    found = false;
    reinit_done = false;

    iMode = 0;


    createActions();
    //createMenus();
    createToolBars();
    createStatusBar();
    createTabWidget();
    //createDockWindows();

    //setWindowTitle(tr("Data Logger"));

    timerHeartBeat = new QTimer(this);

    connect(timerHeartBeat, SIGNAL(timeout()), this, SLOT(CheckDeviceSlot()));

#ifdef LINUX_WAY
    connect(ext_msgPump, SIGNAL(msg_Pumped()), SIGNAL(msg_Pumped()));
    connect(this, SIGNAL(msg_Pumped()), SLOT(msg_PumpedRcvd()));
#endif
    connect(connLED, SIGNAL(clicked()), this, SLOT(connAction()));
    connect(oscope_ip, SIGNAL(currentIndexChanged(int)), this, SLOT(selectNewIPDevice(int)));

    connect(d_fwupdate, SIGNAL(toggled(bool)), this, SLOT(slot_FwUpdate(bool)));

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

    connect(this, SIGNAL(ci2c_data()),   i2cIO, SIGNAL(ci2c_data()));
    connect(this, SIGNAL(ci2c_result()), i2cIO, SIGNAL(ci2c_result()));
    connect(this, SIGNAL(adio_data()),   adio_PIN, SIGNAL(adio_data()));

    connect(this, SIGNAL(rgb_data()),   rgbIO, SIGNAL(ci2c_data()));
    connect(this, SIGNAL(rgb_result()), rgbIO, SIGNAL(ci2c_result()));


    connect(this, SIGNAL(sig_CSCRIPT_Extern(int)), adio_PIN, SLOT(slot_CSCRIPT_Extern(int)));
    //timerHeartBeat->start(20);
    connect(this, SIGNAL(cexecCommand(const QString &)), this, SLOT(slotExecCommand(const QString &)));
    connect(d_reboot, SIGNAL(toggled(bool)), this, SLOT(slot_Reboot(bool)));


    setit_as_esp8266();
    enter_eth_lib();
    currentTab=0;
    //vRegisterCLICommands();

#ifdef LINUX_WAY
    RegisterWin(getpid());
#else
    RegisterWin(this->winId());
#endif

}

void HeartBeat::CheckDeviceSlot()
{
    static int i = 0;
    static int direction = 0;
    if (us_isDevReady())
    {
        //qDebug() << "Device found\r\n" << endl;
        i++;

        if (direction)
            usLED_On(i);
        else
            usLED_Off(i);

        if (i >= 9)
        {
            i = 0;
            direction ^= 0x01;
        }
        fflush(stdout);
    }

}


void HeartBeat::HeartBeatTimerSyncSlot()
{

}


void HeartBeat::slot_Show_Script(bool x)
{


}

void HeartBeat::connAction()
{
    connLED->setColor(Qt::red);
    //deviceDisconnect();
}


void HeartBeat::slot_Run_Script_In_PC(bool x)
{
    //RunCFile("autoexec.c");
    fn_get_ver();
}


void HeartBeat::slot_Edit_OR_PUSH_INI(bool x)
{
    iMode = 1;
    if (timer.isActive())
    {
        timer.stop();
        d_ini_script->setChecked(false);
        printf("INI Single Click\n");
        Q_EMIT sig_pushFile("autoexec.ini");
    }
    else
    {
        timer.setInterval(550);
        timer.setSingleShot(true);
        timer.start();
    }
}

void HeartBeat::slot_Edit_OR_PUSH_C_Script(bool x)
{
    iMode = 2;

    if (timer.isActive())
    {
        timer.stop();
        d_autoexec_script->setChecked(false);
        Q_EMIT sig_pushFile("autoexec.c");
        printf("C Script Single Click\n");
    }
    else
    {
        timer.setInterval(550);
        timer.setSingleShot(true);
        timer.start();
    }
}

void HeartBeat::delayRAMCodeTimerrExpired()
{
    //printf(c_code_data);
    //fflush(stdout);
    /* time to send code to hardware */


}

void HeartBeat::timerExpired()
{
    if (iMode == 1)
    {
        iMode = 0;
    }
    else if (iMode == 2)
    {
        iMode = 0;
    }
    else if (iMode == 3)
    {
        iMode = 0;
        Q_EMIT sig_script_run();
    }
    else if (iMode == 4)
    {
        QString fileName("autoexec.c");
        iMode = 4;
        if (!fileName.isEmpty())
        {
            QFile file(fileName);
            if (!file.open(QFile::ReadOnly))
            {
                return;
            }

            QByteArray data = file.readAll();
            if (data.size() < 4095)
                memcpy(c_code_data, data.data(), data.size());
            else
                memcpy(c_code_data, data.data(), 4095);

            delayRAMCodeTimer.setInterval(550);
            delayRAMCodeTimer.setSingleShot(true);
            delayRAMCodeTimer.start();
        }
    }

    //if(d_ini_script->isChecked())

    fflush(stdout);
}

//coapUDP=new CoAPComm(oscope_ip->itemText(0));

void HeartBeat::PushRTCTimeSlot()
{

}

void HeartBeat::autoExecScriptSlot()
{
    QDesktopServices desk;
    //desk.openUrl(QUrl("file://HeartBeat.cpp"));
    desk.openUrl(QUrl("mainwindow.cpp"));
}

void HeartBeat::newScriptSlot()
{
    QTreeWidget *rem_Filelist;
    rem_Filelist = new QTreeWidget;

    rem_Filelist->setEnabled(false);
    rem_Filelist->setRootIsDecorated(false);
    rem_Filelist->setHeaderLabels(QStringList() << tr("Name") << tr("Size")  << tr("Time"));

    rem_Filelist->header()->setStretchLastSection(true);
    rem_Filelist->setEnabled(true);
    rem_Filelist->style()->pixelMetric(QStyle::PM_ScrollBarExtent);

    rem_Filelist->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    rem_Filelist->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QList<QTreeWidgetItem *> items;
    for (int i = 0; i < 1; ++i)
    {
        items.append(new QTreeWidgetItem((QTreeWidget *)0, QStringList(QString("item: %1").arg(i))));
        rem_Filelist->insertTopLevelItems(0, items);
    }

    for (int i = 0; i < 5; i++)
    {
        //rem_Filelist->resizeColumnToContents(i);
    }
    printf("new script \n");
    fflush(stdout);
    rem_Filelist->show();
}

void HeartBeat::createActions()
{
    connLED    = new QwwLed(Qt::darkGray, QwwLed::Circular, this);
    connLED->setMinimumSize(30, 30);

    d_connect = new QAction(QIcon(icon_connect), "Connect", this);
    d_connect->setShortcut(tr("C-C"));
    d_connect->setCheckable(true);


    rtcClock = new QAction(QIcon(":/images/rtc.png"), tr("&Sync RTC Clock"), this);
    rtcClock->setShortcuts(QKeySequence::New);
    rtcClock->setStatusTip(tr("Create a new form letter"));
    connect(rtcClock, SIGNAL(triggered()), this, SLOT(PushRTCTimeSlot()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save..."), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the current form letter"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

}
void HeartBeat::SaveDeviceIPAddr()
{

    if (oscope_ip->count())
    {
        //qDebug() << "IP= " << oscope_ip->currentText();
        //usSaveIP(oscope_ip->currentText());
    }
}

void HeartBeat::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addWidget(connLED);
    fileToolBar->addAction(d_connect);
    //fileToolBar->addAction(newScript);
    fileToolBar->addAction(rtcClock);
    //fileToolBar->addAction(autoExecScript);
    //fileToolBar->addAction(saveAct);
    //fileToolBar->addAction(printAct);

    d_ram_script = new QAction(QIcon(script_yel_xpm), "Single Click (Push)/Double Click(Edit)", tBar);
    d_ram_script->setCheckable(true);
    fileToolBar->addAction(d_ram_script);

    d_run_script = new QAction(QIcon(":/images/go.png"), tr("&Run \"C\" Script"), tBar);
    d_run_script->setCheckable(true);
    fileToolBar->addAction(d_run_script);

    sdCard = new QAction(QIcon(":/images/floppy.png"), tr("&SaveIP"), this);
    sdCard->setShortcuts(QKeySequence::Save);
    sdCard->setStatusTip(tr("Save this IP Address"));
    connect(sdCard, SIGNAL(triggered()), this, SLOT(SaveDeviceIPAddr()));

    //runLED    = new QwwLed(Qt::yellow, QwwLed::Circular,this);
    //runLED->setMinimumSize(30,30);
    //fileToolBar->addWidget(runLED);

    //editToolBar = addToolBar(tr("Edit"));
    //editToolBar->addAction(undoAct);

    commLED    = new QwwLed(Qt::darkGray, QwwLed::Circular, this);
    commLED->setMinimumSize(30, 30);
    fileToolBar->addWidget(commLED);


    sendButton = new QPushButton(tr("Send"), this);
    fileToolBar->addWidget(sendButton);

    lineEdit = new QLineEdit(this);
    lineEdit->setFixedWidth(100);

    getButton = new QPushButton(tr("Get"), this);
    fileToolBar->addWidget(getButton);


    fileToolBar->addWidget(lineEdit);

    oscope_ip = new QComboBox(this);
    //oscope_ip->setProperty(AdjustToContents);
    //oscope_ip->setMinimumContentsLength(12);
    oscope_ip->setEditable(true);
    oscope_ip->setSizeAdjustPolicy(QComboBox::AdjustToContents);


    //lcd_Number  = new QLCDNumber(this);
    //QPalette pal(Qt::blue);
    //pal.setColor(QPalette::WindowText, Qt::blue);
    //lcd_Number->setPalette(pal);

    //lcd_Number->setGeometry(QRect(0, 390, 271, 71));
    //lcd_Number->setGeometry(QRect(80, 350, 311, 61));
    //lcd_Number->setSmallDecimalPoint(true);
    //lcd_Number->setNumDigits(2);
    //lcd_Number->setSegmentStyle(QLCDNumber::Flat);
    //lcd_Number->display(0);

    qlbl = new QLabel(this);
    qlbl->setObjectName(QString::fromUtf8("qlbl"));
    qlbl->setGeometry(QRect(30, 30, 821, 61));
    QFont font3;
    font3.setPointSize(16);
    font3.setBold(true);
    font3.setWeight(75);
    qlbl->setFont(font3);

    oscope_ip->setFont(font3);
    lineEdit->setFont(font3);

    qlbl->setText("CNT#0");


    fileToolBar->addWidget(oscope_ip);
    //fileToolBar->addWidget(lcd_Number);
    fileToolBar->addWidget(qlbl);
    fileToolBar->addAction(sdCard);

    //oscope_ip->setIcon(QIcon(oscilloscope));
    //_led3    = new QwwLed(Qt::darkGray, QwwLed::Circular,this);
    //fileToolBar->addWidget(_led3);

    d_ini_script = new QAction(QIcon(script_red), "INI", tBar);
    d_ini_script->setCheckable(true);

    d_autoexec_script = new QAction(QIcon(script_grn_xpm), "AutoExec", tBar);
    d_autoexec_script->setCheckable(true);

    //fileToolBar->addWidget(rem_Filelist);
    fileToolBar->addAction(d_ini_script);
    fileToolBar->addAction(d_autoexec_script);


    d_format = new QAction(QIcon(":/images/format.png"), tr("&Format SD Card"), tBar);
    d_format->setCheckable(true);
    fileToolBar->addAction(d_format);

    d_reboot = new QAction(QIcon(":/images/reboot.png"), tr("&Reboot"), tBar);
    d_reboot->setCheckable(true);
    fileToolBar->addAction(d_reboot);

    d_fwupdate = new QAction(QIcon(":/images/fwupdate.png"), tr("&Firmware"), tBar);
    d_fwupdate->setCheckable(true);
    fileToolBar->addAction(d_fwupdate);

    labelVersion = new QLabel(this);

    QFont font;
    //font.setFamily(QString::fromUtf8("Courier New"));
    font.setPointSize(8);
    font.setBold(true);
    font.setStyleStrategy(QFont::PreferDefault);
    labelVersion->setFont(font);

    fileToolBar->addWidget(labelVersion);
}

//! [8]
void HeartBeat::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}
//! [8]

void HeartBeat::createTabWidget()
{
    tabWidget    = new QTabWidget(this);
    setCentralWidget(tabWidget);

    tabWidget->addTab(adio_PIN, tr("\"GEN IO\""));
    tabWidget->addTab(i2cIO, tr("\"I2C\""));
    tabWidget->addTab(rgbIO, tr("\"RGBI2C\""));
    tabWidget->addTab(hum, tr("\"HUMIDITY\""));
}

void HeartBeat::comm_start()
{
    commLED->setColor(Qt::red);
}

void HeartBeat::comm_end()
{
    commLED->setColor(Qt::yellow);
}

//! [9]
void HeartBeat::createDockWindows()
{
    QDockWidget *dock = new QDockWidget(tr("UART1RX"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    textEdit1 = new QTextEdit(dock);
    dock->setWidget(textEdit1);

    addDockWidget(Qt::RightDockWidgetArea, dock);
    //viewMenu->addAction(dock->toggleViewAction());

    dock = new QDockWidget(tr("UART2RX"), this);
    textEdit2 = new QTextEdit(dock);
    dock->setWidget(textEdit2);

    addDockWidget(Qt::LeftDockWidgetArea, dock);
    //viewMenu->addAction(dock->toggleViewAction());
}

bool HeartBeat::eventFilter(QObject *object, QEvent *e)
{
    if ( e->type() == QEvent::Resize )
    {
        //const QSize &size = ((QResizeEvent *)e)->size();
        //lineEdit->setText("*******");

    }

    return HeartBeat::eventFilter(object, e);
}

void HeartBeat::closeEvent(QCloseEvent *event)
{
    //if (maybeSave()) {
    if (true)
    {
        //writeSettings();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}


void HeartBeat::FoundDataLoggerHWString(QString s)
{
    int i;

    unsigned short len;
    /* device ping received, ask version info */

    for (i = 0u; i < oscope_ip->count(); i++ )
    {
        QString s1 = oscope_ip->itemText(i);
        if (s == s1) found = true;
    }

    if (!found)
    {
        oscope_ip->addItem(s);
        connLED->setColor(Qt::green);
    }

}

void HeartBeat::slotEditAutoExecScriptC(bool n)
{
    if (timer.isActive())
    {
        iMode = 0;
        timer.stop();

        tabWidget->setCurrentIndex(0);
    }
    else
    {
        timer.setInterval(550);
        timer.setSingleShot(true);
        timer.start();
        iMode = 4;
    }
}

void HeartBeat::update_progress(qint64 currPos, qint64 endPos)
{
    char buff[80];
    if (endPos == 0)
    {
        sprintf(buff, "A%d", currPos);
        lineEdit->setText(buff);
    }
    else
    {
        sprintf(buff, "P%d", 100 * currPos / endPos);
        lineEdit->setText(buff);
    }
}

void HeartBeat::getFile()
{
    if (((lineEdit->text()).size()) > 0)
    {
        fileName = lineEdit->text();
        Q_EMIT sig_pullFile(fileName);
        printf("file name\n");
        fflush(stdout);
    }

}

void HeartBeat::browse_and_send()
{
    QFileDialog::Options options;
    QString selectedFilter;
    fileName = QFileDialog::getOpenFileName(this,
                                            tr("QFileDialog::getOpenFileName()"),
                                            "",
                                            tr("All Files (*);;INI Files (*.ini);;C Files (*.c);;C Files (*.log)"),
                                            &selectedFilter,
                                            options);

    if (!fileName.isEmpty())
    {
        Q_EMIT sig_pushFile(fileName);
    }
}

void HeartBeat::slot_script_load_and_run()
{
    unsigned short len;

}

void HeartBeat::slot_script_run()
{
    unsigned short len;
}

HeartBeat::~HeartBeat()
{
    exit_eth_lib();
    fflush(stdout);
    delete tBar;
#ifdef LINUX_WAY
    delete ext_msgPump;
#endif
}

void HeartBeat::msg_PumpedRcvd()
{
    commonMsgHandle();
}

#ifndef LINUX_WAY
bool HeartBeat::winEvent(MSG *m, long *result)
{

    if ((m->message) >= (WM_USER_MSG))
    {
        commonMsgHandle();
        return true;
    }
    else
    {
        switch (m->message)
        {
        case (WM_DEVICECHANGE):
            return true;
        default:
            return false;
        }
    }

    return 0;
}
#endif

void HeartBeat::GUI_UPDATE(ioTxtGrpthExchStrct &)
{
    printf("run-->%s\n", __FUNCTION__);
    fflush(stdout);
}

void HeartBeat::slot_ReInitDevList()
{
    char textbuff[256];
    unsigned short l, ltdev;

    oscope_ip->clear();

    memset(textbuff, '\0', sizeof(textbuff));
    ltdev = usGetConnectedDeviceIP(0, textbuff);

    //qDebug() << "Num Devices=" << ltdev << "\r\n";
    //fflush(stdout);

    if (ltdev == 0) connLED->setColor(Qt::red);
    else
    {
        for (l = 0; l < ltdev; l++)
        {
            memset(textbuff, '\0', sizeof(textbuff));
            ltdev = usGetConnectedDeviceIP(l, textbuff);
            oscope_ip->addItem(textbuff);
            connLED->setColor(Qt::green);
        }
        sprintf(textbuff, "CNT#%d", ltdev);
        qlbl->setText(textbuff);
    }


}

void HeartBeat::commonMsgHandle(void)
{
    unsigned short winMsg, wPar, lPar;
    //unsigned char  ucArr[4];
    float fVal;
    char        strMsg[1023];
    //while(usPullStrWinHLQ(winMsg,ucArr,fVal,strMsg))
    while (usPullStrWinQ(winMsg, wPar, lPar, fVal, strMsg))
    {
        switch (winMsg)
        {
        case WMTM_ARRIVED_MSG:
            //qDebug() << "debug test message";
            //qDebug() << strMsg;
            labelVersion->setText(strMsg);
            //Q_EMIT hw_connected();
            //lineEdit->setText("Scope Connected*******");
            //PullCCFrame(0,&lfrm);
            //memset(buffer1,'\0',100);
            //strcpy(buffer1,strMsg);
            //infolineEdit->setText(buffer1);
            break;
        case WM_CONNECTED:
            fn_get_ver();
            slot_ReInitDevList();
            break;
        case WM_I2C_READ:
            if(currentTab==1)
            {
            Q_EMIT ci2c_data();
            }
            else
            {
            Q_EMIT rgb_data();
            }
            break;
        case WM_GEN_ADIO:
            {
            Q_EMIT adio_data();
            }
            break;
        case WM_I2C_SCAN_RES:
            if(currentTab==1)
            {
            Q_EMIT ci2c_result();
            }
            else
            {
            Q_EMIT rgb_result();
            }
            break;

        case WM_DISCONNECT:
            slot_ReInitDevList();
            break;

        case WM_CHANGE_TAB:
            if (wPar < 2)
            {
                tabWidget->setCurrentIndex(wPar);
            }
            break;

        case WM_PLUGIN_TAB:
            tabWidget->setCurrentIndex(2);
            break;
        case WM_SCRIPT_TAB:
            tabWidget->setCurrentIndex(3);
            break;

        case WM_CLI:
        {
            DEBUG_LINE_OPTION
            Q_EMIT cSigUARTExecResult();
            //uart_buffer_len= local_uart_read(uart_buffer,1024);
            //if(uart_buffer_len) { Q_EMIT cSigUARTExecResult(); }
        }
        break;

        case WM_CSCRIPT_SETTINGS:
            switch (wPar)
            {
            case CSCRIPT_SCOPE:
                Q_EMIT sig_CSCRIPT_SCOPE(wPar);
                break;
            case CSCRIPT_SIGNAL:
                Q_EMIT sig_CSCRIPT_SIGNAL(wPar);
                break;
            case CSCRIPT_GPIO:
                DEBUG_LINE_OPTION
                break;
            case CSCRIPT_I2C:
                DEBUG_LINE_OPTION
                break;
            case CSCRIPT_I2C_READ:
                DEBUG_LINE_OPTION
                break;
            case CSCRIPT_I2C_WRITE:
                DEBUG_LINE_OPTION
                break;
            case CSCRIPT_INCAP:
                DEBUG_LINE_OPTION
                break;
            case CSCRIPT_OUTCAP:
                DEBUG_LINE_OPTION
                break;
            case CSCRIPT_UART:
                DEBUG_LINE_OPTION
                break;
            case CSCRIPT_UART_READ:
                DEBUG_LINE_OPTION
                break;
            case CSCRIPT_UART_WRITE:
                DEBUG_LINE_OPTION
                break;
            case CSCRIPT_LINE:
                lineEdit->setText(strMsg);
                break;
            case CSCRIPT_LED:
            case CSCRIPT_LCD_DEC:
            case CSCRIPT_LCD_HEX:
            case CSCRIPT_LCD_BIN:
            case CSCRIPT_BANNER:
            case CSCRIPT_GRAPH1:
            case CSCRIPT_GRAPH2:
            case CSCRIPT_TEXT:
            case CSCRIPT_CLEAR_TEXT:
                Q_EMIT sig_CSCRIPT_Extern(wPar);
                break;
            default:
                DEBUG_LINE_OPTION
                break;

            };

            break;

        default:

            break;
        }
    }
}

void HeartBeat::slot_FormatSDCard()
{
    unsigned short len;
    printf("format SD card clicked\r\n");
    fflush(stdout);
}

void HeartBeat::selectNewIPDevice(int index)
{
    //usSelectDev(index);
}

void HeartBeat::slot_FwUpdate(bool x)
{
    //push_fw_download_file("../../../scratch1/mcapi/v01/direct.bin");
    push_fw_download_file("/tmp/direct.bin");
#if 0
    QFileDialog::Options options;
    QString selectedFilter;
    fileName = QFileDialog::getOpenFileName(this,
                                            tr("QFileDialog::getOpenFileName()"),
                                            "",
                                            tr("All Files (*);;BIN Files (*.bin)"),
                                            &selectedFilter,
                                            options);

    if (!fileName.isEmpty())
    {
        memset(asciifileName, '\0', sizeof(asciifileName));
        strcpy(asciifileName, fileName.toStdString().c_str());
        push_fw_download_file((const char *)asciifileName);
        //Q_EMIT sig_pushFile(fileName);
    }
#endif
}

static char cli_commands[256];
static char clo_commands[256];
static unsigned char cli_index = 0;

void HeartBeat::slotExecCommand(const QString &s)
{
    QString s1;
    unsigned char xMoreDataToFollow;

    memset(cli_commands, '\0', sizeof(cli_commands));
    //qDebug() << s ;
    s1 = s;
    QByteArray ba = s.toLatin1();
    strcpy(cli_commands, ba.data());

    //qDebug() << cli_commands;
    //xMoreDataToFollow = CLIProcessCmd(cli_commands, clo_commands, 255);
    //s1=clo_commands;
    //qDebug() << clo_commands;
    Q_EMIT cSigExecResult(cli_commands, strlen(cli_commands));
    wake_cli(cli_commands);
}

void HeartBeat::slot_Reboot(bool x)
{
    //Q_EMIT cSigExecResult(s);
    RebootCurrentDevice();
}

void HeartBeat::tabChanged(int i)
{
    currentTab=i;
}
