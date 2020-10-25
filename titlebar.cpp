#include "titlebar.h"
#include <QPainter>
#include <QStyleOption>
#include <QMouseEvent>

#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>

TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{
    mouse_pressed = false;
    //is_maximized = emit isWindowMaximized();

    // бля ну это тупо. хз какой способ использовать
    if (parent->parent()->objectName() == "FramelessWindow"){
        frameless_window = qobject_cast<FramelessWindow*>(parent->parent());
    } else {
        qDebug()<<"cant find object FramelessWindow in titlebar.cpp"<<"\nTitle bar disabled";
    }
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    //emit doubleClicked();
    if (frameless_window->isMaximized()){
        frameless_window->setWindowState(Qt::WindowNoState);
    } else {
        frameless_window->setWindowState(Qt::WindowMaximized);
    }
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    press_pos = event->pos();
    mouse_pressed = true;

    QWidget::mousePressEvent(event); // ?
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    mouse_pressed = false;

    QWidget::mouseReleaseEvent(event); // ?
}
#include <QElapsedTimer>
void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if (mouse_pressed)
    {
        QCursor cur = cursor(); // faster
        if (cur == Qt::SizeFDiagCursor || cur == Qt::SizeBDiagCursor || // тупо?
                cur == Qt::SizeHorCursor || cur == Qt::SizeVerCursor){
            return;
        }
        else if (frameless_window->isMaximized()){
            TryMoveWidget(event);
        } else {
            QPoint diff = event->pos() - press_pos;
            window()->move(window()->pos() + diff);
        }
    }
    ////QWidget::mouseMoveEvent(event);
}

void TitleBar::TryMoveWidget(QMouseEvent *event)
{
//    QPoint distance = event->globalPos().manhattanLength();
//    int length = distance.manhattanLength();

    if(event->globalPos().manhattanLength() - press_pos.manhattanLength() > 5 ||
       event->globalPos().manhattanLength() - press_pos.manhattanLength() < -5)
    {
        QRect rect = frameless_window->normalGeometry();
        int dest_x = press_pos.x() * rect.width() / frameless_window->geometry().width();
        int dest_y = press_pos.y();
        rect.moveTopLeft(event->globalPos() - QPoint(dest_x, dest_y));

        frameless_window->showNormal();
        frameless_window->setGeometry(rect);

        press_pos = QPoint(dest_x, dest_y); // press_pos
    }
}



void TitleBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
