#include <QMainWindow>
#include <QtGui>
#include <QColorDialog>
#include <qapplication.h>
#include <qmainwindow.h>
#include <QtNetwork>
#include <qtoolbar.h>
#include <qcombobox.h>

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
#include "console.h"

#include "ext_udp.h"
#define SERIAL_INI  "serial.ini"

void HeartBeat::supply_heartbeat()
{
    heartbeat_1ms();
}

HeartBeat::HeartBeat(QWidget *parent): settingsUi(new Ui::SettingsDialog)
{
    printf("starting listerner \n");
    tBar          = new QToolBar();

    editor         = new CodeEditor(0);
    cConsole3      = new Console(3,this);
    cConsole2      = new Console(2,this);
    cConsole1      = new Console(1,this);

    settingsUi->setupUi(&settingsDialog);

#ifdef LINUX_WAY
    ext_msgPump = new ExternSig();
#endif

    found = false;
    reinit_done = false;

    iMode = 0;

    createActions();
    createToolBars();
    createStatusBar();

    tabWidget    = new QTabWidget(this);
    setCentralWidget(tabWidget);

    tabWidget->addTab(cConsole1, tr("\"(RS485) UART1\""));
    tabWidget->addTab(cConsole2, tr("\"Console UART2\""));
    tabWidget->addTab(cConsole3, tr("\"Console UART3\""));
    tabWidget->addTab(editor,  tr("\"Edit\" autoexec.c \"C\""));

    tabWidget->setCurrentIndex(0);

    connect(this, SIGNAL(consoleInp3()), cConsole3, SLOT(consoleInpSlot()));
    connect(this, SIGNAL(consoleInp2()), cConsole2, SLOT(consoleInpSlot()));
    connect(this, SIGNAL(consoleInp1()), cConsole1, SLOT(consoleInpSlot()));

#ifdef LINUX_WAY
    connect(ext_msgPump, SIGNAL(msg_Pumped()), SIGNAL(msg_Pumped()));
    connect(this, SIGNAL(msg_Pumped()), SLOT(msg_PumpedRcvd()));
#endif
    connect(oscope_ip, SIGNAL(currentIndexChanged(int)), this, SLOT(selectNewIPDevice(int)));


    connect(editor, SIGNAL(code_edit_text_changed()), this, SLOT(update_code_buffer()));
    connect(&delayRAMCodeTimer, SIGNAL(timeout()), this, SLOT(delayRAMCodeTimerrExpired()));

    connect(d_ram_script, SIGNAL(toggled(bool)), this, SLOT(slot_Show_Script(bool)));
    connect(d_sync, SIGNAL(toggled(bool)), this, SLOT(slot_Sync_CScript(bool)));
    connect(d_run_script, SIGNAL(toggled(bool)), this, SLOT(slot_Run_Script_In_Device(bool)));


    connect(d_reboot, SIGNAL(toggled(bool)), this, SLOT(slot_Reboot(bool)));
    connect(d_fwupdate, SIGNAL(toggled(bool)), this, SLOT(slot_FwUpdate(bool)));

#ifdef LINUX_WAY
    RegisterWin(getpid());
#else
    RegisterWin(this->winId());
#endif

    enter_eth_lib();

}

void HeartBeat::CheckDeviceSlot()
{
    static int i = 0;
    static int direction = 0;
#if 0
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
#endif

}


void HeartBeat::HeartBeatTimerSyncSlot()
{

}


void HeartBeat::slot_Show_Script(bool x)
{

    editor->setMode(0);
    editor->setFileName("autoexec.c");
    editor->show();
    editor->setWindowTitle(QObject::tr("AutoExec.C"));
    tabWidget->setCurrentIndex(3);
}

void HeartBeat::slot_Run_Script_In_Device(bool x)
{
    RunEmbCScript();
}

void HeartBeat::slot_Sync_CScript(bool x)
{
    push_download_cfile("autoexec.c");
}

