#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMenu>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE


struct myPoint
{
    QPoint point;
    QPoint movePoint;

    int m_r;
    int m_g;
    int m_b;
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void CreateMenu();
    bool needUpdate;

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


public slots:
    void onDeleteClicked();
    void onColorClicked();

private slots:
    void on_Update_clicked();

private:
    Ui::Widget *ui;

    bool m_bCliked=false;
    QPoint m_Point;
    QPoint m_movePoint;

    QMenu *m_pMenu=nullptr;

    QFont m_Font;

    QList<myPoint> pointList;

    int m_R=255;
    int m_G=0;
    int m_B=0;
};
#endif // WIDGET_H
