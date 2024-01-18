#include "widget.h"
#include "ui_widget.h"
#include <QMouseEvent>
#include <qdebug.h>
#include <QTimer>
#include <QTime>
#include <QMenu>
#include <QAction>
#include <QColorDialog>
#include <QLabel>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    CreateMenu();
    needUpdate = false;
    m_Font.setPointSize(5);
    m_Font.setFamily("Microsoft YaHei");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::CreateMenu()
{
    m_pMenu=new QMenu(this);
    QAction *clearAction=new QAction(tr("Clear"),this);
    QAction *colorAction=new QAction(tr("Color"),this);
    m_pMenu->addAction(colorAction);
    m_pMenu->addSeparator();
    m_pMenu->addAction(clearAction);
    // 当用户选择 "Clear" 菜单项时，将触发 onDeleteClicked 函数。
    connect(clearAction,&QAction::triggered,this,&Widget::onDeleteClicked);
    connect(colorAction,&QAction::triggered,this,&Widget::onColorClicked);
}

void Widget::paintEvent(QPaintEvent *event)
{
    qDebug()<<"此时调用了paintEvent函数UI";   // 程序一开始会自动调用一次，后续按住鼠标移动时也会一直调用
    // 创建一个QPixmap对象，作为绘图设备
    QPixmap pixmap(ui->TestLabel->size());
    int w = ui->TestLabel->size().width();
    int h = ui->TestLabel->size().height();
    qDebug()<<w<<h;
//    pixmap.fill(Qt::white);  // 设置背景色，默认是黑色

    // copy之后坐标原点向上平移了。如果把这句话注释掉，那么就不会有偏差，但也就没有图片，变成黑屏了。
    // 原来：看似平移，实则因为初始显示的图片是左下角的区域，而Qt在用点坐标画图时，是拿完整图片的左上角当原点，所以自然有y轴漂移的情况。
    pixmap = ui->TestLabel->pixmap()->copy(QRect(0,0,w,h));        // 可以画在上面且只画在图上，但是在这上面画图，显示的位置在画的点上面，x轴倒是一致

    QPainter painter(&pixmap);

//    QPainter painter(ui->TestLabel);
//    QPainter painter(this);


    painter.setFont(m_Font);
    qDebug()<<pointList.length();   // 实时输出当前点的个数
    // 在画布pixmap上，以某点为原点建立坐标系，根据列表里的点坐标画点
    for(int i=0;i<pointList.size();i++)
    {
        myPoint mypoint=pointList[i];
        QPen pen(QBrush(QColor(mypoint.m_r,mypoint.m_g,mypoint.m_b)),5);  // 线条宽度为5个像素
        painter.setPen(pen);
        painter.drawLine(mypoint.point.x(),mypoint.point.y(),mypoint.movePoint.x(),mypoint.movePoint.y());
    }

    if(needUpdate){
        // 将绘制好的图像设置为Label的显示内容
        ui->TestLabel->setPixmap(pixmap);
        // 可以限制画在框中，但是位置往右下方漂移了不少距离
        //（估计是因为保存的坐标是按Widget为坐标轴算的，但是画的时候是QLabel为坐标轴）
        // 更新完毕后将标志设置为 false，避免无限循环
        needUpdate = false;
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons()==Qt::LeftButton)
    {
        m_bCliked=true;
        m_Point=event->pos();
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons()==Qt::LeftButton&&m_bCliked)
    {
//        m_movePoint = event->pos();
//        m_Point = event->pos();

        // 把坐标原点从窗口左上角转为QLabel左上角，得到转化坐标系后的点坐标，保存到pointList
        // 如果干脆不转了，画图的画布也直接用this，结果就是图片置顶覆盖了画在this的笔迹
        QPoint labelPos = ui->TestLabel->mapFrom(this, event->pos());  // 法一
//        QPoint labelPos = ui->TestLabel->mapFromGlobal(event->pos());  // 法二，好像不行
        m_movePoint = labelPos;
        m_Point = labelPos;

        qDebug() << "转换坐标系后的Point Coordinates: (" << m_Point.x() << ", " << m_Point.y() << ")";

        myPoint mypoint;
        mypoint.point=m_Point;
        mypoint.movePoint=m_movePoint;
        mypoint.m_r=m_R;
        mypoint.m_g=m_G;
        mypoint.m_b=m_B;

        pointList.append(mypoint);
    }
    update();
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    m_bCliked=false;

    if(event->button()==Qt::RightButton)   // 点击右键时才执行
    {
        m_pMenu->move(mapToGlobal(event->pos()));
        m_pMenu->show();
    }

    needUpdate = true;
    update();  // 调用 update 函数触发 paintEvent
}

void Widget::onDeleteClicked()
{
    pointList.clear();
    update();
}

void Widget::onColorClicked()
{
    QColor color = QColorDialog::getColor(QColor(255,0,0));

    m_R=color.red();
    m_G=color.green();
    m_B=color.blue();
}


void Widget::on_Update_clicked()   // 此方法已集成到“松开鼠标左键事件”
{
    needUpdate = true;
    update();  // 调用 update 函数触发 paintEvent
}

void Widget::on_pushButton_clicked()
{
    qDebug()<<"hello world";
}
