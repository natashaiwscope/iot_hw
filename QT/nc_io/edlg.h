#ifndef _PANEL_H_
#define _PANEL_H_ 1

#include <qtabwidget.h>
#include <QCheckBox>
#include <QSettings>
#include <QMutex>
#include "led.h"
#include "q_ring.h"
#include "shared_struct.h"

class QLCDNumber;
class QComboBox;
class QComboBox;
class SpinBox;
class FSpinBox;
class CheckBox;
class QLineEdit;
class QPushButton;
class QRadioButton;
class QwwLed;
class QCheckBox;
class QwwColorComboBox;

typedef struct
{
    QPushButton *but_Baud;
    QComboBox   *d_BaudRate;

    QPushButton *but_StopBit;
    QComboBox   *d_StopBit;

    QPushButton *but_Parity;
    QComboBox   *d_Parity;

    QPushButton *but_DBits;
    QComboBox   *d_DataBits;

    QPushButton *but_Fmt;
    QComboBox   *d_FmtOptions;

    QPushButton *but_Buff;
    SpinBox    *d_BuffSize;


    QLabel      *fontLabel;

    QPushButton *but_Start;
    QPushButton *but_Stop;
    QPushButton *but_Save;
    QPushButton *but_Font;

    LEDWidget   *led;

    QSettings   *settings;

    unsigned int uart_baud;
    unsigned int uart_stop;
    unsigned int uart_databits;
    unsigned int uart_parity;
    unsigned int uart_format;
    unsigned int uart_dsize;
    QString      fontStr;

    //Ring_Q        rx;
    //Ring_Q        tx;

    unsigned char *rx_buff;
    unsigned char *tx_buff;

    QLCDNumber *lcd_Rx;
    QLCDNumber *lcd_Tx;

    QPushButton *but_RxCnt;
    QPushButton *but_TxCnt;
    QPushButton *but_ClrCnt;
} PAR;

class EDlg: public QTabWidget
{
    Q_OBJECT

public:
    EDlg(QWidget * = NULL);
    ~EDlg();

    PAR           par[4];
    unsigned char b[1288];

    ioExchangeUnion tx_glu;
    ioExchangeUnion rx_glu;

private:
    void          PopulateDlg(QWidget *page, int i);
private:
    QWidget *createUART2Tab(QWidget *);
    QWidget *createUART3Tab(QWidget *);
    QWidget *createUART23Tab(QWidget *);
    QWidget *createRS485Tab(QWidget *);
    SpinBox *d_Elem;
    int     currTab;
    QTimer  guiTimer;

public slots:
    void pushsettings();
    void setFont();
    void timeoutSlot();

    void tabChanged(int);
    void startClicked();
    void stopClicked();
    void saveClicked();

    void updateTxLCD(int, int);
    void updateRxLCD(int, int);

Q_SIGNALS:

    void fontChanged(QFont &, int);
    void push_trigger(unsigned char *b, unsigned short len);

};

#endif
