#ifndef WINDOWDRAGGER_H
#define WINDOWDRAGGER_H

#include <QWidget>
#include <QIcon>
#include "framelesswindow.h"

class TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent = nullptr);

signals:
    void doubleClicked();   // rename
//    void customContextMenuRequested(const QPoint &pos);
//    bool isWindowMaximized();

//private slots:
//    void ShowContextMenu(const QPoint &pos);

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent* event) override;
    virtual void changeEvent(QEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

    void TryMoveWidget(QMouseEvent* event);

private:
    bool mouse_pressed;
    bool is_maximized;
    QPoint press_pos;
    QPoint drag_pos;

    QIcon PixmapToIcon(QPixmap pixmap, const QColor &color);
    void PaintingIcon();
    QIcon ico_minimize;
    QIcon ico_maximize;
    QIcon ico_restore;
    QIcon ico_close;

    FramelessWindow *frameless_window;
};

#endif // WINDOWDRAGGER_H
