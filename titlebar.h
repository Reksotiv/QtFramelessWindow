#ifndef WINDOWDRAGGER_H
#define WINDOWDRAGGER_H

#include <QWidget>
#include "framelesswindow.h"

class TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent = nullptr);

signals:
    void doubleClicked();
//    bool isWindowMaximized();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    void TryMoveWidget(QMouseEvent* event);

private:
    bool mouse_pressed;
    bool is_maximized;
    QPoint press_pos;
    QPoint drag_pos;

    FramelessWindow *frameless_window;
};

#endif // WINDOWDRAGGER_H
