#ifndef __I2C_RGB_INTERFACE_H
#define __I2C_RGB_INTERFACE_H

#include "ui_rgbi2c.h"

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
class RGBI2C : public QWidget
{
private:
    Q_OBJECT

public:
    RGBI2C(QWidget *parent = 0);
    ~RGBI2C();

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
    Ui::RGBI2C ui;
    char general_buffer[1024];
    unsigned char  vPtrUchar[64];
    unsigned char led_arr[10];
    unsigned char lastCmd;
    unsigned char  b[256];

private:
    //Ui::Ui_MainWindow ui;
public slots:
    void ReadDeviceContinouous();
    void scanI2CDevice();
    void ci2c_data_slot();
    void ci2c_result_slot();

    void readCHIP_ID();
    void Enable_RGB();
    void ReadRGB();
    int i2c_read(unsigned char chip_addr, unsigned short int_addr, unsigned short int_addr_len, unsigned char *buff, unsigned short dlen);
    int i2c_write(unsigned char chip_addr, unsigned short int_addr, unsigned short int_addr_len, unsigned char *buff, unsigned short dlen);


Q_SIGNALS:
    void ci2c_data();
    void ci2c_result();
private:
};
//! [1]

#endif
