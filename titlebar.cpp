#include "titlebar.h"
#include <QPainter>
#include <QStyleOption>
#include <QMouseEvent>

#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <QMenu>

TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{
    mouse_pressed = false;

    // бля ну это тупо. хз какой способ использовать
    if (parent->parent()->objectName() == "FramelessWindow"){
        frameless_window = qobject_cast<FramelessWindow*>(parent->parent());
    } else {
        qDebug()<<"cant find object FramelessWindow in titlebar.cpp"<<"\nTitle bar disabled";
    }
    this->setContextMenuPolicy(Qt::DefaultContextMenu);
    PaintingIcon();
}

void TitleBar::contextMenuEvent(QContextMenuEvent* e)
{
    QMenu menu(this);
    // restore or maximize button
    QAction* rest_max;
    if (frameless_window->isMaximized()){
        rest_max = new QAction(tr("Restore"), this);
        rest_max->setIcon(ico_restore);
        rest_max->setShortcut(QKeySequence(Qt::ALT + Qt::Key_Enter));
        connect(rest_max, SIGNAL(triggered()), frameless_window, SLOT(on_pushButton_Maximize_clicked()));
        menu.addAction(rest_max);
    } else {
        rest_max = new QAction(tr("Maximize"), this);
        rest_max->setIcon(ico_maximize);
        rest_max->setShortcut(QKeySequence(Qt::ALT + Qt::Key_Enter));
        connect(rest_max, SIGNAL(triggered()), frameless_window, SLOT(on_pushButton_Maximize_clicked()));
        menu.addAction(rest_max);
    }
    // mininize button
    QAction *minim = new QAction(tr("Minimize"), this); //= menu.addAction(tr("Minimize"));
    minim->setIcon(ico_minimize);
    connect(minim, SIGNAL(triggered()), frameless_window, SLOT(on_pushButton_Minimize_clicked()));
    menu.addAction(minim);
    // close button
    QAction *close = new QAction(tr("Close"), this);//, &MainWindow::about);
    close->setIcon(ico_close);
    close->setShortcut(QKeySequence(Qt::ALT + Qt::Key_F4));
    connect(close, SIGNAL(triggered()), frameless_window, SLOT(on_pushButton_Exit_clicked()));
    menu.addAction(close);

    menu.exec(e->globalPos());
}

//! QPixmap gray-scale image (an alpha map) to colored QIcon
QIcon TitleBar::PixmapToIcon(QPixmap pixmap, const QColor &color)
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

void TitleBar::PaintingIcon()
{
    QColor color = QColor(palette().color(QPalette::WindowText));
    ico_minimize = PixmapToIcon(QPixmap(":/Images/minimize.png"), color);
    ico_maximize = PixmapToIcon(QPixmap(":/Images/maximize.png"), color);
    ico_restore = PixmapToIcon(QPixmap(":/Images/restore.png"), color);
    ico_close = PixmapToIcon(QPixmap(":/Images/exit.png"), color);
}


void TitleBar::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::AltModifier){
        if (event->key() == Qt::Key_Return){
            emit doubleClicked();
        }
    }
}

void TitleBar::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::StyleChange){
        static QColor color;
        if (color != QColor(palette().color(QPalette::WindowText))){
            PaintingIcon();
            color = QColor(palette().color(QPalette::WindowText));
        }
    }
    QWidget::changeEvent(event);
}


void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if (event->buttons() & Qt::LeftButton){
        emit doubleClicked();
    }
//    if (frameless_window->isMaximized()){
//        frameless_window->setWindowState(Qt::WindowNoState);
//    } else {
//        frameless_window->setWindowState(Qt::WindowMaximized);
//    }
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if (event->buttons() & Qt::LeftButton){
        press_pos = event->pos();
        mouse_pressed = true;
//    } else if (event->buttons() & Qt::RightButton){
//        emit customContextMenuRequested(event->pos());
    }

    QWidget::mousePressEvent(event); // ?
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    //if (event->buttons() & Qt::LeftButton){
        mouse_pressed = false;
    //}

    QWidget::mouseReleaseEvent(event); // ?
}

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

        emit doubleClicked();   // пиздец
        //frameless_window->showNormal();
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
