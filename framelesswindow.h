#ifndef FRAMELESSWINDOW_H
#define FRAMELESSWINDOW_H

#include <QWidget>

namespace Ui {
class FramelessWindow;
}

class FramelessWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FramelessWindow(QWidget *parent = nullptr);
    ~FramelessWindow();

    void SetContent(QWidget*);

private slots:
    void on_pushButton_Minimize_clicked();
    void on_pushButton_Maximize_clicked();
    void on_pushButton_Exit_clicked();

//    void on_widget_TitleBar_doubleClicked();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    virtual bool eventFilter(QObject *obj, QEvent *event);

    virtual void MouseBorderCheck(QMouseEvent *event);


private:
    Ui::FramelessWindow *ui;

    bool LeftBorderHit(const QPoint &pos, int additional_size = 0) const;
    bool RightBorderHit(const QPoint &pos, int additional_size = 0) const;
    bool TopBorderHit(const QPoint &pos, int additional_size = 0) const;
    bool BottomBorderHit(const QPoint &pos, int additional_size = 0) const;
    const qint8 DRAG_BORDER_SIZE = 5;

    void BorderCursorCheck(const QPoint &pos);
    void BorderDragging(const QPoint &pos);

    QPoint press_pos;
    QRect press_geometry;
    //int press_to_right_border;
    //int press_to_bottom_border;
    bool mouse_pressed;
    bool left_drag;
    bool right_drag;
    bool top_drag;
    bool bottom_drag;
};

#endif // FRAMELESSWINDOW_H
