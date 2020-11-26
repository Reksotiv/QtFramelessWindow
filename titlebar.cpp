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

#include "titlebar.h"
#include "ui_titlebar.h"
#include <QPainter>
#include <QStyleOption>
#include <QMouseEvent>

#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <QMenu>


TitleBar::TitleBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TitleBar)
{
    ui->setupUi(this);

    m_mousePressed = false;
    m_menuEnabled = true;

    m_parent = parent; // parent change

    this->setContextMenuPolicy(Qt::DefaultContextMenu);

    // create context menu         // жирно, если menu_enabled отключить, то меню всё равно создано
    m_menu = new QMenu(this);
    // restore action
    m_actRestore = new QAction(tr("Restore"), this);    // чекнуть утечку // this menu?
    m_actRestore->setShortcut(QKeySequence(Qt::ALT + Qt::Key_Enter));
    connect(m_actRestore, SIGNAL(triggered()), this, SLOT(on_pushButton_Maximize_clicked()));
    // maximize action
    m_actMaximize = new QAction(tr("Maximize"), this);
    m_actMaximize->setShortcut(QKeySequence(Qt::ALT + Qt::Key_Enter));
    connect(m_actMaximize, SIGNAL(triggered()), this, SLOT(on_pushButton_Maximize_clicked()));
    if (m_parent->isMaximized()){
        m_menu->addAction(m_actRestore);
    } else {
        m_menu->addAction(m_actMaximize);
    }
    // mininize action
    m_actMinimize = new QAction(tr("Minimize"), this); //= menu.addAction(tr("Minimize"));
    connect(m_actMinimize, SIGNAL(triggered()), this, SLOT(on_pushButton_Minimize_clicked()));
    m_menu->addAction(m_actMinimize);
    // close action
    m_actClose = new QAction(tr("Close"), this);//, &MainWindow::about);
    m_actClose->setShortcut(QKeySequence(Qt::ALT + Qt::Key_F4));
    connect(m_actClose, SIGNAL(triggered()), this, SLOT(on_pushButton_Exit_clicked()));
    m_menu->addAction(m_actClose);

    // paint actions icons
    paintingActIcon();

    connect(this, SIGNAL(windowTitleChanged(const QString &)),
            this, SLOT(setWindowTitle_p(const QString &)));
    connect(this, SIGNAL(windowIconChanged(const QIcon &)),
            this, SLOT(setWindowIcon_p(const QIcon &)));
    updateMenu();
    QApplication::instance()->installEventFilter(this); //tmp
}

TitleBar::~TitleBar()
{
    delete ui;
}

void TitleBar::updateMenu()
{

}

bool TitleBar::eventFilter(QObject *obj, QEvent *event) //tmp
{
//    qDebug()<<event;
    return QWidget::eventFilter(obj, event);
}


void TitleBar::setWindowTitle_p(const QString &title)
{
    ui->label_WindowTitle->setText(title);
}

void TitleBar::setWindowIcon_p(const QIcon &icon)
{
    ui->label_WindowIcon->setPixmap(icon.pixmap(16, 16));
}


// help button
void TitleBar::on_pushButton_Help_clicked()
{

}
// minimize button
void TitleBar::on_pushButton_Minimize_clicked()
{
        m_parent->setWindowState(Qt::WindowMinimized);
}
// maximize button
void TitleBar::on_pushButton_Maximize_clicked()
{
//    if (maximize_enabled){
        if (m_parent->isMaximized()){
            m_parent->setWindowState(Qt::WindowNoState);
        } else {
            m_parent->setWindowState(Qt::WindowMaximized);
        }
//    }
}
// close button
void TitleBar::on_pushButton_Exit_clicked()
{
    m_parent->close();
}

// visible
void TitleBar::setHelpVisible(bool visible)
{
    ui->pushButton_Help->setVisible(visible);
}
void TitleBar::setMinimizeVisible(bool visible)
{
    ui->pushButton_Minimize->setVisible(visible);
    if (visible){
        m_menu->addAction(m_actMinimize);
    } else {
        m_menu->removeAction(m_actMinimize);
    }
}
void TitleBar::setMaximizeVisible(bool visible)
{
    ui->pushButton_Maximize->setVisible(visible);
    if (visible){
        if (m_parent->isMaximized()){
            m_menu->addAction(m_actRestore);
        } else {
            m_menu->addAction(m_actMaximize);
        }
    } else {
        m_menu->removeAction(m_actMaximize);
        m_menu->removeAction(m_actRestore);
    }
}
void TitleBar::setCloseVisible(bool visible)
{
    ui->pushButton_Exit->setVisible(visible);
    if (visible){
        m_menu->addAction(m_actClose);
    } else {
        m_menu->removeAction(m_actClose);
    }
}


