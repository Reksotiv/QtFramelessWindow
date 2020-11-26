/*
###############################################################################
#                                                                             #
#                           The MIT License                                   #
# Copyright (C) 2020 by Reksotiv (hellfire0h@gmail.com)                       #
#                https://github.com/Reksotiv                                  #
# Sources code: https://github.com/Reksotiv/QtFramelessWindow                 #
#                                                                             #
###############################################################################
*/

#include "framelesswindow.h"
#include "ui_framelesswindow.h"
#include "titlebar.h"
#include "customshadoweffect.h"

#include <QPainter>
#include <QStyleOption>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>

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

FramelessWindow::FramelessWindow(QWidget *parent) : // QWidget::createWindowContainer()
    QWidget(parent),
    ui(new Ui::FramelessWindow)
{
    setWindowFlags(windowFlags() | Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setWindowFlag(Qt::FramelessWindowHint);
#if defined(Q_OS_WIN)
   setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
#endif
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground);

    ui->setupUi(this);

    m_minimizeEbabled = true;
    m_maximizeEnabled = true;
    m_mousePressed = false;
    m_rightDrag = false;
    m_leftDrag = false;
    m_topDrag = false;
    m_bottomDrag = false;
    m_isResizable = true;

    setMouseTracking(true);

    layout()->setMargin(5);
    ui->layoutV_Dragger->activate();
    setWindowShadow();

    connect(this, SIGNAL(windowTitleChanged(const QString &)),
            this, SLOT(setWindowTitle_p(const QString &)));
    connect(this, SIGNAL(windowIconChanged(const QIcon &)),
            this, SLOT(setWindowIcon_p(const QIcon &)));

    QApplication::instance()->installEventFilter(this);
}

FramelessWindow::~FramelessWindow()
{
    delete ui;
}

//! Return a pointer to title bar context menu
QMenu *FramelessWindow::titleBarMenu() const
{
    return ui->widget_TitleBar->menu();
}


void FramelessWindow::setWindowShadow()
{
    deleteGraphicEffect();
    CustomShadowEffect *shadow = new CustomShadowEffect(this);
    shadow->setBlurRadius(10);
    shadow->setOffset(0);
//    //setAutoFillBackground(true);
    setGraphicsEffect(shadow);
}

void FramelessWindow::deleteGraphicEffect()
{
    QGraphicsEffect *cur_shadow = graphicsEffect();
    if (cur_shadow){
        delete cur_shadow;
    }
    setGraphicsEffect(nullptr);
}

void FramelessWindow::widgetFlagsToTitle(QWidget* widget)
{
    Qt::WindowFlags type = (widget->windowFlags() & Qt::WindowType_Mask);
    if (type == Qt::Window){
        qDebug()<<"Window";
        ui->widget_TitleBar->setHelpVisible(false);
    }
    else if (type == Qt::Dialog){
        qDebug()<<"Dialog";
        ui->widget_TitleBar->setMinimizeVisible(false);
        ui->widget_TitleBar->setMaximizeVisible(false);
    }
    else if (type == Qt::Sheet){
        qDebug()<<"Sheet";
        ui->widget_TitleBar->setMinimizeVisible(false);
        ui->widget_TitleBar->setMaximizeVisible(false);
    }
    else if (type == Qt::Drawer){
        qDebug()<<"Drawer";
        ui->widget_TitleBar->setHelpVisible(false);
        ui->widget_TitleBar->setMinimizeVisible(false);
        ui->widget_TitleBar->setMaximizeVisible(false);
    }
    else if (type == Qt::Popup){
        qDebug()<<"Popup";
    }
    else if (type == Qt::Tool){
        qDebug()<<"Tool";
        ui->widget_TitleBar->setHelpVisible(false);
        ui->widget_TitleBar->setMinimizeVisible(false);
        ui->widget_TitleBar->setMaximizeVisible(false);
    }
    else if (type == Qt::ToolTip){
        qDebug()<<"ToolTip";
    }
    else if (type == Qt::SplashScreen){
        qDebug()<<"SplashScreen";
    }
}

