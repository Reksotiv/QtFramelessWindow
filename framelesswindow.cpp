#include "framelesswindow.h"
#include "ui_framelesswindow.h"
#include "titlebar.h"

#include <QPainter>
#include <QStyleOption>
#include <QMouseEvent>

#include <QDebug>

FramelessWindow::FramelessWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FramelessWindow)
{
    setWindowFlags(Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_NoSystemBackground, true);
    //setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);
    ui->setupUi(this);


    QApplication::instance()->installEventFilter(this);
}

FramelessWindow::~FramelessWindow()
{
    delete ui;
}


void FramelessWindow::on_pushButton_Minimize_clicked()
{
    setWindowState(Qt::WindowMinimized);
}

void FramelessWindow::on_pushButton_Maximize_clicked()
{
    if (this->isMaximized()){
        setWindowState(Qt::WindowNoState);
    } else {
        setWindowState(Qt::WindowMaximized);
    }
}

void FramelessWindow::on_pushButton_Exit_clicked()
{
    close();
}


void FramelessWindow::SetContent(QWidget* widget)
{
    ui->layoutV_Content->addWidget(widget);
}

void FramelessWindow::MouseBorderCheck(QMouseEvent *event)
{
    if (isMaximized()) {
      return;
    }

    QPoint mouse_global_pos = event->globalPos();
//    event->globalPos()
//    this->geometry().x()
    //if (event->globalPos().x() <= this->geometry().x()){
    if (RightBorderHit(mouse_global_pos) && BottomBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeFDiagCursor);
    }
    else if (LeftBorderHit(mouse_global_pos) && BottomBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeBDiagCursor);
    }
    else if (RightBorderHit(mouse_global_pos) && TopBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeBDiagCursor);
    }
    else if (LeftBorderHit(mouse_global_pos) && TopBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeFDiagCursor);
    }
    else if (LeftBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeHorCursor);
    }
    else if (RightBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeHorCursor);
    }
    else if (TopBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeVerCursor);
    }
    else if (BottomBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeVerCursor);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
    }
}

bool FramelessWindow::LeftBorderHit(const QPoint &pos)
{
    QRect rect = this->geometry();
    if (pos.x() >= rect.x() && pos.x() <= rect.x() + 5){
        return true;
    }
    return false;
}

bool FramelessWindow::RightBorderHit(const QPoint &pos)
{
    QRect rect = this->geometry();
    if (pos.x() <= rect.x() + rect.width() && pos.x() >= rect.x() + rect.width() - 5){
        return true;
    }
    return false;
}

bool FramelessWindow::TopBorderHit(const QPoint &pos)
{
    QRect rect = this->geometry();
    if (pos.y() >= rect.y() && pos.y() <= rect.y() + 5){
        return true;
    }
    return false;
}

bool FramelessWindow::BottomBorderHit(const QPoint &pos)
{
    QRect rect = this->geometry();
    if (pos.y() <= rect.y() + rect.height() && pos.y() >= rect.y() + rect.height() - 5){
        return true;
    }
    return false;
}


bool FramelessWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (isMaximized()){
        return QWidget::eventFilter(obj, event);
    }

    if (event->type() == QEvent::MouseMove){
        QMouseEvent *mouse = dynamic_cast<QMouseEvent*>(event);
        if (mouse){
            MouseBorderCheck(mouse);
        }
    }

    return QWidget::eventFilter(obj, event);
}
