#ifndef CONSOLE_H
#define CONSOLE_H

#include <QtGui>
#include <QMainWindow>
#include <QByteArray>
#include <QList>
#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include <qcombobox.h>
#include "ui_settingsdialog.h"

namespace Ui
{
class SettingsDialog;
}


class Console : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit Console(short pNum,QWidget *parent = 0);
    short portNum;
protected:
    //void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void contextMenuEvent(QContextMenuEvent *);
private:
    bool eventFilter(QObject *obj, QEvent *event);
    QString prompt;
    bool isLocked;
    QStringList *history;
    int historyPos;
    char rx_buff[1024];
    char tx_buff[1024];
    char tmp_buff[1024];
    QTimer  dTimer;
    Ui::SettingsDialog *settingsUi;

    void historyAdd(QString);
    void historyBack();
    void historyForward();
signals:
    void onCommand(QString);
    void onChange(QString);
//public slots:
private slots:
    void pollSerial(void);
    void onActionCommand(QString);
public slots:
    void consoleInpSlot();
    void dTimerExpired();
};

#endif // CONSOLE_H
