/*
 *
 *
 *  Created on: 2018年10月1日
 *      Author: Lzy
 */
#include "linechart.h"

LineChart::LineChart(QWidget *parent) : QWidget(parent)
{
    mChart = new QChart();
    //const QStringList titles{"Blue", "Green", "Red"};

    mSeries1 = new QSplineSeries(this);
    mSeries1->setColor(QColor("Blue"));
    mSeries1->setName("ELoad1");
    mChart->addSeries(mSeries1);

    mSeries2 = new QSplineSeries(this);
    mSeries2->setColor(QColor("Green"));
    mSeries2->setName("ELoad2");
    mChart->addSeries(mSeries2);

    mSeries3 = new QSplineSeries(this);
    mSeries3->setColor(QColor("Red"));
    mSeries3->setName("ELoad3");
    mChart->addSeries(mSeries3);

    mChart->legend()->show();
    mChart->createDefaultAxes();

    mChartView = new QChartView(mChart);
    mChartView->setRenderHint(QPainter::Antialiasing);

    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(0, 0, 0, 10);
    gridLayout->addWidget(mChartView);

    initAxisX();
    initAxisY();
}

void LineChart::setTitle(const QString &title)
{
    mChart->setTitle(title);
}

void LineChart::initAxisY()
{
    QAbstractAxis *axis =  mChart->axisY();
    axis->setTitleText(tr("温度"));
    mY = 0; setAxisYRange(40);
}

void LineChart::initAxisX()
{
    QAbstractAxis *axis =  mChart->axisX();
    mChart->removeAxis(axis);

    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("hh:mm:ss");
    axisX->setTitleText("Time");
    mChart->addAxis(axisX, Qt::AlignBottom);
    mSeries1->attachAxis(axisX);
    mSeries2->attachAxis(axisX);
    mSeries3->attachAxis(axisX);

    initAxisXTime();
}

void LineChart::initAxisXTime()
{
    QAbstractAxis *axis =  mChart->axisX();

    QDateTime time = QDateTime::currentDateTime();
    mYtime = time.addSecs(60);
    axis->setRange(time, mYtime);
}

void LineChart::setAxisYRange(int value)
{
    int y = int (mY * 3 / 4.0);
    if(value > y)  {
        mY = int (value * 3 / 2.0);
        mChart->axisY()->setRange(0, mY);
    }
}

void LineChart::setAxisXRange()
{
    if(QDateTime::currentDateTime() > mYtime)   {
        QDateTime time = QDateTime::currentDateTime();
        mYtime = time.addSecs(60);
        mChart->axisX()->setMax(mYtime);
    }
}

void LineChart::append(qreal y,int index)
{
    setAxisXRange();
    setAxisYRange(y);

    qint64 x = QDateTime::currentDateTime().toMSecsSinceEpoch();
    switch(index)
    {
    case 1:
        mSeries1->append(x, y);
        break;
    case 2:
        mSeries2->append(x, y);
        break;
    case 3:
        mSeries3->append(x, y);
        break;
    default:
        mSeries1->append(x, y);
        break;
    }


}

void LineChart::clearChart()
{
    mSeries1->clear();
    mSeries2->clear();
    mSeries3->clear();
    initAxisXTime();
}
