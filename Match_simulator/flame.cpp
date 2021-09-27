#include "flame.h"

void Flame::setDefaultTop(const QPair<int, int>& pos1, const QPair<int, int>& pos2, const int height)
{
    top.clear();
    top.push_back(QPoint(pos1.first+10, pos1.second-10));
    top.push_back(QPoint((pos1.first + pos2.first) / 2, (pos1.second + pos2.second) / 2 - height));
    top.push_back(QPoint(pos2.first+50, pos2.second-10));
}

void Flame::setBase(const QPair<int, int>& pos1, const QPair<int, int>& pos2, int dx)
{
    int width = pos2.first - pos1.first;
    //int dx = width * 0.1;
    DEF_BASE_HEIGHT = height / 3;

    base = QRect(QPoint(dx + pos1.first + width / 30+10, pos1.second - DEF_BASE_HEIGHT +10),
        QPoint(dx + pos2.first - width / 30+50, pos2.second+5)
    );
}

Flame::Flame(QPair<int, int> pos1, QPair<int, int> pos2, int height)
{
    this->height = height;
    setDefaultTop(pos1, pos2, height);
    setBase(pos1, pos2);
}

const QPolygon Flame::getTop()
{
    return QPolygon(top);
}

const QRect& Flame::getBase()
{
    return base;
}

int Flame::Randomer::poiss(float m)
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

float Flame::Randomer::norm(float m, float s)
{
    float res = 0;
    for (int i = 0; i < 12; i++)
        res += (float)(rand()) / (float) (RAND_MAX);
    res -= 6;

    return m + res * s;
}

QPoint Flame::Randomer::generateNormPoint(QPoint m, QPoint d)
{
    return QPoint(
        norm(m.x(), d.x()), norm(m.y(), d.y())
    );
}

void Flame::setRandStruct()
{
    unsigned int npoints = Flame::Randomer::poiss() + MINFlames; // число максимумов у полигона пламени (кроме начальной и конечной точек)
    QVector<QPoint> res; // новый полигон
    QVector<QPoint> maximums;

    float xcenter = (top.back().x() - top.front().x()) / 2;

    res.push_back(top.front());
    res.push_back(
        Flame::Randomer::generateNormPoint(QPoint(top.front().x() + xcenter / 5, top.front().y() - height * 0.7),
            QPoint(3, 3))
    );

    for (int n = 0; n < npoints; n++)
    {
        maximums.push_back(Flame::Randomer::generateNormPoint(QPoint(top.front().x() + xcenter, top.front().y() - height),
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
        Flame::Randomer::generateNormPoint(QPoint(top.back().x() - xcenter / 5, top.back().y() - height * 0.7),
            QPoint(3, 3))
    );

    top = res;
}

QRect Flame::getLightEllipse()
{
    int realCenterX = Flame::Utils::average(top, [](QPoint& p) -> int { return p.x(); }),
        centerX = top.front().x() + (top.back().x() - top.front().x()) / 2;

    int dx = centerX - realCenterX;

    int dOutX = top.back().x() - top.front().x();
    int xcenter = top.front().x() + dOutX / 2;

    QRect out(QPoint(-dx + xcenter - W_OUT * dOutX, top.front().y() - height),
        QPoint(-dx + xcenter + W_OUT * dOutX, top.back().y() - height / 4)
    );

    return out;
}

int Flame::Utils::average(QVector<QPoint>& elems, std::function <int(QPoint &)> getter)
{
    int res = 0;
    for (auto e : elems)
        res += getter(e);

    return res / elems.size();
}
