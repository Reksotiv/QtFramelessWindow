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

#ifndef WINDOWDRAGGER_H
#define WINDOWDRAGGER_H

#include <QWidget>
#include <QIcon>
#include <QMenu>

//#include "framelesswindow.h"
class FramelessWindow;

namespace Ui {
class TitleBar;
}

class TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent);
    ~TitleBar();

    void setContextMenuEnable(bool is_enabled);
    QMenu *menu() const;

signals:
    void doubleClicked();   // rename
//    void customContextMenuRequested(const QPoint &pos);
//    bool isWindowMaximized();
    //WindowStates windowState;

//private slots:
//    void ShowContextMenu(const QPoint &pos);
public slots:
    void setHelpVisible(bool visible);
    void setMinimizeVisible(bool visible);
    void setMaximizeVisible(bool visible);
    void setCloseVisible(bool visible);


private slots:
    void on_pushButton_Help_clicked();
    void on_pushButton_Minimize_clicked();
    void on_pushButton_Maximize_clicked();
    void on_pushButton_Exit_clicked();

    void setWindowTitle_p(const QString &title);
    void setWindowIcon_p(const QIcon &icon);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;
    void changeEvent(QEvent *event) override;

    void tryMoveWidget(QMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::TitleBar *ui;
    void updateMenu();

    bool m_mousePressed;
    bool m_isMaximized;
    QPoint m_pressPos;
    QPoint m_dragPos;

    QIcon pixmapToIcon(QPixmap pixmap, const QColor &color);
    void paintingActIcon();

    QWidget *m_parent;

    QList <QAction*> m_actionList;  // ?
    QColor m_tmpColor;
    QMenu *m_menu;
    bool m_menuEnabled;
    QAction *m_actRestore;
    QAction *m_actMaximize;
    QAction *m_actMinimize;
    QAction *m_actClose;

};

#endif // WINDOWDRAGGER_H
