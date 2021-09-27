#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "match.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void paintEvent(QPaintEvent *) override;
private:
    Ui::Widget *ui;
    QPointF first_Point;
    QPointF sec_Point;
    QPointF third_Point;
    QPointF last_Point;
    QPointF topRight;
    QPointF bottomRight;

    double widthDinamics = 0;
    double highDinamics  = 10;
    double randDouble = 0;
    Match* match;

    void drawTop(QPainter& painter);
    void drawStatic(QPainter& painter);
};
#endif // WIDGET_H
