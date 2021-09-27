#ifndef MATCH_H
#define MATCH_H

#include <QGraphicsItem>
#include <QPair>
#include <QRect>
#include <QVector>
#include <QPolygon>
#include <functional>
#include <QSet>
#include <math.h>
#include <QtGui>

class Match : public QGraphicsItem
{
    int DEF_BASE_HEIGHT = 100;
    float DEF_DSIGMA = 35;
    const float GO_INSIDE = 0.4f;
    const float W_OUT = 1.3f;
    int MINFlames = 7;
    QVector<QPoint> top;
    QRect base;
    int height;
    QPolygonF* body;

    class Randomer {
        static int poiss(float m = 0.9F);
        static float norm(float m, float s);
        static float random();
        static QPoint generateNormPoint(QPoint m, QPoint d);

        friend class Match;
    };

    class Utils {
        static int average(QVector<QPoint>& elems, std::function <int(QPoint&)> getter);

        friend class Match;
    };
public:
    static float random();
    Match();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    Match(QPair<int, int> pos1, QPair<int, int> pos2, int height);

    void setDefaultTop(const QPair<int, int>& pos1, const QPair<int, int>& pos2, const int height);
    void setBase(const QPair<int, int>& pos1, const QPair<int, int>& pos2, int dx = 0);

    const QPolygon getTop();
    const QRect& getBase();
    void setRandStruct();
    QRect getLightEllipse();

};

#endif // MATCH_H
