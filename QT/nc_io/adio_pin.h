#ifndef __PYTHON_INTERFACE_H
#define __PYTHON_INTERFACE_H

#include "ui_adio_pin.h"

QT_BEGIN_NAMESPACE
class Plot;
class QwtPlotCurve;
class Knob;
class WheelBox;
class QLineEdit;
class  QtColorPicker;
class QGroupBox;
class QGridLayout;
class QGroupBox;
QT_END_NAMESPACE
//! [0]

//! [1]
class ADIO_PIN : public QWidget
{
private:
    Q_OBJECT

public:
    ADIO_PIN(QWidget *parent = 0);
    ~ADIO_PIN();
    QwtPlotCurve *pl_Channel_B2;

private:
    Ui::ADIO_PIN ui;
    char general_buffer[1024];
    unsigned char  vPtrUchar[64];
    unsigned char led_arr[10];
    unsigned short mode1;
    unsigned short mode2;
    unsigned short mode3;
    unsigned short mode4;

private:
    //Ui::Ui_MainWindow ui;
public slots:
    void selectIndex1(int index);
    void selectIndex2(int index);
    void selectIndex3(int index);
    void selectIndex4(int index);
    void led1Action();
    void led2Action();
    void led3Action();
    void led4Action();
    void led5Action();
    void led6Action();
    void Ex1_Procedure(void);
    void Ex2_Procedure(void);
    void Ex3_Procedure(void);
    void BootMode(void);
    void slot_CSCRIPT_Extern(int);

    void Banner_Action(int);
    void LED_Action(volatile int, volatile int);
    void Winlog_Action(void);
    void UserIn(void);

    void RefreshADIO();
    void adio_data_slot();
private:
Q_SIGNALS:
    void adio_data();
};
//! [1]

#endif