// context menu (right click)
void TitleBar::contextMenuEvent(QContextMenuEvent* e)
{
    if (m_menuEnabled){
        // insert action before minimize(on top)????
        if (ui->pushButton_Maximize->isVisible()){
            if (m_parent->isMaximized()){
                m_menu->insertAction(m_actMinimize, m_actRestore);
                m_menu->removeAction(m_actMaximize);
            } else {
                m_menu->insertAction(m_actMinimize, m_actMaximize);
                m_menu->removeAction(m_actRestore);
            }
        }
        m_menu->exec(e->globalPos());// QString name, auto widget,
    }
}

void TitleBar::setContextMenuEnable(bool is_enabled)
{
    m_menuEnabled = is_enabled;
}

QMenu *TitleBar::menu() const
{
    return m_menu;
}

// красит иконку в заданный цвет (в цвет текста для контекст меню)
//! QPixmap gray-scale image (an alpha map) to colored QIcon
QIcon TitleBar::pixmapToIcon(QPixmap pixmap, const QColor &color)
{
    // initialize painter to draw on a pixmap and set composition mode
    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    // set color
    painter.setBrush(color);
    painter.setPen(color);
    // paint rect
    painter.drawRect(pixmap.rect());
    // Here is our new colored icon!
    return QIcon(pixmap);
}


void TitleBar::paintingActIcon()
{
    //QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(32, 32)));
    QColor color = QColor(palette().color(QPalette::WindowText));
    m_actRestore->setIcon(pixmapToIcon(QPixmap(":/Images/restore.png"), color));
    m_actMaximize->setIcon(pixmapToIcon(QPixmap(":/Images/maximize.png"), color));
    m_actMinimize->setIcon(pixmapToIcon(QPixmap(":/Images/minimize.png"), color));
    m_actClose->setIcon(pixmapToIcon(QPixmap(":/Images/exit.png"), color));
}


void TitleBar::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::StyleChange){
        if (m_tmpColor != QColor(palette().color(QPalette::WindowText))){
            paintingActIcon();
            m_tmpColor = QColor(palette().color(QPalette::WindowText));
        }
    }
    QWidget::changeEvent(event);
}


void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if (event->buttons() & Qt::LeftButton){
        on_pushButton_Maximize_clicked();
    }
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if (event->buttons() & Qt::LeftButton){
        m_pressPos = event->pos();
        m_mousePressed = true;
    }

    QWidget::mousePressEvent(event);
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    //if (event->buttons() & Qt::LeftButton){
        m_mousePressed = false;
    //}

    QWidget::mouseReleaseEvent(event);
}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if (m_mousePressed)
    {
        QCursor cur = cursor();// тупо?
        if (cur == Qt::SizeFDiagCursor || cur == Qt::SizeBDiagCursor ||
                cur == Qt::SizeHorCursor || cur == Qt::SizeVerCursor){
            return;
        }
        else if (m_parent->isMaximized()){
            tryMoveWidget(event);
        } else {
            int margin = m_parent->layout()->margin();       // вынести отдельно для оптимизации?
            m_parent->move(event->globalPos() - m_pressPos - QPoint(margin, margin));
        }
    }
    QWidget::mouseMoveEvent(event);
}

void TitleBar::tryMoveWidget(QMouseEvent *event)
{
    // немного неправильно вычисляется Y
    if(event->globalPos().manhattanLength() - m_pressPos.manhattanLength() > 5 ||
       event->globalPos().manhattanLength() - m_pressPos.manhattanLength() < -5)
    {
        QRect rect = m_parent->normalGeometry();
        int dest_x = m_pressPos.x() * rect.width() / m_parent->geometry().width();
        int dest_y = m_pressPos.y();
        rect.moveTopLeft(event->globalPos() - QPoint(dest_x, dest_y));

        m_parent->showNormal();
        //m_parent->setGeometry(rect);

        m_pressPos = QPoint(dest_x, dest_y);
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