void FramelessWindow::setContent(QWidget* widget)
{
    // this should be before addWidget()
    widgetFlagsToTitle(widget);
    // add widget to layout
    ui->layoutV_Content->addWidget(widget);
    // set title and icon
    setWindowTitle(widget->windowTitle());
    setWindowIcon(widget->windowIcon());
    // set modality
    setWindowModality(widget->windowModality());

    // растягивание костылём, если titleBar шире QMessageBox// наверх в Dialog?
    if (qobject_cast<QMessageBox*>(widget)) {
        m_isResizable = false;
        int titleBarWidth = ui->widget_TitleBar->sizeHint().width();
        if (widget->size().width() < (titleBarWidth + layout()->margin()*2)) {
            QGridLayout* lt = (QGridLayout*)widget->layout();
            QSpacerItem* horSpacer = new QSpacerItem(titleBarWidth - lt->margin()*2,
                                                            0, QSizePolicy::Minimum, QSizePolicy::Expanding);
            lt->addItem(horSpacer, lt->rowCount(), 0, 1, lt->columnCount());
        }
    }
    // resize frameless window to widget size
    resize(widget->size());
    // fill blackground
    widget->setAutoFillBackground(true);

    if (qobject_cast<QDialog*>(widget)) {
        // show() should be before move()
        show();
        // centered on parent widget of FramelessWindow
        if (QWidget *tmp_parent = qobject_cast<QWidget*>(parent()); tmp_parent) {
            QPoint glob_pos;
            glob_pos = tmp_parent->mapToGlobal(tmp_parent->pos());

            resize(widget->sizeHint().width(),
                   widget->sizeHint().height() + layout()->margin()*2 + ui->widget_TitleBar->height());

            int x = glob_pos.x() + tmp_parent->geometry().center().x() - widget->geometry().center().x();
            int y = glob_pos.y() + tmp_parent->geometry().center().y() - widget->geometry().center().y() - ui->widget_TitleBar->height();
            move(x, y);
        }
    }
    // set shadow
    setWindowShadow();
}

void FramelessWindow::setWindowTitle_p(const QString &title)
{
    ui->widget_TitleBar->setWindowTitle(title);
}

void FramelessWindow::setWindowIcon_p(const QIcon &icon)
{
    ui->widget_TitleBar->setWindowIcon(icon.pixmap(16, 16));
}

//// set buttons visible/hidden
//void FramelessWindow::SetMinimizeVisible(bool visible)
//{
//    ui->pushButton_Minimize->setVisible(visible);
//    minimize_ebabled = visible;
//}
//void FramelessWindow::SetMaximizeVisible(bool visible)
//{
//    ui->pushButton_Maximize->setVisible(visible);
//    maximize_enabled = visible;
//}
//void FramelessWindow::SetCloseVisible(bool visible)
//{
//    ui->pushButton_Exit->setVisible(visible);
//}
//void FramelessWindow::SetHelpVisible(bool visible)
//{
//    ui->pushButton_Help->setVisible(visible);
//}
//// set
//void FramelessWindow::SetResizable(bool resizable)
//{
//    is_resizable_ = resizable;
//}
////
//void FramelessWindow::SetContextMenuEnable(bool is_enabled)
//{
//    ui->widget_TitleBar->SetContextMenuEnable(is_enabled);
//}


void FramelessWindow::MouseBorderCheck(QMouseEvent *event)
{
    if (isMaximized()) {
      return;
    }

    QPoint mouse_global_pos = event->globalPos();

    if (m_mousePressed){
        borderDragging(mouse_global_pos);   // && cursor != Qt::ArrowCursor ??
    } else {
        borderCursorCheck(mouse_global_pos);
    }
}

