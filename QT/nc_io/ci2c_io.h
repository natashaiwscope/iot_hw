#ifndef __I2CPYTHON_INTERFACE_H
#define __I2CPYTHON_INTERFACE_H

#include <qwt_plot.h>
#include <qwt_interval.h>
#include <qwt_system_clock.h>
#include "ui_ci2c_io.h"

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
class CI2C_IO : public QWidget
{
private:
    Q_OBJECT

public:
    CI2C_IO(QWidget *parent = 0);
    ~CI2C_IO();

    QList<QColor> colors;
    unsigned char isCont;
    void paintEvent(QPaintEvent *event); //--> drawing triangles
    void createPushButtons();
    const static int firstButtonX = 0; //--> topleft corner x value of first button
    const static int firstButtonY = 0; //--> topleft corner y value of first button
    const static int buttonWidth = 50;
    const static int buttonHeight = 30;
    const static int triangleWidth = 30;


private:
    Ui::CI2C_IO ui;
    char general_buffer[1024];
    unsigned char  vPtrUchar[64];
    unsigned char led_arr[10];

private:
    //Ui::Ui_MainWindow ui;
public slots:
    void ReadDeviceContinouous();
    void scanI2CDevice();
    void ci2c_data_slot();
    void ci2c_result_slot();
Q_SIGNALS:
    void ci2c_data();
    void ci2c_result();
private:
};
//! [1]

#endif
