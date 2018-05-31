#ifndef LEDWIDGET_H
#define LEDWIDGET_H

#include <QLabel>
#include <QPixmap>
#include <QTimer>

class LEDWidget : public QLabel
{
    Q_OBJECT
public:
    LEDWidget(QWidget *parent = 0);
public slots:
    void flash();
    void Off();
    void On();

    void Sync();
    void ledg();
    void check();
    void ledr();
    void ledy();


private slots:
    void extinguish();

private:
    QPixmap onPixmap;
    QPixmap offPixmap;
    QPixmap syncPixmap;
    QPixmap ledgPixmap;
    QPixmap checkPixmap;
    QPixmap ledrPixmap;
    QPixmap ledyPixmap;

    QTimer flashTimer;
};

#endif // LEDWIDGET_H