void FramelessWindow::borderCursorCheck(const QPoint &mouse_global_pos)
{
//    QPointF position = event->screenPos();  // Определяем позицию курсора на экране
//    qreal x = this->x();                    // координаты окна приложения, ...
//    qreal y = this->y();                    // ... то есть координату левого верхнего угла окна
//    qreal width = this->width();            // А также ширину ...
//    qreal height = this->height();          // ... и высоту окна

//    // Определяем области, в которых может находиться курсор мыши
//    // По ним будет определён статус клика
//    QRectF rectTop(x + 9, y, width - 18, 7);
//    QRectF rectBottom(x + 9, y + height - 7, width - 18, 7);
//    QRectF rectLeft(x, y + 9, 7, height - 18);
//    QRectF rectRight(x + width - 7, y + 9, 7, height - 18);
//    QRectF rectInterface(x + 9, y + 9, width - 18, height - 18);

//    // И в зависимости от области, в которой находится курсор
//    // устанавливаем внешний вид курсора и возвращаем его статус
//    if (rectTop.contains(position)) {
//        setCursor(Qt::SizeVerCursor);
//        return Top;
//    } else if (rectBottom.contains(position)) {
//        setCursor(Qt::SizeVerCursor);
//        return Bottom;
//    } else if (rectLeft.contains(position)) {
//        setCursor(Qt::SizeHorCursor);
//        return Left;
//    } else if (rectRight.contains(position)) {
//        setCursor(Qt::SizeHorCursor);
//        return Right;
//    } else if (rectInterface.contains(position)){
//        setCursor(QCursor());
//        return Move;
//    } else {
//        setCursor(QCursor());
//        return None;
//    }


    if (rightBorderHit(mouse_global_pos, 10) && bottomBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeFDiagCursor);
        m_rightDrag = true;
        m_bottomDrag = true;

        m_leftDrag = false;
        m_topDrag = false;
    }
    else if (leftBorderHit(mouse_global_pos, 10) && bottomBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeBDiagCursor);
        m_leftDrag = true;
        m_bottomDrag = true;

        m_rightDrag = false;
        m_topDrag = false;
    }
    else if (rightBorderHit(mouse_global_pos) && topBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeBDiagCursor);
        m_rightDrag = true;
        m_topDrag = true;

        m_leftDrag = false;
        m_bottomDrag = false;
    }
    else if (leftBorderHit(mouse_global_pos, 10) && topBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeFDiagCursor);
        m_leftDrag = true;
        m_topDrag = true;

        m_rightDrag = false;
        m_bottomDrag = false;
    }
    else if (leftBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeHorCursor);
        m_leftDrag = true;
        m_rightDrag = false;
        m_topDrag = false;
        m_bottomDrag = false;
    }
    else if (rightBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeHorCursor);
        m_rightDrag = true;

        m_leftDrag = false;
        m_topDrag = false;
        m_bottomDrag = false;
    }
    else if (topBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeVerCursor);
        m_topDrag = true;

        m_rightDrag = false;
        m_leftDrag = false;
        m_bottomDrag = false;
    }
    else if (bottomBorderHit(mouse_global_pos)){
        setCursor(Qt::SizeVerCursor);
        m_bottomDrag = true;

        m_rightDrag = false;
        m_leftDrag = false;
        m_topDrag = false;
    }
    else
    {
        setCursor(Qt::ArrowCursor);
        m_rightDrag = false;
        m_leftDrag = false;
        m_topDrag = false;
        m_bottomDrag = false;
    }
}

void FramelessWindow::borderDragging(const QPoint &mouse_global_pos)
{
    int press_to_right_border = m_pressGeometry.width() - m_pressPos.x();     // inside if?
    int press_to_bottom_border = m_pressGeometry.height() - m_pressPos.y();
    // right-bottom
    if (m_rightDrag && m_bottomDrag){
        QRect rect = this->geometry();
        this->setGeometry(rect.x(),
                          rect.y(),
                          mouse_global_pos.x() - rect.x() + press_to_right_border,
                          mouse_global_pos.y() - rect.y() + press_to_bottom_border);
    }
    // left-bottom
    else if (m_leftDrag && m_bottomDrag){
        QRect rect = this->geometry();
        int tmp_width = rect.width() + rect.x() - mouse_global_pos.x() + m_pressPos.x();

        if (tmp_width >= this->minimumWidth()){
            this->setGeometry(mouse_global_pos.x() - m_pressPos.x(),
                              rect.y(),
                              rect.width() + rect.x() - mouse_global_pos.x() + m_pressPos.x(),
                              mouse_global_pos.y() - rect.y() + press_to_bottom_border);
        }
    }
    // right-top
    else if (m_rightDrag && m_topDrag){
        QRect rect = this->geometry();
        int tmp_height = rect.height() + rect.y() - mouse_global_pos.y() + m_pressPos.y();

        if (tmp_height >= this->minimumHeight()){
            this->setGeometry(rect.x(),
                              mouse_global_pos.y() - m_pressPos.y(),
                              mouse_global_pos.x() - rect.x() + press_to_right_border,
                              tmp_height);
        }
    }
    // left-top
    else if (m_leftDrag && m_topDrag){
        QRect rect = this->geometry();
        int tmp_width = rect.width() + rect.x() - mouse_global_pos.x() + m_pressPos.x();
        int tmp_height = rect.height() + rect.y() - mouse_global_pos.y() + m_pressPos.y();

        if (tmp_width >= this->minimumWidth() && tmp_height >= this->minimumHeight()){
            this->setGeometry(mouse_global_pos.x() - m_pressPos.x(),
                              mouse_global_pos.y() - m_pressPos.y(),
                              tmp_width,
                              tmp_height);
        }
    }
    // right
    else if (m_rightDrag){
        QRect rect = this->geometry();
        this->setGeometry(rect.x(),
                          rect.y(),
                          mouse_global_pos.x() - rect.x() + press_to_right_border,
                          rect.height());
    }
    // left
    else if (m_leftDrag){
        QRect rect = this->geometry();
        int tmp_width = rect.width() + rect.x() - mouse_global_pos.x() + m_pressPos.x();

        if (tmp_width >= this->minimumWidth()){
            this->setGeometry(mouse_global_pos.x() - m_pressPos.x(),
                              rect.y(),
                              tmp_width,
                              rect.height());
        }
    }
    // bottom
    else if (m_bottomDrag){
        QRect rect = this->geometry();
        this->setGeometry(rect.x(),
                          rect.y(),
                          rect.width(),
                          mouse_global_pos.y() - rect.y() + press_to_bottom_border);
    }
    // top
    else if (m_topDrag){
        QRect rect = this->geometry();
        int tmp_height = rect.height() + rect.y() - mouse_global_pos.y() + m_pressPos.y();

         if (tmp_height >= this->minimumHeight()){
             this->setGeometry(rect.x(),
                               mouse_global_pos.y() - m_pressPos.y(),
                               rect.width(),
                               tmp_height);
         }
    }
}

