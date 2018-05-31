#ifndef HAL_COMM_H
#define HAL_COMM_H

#include <QMainWindow>
#include <QByteArray>
#include <QList>
#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include <qcombobox.h>

#include "highlighter.h"

#include "codeeditor.h"
#include "shared_struct.h"
#include "ext_udp.h"
#include "console.h"

QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QLCDNumber;
class QTextEdit;
class QLineEdit;
class QListView;
class QTreeItem;
class QListWidgetItem;
class QTreeWidgetItem;
class QTreeWidget;
class QListWidget;
class QListWidgetItem;
class QStackedWidget;
QT_END_NAMESPACE

namespace Ui
{
class HeartBeat;
class SettingsDialog;
}



class HeartBeat : public QMainWindow
{
    Q_OBJECT
private:

public:
    HeartBeat(QWidget *parent = 0);
    ~HeartBeat();
#ifndef LINUX_WAY
    bool winEvent(MSG *m, long *result);
#endif
    bool eventFilter(QObject *object, QEvent *e);
    bool reinit_done;
    unsigned char iMode;
    ioTxtGrpthExchStrct  iGuiUpdate;

    CodeEditor   *editor;
    Console     *cConsole;
    bool        boolSelectReady;

    Ui::HeartBeat      *ui;
    Ui::SettingsDialog *settingsUi;
    QDialog             settingsDialog;


    unsigned char in_data[2048];
    char c_code_data[4095];
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

    void supply_heartbeat();
    void GUI_UPDATE(ioTxtGrpthExchStrct &);
    void timerExpired();
    void delayRAMCodeTimerrExpired();
    void onActionCommand(QString);

    void newScriptSlot();
    void PushRTCTimeSlot();
    void autoExecScriptSlot();

    void slotEditAutoExecScriptC(bool);
    void FoundDataLoggerHWString(QString);
    void HeartBeatTimerSyncSlot();

    void slot_Edit_OR_PUSH_INI(bool x);
    void slot_Edit_OR_PUSH_C_Script(bool x);
    void slot_Show_Script(bool x);
    void slot_Sync_CScript(bool x);
    void slot_Run_Script_In_Device(bool);
    void slot_Reboot(bool x);
    void slot_FwUpdate(bool x);
    void slot_script_load_and_run();
    void slot_script_run();
    void slot_FormatSDCard();

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
    void writeSerialSettings();
    void readSerialSettings();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();

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
    QLabel *qlbl;

    QLabel     *labelVersion;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    QAction *autoExecScript;
    QAction *settingsAct;
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

    QLabel  *connLED;
    QLabel  *commLED;
    QToolBar *formatDisk;

    QTimer  timer;

    QAction *d_ini_script;
    QAction *d_autoexec_script;
    QAction *d_run_script;
    QAction *d_sync;
    QAction *d_ram_script;

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void push_trigger(unsigned char *b, unsigned short len);
    void sig_pullFile(const QString &string);
    void sig_pushFile(const QString &string);
    void sig_script_load_and_run();
    void sig_script_run();
    void do_plot();

    void consoleInp3();
    void consoleInp2();
    void consoleInp1();
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
    void editSerialSettings();
    void slot_ReInitDevList();
    void selectNewIPDevice(int);
    void SaveDeviceIPAddr();
    void CheckDeviceSlot();
    void browse_and_send();
    void getFile();
    void update_code_buffer()
    {
        memcpy(c_code_data, editor->c_code_data, sizeof(c_code_data));
        delayRAMCodeTimer.setInterval(550);
        delayRAMCodeTimer.setSingleShot(true);
        delayRAMCodeTimer.start();
    }
    void comm_start();
    void comm_end();
    void msg_PumpedRcvd();
};

#endif
