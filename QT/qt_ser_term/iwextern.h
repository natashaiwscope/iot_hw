#ifndef __PYTHON_INTERFACE_H
#define __PYTHON_INTERFACE_H

#include <qwt_plot.h>
#include <qwt_interval.h>
#include <qwt_system_clock.h>
#include "ui_extern_inf.h"

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
class IWExtern : public QWidget
{
private:
    Q_OBJECT

public:
    IWExtern(QWidget *parent = 0);
    ~IWExtern();
    QwtPlotCurve *pl_Channel_B2;

private:
    Ui::IWExtern ui;
    char general_buffer[1024];
    unsigned char  vPtrUchar[64];

private:
    //Ui::Ui_MainWindow ui;
public slots:
    void Ex1_Procedure(void);
    void Ex2_Procedure(void);
    void Ex3_Procedure(void);
    void BootMode(void);
    void SoftReset(void);
    void slot_CSCRIPT_Extern(int);

    void Banner_Action(int);
    void LED_Action(volatile int, volatile int);
    void Winlog_Action(void);
    void UserIn(void);
private:
};
//! [1]

#endif
