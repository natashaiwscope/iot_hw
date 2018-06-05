#ifndef __HUMIDITY_INTERFACE_H
#define __HUMIDITY_INTERFACE_H

#include <qwt_plot.h>
#include <qwt_interval.h>
#include <qwt_system_clock.h>
#include "ui_humidity.h"

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
class HUMIDITY : public QWidget
{
private:
    Q_OBJECT

public:
    HUMIDITY(QWidget *parent = 0);
    ~HUMIDITY();

    QList<QColor> colors;
    unsigned char isCont;
    void createPushButtons();
    const static int firstButtonX = 0; //--> topleft corner x value of first button
    const static int firstButtonY = 0; //--> topleft corner y value of first button
    const static int buttonWidth = 50;
    const static int buttonHeight = 30;
    const static int triangleWidth = 30;


private:
    Ui::HUMIDITY ui;
    char general_buffer[1024];
    unsigned char  vPtrUchar[64];
    unsigned char led_arr[10];
    unsigned char lastCmd;
    unsigned char  b[256];

private:
    //Ui::Ui_MainWindow ui;
public slots:



Q_SIGNALS:
private:
};
//! [1]

#endif