bool FramelessWindow::leftBorderHit(const QPoint &pos, int additionalSize) const
{
    QRect rect = this->geometry();
    if (pos.x() >= rect.x() && pos.x() <= rect.x() + m_kDragBorderSize + additionalSize){
        return true;
    }
    return false;
}

bool FramelessWindow::rightBorderHit(const QPoint &pos, int additionalSize) const
{
    QRect rect = this->geometry();
    if (pos.x() <= rect.x() + rect.width() && pos.x() >= rect.x() + rect.width() - m_kDragBorderSize - additionalSize){
        return true;
    }
    return false;
}

bool FramelessWindow::topBorderHit(const QPoint &pos, int additionalSize) const
{
    QRect rect = this->geometry();
    if (pos.y() >= rect.y() && pos.y() <= rect.y() + m_kDragBorderSize + additionalSize){
        return true;
    }
    return false;
}

bool FramelessWindow::bottomBorderHit(const QPoint &pos, int additionalSize) const
{
    QRect rect = this->geometry();
    if (pos.y() <= rect.y() + rect.height() && pos.y() >= rect.y() + rect.height() - m_kDragBorderSize - additionalSize){
        return true;
    }
    return false;
}


void FramelessWindow::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if (event->buttons() & Qt::LeftButton){
        m_mousePressed = true;
        m_pressPos = event->pos();
        m_pressGeometry = this->geometry();
    }
}

void FramelessWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
//    if (event->buttons() & Qt::LeftButton){
        m_mousePressed = false;
//    }
}

void FramelessWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::AltModifier){
        if (event->key() == Qt::Key_Return){
            if (isMaximized()){
                setWindowState(Qt::WindowNoState);
            } else {
                setWindowState(Qt::WindowMaximized);
            }
        } else if (event->key() == Qt::Key_F4){
            close();
        }
    }
}


void FramelessWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::WindowStateChange)
    {
        QWindowStateChangeEvent *state_event = static_cast< QWindowStateChangeEvent* >(event);
        if (state_event->oldState() & Qt::WindowMinimized){
            qDebug() << "Window restored (to normal or maximized state)";
            //ui->layoutV_Dragger->setMargin(5);
            //layout()->setMargin(5);
            //ui->layoutV_Dragger->activate();
            setWindowShadow();
        }
        else if (state_event->oldState() == Qt::WindowMaximized && windowState() == Qt::WindowNoState){
            qDebug() << "Window normal";
            //ui->layoutV_Dragger->setMargin(5);
            layout()->setMargin(5);
            ui->layoutV_Dragger->activate();
            setWindowShadow();
        }
        else if (state_event->oldState() == Qt::WindowNoState && windowState() == Qt::WindowMaximized)
        {
            qDebug() << "Window Maximized";
            //ui->layoutV_Dragger->setMargin(0);
            layout()->setMargin(0);
            ui->layoutV_Dragger->activate();
            deleteGraphicEffect();
        }
        else if (windowState() == Qt::WindowMinimized)
        {
            qDebug() << "Window minimized";
            deleteGraphicEffect();
        }
    }
}

bool FramelessWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (isMaximized()){
        return QWidget::eventFilter(obj, event);
    }

    if (event->type() == QEvent::MouseMove){
        QMouseEvent *mouse = dynamic_cast<QMouseEvent*>(event);
        if (mouse && m_isResizable){
            MouseBorderCheck(mouse);
        }
    }

    return QWidget::eventFilter(obj, event);
}


