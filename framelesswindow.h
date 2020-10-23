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
//    void mousePressEvent(QMouseEvent *event) override;
//    void mouseReleaseEvent(QMouseEvent *event) override;

    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual void MouseBorderCheck(QMouseEvent *event);


private:
    Ui::FramelessWindow *ui;

    bool LeftBorderHit(const QPoint &pos);
    bool RightBorderHit(const QPoint &pos);
    bool TopBorderHit(const QPoint &pos);
    bool BottomBorderHit(const QPoint &pos);

};

#endif // FRAMELESSWINDOW_H
