#ifndef HAL_COMM_H
#define HAL_COMM_H

#include <QMainWindow>
#include <QByteArray>
#include <QList>
#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include <qcombobox.h>
#include <qwwled.h>

#include "highlighter.h"
//#include "euart_main.h"
#include "adio_pin.h"
#include "ci2c_io.h"
#include "rgbi2c.h"
#include "humidity.h"

#include "shared_struct.h"
#include "ext_udp.h"

QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QLCDNumber;
class QTextEdit;
class QLineEdit;
class QwwLed;
class QListView;
class QTreeItem;
class QListWidgetItem;
class QTreeWidgetItem;
class QTreeWidget;
class QListWidget;
class QListWidgetItem;
class QStackedWidget;
QT_END_NAMESPACE

class HeartBeat : public QMainWindow
{
    Q_OBJECT
private:

public:
    HeartBeat();
    ~HeartBeat();
#ifndef LINUX_WAY
    bool winEvent(MSG *m, long *result);
#endif
    bool eventFilter(QObject *object, QEvent *e);
    bool reinit_done;
    unsigned char iMode;
    ioTxtGrpthExchStrct  iGuiUpdate;

    ADIO_PIN     *adio_PIN;
    CI2C_IO     *i2cIO;
    RGBI2C      *rgbIO;
    HUMIDITY    *hum;

    unsigned char in_data[2048];
    char c_code_data[4095];
    char asciifileName[1024];
    unsigned char buff[2048];
    unsigned short len;
    int coap_client_read(unsigned char *ucbuff);
    unsigned short ubufflen;
    QString  addr_from_broadcast;
    QString fileName;
    QString fname;

signals:
    void responseRcvd();
    void msg_Pumped();
    void finished();



private slots:

    void GUI_UPDATE(ioTxtGrpthExchStrct &);
    void timerExpired();
    void delayRAMCodeTimerrExpired();

    void newScriptSlot();
    void PushRTCTimeSlot();
    void autoExecScriptSlot();

    void slotEditAutoExecScriptC(bool);
    void FoundDataLoggerHWString(QString);
    void HeartBeatTimerSyncSlot();

    void slot_Edit_OR_PUSH_INI(bool x);
    void slot_Edit_OR_PUSH_C_Script(bool x);
    void slot_Show_Script(bool x);
    void slot_Run_Script_In_PC(bool x);
    void slot_Reboot(bool x);
    void slot_FwUpdate(bool x);
    void connAction();
    void slot_script_load_and_run();
    void slot_script_run();
    void slot_FormatSDCard();
    void slot_ReInitDevList();
    void selectNewIPDevice(int);

private:
    QTimer      delayRAMCodeTimer;
    QToolBar    *tBar;
#ifdef LINUX_WAY
    ExternSig   *ext_msgPump;
#endif
#ifdef NO_ANALOG_FEATURE
    Oscope      *oscope;
    Osignal     *osignal;
#endif
    QTabWidget  *tabWidget;
    void commonMsgHandle();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
    void createTabWidget();

    bool found;
    Highlighter *highlighter;

    QTextEdit *scratch_pad1;
    QTextEdit *scratch_pad2;
    QTextEdit *scratch_pad3;

    QTextEdit *textEdit1;
    QTextEdit *textEdit2;
    QTextEdit *textEdit3;
    QListWidget *customerList;
    QListWidget *paragraphsList;
    QComboBox    *oscope_ip;
    //QLCDNumber *lcd_Number;
    QLabel *qlbl;

    //QMenu *fileMenu;
    //QMenu *editMenu;
    //QMenu *viewMenu;
    //QMenu *helpMenu;
    QLineEdit  *lineEdit;
    QLabel     *labelVersion;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    QAction *autoExecScript;
    //QAction *newScript;
    QAction *rtcClock;

    QAction *saveAct;
    QAction *sdCard;
    //QAction *printAct;
    //QAction *undoAct;
    //QAction *aboutAct;
    //QAction *aboutQtAct;
    //QAction *quitAct;

    QAction *d_connect;
    QAction *d_format;
    QAction *d_reboot;
    QAction *d_fwupdate;

    QwwLed  *connLED;
    QwwLed  *commLED;
    QwwLed  *fwUpgrade;
    //QwwLed  *runLED;
    //QwwLed  *_led3;
    QToolBar *formatDisk;

    QTimer  timer;

    QAction *d_ini_script;
    QAction *d_autoexec_script;
    QAction *d_run_script;
    QAction *d_ram_script;

    QTimer *timerHeartBeat;
    QPushButton *sendButton;
    QPushButton *getButton;
    int currentTab;


protected:
    void closeEvent(QCloseEvent *event);

signals:
    void push_trigger(unsigned char *b, unsigned short len);
    void sig_pullFile(const QString &string);
    void sig_pushFile(const QString &string);
    void sig_script_load_and_run();
    void sig_script_run();
    void do_plot();

    void sig_CSCRIPT_SCOPE(int);
    void sig_CSCRIPT_SIGNAL(int);
    void sig_CSCRIPT_GPIO(int);
    void sig_CSCRIPT_I2C(int);
    void sig_CSCRIPT_I2C_READ(int);
    void sig_CSCRIPT_I2C_WRITE(int);
    void sig_CSCRIPT_INCAP(int);
    void sig_CSCRIPT_OUTCAP(int);
    void sig_CSCRIPT_UART(int);
    void sig_CSCRIPT_UART_READ(int);
    void sig_CSCRIPT_UART_WRITE(int);
    void sig_CSCRIPT_Extern(int);



public slots:
    void slotExecCommand(const QString &s);
    void SaveDeviceIPAddr();
    void CheckDeviceSlot();
    void update_progress(qint64, qint64);
    void browse_and_send();
    void getFile();
    void comm_start();
    void comm_end();
    void msg_PumpedRcvd();
    void tabChanged(int);

public:
Q_SIGNALS:
    void adio_data();

    void ci2c_data();
    void ci2c_result();

    void rgb_data();
    void rgb_result();

    void cexecCommand(const QString &s);
    void cSigExecResult(const QString &, int len);
    void cSigUARTExecResult();
};

#endif
