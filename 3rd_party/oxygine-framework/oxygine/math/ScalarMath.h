#pragma once
#include <qcolor.h>
#include <QPoint>
#include <QRect>

namespace oxygine
{
    template<class T>
    inline T lerp(T a, T b, float v)
    {
        return T(a + (b - a) * v);
    }
    template<>
    inline QColor lerp<class QColor>(QColor a, QColor b, float v)
    {
        return QColor(lerp(a.red(), b.red(), v),
                      lerp(a.green(), b.green(), v),
                      lerp(a.blue(), b.blue(), v),
                      lerp(a.alpha(), b.alpha(), v));
    }
    template <class T>
    inline T clamp(T v, T min_, T max_)
    {
        if (v < min_)
        {
            return min_;
        }
        else if (v > max_)
        {
            return max_;
        }
        return v;
    }
    /***********************************************************************/
    // color operators
    /***********************************************************************/
    /**
     * @brief operator * multiplies two colors
     * @param c1 first color
     * @param c2 second color
     * @return multiplied color
     */
    static QColor operator*(const QColor& c1, const QColor& c2)
    {
        return QColor((c1.red() * c2.red()) / 255,
                      (c1.green() * c2.green()) / 255,
                      (c1.blue() * c2.blue()) / 255,
                      (c1.alpha() * c2.alpha()) / 255);
    }
    static QColor operator+(const QColor& c1, const QColor& c2)
    {
        QColor v(clamp(c1.red()      + c2.red()  , 0, 255),
                 clamp(c1.green()    + c2.green(), 0, 255),
                 clamp(c1.blue()     + c2.blue() , 0, 255),
                 clamp(c1.alpha()    + c2.alpha(), 0, 255));
        return v;
    }
    static QColor operator-(const QColor& c1, const QColor& c2)
    {
        QColor v(clamp(c1.red()      - c2.red()  , 0, 255),
                 clamp(c1.green()    - c2.green(), 0, 255),
                 clamp(c1.blue()     - c2.blue() , 0, 255),
                 clamp(c1.alpha()    - c2.alpha(), 0, 255));
        return v;
    }
    /**
     * @brief qRgba
     * @param value
     * @return format RRGGBBAA
     */
    static QColor premultiply(QColor value)
    {
        QColor ret((value.red()     * value.alpha()) / 255,
                   (value.green()   * value.alpha()) / 255,
                   (value.blue()    * value.alpha()) / 255,
                   value.alpha());
        return ret;
    }
    /**
     * @brief qRgba
     * @param value
     * @return format RRGGBBAA
     */
    static quint32 qRgba(QColor value)
    {
        //return value;
        quint32 out = 0;
        unsigned char* pOut = (unsigned char*)(&out);
        pOut[0] = value.red();
        pOut[1] = value.green();
        pOut[2] = value.blue();
        pOut[3] = value.alpha();
        return out;
    }
    /***********************************************************************/
    // point, size, rect operators
    /***********************************************************************/
    static QPoint operator*(const QPoint& p1, const QPoint& p2)
    {
        return QPoint(p1.x() * p2.x(),
                      p1.y() * p2.y());
    }
    static QPoint operator/(const QPoint& p1, const QPoint& p2)
    {
        return QPoint(p1.x() / p2.x(),
                      p1.y() / p2.y());
    }
    static QPointF operator*(const QPointF& p1, const QPointF& p2)
    {
        return QPointF(p1.x() * p2.x(),
                      p1.y() * p2.y());
    }
    static QPointF operator/(const QPointF& p1, const QPointF& p2)
    {
        return QPointF(p1.x() / p2.x(),
                      p1.y() / p2.y());
    }
    static QRect operator*(const QRect& r1, const QRect& r2)
    {
        return QRect(r1.x() * r2.x(),
                     r1.y() * r2.y(),
                     r1.width() * r2.width(),
                     r1.height() * r2.height());
    }
    static QRect operator/(const QRect& r1, const QRect& r2)
    {
        return QRect(r1.x()         / r2.x(),
                     r1.y()         / r2.y(),
                     r1.width()     / r2.width(),
                     r1.height()    / r2.height());
    }
    static QRectF operator*(const QRectF& r1, const QRectF& r2)
    {
        return QRectF(r1.x() * r2.x(),
                     r1.y() * r2.y(),
                     r1.width() * r2.width(),
                     r1.height() * r2.height());
    }
    static QRectF operator/(const QRectF& r1, const QRectF& r2)
    {
        return QRectF(r1.x()         / r2.x(),
                     r1.y()         / r2.y(),
                     r1.width()     / r2.width(),
                     r1.height()    / r2.height());
    }
}
