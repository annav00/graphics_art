#include "match.h"

Match::Match()
{
    QPointF beginPoint(50,50);
    QPointF secondPoint(50,100);
    QPointF thirdPoint(100,100);
    QPointF lastPoint(100,50);
    QList<QPointF> list;
    list.push_back(beginPoint);
    list.push_back(secondPoint);
    list.push_back(thirdPoint);
    list.push_back(lastPoint);
    body = new QPolygonF(list);
}

QRectF Match::boundingRect() const
{
    return QRectF();
}

void Match::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}

void Match::setDefaultTop(const QPair<int, int>& pos1, const QPair<int, int>& pos2, const int height)
{
    top.clear();
    top.push_back(QPoint(pos1.first, pos1.second));
    top.push_back(QPoint((pos1.first + pos2.first) / 2, (pos1.second + pos2.second) / 2 - height));
    top.push_back(QPoint(pos2.first+10, pos2.second));
}

void Match::setBase(const QPair<int, int>& pos1, const QPair<int, int>& pos2, int dx)
{
    int width = pos2.first - pos1.first;
    //int dx = width * 0.1;
    DEF_BASE_HEIGHT = height / 3;

    base = QRect(QPoint(dx + pos1.first + width / 30, pos1.second - DEF_BASE_HEIGHT +10),
        QPoint(dx + pos2.first - width / 30+10, pos2.second+5)
    );
}

Match::Match(QPair<int, int> pos1, QPair<int, int> pos2, int height)
{
    this->height = height;
    setDefaultTop(pos1, pos2, height);
    setBase(pos1, pos2);
}

const QPolygon Match::getTop()
{
    return QPolygon(top);
}

const QRect& Match::getBase()
{
    return base;
}

int Match::Randomer::poiss(float m)
{
    float p = exp(-m);
    int x = 0;

    float r = (float) (rand()) / (float)(RAND_MAX) - p;

    while (r >= 0)
    {
        x++;
        p *= m / x;
        r -= p;
    }

    return x;
}

float Match::Randomer::norm(float m, float s)
{
    float res = 0;
    for (int i = 0; i < 12; i++)
        res += (float)(rand()) / (float) (RAND_MAX);
    res -= 6;

    return m + res * s;
}



QPoint Match::Randomer::generateNormPoint(QPoint m, QPoint d)
{
    return QPoint(
        norm(m.x(), d.x()), norm(m.y(), d.y())
    );
}

void Match::setRandStruct()
{
    unsigned int npoints = Match::Randomer::poiss() + MINFlames;
    QVector<QPoint> res;
    QVector<QPoint> maximums;

    float xcenter = (top.back().x() - top.front().x()) / 2;

    res.push_back(top.front());
    res.push_back(
        Match::Randomer::generateNormPoint(QPoint(top.front().x() + xcenter / 5, top.front().y() - height * 0.7),
            QPoint(3, 3))
    );

    for (int n = 0; n < npoints; n++)
    {
        maximums.push_back(Match::Randomer::generateNormPoint(QPoint(top.front().x() + xcenter, top.front().y() - height),
            QPoint(abs((top.back().x() - top.front().x()) / DEF_DSIGMA), height / 100)));
    }
    maximums.push_back(top.back());
    std::sort(maximums.begin(), maximums.end(), [=](QPoint p1, QPoint p2) { return p1.x() < p2.x(); });

    for (auto it = maximums.begin(); it != maximums.end(); it++)
    {
        QPoint m = *it;

        QPoint prev(m.x() - abs(res.back().x() - m.x()) * GO_INSIDE,
                m.y() + abs(res.back().y() - m.y()) * GO_INSIDE);

        if (it + 1 != maximums.end())
            res.push_back(prev);
        res.push_back(m);
    }

    res.insert(res.size() - 1,
        Match::Randomer::generateNormPoint(QPoint(top.back().x() - xcenter / 5, top.back().y() - height * 0.7),
            QPoint(3, 3))
    );

    top = res;
}

QRect Match::getLightEllipse()
{
    int realCenterX = Match::Utils::average(top, [](QPoint& p) -> int { return p.x(); }),
        centerX = top.front().x() + (top.back().x() - top.front().x()) / 2;

    int dx = centerX - realCenterX;

    int dOutX = top.back().x() - top.front().x();
    int xcenter = top.front().x() + dOutX / 2;

    QRect out(QPoint(-dx + xcenter - W_OUT * dOutX, top.front().y() - height),
        QPoint(-dx + xcenter + W_OUT * dOutX, top.back().y() - height / 4)
    );

    return out;
}

int Match::Utils::average(QVector<QPoint>& elems, std::function <int(QPoint &)> getter)
{
    int res = 0;
    for (auto e : elems)
        res += getter(e);

    return res / elems.size();
}
float Match::Randomer::random()
{
    int res = rand();
    return (float)(res % 10 - 5)/100;
}
