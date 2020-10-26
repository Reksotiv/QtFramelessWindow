#include "framelesswindow.h"
#include "ui_framelesswindow.h"
#include "titlebar.h"

#include <QPainter>
#include <QStyleOption>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>

#include <QDebug>

//#include <windows.h>
//#include <WinUser.h>
//#include <windowsx.h>
//#include <dwmapi.h>
//#include <objidl.h>
//#include <gdiplus.h>
//#include <GdiPlusColor.h>
//#pragma comment (lib,"Dwmapi.lib")
//const MARGINS margins = {1,1,1,1};
////HWND hwnd = (HWND)QWidget::winId();
//DwmExtendFrameIntoClientArea(HWND(this->winId()), &margins);

FramelessWindow::FramelessWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FramelessWindow)
{
    setWindowFlags(windowFlags() | Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
#if defined(Q_OS_WIN)
  setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
#endif
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground);

    ui->setupUi(this);

//    press_to_right_border = 0;
//    press_to_bottom_border = 0;
    mouse_pressed = false;
    right_drag = false;
    left_drag = false;
    top_drag = false;
    bottom_drag = false;

    setMouseTracking(true);

    SetWindowShadow();

    QApplication::instance()->installEventFilter(this);
}

FramelessWindow::~FramelessWindow()
{
    delete ui;
}

void FramelessWindow::SetWindowShadow()
{
    DeleteGraphicEffect();
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(10);
    //shadow->setColor(palette().color(QPalette::Dark));
    shadow->setOffset(0);
    ui->widget_WindowFrame->setGraphicsEffect(shadow);
}

void FramelessWindow::DeleteGraphicEffect()
{
    QGraphicsEffect *cur_shadow = ui->widget_WindowFrame->graphicsEffect();
    if (cur_shadow){
        delete cur_shadow;
    }
    ui->widget_WindowFrame->setGraphicsEffect(nullptr);
}


void FramelessWindow::on_pushButton_Minimize_clicked()
{
    DeleteGraphicEffect();
    setWindowState(Qt::WindowMinimized);
}

void FramelessWindow::on_pushButton_Maximize_clicked()
{
    if (this->isMaximized()){
        layout()->setMargin(5);
        setWindowState(Qt::WindowNoState);
        SetWindowShadow();
    } else {
        layout()->setMargin(0);
        DeleteGraphicEffect();
        setWindowState(Qt::WindowMaximized);
    }
}

void FramelessWindow::on_widget_TitleBar_doubleClicked()
{
    on_pushButton_Maximize_clicked();
}

void FramelessWindow::on_pushButton_Exit_clicked()
{
    close();
}


void FramelessWindow::SetContent(QWidget* widget)
{
    ui->layoutV_Content->addWidget(widget);
}

void FramelessWindow::SetWindowTitle(const QString &title)
{
    ui->label_WindowTitle->setText(title);
}

void FramelessWindow::SetWindowIcon(const QIcon &icon)
{
    ui->label_WindowIcon->setPixmap(icon.pixmap(16, 16));
}


void FramelessWindow::MouseBorderCheck(QMouseEvent *event)
{
    if (isMaximized()) {
      return;
    }

    QPoint mouse_global_pos = event->globalPos();

    if (mouse_pressed){
        BorderDragging(mouse_global_pos);   // && cursor != Qt::ArrowCursor ??
    } else {
        BorderCursorCheck(mouse_global_pos);
    }
}

void FramelessWindow::BorderCursorCheck(const QPoint &mouse_global_pos)
{
    if (RightBorderHit(mouse_global_pos, 10) && BottomBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeFDiagCursor);
        right_drag = true;
        bottom_drag = true;

        left_drag = false;
        top_drag = false;
    }
    else if (LeftBorderHit(mouse_global_pos, 10) && BottomBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeBDiagCursor);
        left_drag = true;
        bottom_drag = true;

        right_drag = false;
        top_drag = false;
    }
    else if (RightBorderHit(mouse_global_pos) && TopBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeBDiagCursor);
        right_drag = true;
        top_drag = true;

        left_drag = false;
        bottom_drag = false;
    }
    else if (LeftBorderHit(mouse_global_pos, 10) && TopBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeFDiagCursor);
        left_drag = true;
        top_drag = true;

        right_drag = false;
        bottom_drag = false;
    }
    else if (LeftBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeHorCursor);
        left_drag = true;
        right_drag = false;
        top_drag = false;
        bottom_drag = false;
    }
    else if (RightBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeHorCursor);
        right_drag = true;

        left_drag = false;
        top_drag = false;
        bottom_drag = false;
    }
    else if (TopBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeVerCursor);
        top_drag = true;

        right_drag = false;
        left_drag = false;
        bottom_drag = false;
    }
    else if (BottomBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeVerCursor);
        bottom_drag = true;

        right_drag = false;
        left_drag = false;
        top_drag = false;
    }
    else
    {
        setCursor(Qt::ArrowCursor);
        right_drag = false;
        left_drag = false;
        top_drag = false;
        bottom_drag = false;
    }
}

