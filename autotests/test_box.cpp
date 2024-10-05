/*
    SPDX-FileCopyrightText: 2024 Vlad Zahorodnii <vlad.zahorodnii@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include <QImage>
#include <QTest>

#include "core/box.h"

using namespace KWin;

class TestBox : public QObject
{
    Q_OBJECT

public:
    TestBox() = default;

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
};

void TestBox::equals_data()
{
    QTest::addColumn<Box>("box1");
    QTest::addColumn<Box>("box2");
    QTest::addColumn<bool>("expected");

    QTest::addRow("default == default") << Box() << Box() << true;
    QTest::addRow("2,3 4x5 == default") << Box(2, 3, 4, 5) << Box() << false;
    QTest::addRow("default == 2,3 4x5") << Box() << Box(2, 3, 4, 5) << false;
    QTest::addRow("2,3 4x5 == 2,3 4x5") << Box(2, 3, 4, 5) << Box(2, 3, 4, 5) << true;

    QTest::addRow("2,3 4x5 == 0,3 4x5") << Box(2, 3, 4, 5) << Box(0, 3, 4, 5) << false;
    QTest::addRow("2,3 4x5 == 2,0 4x5") << Box(2, 3, 4, 5) << Box(2, 0, 4, 5) << false;
    QTest::addRow("2,3 4x5 == 2,3 0x5") << Box(2, 3, 4, 5) << Box(2, 3, 0, 5) << false;
    QTest::addRow("2,3 4x5 == 2,3 4x0") << Box(2, 3, 4, 5) << Box(2, 3, 4, 0) << false;
}

void TestBox::equals()
{
    QFETCH(Box, box1);
    QFETCH(Box, box2);

    QTEST(box1 == box2, "expected");
}

void TestBox::notEquals_data()
{
    QTest::addColumn<Box>("box1");
    QTest::addColumn<Box>("box2");
    QTest::addColumn<bool>("expected");

    QTest::addRow("default == default") << Box() << Box() << false;
    QTest::addRow("2,3 4x5 == default") << Box(2, 3, 4, 5) << Box() << true;
    QTest::addRow("default == 2,3 4x5") << Box() << Box(2, 3, 4, 5) << true;
    QTest::addRow("2,3 4x5 == 2,3 4x5") << Box(2, 3, 4, 5) << Box(2, 3, 4, 5) << false;

    QTest::addRow("2,3 4x5 == 0,3 4x5") << Box(2, 3, 4, 5) << Box(0, 3, 4, 5) << true;
    QTest::addRow("2,3 4x5 == 2,0 4x5") << Box(2, 3, 4, 5) << Box(2, 0, 4, 5) << true;
    QTest::addRow("2,3 4x5 == 2,3 0x5") << Box(2, 3, 4, 5) << Box(2, 3, 0, 5) << true;
    QTest::addRow("2,3 4x5 == 2,3 4x0") << Box(2, 3, 4, 5) << Box(2, 3, 4, 0) << true;
}

void TestBox::notEquals()
{
    QFETCH(Box, box1);
    QFETCH(Box, box2);

    QTEST(box1 != box2, "expected");
}

void TestBox::empty_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<bool>("empty");

    QTest::addRow("default") << Box() << true;

    QTest::addRow("0,0 0x0") << Box(0, 0, 0, 0) << true;
    QTest::addRow("2,3 0x0") << Box(2, 3, 0, 0) << true;
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << false;

    QTest::addRow("QPoint(1, 1) QPoint(1, 1)") << Box(QPoint(1, 1), QPoint(1, 1)) << true;
    QTest::addRow("QPoint(1, 1) QPoint(1, 2)") << Box(QPoint(1, 1), QPoint(1, 2)) << true;
    QTest::addRow("QPoint(1, 1) QPoint(2, 1)") << Box(QPoint(1, 1), QPoint(2, 1)) << true;
    QTest::addRow("QPoint(1, 1) QPoint(2, 2)") << Box(QPoint(1, 1), QPoint(2, 2)) << false;

    QTest::addRow("QPoint(1, 1) QSize(0, 0)") << Box(QPoint(1, 1), QSize(0, 0)) << true;
    QTest::addRow("QPoint(1, 1) QSize(0, 1)") << Box(QPoint(1, 1), QSize(0, 1)) << true;
    QTest::addRow("QPoint(1, 1) QSize(1, 0)") << Box(QPoint(1, 1), QSize(1, 0)) << true;
    QTest::addRow("QPoint(1, 1) QSize(1, 1)") << Box(QPoint(1, 1), QSize(1, 1)) << false;

    QTest::addRow("QRect()") << Box(QRect()) << true;
    QTest::addRow("QRect(1, 1, 0, 0)") << Box(QRect(1, 1, 0, 0)) << true;
    QTest::addRow("QRect(1, 1, 0, 1)") << Box(QRect(1, 1, 0, 1)) << true;
    QTest::addRow("QRect(1, 1, 1, 0)") << Box(QRect(1, 1, 1, 0)) << true;
    QTest::addRow("QRect(1, 1, 1, 1)") << Box(QRect(1, 1, 1, 1)) << false;
}

void TestBox::empty()
{
    QFETCH(Box, box);

    QTEST(box.isEmpty(), "empty");
}

void TestBox::containsPoint_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<QPoint>("point");
    QTest::addColumn<bool>("contains");

    QTest::addRow("empty - 0,0") << Box() << QPoint(0, 0) << false;
    QTest::addRow("empty - 1,1") << Box() << QPoint(1, 1) << false;

    QTest::addRow("2,3 4x5 - 1,2") << Box(2, 3, 4, 5) << QPoint(1, 2) << false;
    QTest::addRow("2,3 4x5 - 7,9") << Box(2, 3, 4, 5) << QPoint(7, 9) << false;
    QTest::addRow("2,3 4x5 - 1,9") << Box(2, 3, 4, 5) << QPoint(1, 9) << false;
    QTest::addRow("2,3 4x5 - 7,2") << Box(2, 3, 4, 5) << QPoint(7, 2) << false;

    QTest::addRow("2,3 4x5 - 2,3") << Box(2, 3, 4, 5) << QPoint(2, 3) << true;
    QTest::addRow("2,3 4x5 - 6,3") << Box(2, 3, 4, 5) << QPoint(6, 3) << false;
    QTest::addRow("2,3 4x5 - 5,3") << Box(2, 3, 4, 5) << QPoint(5, 3) << true;
    QTest::addRow("2,3 4x5 - 6,8") << Box(2, 3, 4, 5) << QPoint(6, 8) << false;
    QTest::addRow("2,3 4x5 - 5,7") << Box(2, 3, 4, 5) << QPoint(5, 7) << true;
    QTest::addRow("2,3 4x5 - 2,8") << Box(2, 3, 4, 5) << QPoint(2, 8) << false;
    QTest::addRow("2,3 4x5 - 2,7") << Box(2, 3, 4, 5) << QPoint(2, 7) << true;

    QTest::addRow("2,3 4x5 - 2,5") << Box(2, 3, 4, 5) << QPoint(2, 5) << true;
    QTest::addRow("2,3 4x5 - 1,5") << Box(2, 3, 4, 5) << QPoint(1, 5) << false;
    QTest::addRow("2,3 4x5 - 3,5") << Box(2, 3, 4, 5) << QPoint(3, 5) << true;

    QTest::addRow("2,3 4x5 - 4,3") << Box(2, 3, 4, 5) << QPoint(4, 3) << true;
    QTest::addRow("2,3 4x5 - 4,2") << Box(2, 3, 4, 5) << QPoint(4, 2) << false;
    QTest::addRow("2,3 4x5 - 4,4") << Box(2, 3, 4, 5) << QPoint(4, 4) << true;

    QTest::addRow("2,3 4x5 - 6,5") << Box(2, 3, 4, 5) << QPoint(6, 5) << false;
    QTest::addRow("2,3 4x5 - 5,5") << Box(2, 3, 4, 5) << QPoint(5, 5) << true;
    QTest::addRow("2,3 4x5 - 7,5") << Box(2, 3, 4, 5) << QPoint(7, 5) << false;

    QTest::addRow("2,3 4x5 - 4,8") << Box(2, 3, 4, 5) << QPoint(4, 8) << false;
    QTest::addRow("2,3 4x5 - 4,7") << Box(2, 3, 4, 5) << QPoint(4, 7) << true;
    QTest::addRow("2,3 4x5 - 4,9") << Box(2, 3, 4, 5) << QPoint(4, 9) << false;
}

void TestBox::containsPoint()
{
    QFETCH(Box, box);
    QFETCH(QPoint, point);

    QTEST(box.contains(point), "contains");
    QTEST(box.contains(point.x(), point.y()), "contains");
}

void TestBox::containsRect_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<Box>("other");
    QTest::addColumn<bool>("contains");

    QTest::addRow("empty - empty") << Box() << Box() << false;
    QTest::addRow("empty - 2,3 4x5") << Box() << Box(2, 3, 4, 5) << false;
    QTest::addRow("2,3 4x5 - empty") << Box(2, 3, 4, 5) << Box() << false;

    QTest::addRow("2,3 4x5 - 2,3 4x5") << Box(2, 3, 4, 5) << Box(2, 3, 4, 5) << true;
    QTest::addRow("2,3 4x5 - 2,3 0x0") << Box(2, 3, 4, 5) << Box(2, 3, 0, 0) << true;
    QTest::addRow("2,3 4x5 - 2,3 4x6") << Box(2, 3, 4, 5) << Box(2, 3, 4, 6) << false;
    QTest::addRow("2,3 4x5 - 2,3 5x5") << Box(2, 3, 4, 5) << Box(2, 3, 5, 5) << false;
    QTest::addRow("2,3 4x5 - 1,3 4x5") << Box(2, 3, 4, 5) << Box(1, 3, 4, 5) << false;
    QTest::addRow("2,3 4x5 - 2,2 4x5") << Box(2, 3, 4, 5) << Box(2, 2, 4, 5) << false;

    QTest::addRow("5,5 1x1 - 2,2 1x1") << Box(5, 5, 1, 1) << Box(2, 2, 1, 1) << false;
    QTest::addRow("2,2 1x1 - 5,5 1x1") << Box(2, 2, 1, 1) << Box(5, 5, 1, 1) << false;
}

void TestBox::containsRect()
{
    QFETCH(Box, box);
    QFETCH(Box, other);

    QTEST(box.contains(other), "contains");
}

void TestBox::x_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<int>("x");

    QTest::addRow("default") << Box() << 0;
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << 2;
    QTest::addRow("QPoint(2, 3) QPoint(6, 8)") << Box(QPoint(2, 3), QPoint(6, 8)) << 2;
    QTest::addRow("QPoint(2, 3) QSize(4, 5)") << Box(QPoint(2, 3), QSize(4, 5)) << 2;
    QTest::addRow("QRect(2, 3, 4, 5)") << Box(QRect(2, 3, 4, 5)) << 2;
}

void TestBox::x()
{
    QFETCH(Box, box);

    QTEST(box.x(), "x");
}

void TestBox::setX_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<int>("value");
    QTest::addColumn<Box>("expected");

    QTest::addRow("empty") << Box() << -1 << Box(-1, 0, 0, 0);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << 0 << Box(0, 3, 4, 5);
}

void TestBox::setX()
{
    QFETCH(Box, box);
    QFETCH(int, value);

    box.setX(value);
    QTEST(box, "expected");
}

void TestBox::y_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<int>("y");

    QTest::addRow("default") << Box() << 0;
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << 3;
    QTest::addRow("QPoint(2, 3) QPoint(6, 8)") << Box(QPoint(2, 3), QPoint(6, 8)) << 3;
    QTest::addRow("QPoint(2, 3) QSize(4, 5)") << Box(QPoint(2, 3), QSize(4, 5)) << 3;
    QTest::addRow("QRect(2, 3, 4, 5)") << Box(QRect(2, 3, 4, 5)) << 3;
}

void TestBox::y()
{
    QFETCH(Box, box);

    QTEST(box.y(), "y");
}

void TestBox::setY_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<int>("value");
    QTest::addColumn<Box>("expected");

    QTest::addRow("empty") << Box() << -1 << Box(0, -1, 0, 0);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << 0 << Box(2, 0, 4, 5);
}

void TestBox::setY()
{
    QFETCH(Box, box);
    QFETCH(int, value);

    box.setY(value);
    QTEST(box, "expected");
}

void TestBox::width_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<int>("width");

    QTest::addRow("default") << Box() << 0;
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << 4;
    QTest::addRow("QPoint(2, 3) QPoint(6, 8)") << Box(QPoint(2, 3), QPoint(6, 8)) << 4;
    QTest::addRow("QPoint(2, 3) QSize(4, 5)") << Box(QPoint(2, 3), QSize(4, 5)) << 4;
    QTest::addRow("QRect(2, 3, 4, 5)") << Box(QRect(2, 3, 4, 5)) << 4;
}

void TestBox::width()
{
    QFETCH(Box, box);

    QTEST(box.width(), "width");
}

void TestBox::setWidth_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<int>("value");
    QTest::addColumn<Box>("expected");

    QTest::addRow("empty") << Box() << 1 << Box(0, 0, 1, 0);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << 0 << Box(2, 3, 0, 5);
}

void TestBox::setWidth()
{
    QFETCH(Box, box);
    QFETCH(int, value);

    box.setWidth(value);
    QTEST(box, "expected");
}

void TestBox::height_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<int>("height");

    QTest::addRow("default") << Box() << 0;
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << 5;
    QTest::addRow("QPoint(2, 3) QPoint(6, 8)") << Box(QPoint(2, 3), QPoint(6, 8)) << 5;
    QTest::addRow("QPoint(2, 3) QSize(4, 5)") << Box(QPoint(2, 3), QSize(4, 5)) << 5;
    QTest::addRow("QRect(2, 3, 4, 5)") << Box(QRect(2, 3, 4, 5)) << 5;
}

void TestBox::height()
{
    QFETCH(Box, box);

    QTEST(box.height(), "height");
}

void TestBox::setHeight_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<int>("value");
    QTest::addColumn<Box>("expected");

    QTest::addRow("empty") << Box() << 1 << Box(0, 0, 0, 1);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << 0 << Box(2, 3, 4, 0);
}

void TestBox::setHeight()
{
    QFETCH(Box, box);
    QFETCH(int, value);

    box.setHeight(value);
    QTEST(box, "expected");
}

void TestBox::left_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<int>("left");

    QTest::addRow("default") << Box() << 0;
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << 2;
    QTest::addRow("QPoint(2, 3) QPoint(6, 8)") << Box(QPoint(2, 3), QPoint(6, 8)) << 2;
    QTest::addRow("QPoint(2, 3) QSize(4, 5)") << Box(QPoint(2, 3), QSize(4, 5)) << 2;
    QTest::addRow("QRect(2, 3, 4, 5)") << Box(QRect(2, 3, 4, 5)) << 2;
}

void TestBox::left()
{
    QFETCH(Box, box);

    QTEST(box.left(), "left");
}

void TestBox::setLeft_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<int>("value");
    QTest::addColumn<Box>("expected");

    QTest::addRow("empty") << Box() << -1 << Box(-1, 0, 1, 0);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << 0 << Box(0, 3, 6, 5);
}

void TestBox::setLeft()
{
    QFETCH(Box, box);
    QFETCH(int, value);

    box.setLeft(value);
    QTEST(box, "expected");
}

void TestBox::moveLeft_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<int>("value");
    QTest::addColumn<Box>("expected");

    QTest::addRow("empty") << Box() << -1 << Box(-1, 0, 0, 0);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << 0 << Box(0, 3, 4, 5);
}

void TestBox::moveLeft()
{
    QFETCH(Box, box);
    QFETCH(int, value);

    box.moveLeft(value);
    QTEST(box, "expected");
}

void TestBox::top_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<int>("top");

    QTest::addRow("default") << Box() << 0;
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << 3;
    QTest::addRow("QPoint(2, 3) QPoint(6, 8)") << Box(QPoint(2, 3), QPoint(6, 8)) << 3;
    QTest::addRow("QPoint(2, 3) QSize(4, 5)") << Box(QPoint(2, 3), QSize(4, 5)) << 3;
    QTest::addRow("QRect(2, 3, 4, 5)") << Box(QRect(2, 3, 4, 5)) << 3;
}

void TestBox::top()
{
    QFETCH(Box, box);

    QTEST(box.top(), "top");
}

void TestBox::setTop_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<int>("value");
    QTest::addColumn<Box>("expected");

    QTest::addRow("empty") << Box() << -1 << Box(0, -1, 0, 1);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << 0 << Box(2, 0, 4, 8);
}

void TestBox::setTop()
{
    QFETCH(Box, box);
    QFETCH(int, value);

    box.setTop(value);
    QTEST(box, "expected");
}

void TestBox::moveTop_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<int>("value");
    QTest::addColumn<Box>("expected");

    QTest::addRow("empty") << Box() << -1 << Box(0, -1, 0, 0);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << 0 << Box(2, 0, 4, 5);
}

void TestBox::moveTop()
{
    QFETCH(Box, box);
    QFETCH(int, value);

    box.moveTop(value);
    QTEST(box, "expected");
}

void TestBox::right_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<int>("right");

    QTest::addRow("default") << Box() << 0;
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << 6;
    QTest::addRow("QPoint(2, 3) QPoint(6, 8)") << Box(QPoint(2, 3), QPoint(6, 8)) << 6;
    QTest::addRow("QPoint(2, 3) QSize(4, 5)") << Box(QPoint(2, 3), QSize(4, 5)) << 6;
    QTest::addRow("QRect(2, 3, 4, 5)") << Box(QRect(2, 3, 4, 5)) << 6;
}

void TestBox::right()
{
    QFETCH(Box, box);

    QTEST(box.right(), "right");
}

void TestBox::setRight_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<int>("value");
    QTest::addColumn<Box>("expected");

    QTest::addRow("empty") << Box() << 1 << Box(0, 0, 1, 0);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << 12 << Box(2, 3, 10, 5);
}

void TestBox::setRight()
{
    QFETCH(Box, box);
    QFETCH(int, value);

    box.setRight(value);
    QTEST(box, "expected");
}

void TestBox::moveRight_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<int>("value");
    QTest::addColumn<Box>("expected");

    QTest::addRow("empty") << Box() << -1 << Box(-1, 0, 0, 0);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << 14 << Box(10, 3, 4, 5);
}

void TestBox::moveRight()
{
    QFETCH(Box, box);
    QFETCH(int, value);

    box.moveRight(value);
    QTEST(box, "expected");
}

void TestBox::bottom_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<int>("bottom");

    QTest::addRow("default") << Box() << 0;
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << 8;
    QTest::addRow("QPoint(2, 3) QPoint(6, 8)") << Box(QPoint(2, 3), QPoint(6, 8)) << 8;
    QTest::addRow("QPoint(2, 3) QSize(4, 5)") << Box(QPoint(2, 3), QSize(4, 5)) << 8;
    QTest::addRow("QRect(2, 3, 4, 5)") << Box(QRect(2, 3, 4, 5)) << 8;
}

void TestBox::bottom()
{
    QFETCH(Box, box);

    QTEST(box.bottom(), "bottom");
}

void TestBox::setBottom_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<int>("value");
    QTest::addColumn<Box>("expected");

    QTest::addRow("empty") << Box() << 1 << Box(0, 0, 0, 1);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << 13 << Box(2, 3, 4, 10);
}

void TestBox::setBottom()
{
    QFETCH(Box, box);
    QFETCH(int, value);

    box.setBottom(value);
    QTEST(box, "expected");
}

void TestBox::moveBottom_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<int>("value");
    QTest::addColumn<Box>("expected");

    QTest::addRow("empty") << Box() << -1 << Box(0, -1, 0, 0);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << 15 << Box(2, 10, 4, 5);
}

void TestBox::moveBottom()
{
    QFETCH(Box, box);
    QFETCH(int, value);

    box.moveBottom(value);
    QTEST(box, "expected");
}

void TestBox::center_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<QPoint>("center");

    QTest::addRow("default") << Box() << QPoint(0, 0);
    QTest::addRow("2,3 0x0") << Box(2, 3, 0, 0) << QPoint(2, 3);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << QPoint(4, 5);
    QTest::addRow("2,3 4x6") << Box(2, 3, 4, 6) << QPoint(4, 6);
    QTest::addRow("2,3 5x5") << Box(2, 3, 5, 5) << QPoint(4, 5);
}

void TestBox::center()
{
    QFETCH(Box, box);

    QTEST(box.center(), "center");
}

void TestBox::moveCenter_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<QPoint>("center");
    QTest::addColumn<Box>("expected");

    QTest::addRow("default - 0,0") << Box() << QPoint(0, 0) << Box();
    QTest::addRow("2,3 0x0 - 2,3") << Box(2, 3, 0, 0) << QPoint(2, 3) << Box(2, 3, 0, 0);
    QTest::addRow("2,3 4x5 - 4,5") << Box(2, 3, 4, 5) << QPoint(4, 5) << Box(2, 3, 4, 5);
    QTest::addRow("2,3 4x6 - 4,6") << Box(2, 3, 4, 6) << QPoint(4, 6) << Box(2, 3, 4, 6);
    QTest::addRow("2,3 5x5 - 4,5") << Box(2, 3, 5, 5) << QPoint(4, 5) << Box(2, 3, 5, 5);

    QTest::addRow("2,3 4x5 - 10,10") << Box(2, 3, 4, 5) << QPoint(10, 10) << Box(8, 8, 4, 5);
    QTest::addRow("2,3 4x6 - 10,10") << Box(2, 3, 4, 6) << QPoint(10, 10) << Box(8, 7, 4, 6);

    QTest::addRow("2,3 5x5 - 10,10") << Box(2, 3, 5, 5) << QPoint(10, 10) << Box(8, 8, 5, 5);
    QTest::addRow("2,3 5x6 - 10,10") << Box(2, 3, 5, 6) << QPoint(10, 10) << Box(8, 7, 5, 6);
}

void TestBox::moveCenter()
{
    QFETCH(Box, box);
    QFETCH(QPoint, center);

    box.moveCenter(center);
    QCOMPARE(box.center(), center);
}

void TestBox::topLeft_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<QPoint>("topLeft");

    QTest::addRow("default") << Box() << QPoint(0, 0);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << QPoint(2, 3);
    QTest::addRow("QPoint(2, 3) QPoint(6, 8)") << Box(QPoint(2, 3), QPoint(6, 8)) << QPoint(2, 3);
    QTest::addRow("QPoint(2, 3) QSize(4, 5)") << Box(QPoint(2, 3), QSize(4, 5)) << QPoint(2, 3);
    QTest::addRow("QRect(2, 3, 4, 5)") << Box(QRect(2, 3, 4, 5)) << QPoint(2, 3);
}

void TestBox::topLeft()
{
    QFETCH(Box, box);

    QTEST(box.topLeft(), "topLeft");
}

void TestBox::setTopLeft_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<QPoint>("value");
    QTest::addColumn<Box>("expected");

    QTest::addRow("default") << Box() << QPoint(-1, -2) << Box(-1, -2, 1, 2);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << QPoint(0, -1) << Box(0, -1, 6, 9);
    QTest::addRow("QPoint(2, 3) QPoint(6, 8)") << Box(QPoint(2, 3), QPoint(6, 8)) << QPoint(0, -1) << Box(0, -1, 6, 9);
    QTest::addRow("QPoint(2, 3) QSize(4, 5)") << Box(QPoint(2, 3), QSize(4, 5)) << QPoint(0, -1) << Box(0, -1, 6, 9);
    QTest::addRow("QRect(2, 3, 4, 5)") << Box(QRect(2, 3, 4, 5)) << QPoint(0, -1) << Box(0, -1, 6, 9);
}

void TestBox::setTopLeft()
{
    QFETCH(Box, box);
    QFETCH(QPoint, value);

    box.setTopLeft(value);
    QTEST(box, "expected");
}

void TestBox::moveTopLeft_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<QPoint>("value");
    QTest::addColumn<Box>("expected");

    QTest::addRow("default") << Box() << QPoint(1, 2) << Box(1, 2, 0, 0);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << QPoint(10, 11) << Box(10, 11, 4, 5);
    QTest::addRow("QPoint(2, 3) QPoint(6, 8)") << Box(QPoint(2, 3), QPoint(6, 8)) << QPoint(10, 11) << Box(10, 11, 4, 5);
    QTest::addRow("QPoint(2, 3) QSize(4, 5)") << Box(QPoint(2, 3), QSize(4, 5)) << QPoint(10, 11) << Box(10, 11, 4, 5);
    QTest::addRow("QRect(2, 3, 4, 5)") << Box(QRect(2, 3, 4, 5)) << QPoint(10, 11) << Box(10, 11, 4, 5);
}

void TestBox::moveTopLeft()
{
    QFETCH(Box, box);
    QFETCH(QPoint, value);

    box.moveTopLeft(value);
    QTEST(box, "expected");
}

void TestBox::topRight_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<QPoint>("topRight");

    QTest::addRow("default") << Box() << QPoint(0, 0);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << QPoint(6, 3);
    QTest::addRow("QPoint(2, 3) QPoint(6, 8)") << Box(QPoint(2, 3), QPoint(6, 8)) << QPoint(6, 3);
    QTest::addRow("QPoint(2, 3) QSize(4, 5)") << Box(QPoint(2, 3), QSize(4, 5)) << QPoint(6, 3);
    QTest::addRow("QRect(2, 3, 4, 5)") << Box(QRect(2, 3, 4, 5)) << QPoint(6, 3);
}

void TestBox::topRight()
{
    QFETCH(Box, box);

    QTEST(box.topRight(), "topRight");
}

void TestBox::setTopRight_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<QPoint>("value");
    QTest::addColumn<Box>("expected");

    QTest::addRow("default") << Box() << QPoint(1, -2) << Box(0, -2, 1, 2);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << QPoint(12, 0) << Box(2, 0, 10, 8);
    QTest::addRow("QPoint(2, 3) QPoint(6, 8)") << Box(QPoint(2, 3), QPoint(6, 8)) << QPoint(12, 0) << Box(2, 0, 10, 8);
    QTest::addRow("QPoint(2, 3) QSize(4, 5)") << Box(QPoint(2, 3), QSize(4, 5)) << QPoint(12, 0) << Box(2, 0, 10, 8);
    QTest::addRow("QRect(2, 3, 4, 5)") << Box(QRect(2, 3, 4, 5)) << QPoint(12, 0) << Box(2, 0, 10, 8);
}

void TestBox::setTopRight()
{
    QFETCH(Box, box);
    QFETCH(QPoint, value);

    box.setTopRight(value);
    QTEST(box, "expected");
}

void TestBox::moveTopRight_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<QPoint>("value");
    QTest::addColumn<Box>("expected");

    QTest::addRow("default") << Box() << QPoint(10, 1) << Box(10, 1, 0, 0);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << QPoint(10, 1) << Box(6, 1, 4, 5);
    QTest::addRow("QPoint(2, 3) QPoint(6, 8)") << Box(QPoint(2, 3), QPoint(6, 8)) << QPoint(10, 1) << Box(6, 1, 4, 5);
    QTest::addRow("QPoint(2, 3) QSize(4, 5)") << Box(QPoint(2, 3), QSize(4, 5)) << QPoint(10, 1) << Box(6, 1, 4, 5);
    QTest::addRow("QRect(2, 3, 4, 5)") << Box(QRect(2, 3, 4, 5)) << QPoint(10, 1) << Box(6, 1, 4, 5);
}

void TestBox::moveTopRight()
{
    QFETCH(Box, box);
    QFETCH(QPoint, value);

    box.moveTopRight(value);
    QTEST(box, "expected");
}

void TestBox::bottomRight_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<QPoint>("bottomRight");

    QTest::addRow("default") << Box() << QPoint(0, 0);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << QPoint(6, 8);
    QTest::addRow("QPoint(2, 3) QPoint(6, 8)") << Box(QPoint(2, 3), QPoint(6, 8)) << QPoint(6, 8);
    QTest::addRow("QPoint(2, 3) QSize(4, 5)") << Box(QPoint(2, 3), QSize(4, 5)) << QPoint(6, 8);
    QTest::addRow("QRect(2, 3, 4, 5)") << Box(QRect(2, 3, 4, 5)) << QPoint(6, 8);
}

void TestBox::bottomRight()
{
    QFETCH(Box, box);

    QTEST(box.bottomRight(), "bottomRight");
}

void TestBox::setBottomRight_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<QPoint>("value");
    QTest::addColumn<Box>("expected");

    QTest::addRow("default") << Box() << QPoint(1, 2) << Box(0, 0, 1, 2);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << QPoint(12, 16) << Box(2, 3, 10, 13);
    QTest::addRow("QPoint(2, 3) QPoint(6, 8)") << Box(QPoint(2, 3), QPoint(6, 8)) << QPoint(12, 16) << Box(2, 3, 10, 13);
    QTest::addRow("QPoint(2, 3) QSize(4, 5)") << Box(QPoint(2, 3), QSize(4, 5)) << QPoint(12, 16) << Box(2, 3, 10, 13);
    QTest::addRow("QRect(2, 3, 4, 5)") << Box(QRect(2, 3, 4, 5)) << QPoint(12, 16) << Box(2, 3, 10, 13);
}

void TestBox::setBottomRight()
{
    QFETCH(Box, box);
    QFETCH(QPoint, value);

    box.setBottomRight(value);
    QTEST(box, "expected");
}

void TestBox::moveBottomRight_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<QPoint>("value");
    QTest::addColumn<Box>("expected");

    QTest::addRow("default") << Box() << QPoint(10, 15) << Box(10, 15, 0, 0);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << QPoint(10, 15) << Box(6, 10, 4, 5);
    QTest::addRow("QPoint(2, 3) QPoint(6, 8)") << Box(QPoint(2, 3), QPoint(6, 8)) << QPoint(10, 15) << Box(6, 10, 4, 5);
    QTest::addRow("QPoint(2, 3) QSize(4, 5)") << Box(QPoint(2, 3), QSize(4, 5)) << QPoint(10, 15) << Box(6, 10, 4, 5);
    QTest::addRow("QRect(2, 3, 4, 5)") << Box(QRect(2, 3, 4, 5)) << QPoint(10, 15) << Box(6, 10, 4, 5);
}

void TestBox::moveBottomRight()
{
    QFETCH(Box, box);
    QFETCH(QPoint, value);

    box.moveBottomRight(value);
    QTEST(box, "expected");
}

void TestBox::bottomLeft_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<QPoint>("bottomLeft");

    QTest::addRow("default") << Box() << QPoint(0, 0);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << QPoint(2, 8);
    QTest::addRow("QPoint(2, 3) QPoint(6, 8)") << Box(QPoint(2, 3), QPoint(6, 8)) << QPoint(2, 8);
    QTest::addRow("QPoint(2, 3) QSize(4, 5)") << Box(QPoint(2, 3), QSize(4, 5)) << QPoint(2, 8);
    QTest::addRow("QRect(2, 3, 4, 5)") << Box(QRect(2, 3, 4, 5)) << QPoint(2, 8);
}

void TestBox::bottomLeft()
{
    QFETCH(Box, box);

    QTEST(box.bottomLeft(), "bottomLeft");
}

void TestBox::setBottomLeft_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<QPoint>("value");
    QTest::addColumn<Box>("expected");

    QTest::addRow("default") << Box() << QPoint(-1, 2) << Box(-1, 0, 1, 2);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << QPoint(0, 10) << Box(0, 3, 6, 7);
    QTest::addRow("QPoint(2, 3) QPoint(6, 8)") << Box(QPoint(2, 3), QPoint(6, 8)) << QPoint(0, 10) << Box(0, 3, 6, 7);
    QTest::addRow("QPoint(2, 3) QSize(4, 5)") << Box(QPoint(2, 3), QSize(4, 5)) << QPoint(0, 10) << Box(0, 3, 6, 7);
    QTest::addRow("QRect(2, 3, 4, 5)") << Box(QRect(2, 3, 4, 5)) << QPoint(0, 10) << Box(0, 3, 6, 7);
}

void TestBox::setBottomLeft()
{
    QFETCH(Box, box);
    QFETCH(QPoint, value);

    box.setBottomLeft(value);
    QTEST(box, "expected");
}

void TestBox::moveBottomLeft_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<QPoint>("value");
    QTest::addColumn<Box>("expected");

    QTest::addRow("default") << Box() << QPoint(1, 15) << Box(1, 15, 0, 0);
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << QPoint(1, 15) << Box(1, 10, 4, 5);
    QTest::addRow("QPoint(2, 3) QPoint(6, 8)") << Box(QPoint(2, 3), QPoint(6, 8)) << QPoint(1, 15) << Box(1, 10, 4, 5);
    QTest::addRow("QPoint(2, 3) QSize(4, 5)") << Box(QPoint(2, 3), QSize(4, 5)) << QPoint(1, 15) << Box(1, 10, 4, 5);
    QTest::addRow("QRect(2, 3, 4, 5)") << Box(QRect(2, 3, 4, 5)) << QPoint(1, 15) << Box(1, 10, 4, 5);
}

void TestBox::moveBottomLeft()
{
    QFETCH(Box, box);
    QFETCH(QPoint, value);

    box.moveBottomLeft(value);
    QTEST(box, "expected");
}

void TestBox::transposed_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<Box>("expected");

    QTest::addRow("default") << Box() << Box();
    QTest::addRow("2,3 4x5") << Box(2, 3, 4, 5) << Box(2, 3, 5, 4);
}

void TestBox::transposed()
{
    QFETCH(Box, box);

    QTEST(box.transposed(), "expected");
}

void TestBox::translate_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<QPoint>("offset");
    QTest::addColumn<Box>("expected");

    QTest::addRow("default") << Box() << QPoint(1, 2) << Box(1, 2, 0, 0);
    QTest::addRow("2,3 4x5 by 1,0") << Box(2, 3, 4, 5) << QPoint(1, 0) << Box(3, 3, 4, 5);
    QTest::addRow("2,3 4x5 by 0,1") << Box(2, 3, 4, 5) << QPoint(0, 1) << Box(2, 4, 4, 5);
    QTest::addRow("2,3 4x5 by 1,1") << Box(2, 3, 4, 5) << QPoint(1, 1) << Box(3, 4, 4, 5);
}

void TestBox::translate()
{
    QFETCH(Box, box);
    QFETCH(QPoint, offset);

    {
        Box tmp = box;
        tmp.translate(offset);
        QTEST(tmp, "expected");
    }

    {
        Box tmp = box;
        tmp.translate(offset.x(), offset.y());
        QTEST(tmp, "expected");
    }
}

void TestBox::translated_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<QPoint>("offset");
    QTest::addColumn<Box>("expected");

    QTest::addRow("default") << Box() << QPoint(1, 2) << Box(1, 2, 0, 0);
    QTest::addRow("2,3 4x5 by 1,0") << Box(2, 3, 4, 5) << QPoint(1, 0) << Box(3, 3, 4, 5);
    QTest::addRow("2,3 4x5 by 0,1") << Box(2, 3, 4, 5) << QPoint(0, 1) << Box(2, 4, 4, 5);
    QTest::addRow("2,3 4x5 by 1,1") << Box(2, 3, 4, 5) << QPoint(1, 1) << Box(3, 4, 4, 5);
}

void TestBox::translated()
{
    QFETCH(Box, box);
    QFETCH(QPoint, offset);

    QTEST(box.translated(offset), "expected");
    QTEST(box.translated(offset.x(), offset.y()), "expected");
}

void TestBox::adjust_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<int>("x0");
    QTest::addColumn<int>("y0");
    QTest::addColumn<int>("x1");
    QTest::addColumn<int>("y1");
    QTest::addColumn<Box>("expected");

    QTest::addRow("default - -1,0,0,0") << Box() << -1 << 0 << 0 << 0 << Box(-1, 0, 1, 0);
    QTest::addRow("default - 0,-1,0,0") << Box() << 0 << -1 << 0 << 0 << Box(0, -1, 0, 1);
    QTest::addRow("default - 0,0,1,0") << Box() << 0 << 0 << 1 << 0 << Box(0, 0, 1, 0);
    QTest::addRow("default - 0,0,0,1") << Box() << 0 << 0 << 0 << 1 << Box(0, 0, 0, 1);
    QTest::addRow("default - -1,0,1,0") << Box() << -1 << 0 << 1 << 0 << Box(-1, 0, 2, 0);
    QTest::addRow("default - 0,-1,0,1") << Box() << 0 << -1 << 0 << 1 << Box(0, -1, 0, 2);
    QTest::addRow("default - -1,-1,1,1") << Box() << -1 << -1 << 1 << 1 << Box(-1, -1, 2, 2);

    QTest::addRow("2,3 4x5 - -1,0,1,0") << Box(2, 3, 4, 5) << -1 << 0 << 1 << 0 << Box(1, 3, 6, 5);
    QTest::addRow("2,3 4x5 - 0,-1,0,1") << Box(2, 3, 4, 5) << 0 << -1 << 0 << 1 << Box(2, 2, 4, 7);
}

void TestBox::adjust()
{
    QFETCH(Box, box);
    QFETCH(int, x0);
    QFETCH(int, y0);
    QFETCH(int, x1);
    QFETCH(int, y1);

    box.adjust(x0, y0, x1, y1);
    QTEST(box, "expected");
}

void TestBox::adjusted_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<int>("x0");
    QTest::addColumn<int>("y0");
    QTest::addColumn<int>("x1");
    QTest::addColumn<int>("y1");
    QTest::addColumn<Box>("expected");

    QTest::addRow("default - -1,0,0,0") << Box() << -1 << 0 << 0 << 0 << Box(-1, 0, 1, 0);
    QTest::addRow("default - 0,-1,0,0") << Box() << 0 << -1 << 0 << 0 << Box(0, -1, 0, 1);
    QTest::addRow("default - 0,0,1,0") << Box() << 0 << 0 << 1 << 0 << Box(0, 0, 1, 0);
    QTest::addRow("default - 0,0,0,1") << Box() << 0 << 0 << 0 << 1 << Box(0, 0, 0, 1);
    QTest::addRow("default - -1,0,1,0") << Box() << -1 << 0 << 1 << 0 << Box(-1, 0, 2, 0);
    QTest::addRow("default - 0,-1,0,1") << Box() << 0 << -1 << 0 << 1 << Box(0, -1, 0, 2);
    QTest::addRow("default - -1,-1,1,1") << Box() << -1 << -1 << 1 << 1 << Box(-1, -1, 2, 2);

    QTest::addRow("2,3 4x5 - -1,0,1,0") << Box(2, 3, 4, 5) << -1 << 0 << 1 << 0 << Box(1, 3, 6, 5);
    QTest::addRow("2,3 4x5 - 0,-1,0,1") << Box(2, 3, 4, 5) << 0 << -1 << 0 << 1 << Box(2, 2, 4, 7);
}

void TestBox::adjusted()
{
    QFETCH(Box, box);
    QFETCH(int, x0);
    QFETCH(int, y0);
    QFETCH(int, x1);
    QFETCH(int, y1);

    QTEST(box.adjusted(x0, y0, x1, y1), "expected");
}

void TestBox::marginsAdded_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<QMargins>("margins");
    QTest::addColumn<Box>("expected");

    QTest::addRow("2,3 4x5 - 1,0,0,0") << Box(2, 3, 4, 5) << QMargins(1, 0, 0, 0) << Box(1, 3, 5, 5);
    QTest::addRow("2,3 4x5 - 0,1,0,0") << Box(2, 3, 4, 5) << QMargins(0, 1, 0, 0) << Box(2, 2, 4, 6);
    QTest::addRow("2,3 4x5 - 0,0,1,0") << Box(2, 3, 4, 5) << QMargins(0, 0, 1, 0) << Box(2, 3, 5, 5);
    QTest::addRow("2,3 4x5 - 0,0,0,1") << Box(2, 3, 4, 5) << QMargins(0, 0, 0, 1) << Box(2, 3, 4, 6);
    QTest::addRow("2,3 4x5 - 1,1,1,1") << Box(2, 3, 4, 5) << QMargins(1, 1, 1, 1) << Box(1, 2, 6, 7);
}

void TestBox::marginsAdded()
{
    QFETCH(Box, box);
    QFETCH(QMargins, margins);

    {
        QTEST(box.marginsAdded(margins), "expected");
    }

    {
        Box tmp = box;
        tmp += margins;
        QTEST(tmp, "expected");
    }
}

void TestBox::marginsRemoved_data()
{
    QTest::addColumn<Box>("box");
    QTest::addColumn<QMargins>("margins");
    QTest::addColumn<Box>("expected");

    QTest::addRow("2,3 4x5 - 1,0,0,0") << Box(2, 3, 4, 5) << QMargins(1, 0, 0, 0) << Box(3, 3, 3, 5);
    QTest::addRow("2,3 4x5 - 0,1,0,0") << Box(2, 3, 4, 5) << QMargins(0, 1, 0, 0) << Box(2, 4, 4, 4);
    QTest::addRow("2,3 4x5 - 0,0,1,0") << Box(2, 3, 4, 5) << QMargins(0, 0, 1, 0) << Box(2, 3, 3, 5);
    QTest::addRow("2,3 4x5 - 0,0,0,1") << Box(2, 3, 4, 5) << QMargins(0, 0, 0, 1) << Box(2, 3, 4, 4);
    QTest::addRow("2,3 4x5 - 1,1,1,1") << Box(2, 3, 4, 5) << QMargins(1, 1, 1, 1) << Box(3, 4, 2, 3);
}

void TestBox::marginsRemoved()
{
    QFETCH(Box, box);
    QFETCH(QMargins, margins);

    {
        QTEST(box.marginsRemoved(margins), "expected");
    }

    {
        Box tmp = box;
        tmp -= margins;
        QTEST(tmp, "expected");
    }
}

void TestBox::united_data()
{
    QTest::addColumn<Box>("box1");
    QTest::addColumn<Box>("box2");
    QTest::addColumn<Box>("result");

    QTest::addRow("empty - empty") << Box() << Box() << Box();
    QTest::addRow("2,3 4x5 - empty") << Box(2, 3, 4, 5) << Box() << Box(2, 3, 4, 5);
    QTest::addRow("empty - 2,3 4x5") << Box() << Box(2, 3, 4, 5) << Box(2, 3, 4, 5);
    QTest::addRow("2,3 4x5 - 2,3 4x5") << Box(2, 3, 4, 5) << Box(2, 3, 4, 5) << Box(2, 3, 4, 5);

    QTest::addRow("1,0 1x5 - 5,0 1x5") << Box(1, 0, 1, 5) << Box(5, 0, 1, 5) << Box(1, 0, 5, 5);
    QTest::addRow("0,1 5x1 - 0,5 5x1") << Box(0, 1, 5, 1) << Box(0, 5, 5, 1) << Box(0, 1, 5, 5);

    QTest::addRow("2,3 4x5 - 10,10 0x0") << Box(2, 3, 4, 5) << Box(10, 10, 0, 0) << Box(2, 3, 4, 5);
}

void TestBox::united()
{
    QFETCH(Box, box1);
    QFETCH(Box, box2);

    {
        QTEST(box1.united(box2), "result");
    }

    {
        QTEST(box1 | box2, "result");
    }

    {
        Box tmp = box1;
        tmp |= box2;
        QTEST(tmp, "result");
    }
}

void TestBox::intersects_data()
{
    QTest::addColumn<Box>("box1");
    QTest::addColumn<Box>("box2");
    QTest::addColumn<bool>("intersects");

    QTest::addRow("empty - empty") << Box() << Box() << false;
    QTest::addRow("2,3 4x5 - empty") << Box(2, 3, 4, 5) << Box() << false;
    QTest::addRow("empty - 2,3 4x5") << Box() << Box(2, 3, 4, 5) << false;

    QTest::addRow("0,0 5x5 - 0,-5 5x5") << Box(0, 0, 5, 5) << Box(0, -5, 5, 5) << false;
    QTest::addRow("0,0 5x5 - -5,0 5x5") << Box(0, 0, 5, 5) << Box(-5, 0, 5, 5) << false;
    QTest::addRow("0,0 5x5 - -5,-5 5x5") << Box(0, 0, 5, 5) << Box(-5, -5, 5, 5) << false;
    QTest::addRow("0,0 5x5 - 0,5 5x5") << Box(0, 0, 5, 5) << Box(0, 5, 5, 5) << false;
    QTest::addRow("0,0 5x5 - 5,0 5x5") << Box(0, 0, 5, 5) << Box(5, 0, 5, 5) << false;
    QTest::addRow("0,0 5x5 - 5,5 5x5") << Box(0, 0, 5, 5) << Box(5, 5, 5, 5) << false;

    QTest::addRow("0,0 5x5 - 2,2 5x5") << Box(0, 0, 5, 5) << Box(2, 2, 5, 5) << true;
    QTest::addRow("2,2 5x5 - 0,0 5x5") << Box(2, 2, 5, 5) << Box(0, 0, 5, 5) << true;
}

void TestBox::intersects()
{
    QFETCH(Box, box1);
    QFETCH(Box, box2);

    QTEST(box1.intersects(box2), "intersects");
}

void TestBox::intersected_data()
{
    QTest::addColumn<Box>("box1");
    QTest::addColumn<Box>("box2");
    QTest::addColumn<Box>("result");

    QTest::addRow("empty - empty") << Box() << Box() << Box();
    QTest::addRow("2,3 4x5 - empty") << Box(2, 3, 4, 5) << Box() << Box();
    QTest::addRow("empty - 2,3 4x5") << Box() << Box(2, 3, 4, 5) << Box();

    QTest::addRow("0,0 5x5 - 0,-5 5x5") << Box(0, 0, 5, 5) << Box(0, -5, 5, 5) << Box();
    QTest::addRow("0,0 5x5 - -5,0 5x5") << Box(0, 0, 5, 5) << Box(-5, 0, 5, 5) << Box();
    QTest::addRow("0,0 5x5 - -5,-5 5x5") << Box(0, 0, 5, 5) << Box(-5, -5, 5, 5) << Box();
    QTest::addRow("0,0 5x5 - 0,5 5x5") << Box(0, 0, 5, 5) << Box(0, 5, 5, 5) << Box();
    QTest::addRow("0,0 5x5 - 5,0 5x5") << Box(0, 0, 5, 5) << Box(5, 0, 5, 5) << Box();
    QTest::addRow("0,0 5x5 - 5,5 5x5") << Box(0, 0, 5, 5) << Box(5, 5, 5, 5) << Box();

    QTest::addRow("0,0 5x5 - 2,2 5x5") << Box(0, 0, 5, 5) << Box(2, 2, 5, 5) << Box(2, 2, 3, 3);
    QTest::addRow("2,2 5x5 - 0,0 5x5") << Box(2, 2, 5, 5) << Box(0, 0, 5, 5) << Box(2, 2, 3, 3);
}

void TestBox::intersected()
{
    QFETCH(Box, box1);
    QFETCH(Box, box2);

    {
        QTEST(box1.intersected(box2), "result");
    }

    {
        QTEST(box1 & box2, "result");
    }

    {
        Box tmp = box1;
        tmp &= box2;
        QTEST(tmp, "result");
    }
}

QTEST_MAIN(TestBox)

#include "test_box.moc"
