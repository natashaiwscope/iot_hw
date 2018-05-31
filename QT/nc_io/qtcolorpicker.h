#ifndef QTCOLORPICKER_H
#define QTCOLORPICKER_H
#include <QtGui/QPushButton>
#include <QtCore/QString>
#include <QtGui/QColor>

#include <QtGui/QLabel>
#include <QtCore/QEvent>
#include <QtGui/QFocusEvent>

#if defined(Q_WS_WIN)
#  if !defined(QT_QTCOLORPICKER_EXPORT) && !defined(QT_QTCOLORPICKER_IMPORT)
#    define QT_QTCOLORPICKER_EXPORT
#  elif defined(QT_QTCOLORPICKER_IMPORT)
#    if defined(QT_QTCOLORPICKER_EXPORT)
#      undef QT_QTCOLORPICKER_EXPORT
#    endif
#    define QT_QTCOLORPICKER_EXPORT __declspec(dllimport)
#  elif defined(QT_QTCOLORPICKER_EXPORT)
#    undef QT_QTCOLORPICKER_EXPORT
#    define QT_QTCOLORPICKER_EXPORT __declspec(dllexport)
#  endif
#else
#  define QT_QTCOLORPICKER_EXPORT
#endif

class ColorPickerPopup;

class QT_QTCOLORPICKER_EXPORT QtColorPicker : public QPushButton
{
    Q_OBJECT

    Q_PROPERTY(bool colorDialog READ colorDialogEnabled WRITE setColorDialogEnabled)

public:
    QtColorPicker(QWidget *parent = 0,
                  int columns = -1, bool enableColorDialog = true);

    ~QtColorPicker();

    void insertColor(const QColor &color, const QString &text = QString::null, int index = -1);

    QColor currentColor() const;

    QColor color(int index) const;

    void setColorDialogEnabled(bool enabled);
    bool colorDialogEnabled() const;

    void setStandardColors();

    static QColor getColor(const QPoint &pos, bool allowCustomColors = true);

public Q_SLOTS:
    void setCurrentColor(const QColor &col, const QString &text = QString::null);

Q_SIGNALS:
    void colorChanged(const QColor &);

protected:
    void paintEvent(QPaintEvent *e);

private Q_SLOTS:
    void buttonPressed(bool toggled);
    void popupClosed();

private:
    ColorPickerPopup *popup;
    QColor col;
    bool withColorDialog;
    bool dirty;
    bool firstInserted;
};

#endif
