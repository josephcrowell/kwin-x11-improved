/*
    SPDX-FileCopyrightText: 2024 Vlad Zahorodnii <vlad.zahorodnii@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include <QImage>
#include <QTest>

#include "core/box.h"

using namespace KWin;

class TestBoxF : public QObject
{
    Q_OBJECT

public:
    TestBoxF() = default;

private Q_SLOTS:
    void equals_data();
    void equals();
    void notEquals_data();
    void notEquals();
    void empty_data();
    void empty();
    void containsPoint_data();
    void containsPoint();
    void containsRect_data();
    void containsRect();
    void x_data();
    void x();
    void setX_data();
    void setX();
    void y_data();
    void y();
    void setY_data();
    void setY();
    void width_data();
    void width();
    void setWidth_data();
    void setWidth();
    void height_data();
    void height();
    void setHeight_data();
    void setHeight();
    void left_data();
    void left();
    void setLeft_data();
    void setLeft();
    void moveLeft_data();
    void moveLeft();
    void top_data();
    void top();
    void setTop_data();
    void setTop();
    void moveTop_data();
    void moveTop();
    void right_data();
    void right();
    void setRight_data();
    void setRight();
    void moveRight_data();
    void moveRight();
    void bottom_data();
    void bottom();
    void setBottom_data();
    void setBottom();
    void moveBottom_data();
    void moveBottom();
    void center_data();
    void center();
    void moveCenter_data();
    void moveCenter();
    void topLeft_data();
    void topLeft();
    void setTopLeft_data();
    void setTopLeft();
    void moveTopLeft_data();
    void moveTopLeft();
    void topRight_data();
    void topRight();
    void setTopRight_data();
    void setTopRight();
    void moveTopRight_data();
    void moveTopRight();
    void bottomRight_data();
    void bottomRight();
    void setBottomRight_data();
    void setBottomRight();
    void moveBottomRight_data();
    void moveBottomRight();
    void bottomLeft_data();
    void bottomLeft();
    void setBottomLeft_data();
    void setBottomLeft();
    void moveBottomLeft_data();
    void moveBottomLeft();
    void transposed_data();
    void transposed();
    void translate_data();
    void translate();
    void translated_data();
    void translated();
    void adjust_data();
    void adjust();
    void adjusted_data();
    void adjusted();
    void marginsAdded_data();
    void marginsAdded();
    void marginsRemoved_data();
    void marginsRemoved();
    void united_data();
    void united();
    void intersects_data();
    void intersects();
    void intersected_data();
    void intersected();
    void rounded_data();
    void rounded();
    void roundedOut_data();
    void roundedOut();
};

void TestBoxF::equals_data()
{
    QTest::addColumn<BoxF>("box1");
    QTest::addColumn<BoxF>("box2");
    QTest::addColumn<bool>("expected");

    QTest::addRow("default == default") << BoxF() << BoxF() << true;
    QTest::addRow("2,3 4x5 == default") << BoxF(2, 3, 4, 5) << BoxF() << false;
    QTest::addRow("default == 2,3 4x5") << BoxF() << BoxF(2, 3, 4, 5) << false;
    QTest::addRow("2,3 4x5 == 2,3 4x5") << BoxF(2, 3, 4, 5) << BoxF(2, 3, 4, 5) << true;

    QTest::addRow("2,3 4x5 == 0,3 4x5") << BoxF(2, 3, 4, 5) << BoxF(0, 3, 4, 5) << false;
    QTest::addRow("2,3 4x5 == 2,0 4x5") << BoxF(2, 3, 4, 5) << BoxF(2, 0, 4, 5) << false;
    QTest::addRow("2,3 4x5 == 2,3 0x5") << BoxF(2, 3, 4, 5) << BoxF(2, 3, 0, 5) << false;
    QTest::addRow("2,3 4x5 == 2,3 4x0") << BoxF(2, 3, 4, 5) << BoxF(2, 3, 4, 0) << false;
}

void TestBoxF::equals()
{
    QFETCH(BoxF, box1);
    QFETCH(BoxF, box2);

    QTEST(box1 == box2, "expected");
}

void TestBoxF::notEquals_data()
{
    QTest::addColumn<BoxF>("box1");
    QTest::addColumn<BoxF>("box2");
    QTest::addColumn<bool>("expected");

    QTest::addRow("default == default") << BoxF() << BoxF() << false;
    QTest::addRow("2,3 4x5 == default") << BoxF(2, 3, 4, 5) << BoxF() << true;
    QTest::addRow("default == 2,3 4x5") << BoxF() << BoxF(2, 3, 4, 5) << true;
    QTest::addRow("2,3 4x5 == 2,3 4x5") << BoxF(2, 3, 4, 5) << BoxF(2, 3, 4, 5) << false;

    QTest::addRow("2,3 4x5 == 0,3 4x5") << BoxF(2, 3, 4, 5) << BoxF(0, 3, 4, 5) << true;
    QTest::addRow("2,3 4x5 == 2,0 4x5") << BoxF(2, 3, 4, 5) << BoxF(2, 0, 4, 5) << true;
    QTest::addRow("2,3 4x5 == 2,3 0x5") << BoxF(2, 3, 4, 5) << BoxF(2, 3, 0, 5) << true;
    QTest::addRow("2,3 4x5 == 2,3 4x0") << BoxF(2, 3, 4, 5) << BoxF(2, 3, 4, 0) << true;
}

void TestBoxF::notEquals()
{
    QFETCH(BoxF, box1);
    QFETCH(BoxF, box2);

    QTEST(box1 != box2, "expected");
}

void TestBoxF::empty_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<bool>("empty");

    QTest::addRow("default") << BoxF() << true;

    QTest::addRow("0,0 0x0") << BoxF(0, 0, 0, 0) << true;
    QTest::addRow("2,3 0x0") << BoxF(2, 3, 0, 0) << true;
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << false;

    QTest::addRow("QPointF(1, 1) QPointF(1, 1)") << BoxF(QPointF(1, 1), QPointF(1, 1)) << true;
    QTest::addRow("QPointF(1, 1) QPointF(1, 2)") << BoxF(QPointF(1, 1), QPointF(1, 2)) << true;
    QTest::addRow("QPointF(1, 1) QPointF(2, 1)") << BoxF(QPointF(1, 1), QPointF(2, 1)) << true;
    QTest::addRow("QPointF(1, 1) QPointF(2, 2)") << BoxF(QPointF(1, 1), QPointF(2, 2)) << false;

    QTest::addRow("QPointF(1, 1) QSizeF(0, 0)") << BoxF(QPointF(1, 1), QSizeF(0, 0)) << true;
    QTest::addRow("QPointF(1, 1) QSizeF(0, 1)") << BoxF(QPointF(1, 1), QSizeF(0, 1)) << true;
    QTest::addRow("QPointF(1, 1) QSizeF(1, 0)") << BoxF(QPointF(1, 1), QSizeF(1, 0)) << true;
    QTest::addRow("QPointF(1, 1) QSizeF(1, 1)") << BoxF(QPointF(1, 1), QSizeF(1, 1)) << false;

    QTest::addRow("QRect()") << BoxF(QRect()) << true;
    QTest::addRow("QRect(1, 1, 0, 0)") << BoxF(QRect(1, 1, 0, 0)) << true;
    QTest::addRow("QRect(1, 1, 0, 1)") << BoxF(QRect(1, 1, 0, 1)) << true;
    QTest::addRow("QRect(1, 1, 1, 0)") << BoxF(QRect(1, 1, 1, 0)) << true;
    QTest::addRow("QRect(1, 1, 1, 1)") << BoxF(QRect(1, 1, 1, 1)) << false;
}

void TestBoxF::empty()
{
    QFETCH(BoxF, box);

    QTEST(box.isEmpty(), "empty");
}

void TestBoxF::containsPoint_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<QPointF>("point");
    QTest::addColumn<bool>("contains");

    QTest::addRow("empty - 0,0") << BoxF() << QPointF(0, 0) << false;
    QTest::addRow("empty - 1,1") << BoxF() << QPointF(1, 1) << false;

    QTest::addRow("2,3 4x5 - 1,2") << BoxF(2, 3, 4, 5) << QPointF(1, 2) << false;
    QTest::addRow("2,3 4x5 - 7,9") << BoxF(2, 3, 4, 5) << QPointF(7, 9) << false;
    QTest::addRow("2,3 4x5 - 1,9") << BoxF(2, 3, 4, 5) << QPointF(1, 9) << false;
    QTest::addRow("2,3 4x5 - 7,2") << BoxF(2, 3, 4, 5) << QPointF(7, 2) << false;

    QTest::addRow("2,3 4x5 - 2,3") << BoxF(2, 3, 4, 5) << QPointF(2, 3) << true;
    QTest::addRow("2,3 4x5 - 6,3") << BoxF(2, 3, 4, 5) << QPointF(6, 3) << false;
    QTest::addRow("2,3 4x5 - 5,3") << BoxF(2, 3, 4, 5) << QPointF(5, 3) << true;
    QTest::addRow("2,3 4x5 - 6,8") << BoxF(2, 3, 4, 5) << QPointF(6, 8) << false;
    QTest::addRow("2,3 4x5 - 5,7") << BoxF(2, 3, 4, 5) << QPointF(5, 7) << true;
    QTest::addRow("2,3 4x5 - 2,8") << BoxF(2, 3, 4, 5) << QPointF(2, 8) << false;
    QTest::addRow("2,3 4x5 - 2,7") << BoxF(2, 3, 4, 5) << QPointF(2, 7) << true;

    QTest::addRow("2,3 4x5 - 2,5") << BoxF(2, 3, 4, 5) << QPointF(2, 5) << true;
    QTest::addRow("2,3 4x5 - 1,5") << BoxF(2, 3, 4, 5) << QPointF(1, 5) << false;
    QTest::addRow("2,3 4x5 - 3,5") << BoxF(2, 3, 4, 5) << QPointF(3, 5) << true;

    QTest::addRow("2,3 4x5 - 4,3") << BoxF(2, 3, 4, 5) << QPointF(4, 3) << true;
    QTest::addRow("2,3 4x5 - 4,2") << BoxF(2, 3, 4, 5) << QPointF(4, 2) << false;
    QTest::addRow("2,3 4x5 - 4,4") << BoxF(2, 3, 4, 5) << QPointF(4, 4) << true;

    QTest::addRow("2,3 4x5 - 6,5") << BoxF(2, 3, 4, 5) << QPointF(6, 5) << false;
    QTest::addRow("2,3 4x5 - 5,5") << BoxF(2, 3, 4, 5) << QPointF(5, 5) << true;
    QTest::addRow("2,3 4x5 - 7,5") << BoxF(2, 3, 4, 5) << QPointF(7, 5) << false;

    QTest::addRow("2,3 4x5 - 4,8") << BoxF(2, 3, 4, 5) << QPointF(4, 8) << false;
    QTest::addRow("2,3 4x5 - 4,7") << BoxF(2, 3, 4, 5) << QPointF(4, 7) << true;
    QTest::addRow("2,3 4x5 - 4,9") << BoxF(2, 3, 4, 5) << QPointF(4, 9) << false;
}

void TestBoxF::containsPoint()
{
    QFETCH(BoxF, box);
    QFETCH(QPointF, point);

    QTEST(box.contains(point), "contains");
    QTEST(box.contains(point.x(), point.y()), "contains");
}

void TestBoxF::containsRect_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<BoxF>("other");
    QTest::addColumn<bool>("contains");

    QTest::addRow("empty - empty") << BoxF() << BoxF() << false;
    QTest::addRow("empty - 2,3 4x5") << BoxF() << BoxF(2, 3, 4, 5) << false;
    QTest::addRow("2,3 4x5 - empty") << BoxF(2, 3, 4, 5) << BoxF() << false;

    QTest::addRow("2,3 4x5 - 2,3 4x5") << BoxF(2, 3, 4, 5) << BoxF(2, 3, 4, 5) << true;
    QTest::addRow("2,3 4x5 - 2,3 0x0") << BoxF(2, 3, 4, 5) << BoxF(2, 3, 0, 0) << true;
    QTest::addRow("2,3 4x5 - 2,3 4x6") << BoxF(2, 3, 4, 5) << BoxF(2, 3, 4, 6) << false;
    QTest::addRow("2,3 4x5 - 2,3 5x5") << BoxF(2, 3, 4, 5) << BoxF(2, 3, 5, 5) << false;
    QTest::addRow("2,3 4x5 - 1,3 4x5") << BoxF(2, 3, 4, 5) << BoxF(1, 3, 4, 5) << false;
    QTest::addRow("2,3 4x5 - 2,2 4x5") << BoxF(2, 3, 4, 5) << BoxF(2, 2, 4, 5) << false;

    QTest::addRow("5,5 1x1 - 2,2 1x1") << BoxF(5, 5, 1, 1) << BoxF(2, 2, 1, 1) << false;
    QTest::addRow("2,2 1x1 - 5,5 1x1") << BoxF(2, 2, 1, 1) << BoxF(5, 5, 1, 1) << false;
}

void TestBoxF::containsRect()
{
    QFETCH(BoxF, box);
    QFETCH(BoxF, other);

    QTEST(box.contains(other), "contains");
}

void TestBoxF::x_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<qreal>("x");

    QTest::addRow("default") << BoxF() << qreal(0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << qreal(2);
    QTest::addRow("QPointF(2, 3) QPointF(6, 8)") << BoxF(QPointF(2, 3), QPointF(6, 8)) << qreal(2);
    QTest::addRow("QPointF(2, 3) QSizeF(4, 5)") << BoxF(QPointF(2, 3), QSizeF(4, 5)) << qreal(2);
    QTest::addRow("QRect(2, 3, 4, 5)") << BoxF(QRect(2, 3, 4, 5)) << qreal(2);
}

void TestBoxF::x()
{
    QFETCH(BoxF, box);

    QTEST(box.x(), "x");
}

void TestBoxF::setX_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<qreal>("value");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("empty") << BoxF() << qreal(-1) << BoxF(-1, 0, 0, 0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << qreal(0) << BoxF(0, 3, 4, 5);
}

void TestBoxF::setX()
{
    QFETCH(BoxF, box);
    QFETCH(qreal, value);

    box.setX(value);
    QTEST(box, "expected");
}

void TestBoxF::y_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<qreal>("y");

    QTest::addRow("default") << BoxF() << qreal(0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << qreal(3);
    QTest::addRow("QPointF(2, 3) QPointF(6, 8)") << BoxF(QPointF(2, 3), QPointF(6, 8)) << qreal(3);
    QTest::addRow("QPointF(2, 3) QSizeF(4, 5)") << BoxF(QPointF(2, 3), QSizeF(4, 5)) << qreal(3);
    QTest::addRow("QRect(2, 3, 4, 5)") << BoxF(QRect(2, 3, 4, 5)) << qreal(3);
}

void TestBoxF::y()
{
    QFETCH(BoxF, box);

    QTEST(box.y(), "y");
}

void TestBoxF::setY_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<qreal>("value");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("empty") << BoxF() << qreal(-1) << BoxF(0, -1, 0, 0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << qreal(0) << BoxF(2, 0, 4, 5);
}

void TestBoxF::setY()
{
    QFETCH(BoxF, box);
    QFETCH(qreal, value);

    box.setY(value);
    QTEST(box, "expected");
}

void TestBoxF::width_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<qreal>("width");

    QTest::addRow("default") << BoxF() << qreal(0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << qreal(4);
    QTest::addRow("QPointF(2, 3) QPointF(6, 8)") << BoxF(QPointF(2, 3), QPointF(6, 8)) << qreal(4);
    QTest::addRow("QPointF(2, 3) QSizeF(4, 5)") << BoxF(QPointF(2, 3), QSizeF(4, 5)) << qreal(4);
    QTest::addRow("QRect(2, 3, 4, 5)") << BoxF(QRect(2, 3, 4, 5)) << qreal(4);
}

void TestBoxF::width()
{
    QFETCH(BoxF, box);

    QTEST(box.width(), "width");
}

void TestBoxF::setWidth_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<qreal>("value");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("empty") << BoxF() << qreal(1) << BoxF(0, 0, 1, 0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << qreal(0) << BoxF(2, 3, 0, 5);
}

void TestBoxF::setWidth()
{
    QFETCH(BoxF, box);
    QFETCH(qreal, value);

    box.setWidth(value);
    QTEST(box, "expected");
}

void TestBoxF::height_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<qreal>("height");

    QTest::addRow("default") << BoxF() << qreal(0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << qreal(5);
    QTest::addRow("QPointF(2, 3) QPointF(6, 8)") << BoxF(QPointF(2, 3), QPointF(6, 8)) << qreal(5);
    QTest::addRow("QPointF(2, 3) QSizeF(4, 5)") << BoxF(QPointF(2, 3), QSizeF(4, 5)) << qreal(5);
    QTest::addRow("QRect(2, 3, 4, 5)") << BoxF(QRect(2, 3, 4, 5)) << qreal(5);
}

void TestBoxF::height()
{
    QFETCH(BoxF, box);

    QTEST(box.height(), "height");
}

void TestBoxF::setHeight_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<qreal>("value");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("empty") << BoxF() << qreal(1) << BoxF(0, 0, 0, 1);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << qreal(0) << BoxF(2, 3, 4, 0);
}

void TestBoxF::setHeight()
{
    QFETCH(BoxF, box);
    QFETCH(qreal, value);

    box.setHeight(value);
    QTEST(box, "expected");
}

void TestBoxF::left_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<qreal>("left");

    QTest::addRow("default") << BoxF() << qreal(0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << qreal(2);
    QTest::addRow("QPointF(2, 3) QPointF(6, 8)") << BoxF(QPointF(2, 3), QPointF(6, 8)) << qreal(2);
    QTest::addRow("QPointF(2, 3) QSizeF(4, 5)") << BoxF(QPointF(2, 3), QSizeF(4, 5)) << qreal(2);
    QTest::addRow("QRect(2, 3, 4, 5)") << BoxF(QRect(2, 3, 4, 5)) << qreal(2);
}

void TestBoxF::left()
{
    QFETCH(BoxF, box);

    QTEST(box.left(), "left");
}

void TestBoxF::setLeft_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<qreal>("value");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("empty") << BoxF() << qreal(-1) << BoxF(-1, 0, 1, 0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << qreal(0) << BoxF(0, 3, 6, 5);
}

void TestBoxF::setLeft()
{
    QFETCH(BoxF, box);
    QFETCH(qreal, value);

    box.setLeft(value);
    QTEST(box, "expected");
}

void TestBoxF::moveLeft_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<qreal>("value");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("empty") << BoxF() << qreal(-1) << BoxF(-1, 0, 0, 0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << qreal(0) << BoxF(0, 3, 4, 5);
}

void TestBoxF::moveLeft()
{
    QFETCH(BoxF, box);
    QFETCH(qreal, value);

    box.moveLeft(value);
    QTEST(box, "expected");
}

void TestBoxF::top_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<qreal>("top");

    QTest::addRow("default") << BoxF() << qreal(0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << qreal(3);
    QTest::addRow("QPointF(2, 3) QPointF(6, 8)") << BoxF(QPointF(2, 3), QPointF(6, 8)) << qreal(3);
    QTest::addRow("QPointF(2, 3) QSizeF(4, 5)") << BoxF(QPointF(2, 3), QSizeF(4, 5)) << qreal(3);
    QTest::addRow("QRect(2, 3, 4, 5)") << BoxF(QRect(2, 3, 4, 5)) << qreal(3);
}

void TestBoxF::top()
{
    QFETCH(BoxF, box);

    QTEST(box.top(), "top");
}

void TestBoxF::setTop_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<qreal>("value");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("empty") << BoxF() << qreal(-1) << BoxF(0, -1, 0, 1);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << qreal(0) << BoxF(2, 0, 4, 8);
}

void TestBoxF::setTop()
{
    QFETCH(BoxF, box);
    QFETCH(qreal, value);

    box.setTop(value);
    QTEST(box, "expected");
}

void TestBoxF::moveTop_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<qreal>("value");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("empty") << BoxF() << qreal(-1) << BoxF(0, -1, 0, 0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << qreal(0) << BoxF(2, 0, 4, 5);
}

void TestBoxF::moveTop()
{
    QFETCH(BoxF, box);
    QFETCH(qreal, value);

    box.moveTop(value);
    QTEST(box, "expected");
}

void TestBoxF::right_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<qreal>("right");

    QTest::addRow("default") << BoxF() << qreal(0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << qreal(6);
    QTest::addRow("QPointF(2, 3) QPointF(6, 8)") << BoxF(QPointF(2, 3), QPointF(6, 8)) << qreal(6);
    QTest::addRow("QPointF(2, 3) QSizeF(4, 5)") << BoxF(QPointF(2, 3), QSizeF(4, 5)) << qreal(6);
    QTest::addRow("QRect(2, 3, 4, 5)") << BoxF(QRect(2, 3, 4, 5)) << qreal(6);
}

void TestBoxF::right()
{
    QFETCH(BoxF, box);

    QTEST(box.right(), "right");
}

void TestBoxF::setRight_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<qreal>("value");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("empty") << BoxF() << qreal(1) << BoxF(0, 0, 1, 0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << qreal(12) << BoxF(2, 3, 10, 5);
}

void TestBoxF::setRight()
{
    QFETCH(BoxF, box);
    QFETCH(qreal, value);

    box.setRight(value);
    QTEST(box, "expected");
}

void TestBoxF::moveRight_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<qreal>("value");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("empty") << BoxF() << qreal(-1) << BoxF(-1, 0, 0, 0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << qreal(14) << BoxF(10, 3, 4, 5);
}

void TestBoxF::moveRight()
{
    QFETCH(BoxF, box);
    QFETCH(qreal, value);

    box.moveRight(value);
    QTEST(box, "expected");
}

void TestBoxF::bottom_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<qreal>("bottom");

    QTest::addRow("default") << BoxF() << qreal(0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << qreal(8);
    QTest::addRow("QPointF(2, 3) QPointF(6, 8)") << BoxF(QPointF(2, 3), QPointF(6, 8)) << qreal(8);
    QTest::addRow("QPointF(2, 3) QSizeF(4, 5)") << BoxF(QPointF(2, 3), QSizeF(4, 5)) << qreal(8);
    QTest::addRow("QRect(2, 3, 4, 5)") << BoxF(QRect(2, 3, 4, 5)) << qreal(8);
}

void TestBoxF::bottom()
{
    QFETCH(BoxF, box);

    QTEST(box.bottom(), "bottom");
}

void TestBoxF::setBottom_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<qreal>("value");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("empty") << BoxF() << qreal(1) << BoxF(0, 0, 0, 1);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << qreal(13) << BoxF(2, 3, 4, 10);
}

void TestBoxF::setBottom()
{
    QFETCH(BoxF, box);
    QFETCH(qreal, value);

    box.setBottom(value);
    QTEST(box, "expected");
}

void TestBoxF::moveBottom_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<qreal>("value");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("empty") << BoxF() << qreal(-1) << BoxF(0, -1, 0, 0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << qreal(15) << BoxF(2, 10, 4, 5);
}

void TestBoxF::moveBottom()
{
    QFETCH(BoxF, box);
    QFETCH(qreal, value);

    box.moveBottom(value);
    QTEST(box, "expected");
}

void TestBoxF::center_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<QPointF>("center");

    QTest::addRow("default") << BoxF() << QPointF(0, 0);
    QTest::addRow("2,3 0x0") << BoxF(2, 3, 0, 0) << QPointF(2, 3);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << QPointF(4, 5.5);
    QTest::addRow("2,3 4x6") << BoxF(2, 3, 4, 6) << QPointF(4, 6);
    QTest::addRow("2,3 5x5") << BoxF(2, 3, 5, 5) << QPointF(4.5, 5.5);
}

void TestBoxF::center()
{
    QFETCH(BoxF, box);

    QTEST(box.center(), "center");
}

void TestBoxF::moveCenter_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<QPointF>("center");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("default - 0,0") << BoxF() << QPointF(0, 0) << BoxF();
    QTest::addRow("2,3 0x0 - 2,3") << BoxF(2, 3, 0, 0) << QPointF(2, 3) << BoxF(2, 3, 0, 0);
    QTest::addRow("2,3 4x5 - 4,5") << BoxF(2, 3, 4, 5) << QPointF(4, 5) << BoxF(2, 3, 4, 5);
    QTest::addRow("2,3 4x6 - 4,6") << BoxF(2, 3, 4, 6) << QPointF(4, 6) << BoxF(2, 3, 4, 6);
    QTest::addRow("2,3 5x5 - 4,5") << BoxF(2, 3, 5, 5) << QPointF(4, 5) << BoxF(2, 3, 5, 5);

    QTest::addRow("2,3 4x5 - 10,10") << BoxF(2, 3, 4, 5) << QPointF(10, 10) << BoxF(8, 8, 4, 5);
    QTest::addRow("2,3 4x6 - 10,10") << BoxF(2, 3, 4, 6) << QPointF(10, 10) << BoxF(8, 7, 4, 6);

    QTest::addRow("2,3 5x5 - 10,10") << BoxF(2, 3, 5, 5) << QPointF(10, 10) << BoxF(8, 8, 5, 5);
    QTest::addRow("2,3 5x6 - 10,10") << BoxF(2, 3, 5, 6) << QPointF(10, 10) << BoxF(8, 7, 5, 6);
}

void TestBoxF::moveCenter()
{
    QFETCH(BoxF, box);
    QFETCH(QPointF, center);

    box.moveCenter(center);
    QCOMPARE(box.center(), center);
}

void TestBoxF::topLeft_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<QPointF>("topLeft");

    QTest::addRow("default") << BoxF() << QPointF(0, 0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << QPointF(2, 3);
    QTest::addRow("QPointF(2, 3) QPointF(6, 8)") << BoxF(QPointF(2, 3), QPointF(6, 8)) << QPointF(2, 3);
    QTest::addRow("QPointF(2, 3) QSizeF(4, 5)") << BoxF(QPointF(2, 3), QSizeF(4, 5)) << QPointF(2, 3);
    QTest::addRow("QRect(2, 3, 4, 5)") << BoxF(QRect(2, 3, 4, 5)) << QPointF(2, 3);
}

void TestBoxF::topLeft()
{
    QFETCH(BoxF, box);

    QTEST(box.topLeft(), "topLeft");
}

void TestBoxF::setTopLeft_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<QPointF>("value");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("default") << BoxF() << QPointF(-1, -2) << BoxF(-1, -2, 1, 2);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << QPointF(0, -1) << BoxF(0, -1, 6, 9);
    QTest::addRow("QPointF(2, 3) QPointF(6, 8)") << BoxF(QPointF(2, 3), QPointF(6, 8)) << QPointF(0, -1) << BoxF(0, -1, 6, 9);
    QTest::addRow("QPointF(2, 3) QSizeF(4, 5)") << BoxF(QPointF(2, 3), QSizeF(4, 5)) << QPointF(0, -1) << BoxF(0, -1, 6, 9);
    QTest::addRow("QRect(2, 3, 4, 5)") << BoxF(QRect(2, 3, 4, 5)) << QPointF(0, -1) << BoxF(0, -1, 6, 9);
}

void TestBoxF::setTopLeft()
{
    QFETCH(BoxF, box);
    QFETCH(QPointF, value);

    box.setTopLeft(value);
    QTEST(box, "expected");
}

void TestBoxF::moveTopLeft_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<QPointF>("value");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("default") << BoxF() << QPointF(1, 2) << BoxF(1, 2, 0, 0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << QPointF(10, 11) << BoxF(10, 11, 4, 5);
    QTest::addRow("QPointF(2, 3) QPointF(6, 8)") << BoxF(QPointF(2, 3), QPointF(6, 8)) << QPointF(10, 11) << BoxF(10, 11, 4, 5);
    QTest::addRow("QPointF(2, 3) QSizeF(4, 5)") << BoxF(QPointF(2, 3), QSizeF(4, 5)) << QPointF(10, 11) << BoxF(10, 11, 4, 5);
    QTest::addRow("QRect(2, 3, 4, 5)") << BoxF(QRect(2, 3, 4, 5)) << QPointF(10, 11) << BoxF(10, 11, 4, 5);
}

void TestBoxF::moveTopLeft()
{
    QFETCH(BoxF, box);
    QFETCH(QPointF, value);

    box.moveTopLeft(value);
    QTEST(box, "expected");
}

void TestBoxF::topRight_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<QPointF>("topRight");

    QTest::addRow("default") << BoxF() << QPointF(0, 0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << QPointF(6, 3);
    QTest::addRow("QPointF(2, 3) QPointF(6, 8)") << BoxF(QPointF(2, 3), QPointF(6, 8)) << QPointF(6, 3);
    QTest::addRow("QPointF(2, 3) QSizeF(4, 5)") << BoxF(QPointF(2, 3), QSizeF(4, 5)) << QPointF(6, 3);
    QTest::addRow("QRect(2, 3, 4, 5)") << BoxF(QRect(2, 3, 4, 5)) << QPointF(6, 3);
}

void TestBoxF::topRight()
{
    QFETCH(BoxF, box);

    QTEST(box.topRight(), "topRight");
}

void TestBoxF::setTopRight_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<QPointF>("value");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("default") << BoxF() << QPointF(1, -2) << BoxF(0, -2, 1, 2);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << QPointF(12, 0) << BoxF(2, 0, 10, 8);
    QTest::addRow("QPointF(2, 3) QPointF(6, 8)") << BoxF(QPointF(2, 3), QPointF(6, 8)) << QPointF(12, 0) << BoxF(2, 0, 10, 8);
    QTest::addRow("QPointF(2, 3) QSizeF(4, 5)") << BoxF(QPointF(2, 3), QSizeF(4, 5)) << QPointF(12, 0) << BoxF(2, 0, 10, 8);
    QTest::addRow("QRect(2, 3, 4, 5)") << BoxF(QRect(2, 3, 4, 5)) << QPointF(12, 0) << BoxF(2, 0, 10, 8);
}

void TestBoxF::setTopRight()
{
    QFETCH(BoxF, box);
    QFETCH(QPointF, value);

    box.setTopRight(value);
    QTEST(box, "expected");
}

void TestBoxF::moveTopRight_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<QPointF>("value");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("default") << BoxF() << QPointF(10, 1) << BoxF(10, 1, 0, 0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << QPointF(10, 1) << BoxF(6, 1, 4, 5);
    QTest::addRow("QPointF(2, 3) QPointF(6, 8)") << BoxF(QPointF(2, 3), QPointF(6, 8)) << QPointF(10, 1) << BoxF(6, 1, 4, 5);
    QTest::addRow("QPointF(2, 3) QSizeF(4, 5)") << BoxF(QPointF(2, 3), QSizeF(4, 5)) << QPointF(10, 1) << BoxF(6, 1, 4, 5);
    QTest::addRow("QRect(2, 3, 4, 5)") << BoxF(QRect(2, 3, 4, 5)) << QPointF(10, 1) << BoxF(6, 1, 4, 5);
}

void TestBoxF::moveTopRight()
{
    QFETCH(BoxF, box);
    QFETCH(QPointF, value);

    box.moveTopRight(value);
    QTEST(box, "expected");
}

void TestBoxF::bottomRight_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<QPointF>("bottomRight");

    QTest::addRow("default") << BoxF() << QPointF(0, 0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << QPointF(6, 8);
    QTest::addRow("QPointF(2, 3) QPointF(6, 8)") << BoxF(QPointF(2, 3), QPointF(6, 8)) << QPointF(6, 8);
    QTest::addRow("QPointF(2, 3) QSizeF(4, 5)") << BoxF(QPointF(2, 3), QSizeF(4, 5)) << QPointF(6, 8);
    QTest::addRow("QRect(2, 3, 4, 5)") << BoxF(QRect(2, 3, 4, 5)) << QPointF(6, 8);
}

void TestBoxF::bottomRight()
{
    QFETCH(BoxF, box);

    QTEST(box.bottomRight(), "bottomRight");
}

void TestBoxF::setBottomRight_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<QPointF>("value");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("default") << BoxF() << QPointF(1, 2) << BoxF(0, 0, 1, 2);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << QPointF(12, 16) << BoxF(2, 3, 10, 13);
    QTest::addRow("QPointF(2, 3) QPointF(6, 8)") << BoxF(QPointF(2, 3), QPointF(6, 8)) << QPointF(12, 16) << BoxF(2, 3, 10, 13);
    QTest::addRow("QPointF(2, 3) QSizeF(4, 5)") << BoxF(QPointF(2, 3), QSizeF(4, 5)) << QPointF(12, 16) << BoxF(2, 3, 10, 13);
    QTest::addRow("QRect(2, 3, 4, 5)") << BoxF(QRect(2, 3, 4, 5)) << QPointF(12, 16) << BoxF(2, 3, 10, 13);
}

void TestBoxF::setBottomRight()
{
    QFETCH(BoxF, box);
    QFETCH(QPointF, value);

    box.setBottomRight(value);
    QTEST(box, "expected");
}

void TestBoxF::moveBottomRight_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<QPointF>("value");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("default") << BoxF() << QPointF(10, 15) << BoxF(10, 15, 0, 0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << QPointF(10, 15) << BoxF(6, 10, 4, 5);
    QTest::addRow("QPointF(2, 3) QPointF(6, 8)") << BoxF(QPointF(2, 3), QPointF(6, 8)) << QPointF(10, 15) << BoxF(6, 10, 4, 5);
    QTest::addRow("QPointF(2, 3) QSizeF(4, 5)") << BoxF(QPointF(2, 3), QSizeF(4, 5)) << QPointF(10, 15) << BoxF(6, 10, 4, 5);
    QTest::addRow("QRect(2, 3, 4, 5)") << BoxF(QRect(2, 3, 4, 5)) << QPointF(10, 15) << BoxF(6, 10, 4, 5);
}

void TestBoxF::moveBottomRight()
{
    QFETCH(BoxF, box);
    QFETCH(QPointF, value);

    box.moveBottomRight(value);
    QTEST(box, "expected");
}

void TestBoxF::bottomLeft_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<QPointF>("bottomLeft");

    QTest::addRow("default") << BoxF() << QPointF(0, 0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << QPointF(2, 8);
    QTest::addRow("QPointF(2, 3) QPointF(6, 8)") << BoxF(QPointF(2, 3), QPointF(6, 8)) << QPointF(2, 8);
    QTest::addRow("QPointF(2, 3) QSizeF(4, 5)") << BoxF(QPointF(2, 3), QSizeF(4, 5)) << QPointF(2, 8);
    QTest::addRow("QRect(2, 3, 4, 5)") << BoxF(QRect(2, 3, 4, 5)) << QPointF(2, 8);
}

void TestBoxF::bottomLeft()
{
    QFETCH(BoxF, box);

    QTEST(box.bottomLeft(), "bottomLeft");
}

void TestBoxF::setBottomLeft_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<QPointF>("value");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("default") << BoxF() << QPointF(-1, 2) << BoxF(-1, 0, 1, 2);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << QPointF(0, 10) << BoxF(0, 3, 6, 7);
    QTest::addRow("QPointF(2, 3) QPointF(6, 8)") << BoxF(QPointF(2, 3), QPointF(6, 8)) << QPointF(0, 10) << BoxF(0, 3, 6, 7);
    QTest::addRow("QPointF(2, 3) QSizeF(4, 5)") << BoxF(QPointF(2, 3), QSizeF(4, 5)) << QPointF(0, 10) << BoxF(0, 3, 6, 7);
    QTest::addRow("QRect(2, 3, 4, 5)") << BoxF(QRect(2, 3, 4, 5)) << QPointF(0, 10) << BoxF(0, 3, 6, 7);
}

void TestBoxF::setBottomLeft()
{
    QFETCH(BoxF, box);
    QFETCH(QPointF, value);

    box.setBottomLeft(value);
    QTEST(box, "expected");
}

void TestBoxF::moveBottomLeft_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<QPointF>("value");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("default") << BoxF() << QPointF(1, 15) << BoxF(1, 15, 0, 0);
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << QPointF(1, 15) << BoxF(1, 10, 4, 5);
    QTest::addRow("QPointF(2, 3) QPointF(6, 8)") << BoxF(QPointF(2, 3), QPointF(6, 8)) << QPointF(1, 15) << BoxF(1, 10, 4, 5);
    QTest::addRow("QPointF(2, 3) QSizeF(4, 5)") << BoxF(QPointF(2, 3), QSizeF(4, 5)) << QPointF(1, 15) << BoxF(1, 10, 4, 5);
    QTest::addRow("QRect(2, 3, 4, 5)") << BoxF(QRect(2, 3, 4, 5)) << QPointF(1, 15) << BoxF(1, 10, 4, 5);
}

void TestBoxF::moveBottomLeft()
{
    QFETCH(BoxF, box);
    QFETCH(QPointF, value);

    box.moveBottomLeft(value);
    QTEST(box, "expected");
}

void TestBoxF::transposed_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("default") << BoxF() << BoxF();
    QTest::addRow("2,3 4x5") << BoxF(2, 3, 4, 5) << BoxF(2, 3, 5, 4);
}

void TestBoxF::transposed()
{
    QFETCH(BoxF, box);

    QTEST(box.transposed(), "expected");
}

void TestBoxF::translate_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<QPointF>("offset");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("default") << BoxF() << QPointF(1, 2) << BoxF(1, 2, 0, 0);
    QTest::addRow("2,3 4x5 by 1,0") << BoxF(2, 3, 4, 5) << QPointF(1, 0) << BoxF(3, 3, 4, 5);
    QTest::addRow("2,3 4x5 by 0,1") << BoxF(2, 3, 4, 5) << QPointF(0, 1) << BoxF(2, 4, 4, 5);
    QTest::addRow("2,3 4x5 by 1,1") << BoxF(2, 3, 4, 5) << QPointF(1, 1) << BoxF(3, 4, 4, 5);
}

void TestBoxF::translate()
{
    QFETCH(BoxF, box);
    QFETCH(QPointF, offset);

    {
        BoxF tmp = box;
        tmp.translate(offset);
        QTEST(tmp, "expected");
    }

    {
        BoxF tmp = box;
        tmp.translate(offset.x(), offset.y());
        QTEST(tmp, "expected");
    }
}

void TestBoxF::translated_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<QPointF>("offset");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("default") << BoxF() << QPointF(1, 2) << BoxF(1, 2, 0, 0);
    QTest::addRow("2,3 4x5 by 1,0") << BoxF(2, 3, 4, 5) << QPointF(1, 0) << BoxF(3, 3, 4, 5);
    QTest::addRow("2,3 4x5 by 0,1") << BoxF(2, 3, 4, 5) << QPointF(0, 1) << BoxF(2, 4, 4, 5);
    QTest::addRow("2,3 4x5 by 1,1") << BoxF(2, 3, 4, 5) << QPointF(1, 1) << BoxF(3, 4, 4, 5);
}

void TestBoxF::translated()
{
    QFETCH(BoxF, box);
    QFETCH(QPointF, offset);

    QTEST(box.translated(offset), "expected");
    QTEST(box.translated(offset.x(), offset.y()), "expected");
}

void TestBoxF::adjust_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<int>("x0");
    QTest::addColumn<int>("y0");
    QTest::addColumn<int>("x1");
    QTest::addColumn<int>("y1");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("default - -1,0,0,0") << BoxF() << -1 << 0 << 0 << 0 << BoxF(-1, 0, 1, 0);
    QTest::addRow("default - 0,-1,0,0") << BoxF() << 0 << -1 << 0 << 0 << BoxF(0, -1, 0, 1);
    QTest::addRow("default - 0,0,1,0") << BoxF() << 0 << 0 << 1 << 0 << BoxF(0, 0, 1, 0);
    QTest::addRow("default - 0,0,0,1") << BoxF() << 0 << 0 << 0 << 1 << BoxF(0, 0, 0, 1);
    QTest::addRow("default - -1,0,1,0") << BoxF() << -1 << 0 << 1 << 0 << BoxF(-1, 0, 2, 0);
    QTest::addRow("default - 0,-1,0,1") << BoxF() << 0 << -1 << 0 << 1 << BoxF(0, -1, 0, 2);
    QTest::addRow("default - -1,-1,1,1") << BoxF() << -1 << -1 << 1 << 1 << BoxF(-1, -1, 2, 2);

    QTest::addRow("2,3 4x5 - -1,0,1,0") << BoxF(2, 3, 4, 5) << -1 << 0 << 1 << 0 << BoxF(1, 3, 6, 5);
    QTest::addRow("2,3 4x5 - 0,-1,0,1") << BoxF(2, 3, 4, 5) << 0 << -1 << 0 << 1 << BoxF(2, 2, 4, 7);
}

void TestBoxF::adjust()
{
    QFETCH(BoxF, box);
    QFETCH(int, x0);
    QFETCH(int, y0);
    QFETCH(int, x1);
    QFETCH(int, y1);

    box.adjust(x0, y0, x1, y1);
    QTEST(box, "expected");
}

void TestBoxF::adjusted_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<int>("x0");
    QTest::addColumn<int>("y0");
    QTest::addColumn<int>("x1");
    QTest::addColumn<int>("y1");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("default - -1,0,0,0") << BoxF() << -1 << 0 << 0 << 0 << BoxF(-1, 0, 1, 0);
    QTest::addRow("default - 0,-1,0,0") << BoxF() << 0 << -1 << 0 << 0 << BoxF(0, -1, 0, 1);
    QTest::addRow("default - 0,0,1,0") << BoxF() << 0 << 0 << 1 << 0 << BoxF(0, 0, 1, 0);
    QTest::addRow("default - 0,0,0,1") << BoxF() << 0 << 0 << 0 << 1 << BoxF(0, 0, 0, 1);
    QTest::addRow("default - -1,0,1,0") << BoxF() << -1 << 0 << 1 << 0 << BoxF(-1, 0, 2, 0);
    QTest::addRow("default - 0,-1,0,1") << BoxF() << 0 << -1 << 0 << 1 << BoxF(0, -1, 0, 2);
    QTest::addRow("default - -1,-1,1,1") << BoxF() << -1 << -1 << 1 << 1 << BoxF(-1, -1, 2, 2);

    QTest::addRow("2,3 4x5 - -1,0,1,0") << BoxF(2, 3, 4, 5) << -1 << 0 << 1 << 0 << BoxF(1, 3, 6, 5);
    QTest::addRow("2,3 4x5 - 0,-1,0,1") << BoxF(2, 3, 4, 5) << 0 << -1 << 0 << 1 << BoxF(2, 2, 4, 7);
}

void TestBoxF::adjusted()
{
    QFETCH(BoxF, box);
    QFETCH(int, x0);
    QFETCH(int, y0);
    QFETCH(int, x1);
    QFETCH(int, y1);

    QTEST(box.adjusted(x0, y0, x1, y1), "expected");
}

void TestBoxF::marginsAdded_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<QMargins>("margins");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("2,3 4x5 - 1,0,0,0") << BoxF(2, 3, 4, 5) << QMargins(1, 0, 0, 0) << BoxF(1, 3, 5, 5);
    QTest::addRow("2,3 4x5 - 0,1,0,0") << BoxF(2, 3, 4, 5) << QMargins(0, 1, 0, 0) << BoxF(2, 2, 4, 6);
    QTest::addRow("2,3 4x5 - 0,0,1,0") << BoxF(2, 3, 4, 5) << QMargins(0, 0, 1, 0) << BoxF(2, 3, 5, 5);
    QTest::addRow("2,3 4x5 - 0,0,0,1") << BoxF(2, 3, 4, 5) << QMargins(0, 0, 0, 1) << BoxF(2, 3, 4, 6);
    QTest::addRow("2,3 4x5 - 1,1,1,1") << BoxF(2, 3, 4, 5) << QMargins(1, 1, 1, 1) << BoxF(1, 2, 6, 7);
}

void TestBoxF::marginsAdded()
{
    QFETCH(BoxF, box);
    QFETCH(QMargins, margins);

    {
        QTEST(box.marginsAdded(margins), "expected");
    }

    {
        BoxF tmp = box;
        tmp += margins;
        QTEST(tmp, "expected");
    }
}

void TestBoxF::marginsRemoved_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<QMargins>("margins");
    QTest::addColumn<BoxF>("expected");

    QTest::addRow("2,3 4x5 - 1,0,0,0") << BoxF(2, 3, 4, 5) << QMargins(1, 0, 0, 0) << BoxF(3, 3, 3, 5);
    QTest::addRow("2,3 4x5 - 0,1,0,0") << BoxF(2, 3, 4, 5) << QMargins(0, 1, 0, 0) << BoxF(2, 4, 4, 4);
    QTest::addRow("2,3 4x5 - 0,0,1,0") << BoxF(2, 3, 4, 5) << QMargins(0, 0, 1, 0) << BoxF(2, 3, 3, 5);
    QTest::addRow("2,3 4x5 - 0,0,0,1") << BoxF(2, 3, 4, 5) << QMargins(0, 0, 0, 1) << BoxF(2, 3, 4, 4);
    QTest::addRow("2,3 4x5 - 1,1,1,1") << BoxF(2, 3, 4, 5) << QMargins(1, 1, 1, 1) << BoxF(3, 4, 2, 3);
}

void TestBoxF::marginsRemoved()
{
    QFETCH(BoxF, box);
    QFETCH(QMargins, margins);

    {
        QTEST(box.marginsRemoved(margins), "expected");
    }

    {
        BoxF tmp = box;
        tmp -= margins;
        QTEST(tmp, "expected");
    }
}

void TestBoxF::united_data()
{
    QTest::addColumn<BoxF>("box1");
    QTest::addColumn<BoxF>("box2");
    QTest::addColumn<BoxF>("result");

    QTest::addRow("empty - empty") << BoxF() << BoxF() << BoxF();
    QTest::addRow("2,3 4x5 - empty") << BoxF(2, 3, 4, 5) << BoxF() << BoxF(2, 3, 4, 5);
    QTest::addRow("empty - 2,3 4x5") << BoxF() << BoxF(2, 3, 4, 5) << BoxF(2, 3, 4, 5);
    QTest::addRow("2,3 4x5 - 2,3 4x5") << BoxF(2, 3, 4, 5) << BoxF(2, 3, 4, 5) << BoxF(2, 3, 4, 5);

    QTest::addRow("1,0 1x5 - 5,0 1x5") << BoxF(1, 0, 1, 5) << BoxF(5, 0, 1, 5) << BoxF(1, 0, 5, 5);
    QTest::addRow("0,1 5x1 - 0,5 5x1") << BoxF(0, 1, 5, 1) << BoxF(0, 5, 5, 1) << BoxF(0, 1, 5, 5);

    QTest::addRow("2,3 4x5 - 10,10 0x0") << BoxF(2, 3, 4, 5) << BoxF(10, 10, 0, 0) << BoxF(2, 3, 4, 5);
}

void TestBoxF::united()
{
    QFETCH(BoxF, box1);
    QFETCH(BoxF, box2);

    {
        QTEST(box1.united(box2), "result");
    }

    {
        QTEST(box1 | box2, "result");
    }

    {
        BoxF tmp = box1;
        tmp |= box2;
        QTEST(tmp, "result");
    }
}

void TestBoxF::intersects_data()
{
    QTest::addColumn<BoxF>("box1");
    QTest::addColumn<BoxF>("box2");
    QTest::addColumn<bool>("intersects");

    QTest::addRow("empty - empty") << BoxF() << BoxF() << false;
    QTest::addRow("2,3 4x5 - empty") << BoxF(2, 3, 4, 5) << BoxF() << false;
    QTest::addRow("empty - 2,3 4x5") << BoxF() << BoxF(2, 3, 4, 5) << false;

    QTest::addRow("0,0 5x5 - 0,-5 5x5") << BoxF(0, 0, 5, 5) << BoxF(0, -5, 5, 5) << false;
    QTest::addRow("0,0 5x5 - -5,0 5x5") << BoxF(0, 0, 5, 5) << BoxF(-5, 0, 5, 5) << false;
    QTest::addRow("0,0 5x5 - -5,-5 5x5") << BoxF(0, 0, 5, 5) << BoxF(-5, -5, 5, 5) << false;
    QTest::addRow("0,0 5x5 - 0,5 5x5") << BoxF(0, 0, 5, 5) << BoxF(0, 5, 5, 5) << false;
    QTest::addRow("0,0 5x5 - 5,0 5x5") << BoxF(0, 0, 5, 5) << BoxF(5, 0, 5, 5) << false;
    QTest::addRow("0,0 5x5 - 5,5 5x5") << BoxF(0, 0, 5, 5) << BoxF(5, 5, 5, 5) << false;

    QTest::addRow("0,0 5x5 - 2,2 5x5") << BoxF(0, 0, 5, 5) << BoxF(2, 2, 5, 5) << true;
    QTest::addRow("2,2 5x5 - 0,0 5x5") << BoxF(2, 2, 5, 5) << BoxF(0, 0, 5, 5) << true;
}

void TestBoxF::intersects()
{
    QFETCH(BoxF, box1);
    QFETCH(BoxF, box2);

    QTEST(box1.intersects(box2), "intersects");
}

void TestBoxF::intersected_data()
{
    QTest::addColumn<BoxF>("box1");
    QTest::addColumn<BoxF>("box2");
    QTest::addColumn<BoxF>("result");

    QTest::addRow("empty - empty") << BoxF() << BoxF() << BoxF();
    QTest::addRow("2,3 4x5 - empty") << BoxF(2, 3, 4, 5) << BoxF() << BoxF();
    QTest::addRow("empty - 2,3 4x5") << BoxF() << BoxF(2, 3, 4, 5) << BoxF();

    QTest::addRow("0,0 5x5 - 0,-5 5x5") << BoxF(0, 0, 5, 5) << BoxF(0, -5, 5, 5) << BoxF();
    QTest::addRow("0,0 5x5 - -5,0 5x5") << BoxF(0, 0, 5, 5) << BoxF(-5, 0, 5, 5) << BoxF();
    QTest::addRow("0,0 5x5 - -5,-5 5x5") << BoxF(0, 0, 5, 5) << BoxF(-5, -5, 5, 5) << BoxF();
    QTest::addRow("0,0 5x5 - 0,5 5x5") << BoxF(0, 0, 5, 5) << BoxF(0, 5, 5, 5) << BoxF();
    QTest::addRow("0,0 5x5 - 5,0 5x5") << BoxF(0, 0, 5, 5) << BoxF(5, 0, 5, 5) << BoxF();
    QTest::addRow("0,0 5x5 - 5,5 5x5") << BoxF(0, 0, 5, 5) << BoxF(5, 5, 5, 5) << BoxF();

    QTest::addRow("0,0 5x5 - 2,2 5x5") << BoxF(0, 0, 5, 5) << BoxF(2, 2, 5, 5) << BoxF(2, 2, 3, 3);
    QTest::addRow("2,2 5x5 - 0,0 5x5") << BoxF(2, 2, 5, 5) << BoxF(0, 0, 5, 5) << BoxF(2, 2, 3, 3);
}

void TestBoxF::intersected()
{
    QFETCH(BoxF, box1);
    QFETCH(BoxF, box2);

    {
        QTEST(box1.intersected(box2), "result");
    }

    {
        QTEST(box1 & box2, "result");
    }

    {
        BoxF tmp = box1;
        tmp &= box2;
        QTEST(tmp, "result");
    }
}

void TestBoxF::rounded_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<Box>("expected");

    QTest::addRow("default") << BoxF() << Box();

    QTest::addRow("1,2 3,4") << BoxF(QPointF(1, 2), QPointF(3, 4)) << Box(QPoint(1, 2), QPoint(3, 4));
    QTest::addRow("1.1,2.1 3.1,4.1") << BoxF(QPointF(1.1, 2.1), QPointF(3.1, 4.1)) << Box(QPoint(1, 2), QPoint(3, 4));
    QTest::addRow("1.5,2.5 3.5,4.5") << BoxF(QPointF(1.5, 2.5), QPointF(3.5, 4.5)) << Box(QPoint(2, 3), QPoint(4, 5));
    QTest::addRow("1.9,2.9 3.9,4.9") << BoxF(QPointF(1.9, 2.9), QPointF(3.9, 4.9)) << Box(QPoint(2, 3), QPoint(4, 5));

    QTest::addRow("-3,-4 -1,-2") << BoxF(QPointF(-3, -4), QPointF(-1, -2)) << Box(QPoint(-3, -4), QPoint(-1, -2));
    QTest::addRow("-3.1,-4.1 -1.1,-2.1") << BoxF(QPointF(-3.1, -4.1), QPointF(-1.1, -2.1)) << Box(QPoint(-3, -4), QPoint(-1, -2));
    QTest::addRow("-3.5,-4.5 -1.5,-2.5") << BoxF(QPointF(-3.5, -4.5), QPointF(-1.5, -2.5)) << Box(QPoint(-4, -5), QPoint(-2, -3));
    QTest::addRow("-3.9,-4.9 -1.9,-2.9") << BoxF(QPointF(-3.9, -4.9), QPointF(-1.9, -2.9)) << Box(QPoint(-4, -5), QPoint(-2, -3));
}

void TestBoxF::rounded()
{
    QFETCH(BoxF, box);
    QFETCH(Box, expected);

    const Box rounded = box.rounded();
    QCOMPARE(rounded, expected);
    QVERIFY(std::abs(box.left() - rounded.left()) <= 0.5);
    QVERIFY(std::abs(box.top() - rounded.top()) <= 0.5);
    QVERIFY(std::abs(box.right() - rounded.right()) <= 0.5);
    QVERIFY(std::abs(box.bottom() - rounded.bottom()) <= 0.5);
}

void TestBoxF::roundedOut_data()
{
    QTest::addColumn<BoxF>("box");
    QTest::addColumn<Box>("expected");

    QTest::addRow("default") << BoxF() << Box();

    QTest::addRow("1,2 3,4") << BoxF(QPointF(1, 2), QPointF(3, 4)) << Box(QPoint(1, 2), QPoint(3, 4));
    QTest::addRow("1.1,2.1 3.1,4.1") << BoxF(QPointF(1.1, 2.1), QPointF(3.1, 4.1)) << Box(QPoint(1, 2), QPoint(4, 5));
    QTest::addRow("1.5,2.5 3.5,4.5") << BoxF(QPointF(1.5, 2.5), QPointF(3.5, 4.5)) << Box(QPoint(1, 2), QPoint(4, 5));
    QTest::addRow("1.9,2.9 3.9,4.9") << BoxF(QPointF(1.9, 2.9), QPointF(3.9, 4.9)) << Box(QPoint(1, 2), QPoint(4, 5));

    QTest::addRow("-3,-4 -1,-2") << BoxF(QPointF(-3, -4), QPointF(-1, -2)) << Box(QPoint(-3, -4), QPoint(-1, -2));
    QTest::addRow("-3.1,-4.1 -1.1,-2.1") << BoxF(QPointF(-3.1, -4.1), QPointF(-1.1, -2.1)) << Box(QPoint(-4, -5), QPoint(-1, -2));
    QTest::addRow("-3.5,-4.5 -1.5,-2.5") << BoxF(QPointF(-3.5, -4.5), QPointF(-1.5, -2.5)) << Box(QPoint(-4, -5), QPoint(-1, -2));
    QTest::addRow("-3.9,-4.9 -1.9,-2.9") << BoxF(QPointF(-3.9, -4.9), QPointF(-1.9, -2.9)) << Box(QPoint(-4, -5), QPoint(-1, -2));
}

void TestBoxF::roundedOut()
{
    QFETCH(BoxF, box);
    QFETCH(Box, expected);

    const Box rounded = box.roundedOut();
    QCOMPARE(rounded, expected);
    QVERIFY(std::abs(box.left() - rounded.left()) < 1.0);
    QVERIFY(std::abs(box.top() - rounded.top()) < 1.0);
    QVERIFY(std::abs(box.right() - rounded.right()) < 1.0);
    QVERIFY(std::abs(box.bottom() - rounded.bottom()) < 1.0);
}

QTEST_MAIN(TestBoxF)

#include "test_boxf.moc"