void HeartBeat::slot_Reboot(bool x)
{
    RebootCurrentDevice();
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
        editor->setMode(0);
        editor->setFileName("autoexec.ini");
        editor->show();
        editor->setWindowTitle(QObject::tr("AutoExec.INI"));
        iMode = 0;
    }
    else if (iMode == 2)
    {
        editor->setMode(0);
        editor->setFileName("autoexec.c");
        editor->show();
        editor->setWindowTitle(QObject::tr("AutoExec.C"));
        iMode = 0;
    }
    else if (iMode == 3)
    {
        editor->setMode(0);
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
    connLED    = new QLabel(this);
    connLED->setMinimumSize(30, 30);

    commLED    = new QLabel(this);
    commLED->setMinimumSize(30, 30);

    connLED->setStyleSheet("background: red");
    commLED->setStyleSheet("background: red");

    d_connect = new QAction(QIcon(icon_connect), "Connect", this);
    d_connect->setShortcut(tr("C-C"));
    d_connect->setCheckable(true);

    settingsAct = new QAction(QIcon(":/images/settings.png"), tr("Serial Se&ttings"), this);
    settingsAct->setShortcut(tr("Ctrl+Shift+T"));
    settingsAct->setStatusTip(tr("Edit Kompotr settings"));
    QObject::connect(settingsAct, SIGNAL(triggered()), this, SLOT(editSerialSettings()));

#if 0
    newScript = new QAction(QIcon(":/images/new.png"), tr("&New \"C\" Script"), this);
    newScript->setShortcuts(QKeySequence::New);
    newScript->setStatusTip(tr("Create a new form letter"));
    connect(newScript, SIGNAL(triggered()), this, SLOT(newScriptSlot()));

    autoExecScript = new QAction(QIcon(":/images/new.png"), tr("&AutoExec \"C\" Script"), this);
    autoExecScript->setShortcuts(QKeySequence::New);
    autoExecScript->setStatusTip(tr("Create a new form letter"));
    connect(autoExecScript, SIGNAL(triggered()), this, SLOT(autoExecScriptSlot()));
#endif

    rtcClock = new QAction(QIcon(":/images/rtc.png"), tr("&Sync RTC Clock"), this);
    rtcClock->setShortcuts(QKeySequence::New);
    rtcClock->setStatusTip(tr("Create a new form letter"));
    connect(rtcClock, SIGNAL(triggered()), this, SLOT(PushRTCTimeSlot()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save..."), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the current form letter"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

#if 0
    printAct = new QAction(QIcon(":/images/print.png"), tr("&Print..."), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the current form letter"));
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));


    undoAct = new QAction(QIcon(":/images/undo.png"), tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last editing action"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
#endif
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
    fileToolBar->addAction(rtcClock);
    fileToolBar->addAction(settingsAct);

    d_ram_script = new QAction(QIcon(script_yel_xpm), "Single Click (Push)/Double Click(Edit)", tBar);
    d_ram_script->setCheckable(true);
    fileToolBar->addAction(d_ram_script);


    d_sync = new QAction(QIcon(":/images/led_sync.png"), tr("&Download \"C\" Script"), tBar);
    d_sync->setCheckable(true);
    fileToolBar->addAction(d_sync);

    d_run_script = new QAction(QIcon(":/images/go.png"), tr("&Run \"C\" Script"), tBar);
    d_run_script->setCheckable(true);
    fileToolBar->addAction(d_run_script);


    sdCard = new QAction(QIcon(":/images/floppy.png"), tr("&SaveIP"), this);
    sdCard->setShortcuts(QKeySequence::Save);
    sdCard->setStatusTip(tr("Save this IP Address"));
    connect(sdCard, SIGNAL(triggered()), this, SLOT(SaveDeviceIPAddr()));

    fileToolBar->addWidget(commLED);

    oscope_ip = new QComboBox(this);

    //oscope_ip->setEditable(true);
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
    font3.setPointSize(14);
    font3.setBold(true);
    font3.setWeight(75);
    qlbl->setFont(font3);

    oscope_ip->setFont(font3);
    //lineEdit->setFont(font3);

    qlbl->setText("CNT#0");


    fileToolBar->addWidget(oscope_ip);
    //fileToolBar->addWidget(lcd_Number);
    fileToolBar->addWidget(qlbl);
    fileToolBar->addAction(sdCard);

    labelVersion = new QLabel(this);
    QFont font;
    font.setPointSize(8);
    font.setBold(true);
    font.setStyleStrategy(QFont::PreferDefault);
    labelVersion->setFont(font);
    fileToolBar->addWidget(labelVersion);

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


void HeartBeat::comm_start()
{
   connLED->setStyleSheet("background: red");
}

void HeartBeat::comm_end()
{
   connLED->setStyleSheet("background: yellow");
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
        connLED->setStyleSheet("background: lightGreen");
    }

}

void HeartBeat::slotEditAutoExecScriptC(bool n)
{
    //editor=new CodeEditor("autoexec.c");
    if (timer.isActive())
    {
        iMode = 0;
        timer.stop();

        editor->setMode(1);
        editor->setFileName("autoexec.c");
        editor->show();
        editor->setWindowTitle(QObject::tr("AutoExec.C"));
        tabWidget->setCurrentIndex(2);
    }
    else
    {
        timer.setInterval(550);
        timer.setSingleShot(true);
        timer.start();
        iMode = 4;
    }
}


void HeartBeat::getFile()
{

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
    osDelay(100);

    fflush(stdout);
    delete tBar;
    delete editor;
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
        case WM_CONSOLE1:
            Q_EMIT consoleInp1();
            //qDebug() << "console1 rx" << endl; fflush(stdout);
            break;
        case WM_CONSOLE2:
            Q_EMIT consoleInp2();
            //qDebug() << "console2 rx" << endl; fflush(stdout);
            break;
        case WM_CONSOLE3:
            /* data just arrived from remote device send signal */
            Q_EMIT consoleInp3();
            //qDebug() << "console3 rx" << endl; fflush(stdout);
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
        case WM_CONSOLE:
            Q_EMIT consoleInp3();
            //qDebug() << "console message rx" << endl;
            //fflush(stdout);
            break;
        case WM_WIEGAND:
            //DEBUG_LINE_OPTION;
            switch (wPar)
            {
            default:
                break;

            };

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
                //lineEdit->setText(strMsg);
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
    if(oscope_ip->count())
    {
        usSelectDevIndex(index);
    }
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


void HeartBeat::onActionCommand(QString s)
{
    unsigned char buff[1000];
    unsigned short len = 0;


    qDebug() << "cmd=" << s << "\n";
    fflush(stdout);
    memset(buff, '\0', sizeof(buff));
    Q2C(s, (char *)buff);
    len = strlen((char *)buff);
    //local_uart_write(buff,len);
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

    if (ltdev == 0) connLED->setStyleSheet("background: red");
    else
    {
        for (l = 0; l < ltdev; l++)
        {
            memset(textbuff, '\0', sizeof(textbuff));
            ltdev = usGetConnectedDeviceIP(l, textbuff);
            oscope_ip->addItem(textbuff);
            connLED->setStyleSheet("background: green");
        }
        sprintf(textbuff, "CNT#%d", ltdev);
        qlbl->setText(textbuff);
    }


}

void HeartBeat::editSerialSettings()
{
    readSerialSettings();
    if (settingsDialog.exec() == QDialog::Accepted)
    {
        writeSerialSettings();
    }
}

void HeartBeat::writeSerialSettings()
{
    QSettings settings(SERIAL_INI, QSettings::IniFormat);
    settings.beginGroup("device");
    settings.setValue("baud",   settingsUi->BaudRateComboBox->currentIndex());
    settings.setValue("dbits",  settingsUi->DataBitsComboBox->currentIndex());
    settings.setValue("sbits",  settingsUi->StopBitsComboBox->currentIndex());
    settings.setValue("parity", settingsUi->ParityComboBox->currentIndex());
    settings.endGroup();
    settings.sync();

    fun_uart_term(0, settingsUi->BaudRateComboBox->currentIndex(), settingsUi->StopBitsComboBox->currentIndex(), settingsUi->DataBitsComboBox->currentIndex(), settingsUi->ParityComboBox->currentIndex());

}

void HeartBeat::readSerialSettings()
{
    QSettings settings(SERIAL_INI, QSettings::IniFormat);
    settings.beginGroup("device");

    settingsUi->BaudRateComboBox->setCurrentIndex(settings.value("baud", 0).toInt());
    settingsUi->DataBitsComboBox->setCurrentIndex(settings.value("dbits", 0).toInt());
    settingsUi->StopBitsComboBox->setCurrentIndex(settings.value("sbits", 0).toInt());
    settingsUi->ParityComboBox->setCurrentIndex(settings.value("parity", 0).toInt());
}

