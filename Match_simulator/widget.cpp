#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    first_Point.setX(100);
    first_Point.setY(125);
    sec_Point.setX(100);
    sec_Point.setY(135);
    third_Point.setX(400);
    third_Point.setY(135);
    last_Point.setX(400);
    last_Point.setY(125);
    topRight.setX(400);
    topRight.setY(125);
    bottomRight.setX(400);
    bottomRight.setY(135);
    match = new Match(QPair<int, int>(third_Point.x(), third_Point.y()), QPair<int, int>(third_Point.x()+20, third_Point.y()), 80);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::drawTop(QPainter& painter)
{
    match->setRandStruct();
    QPainterPath pp;
    auto points = match->getTop();
    if (points.size() % 2 == 0)
        points.remove(points.size() / 2);
    QPoint& first = *points.begin();
    pp.addPolygon(QPolygon(QVector<QPoint>(1, first)));

    for (auto it = points.begin() + 1; it != points.end() && it + 1 != points.end(); it += 2)
    {
        pp.quadTo(*it, *(it + 1));
    }

    QPainterPath pp_base;
    auto base = match->getBase();
    pp_base.addEllipse(base);

    pp_base.addRect(QRect(QPoint(window()->geometry().left(), base.center().y()), window()->geometry().bottomRight()));
    pp -= pp_base;

    auto el = match->getLightEllipse();
    QLinearGradient gr(QPoint(el.center().x(), el.top()), QPoint(el.center().x(), el.bottom()));
    gr.setColorAt(0.0, QColor(244, 196, 133));
    gr.setColorAt(1.0, QColor(255, 254, 230));

    painter.setBrush(QBrush(gr));
    painter.setPen(QPen(Qt::NoPen));
    painter.drawPath(pp);
}

void Widget::drawStatic(QPainter& painter)
{
    auto base = match->getBase();
    QLinearGradient gr;
    gr.setStart(QPoint(base.center().x(), base.top()));
    gr.setFinalStop(QPoint(base.center().x(), base.bottom()));
    gr.setColorAt(0.0, QColor(255, 254, 230));
    gr.setColorAt(1.0, QColor(56, 91, 194, 128));

    painter.setPen(QPen(QBrush(gr), 3));
    painter.setBrush(QBrush(gr));
    painter.drawEllipse(base);
}
void Widget::paintEvent(QPaintEvent *)
 {
    float speed=1.5f;
    QList<QPointF> list;
    QPainterPath path;
    list.push_back(first_Point);
    list.push_back(sec_Point);
    list.push_back(third_Point);
    list.push_back(last_Point);
    list.push_back(first_Point);
    path.addPolygon(QPolygonF(list));

    QPainter painter(this);

    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);

    if(third_Point.rx() - sec_Point.rx() > 150)
    {
        third_Point.setX(third_Point.rx() - 0.05*speed);
        last_Point.setX(last_Point.rx() - 0.05*speed);

        bottomRight.setY(bottomRight.ry() + 0.01*speed);
        topRight.setY(topRight.ry() + 0.01*speed);
        bottomRight.setX(bottomRight.rx() - 0.025*speed);
        topRight.setX(topRight.rx() - 0.025*speed);
        path.moveTo(third_Point);
        path.quadTo(QPointF(topRight.rx()-5, last_Point.ry()-5),bottomRight);
        path.lineTo(topRight);
        path.quadTo(QPointF(topRight.rx()-5, last_Point.ry()), last_Point);
        path.moveTo(third_Point);
        path.setFillRule(Qt::WindingFill);
        painter.fillPath(path, QBrush(Qt::black));
        path.addEllipse(topRight.rx() - 5, topRight.ry(), 10, 10);
        painter.fillPath(path, QBrush(Qt::black));
        painter.fillRect(QRectF(first_Point, third_Point), QBrush(QPixmap("C:/Users/second/Documents/Match_simulator/wood.jpg")));
        painter.drawPath(path);

        widthDinamics += 0.01;
        highDinamics -= 0.015;
        randDouble = ((double)(rand() % 150 - 75))/(double)10;
        match->setDefaultTop(QPair<int, int>(third_Point.x()-10, third_Point.y() + highDinamics + randDouble), QPair<int, int>(third_Point.x()+10+widthDinamics, third_Point.y()), 50);
        match->setBase(QPair<int, int>(third_Point.x()-10, third_Point.y()), QPair<int, int>(third_Point.x()+10+widthDinamics, third_Point.y()));
        drawTop(painter);
        drawStatic(painter);
        update();
    }
    else{
        if(third_Point.rx() - sec_Point.rx() > 3)
        {
            third_Point.setX(third_Point.rx() - 0.05*speed);
            last_Point.setX(last_Point.rx() - 0.05*speed);

            bottomRight.setY(bottomRight.ry() + 0.01*speed);
            topRight.setY(topRight.ry() + 0.01*speed);
            bottomRight.setX(bottomRight.rx() - 0.025*speed);
            topRight.setX(topRight.rx() - 0.025*speed);
            path.moveTo(third_Point);
            path.quadTo(QPointF(topRight.rx()-5, last_Point.ry()-5),bottomRight);
            path.lineTo(topRight);
            path.quadTo(QPointF(topRight.rx()-5, last_Point.ry()), last_Point);
            path.moveTo(third_Point);
            path.setFillRule(Qt::WindingFill);
            painter.fillPath(path, QBrush(Qt::black));
            path.addEllipse(topRight.rx() - 5, topRight.ry(), 10, 10);
            painter.fillPath(path, QBrush(Qt::black));
            painter.fillRect(QRectF(first_Point, third_Point), QBrush(QPixmap("C:/Users/second/Documents/Match_simulator/wood.jpg")));
            painter.drawPath(path);

            widthDinamics -= 0.02;
            highDinamics += 0.032;
            randDouble = ((double)(rand() % 100 - 50))/(double)10;
            match->setDefaultTop(QPair<int, int>(third_Point.x()-10, third_Point.y() + highDinamics + randDouble), QPair<int, int>(third_Point.x()+10+widthDinamics, third_Point.y()), 50);
            match->setBase(QPair<int, int>(third_Point.x()-10, third_Point.y()), QPair<int, int>(third_Point.x()+10+widthDinamics, third_Point.y()));
            drawTop(painter);
            drawStatic(painter);
            update();
        }
        else{
            third_Point.setX(third_Point.rx() - 0.05*speed);
            last_Point.setX(last_Point.rx() - 0.05*speed);

            bottomRight.setY(bottomRight.ry() + 0.01*speed);
            topRight.setY(topRight.ry() + 0.01*speed);
            bottomRight.setX(bottomRight.rx() - 0.025*speed);
            topRight.setX(topRight.rx() - 0.025*speed);
            path.moveTo(third_Point);
            path.quadTo(QPointF(topRight.rx()-5, last_Point.ry()-5),bottomRight);
            path.lineTo(topRight);
            path.quadTo(QPointF(topRight.rx()-5, last_Point.ry()), last_Point);
            path.moveTo(third_Point);
            path.setFillRule(Qt::WindingFill);
            painter.fillPath(path, QBrush(Qt::black));
            path.addEllipse(topRight.rx() - 5, topRight.ry(), 10, 10);
            painter.fillPath(path, QBrush(Qt::black));
            painter.fillRect(QRectF(first_Point, third_Point), QBrush(QPixmap("C:/Users/second/Documents/Match_simulator/wood.jpg")));
            painter.drawPath(path);
        }

    }
}




