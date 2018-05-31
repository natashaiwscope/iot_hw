#include "led.h"

LEDWidget::LEDWidget(QWidget *parent)
    : QLabel(parent),
      onPixmap(":/ledon.png"),
      offPixmap(":/ledoff.png"),
      syncPixmap(":/led_sync.png"),
      ledgPixmap(":/ledg.png"),
      checkPixmap(":/check.png"),
      ledrPixmap(":/ledr.png"),
      ledyPixmap(":/ledy.png")
{
    setPixmap(offPixmap);
    //flashTimer.setInterval(200);
    //flashTimer.setSingleShot(true);
    //connect(&flashTimer, SIGNAL(timeout()), this, SLOT(extinguish()));
};

void LEDWidget::extinguish()
{
    setPixmap(offPixmap);
}

void LEDWidget::flash()
{
    setPixmap(onPixmap);
    flashTimer.start();
}

void LEDWidget::Off()
{
    setPixmap(offPixmap);
}
void LEDWidget::On()
{
    setPixmap(onPixmap);
}
void LEDWidget::Sync()
{
    setPixmap(syncPixmap);
}
void LEDWidget::ledg()
{
    setPixmap(ledgPixmap);
}
void LEDWidget::check()
{
    setPixmap(checkPixmap);
}
void LEDWidget::ledr()
{
    setPixmap(ledrPixmap);
}
void LEDWidget::ledy()
{
    setPixmap(ledyPixmap);
}