void FramelessWindow::BorderDragging(const QPoint &mouse_global_pos)
{
    int press_to_right_border = press_geometry.width() - press_pos.x();     // inside if?
    int press_to_bottom_border = press_geometry.height() - press_pos.y();
    // right-bottom
    if (right_drag && bottom_drag){
        QRect rect = this->geometry();
        this->setGeometry(rect.x(),
                          rect.y(),
                          mouse_global_pos.x() - rect.x() + press_to_right_border,
                          mouse_global_pos.y() - rect.y() + press_to_bottom_border);
    }
    // left-bottom
    else if (left_drag && bottom_drag){
        QRect rect = this->geometry();
        int tmp_width = rect.width() + rect.x() - mouse_global_pos.x() + press_pos.x();

        if (tmp_width >= this->minimumWidth()){
            this->setGeometry(mouse_global_pos.x() - press_pos.x(),
                              rect.y(),
                              rect.width() + rect.x() - mouse_global_pos.x() + press_pos.x(),
                              mouse_global_pos.y() - rect.y() + press_to_bottom_border);
        }
    }
    // right-top
    else if (right_drag && top_drag){
        QRect rect = this->geometry();
        int tmp_height = rect.height() + rect.y() - mouse_global_pos.y() + press_pos.y();

        if (tmp_height >= this->minimumHeight()){
            this->setGeometry(rect.x(),
                              mouse_global_pos.y() - press_pos.y(),
                              mouse_global_pos.x() - rect.x() + press_to_right_border,
                              tmp_height);
        }
    }
    // left-top
    else if (left_drag && top_drag){
        QRect rect = this->geometry();
        int tmp_width = rect.width() + rect.x() - mouse_global_pos.x() + press_pos.x();
        int tmp_height = rect.height() + rect.y() - mouse_global_pos.y() + press_pos.y();

        if (tmp_width >= this->minimumWidth() && tmp_height >= this->minimumHeight()){
            this->setGeometry(mouse_global_pos.x() - press_pos.x(),
                              mouse_global_pos.y() - press_pos.y(),
                              tmp_width,
                              tmp_height);
        }
    }
    // right
    else if (right_drag){
        QRect rect = this->geometry();
        this->setGeometry(rect.x(),
                          rect.y(),
                          mouse_global_pos.x() - rect.x() + press_to_right_border,
                          rect.height());
    }
    // left
    else if (left_drag){
        QRect rect = this->geometry();
        int tmp_width = rect.width() + rect.x() - mouse_global_pos.x() + press_pos.x();

        if (tmp_width >= this->minimumWidth()){
            this->setGeometry(mouse_global_pos.x() - press_pos.x(),
                              rect.y(),
                              tmp_width,
                              rect.height());
        }
    }
    // bottom
    else if (bottom_drag){
        QRect rect = this->geometry();
        this->setGeometry(rect.x(),
                          rect.y(),
                          rect.width(),
                          mouse_global_pos.y() - rect.y() + press_to_bottom_border);
    }
    // top
    else if (top_drag){
        QRect rect = this->geometry();
        int tmp_height = rect.height() + rect.y() - mouse_global_pos.y() + press_pos.y();

         if (tmp_height >= this->minimumHeight()){
             this->setGeometry(rect.x(),
                               mouse_global_pos.y() - press_pos.y(),
                               rect.width(),
                               tmp_height);
         }
    }
}

bool FramelessWindow::LeftBorderHit(const QPoint &pos, int additional_size) const
{
    QRect rect = this->geometry();
    if (pos.x() >= rect.x() && pos.x() <= rect.x() + DRAG_BORDER_SIZE + additional_size){
        return true;
    }
    return false;
}

bool FramelessWindow::RightBorderHit(const QPoint &pos, int additional_size) const
{
    QRect rect = this->geometry();
    if (pos.x() <= rect.x() + rect.width() && pos.x() >= rect.x() + rect.width() - DRAG_BORDER_SIZE - additional_size){
        return true;
    }
    return false;
}

bool FramelessWindow::TopBorderHit(const QPoint &pos, int additional_size) const
{
    QRect rect = this->geometry();
    if (pos.y() >= rect.y() && pos.y() <= rect.y() + DRAG_BORDER_SIZE + additional_size){
        return true;
    }
    return false;
}

bool FramelessWindow::BottomBorderHit(const QPoint &pos, int additional_size) const
{
    QRect rect = this->geometry();
    if (pos.y() <= rect.y() + rect.height() && pos.y() >= rect.y() + rect.height() - DRAG_BORDER_SIZE - additional_size){
        return true;
    }
    return false;
}


void FramelessWindow::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    mouse_pressed = true;
    press_pos = event->pos();
    press_geometry = this->geometry();


//    press_to_right_border = press_geometry.width() - press_pos.x();
//    press_to_bottom_border = press_geometry.height() - press_pos.y();

}

void FramelessWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    mouse_pressed = false;
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
