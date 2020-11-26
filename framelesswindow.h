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

#ifndef FRAMELESSWINDOW_H
#define FRAMELESSWINDOW_H

#include <QWidget>
#include <QMenu>

namespace Ui {
class FramelessWindow;
}

class FramelessWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FramelessWindow(QWidget *parent = nullptr);
    ~FramelessWindow();

    void setContent(QWidget *);
    //void setWindowTitle_FL(const QString &title);   // virtual?
    //void setWindowIcon_FL(const QIcon &icon);   // virtual?

//    void SetHelpVisible(bool visible);
//    void SetMinimizeVisible(bool visible);
//    void SetMaximizeVisible(bool visible);
//    void SetCloseVisible(bool visible);

//    void SetResizable(bool resizable);
//    void SetContextMenuEnable(bool is_enabled);

    QMenu *titleBarMenu() const;

private slots:
    void setWindowTitle_p(const QString &title);
    void setWindowIcon_p(const QIcon &icon);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void changeEvent(QEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

    virtual void MouseBorderCheck(QMouseEvent *event);

    virtual void widgetFlagsToTitle(QWidget *widget);

private:
    Ui::FramelessWindow *ui;

    bool leftBorderHit(const QPoint &pos, int additionalSize = 0) const;
    bool rightBorderHit(const QPoint &pos, int additionalSize = 0) const;
    bool topBorderHit(const QPoint &pos, int additionalSize = 0) const;
    bool bottomBorderHit(const QPoint &pos, int additionalSize = 0) const;
    const qint8 m_kDragBorderSize = 5;

    void borderCursorCheck(const QPoint &pos);
    void borderDragging(const QPoint &pos);

    void setWindowShadow();
    void deleteGraphicEffect();

    bool m_minimizeEbabled;
    bool m_maximizeEnabled;

    QPoint m_pressPos;
    QRect m_pressGeometry;
    bool m_mousePressed;
    bool m_leftDrag;
    bool m_rightDrag;
    bool m_topDrag;
    bool m_bottomDrag;
    bool m_isResizable;
};

#endif // FRAMELESSWINDOW_H
