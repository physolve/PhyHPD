#include "customplotitem.h"
#include "../lib/qcustomplot.h"
#include <QDebug>

CustomPlotItem::CustomPlotItem(QQuickItem *parent)
    : QQuickPaintedItem(parent), m_CustomPlot(nullptr), rescalingON(true) {
    setFlag(QQuickItem::ItemHasContents, true);
    setAcceptedMouseButtons(Qt::AllButtons);

    connect(this, &QQuickPaintedItem::widthChanged, this,
            &CustomPlotItem::updateCustomPlotSize);
    connect(this, &QQuickPaintedItem::heightChanged, this,
            &CustomPlotItem::updateCustomPlotSize);
    qDebug() << "CustomPlotItem Created";
}

CustomPlotItem::~CustomPlotItem() {
    delete m_CustomPlot;
    m_CustomPlot = nullptr;
    qDebug() << "CustomPlotItem Destroyed";
  
}

void CustomPlotItem::initCustomPlot(int index) {
    if(!m_CustomPlot){
        m_CustomPlot = new QCustomPlot();
        m_index = index;
        connect( m_CustomPlot, &QCustomPlot::destroyed, this, [=](){ qDebug() << QString(" QCustomPlot (%1) pointer is destroyed ").arg(index); });
        updateCustomPlotSize();

        m_CustomPlot->setOpenGl(true);
        m_CustomPlot->setNoAntialiasingOnDrag(true);
        // m_CustomPlot->layer(0)->setMode(QCPLayer::LayerMode::lmBuffered);
        QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
        timeTicker->setTimeFormat("%h:%m:%s");
        m_CustomPlot->xAxis->setTicker(timeTicker);
        m_CustomPlot->yAxis->setRange(0.0, 1.0);
        m_CustomPlot->axisRect()->setupFullAxesBox(); //?
        m_CustomPlot->yAxis->setRange(0.0, 1.0);

        //make left and bottom axes transfer their ranges to right and top axes:
        connect(m_CustomPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), m_CustomPlot->xAxis2, SLOT(setRange(QCPRange))); //?
        connect(m_CustomPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), m_CustomPlot->yAxis2, SLOT(setRange(QCPRange))); //?


        m_CustomPlot->xAxis->setLabel("Время, с");
        m_CustomPlot->xAxis->setLabelColor(Qt::white);
        m_CustomPlot->yAxis->setLabel("Давление, бар");
        m_CustomPlot->yAxis->setLabelColor(Qt::white);
        m_CustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        //startTimer(500);

        connect(m_CustomPlot, &QCustomPlot::afterReplot, this, &CustomPlotItem::onCustomReplot);
        m_CustomPlot->legend->setVisible(true);
        m_CustomPlot->legend->setBrush(Qt::NoBrush);
        m_CustomPlot->legend->setTextColor(Qt::white);
        m_CustomPlot->legend->setBorderPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
        qDebug() << QString("QCustomplot (%1) Initialized").arg(index);

        // set some pens, brushes and backgrounds:
        m_CustomPlot->xAxis->setBasePen(QPen(Qt::white, 1));
        m_CustomPlot->yAxis->setBasePen(QPen(Qt::white, 1));
        m_CustomPlot->xAxis->setTickPen(QPen(Qt::white, 1));
        m_CustomPlot->yAxis->setTickPen(QPen(Qt::white, 1));
        m_CustomPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
        m_CustomPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
        m_CustomPlot->xAxis->setTickLabelColor(Qt::white);
        m_CustomPlot->yAxis->setTickLabelColor(Qt::white);
        m_CustomPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
        m_CustomPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
        m_CustomPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
        m_CustomPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
        m_CustomPlot->xAxis->grid()->setSubGridVisible(true);
        m_CustomPlot->yAxis->grid()->setSubGridVisible(true);
        m_CustomPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
        m_CustomPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
        m_CustomPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
        m_CustomPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
        QLinearGradient plotGradient;
        plotGradient.setStart(0, 0);
        plotGradient.setFinalStop(0, 350);
        plotGradient.setColorAt(0, QColor(80, 80, 80));
        plotGradient.setColorAt(1, QColor(50, 50, 50));
        m_CustomPlot->setBackground(plotGradient);
        QLinearGradient axisRectGradient;
        axisRectGradient.setStart(0, 0);
        axisRectGradient.setFinalStop(0, 350);
        axisRectGradient.setColorAt(0, QColor(80, 80, 80));
        axisRectGradient.setColorAt(1, QColor(30, 30, 30));
        m_CustomPlot->axisRect()->setBackground(axisRectGradient);
    }
    //m_CustomPlot->replot();
}

void CustomPlotItem::placePointerGraph(const QString &name, QSharedPointer<ControllerData> sensor_ptr){
    if(sensor_ptr->m_name != name){
        qDebug() << "You ruined " << name << " sensor in placePointerGraph"; 
        return;
    }
    qDebug() << "add " << name << " to plot " << m_index;
    m_sensors.append(sensor_ptr);
    m_CustomPlot->addGraph();
    QStringList lineColors = {"#cb8175", "#e2a97e", "#f0cf8e", "#f6edcd", "#a8c8a6", "#6d8d8a", "#655057" };
    
    auto pen = QPen(QColor(lineColors.value(m_plotNames.count())), 1.5);
    
    m_CustomPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, pen, QBrush(Qt::white), 9));
    m_CustomPlot->graph()->setPen(QPen(QColor(120, 120, 120), 2));
    m_CustomPlot->graph()->setAdaptiveSampling(true);
    m_CustomPlot->graph()->setName(name);
    m_plotNames << name;

}

void CustomPlotItem::placeExpDataGraph(const QString &name, QSharedPointer<ExpData> data_ptr){
    if(data_ptr->m_name != name){
        qDebug() << "You ruined " << name << " sensor in placeExpDataGraph"; 
        return;
    }
    qDebug() << "add " << name << " to plot " << m_index;
    m_sensors.append(data_ptr);
    m_CustomPlot->addGraph();
    QStringList lineColors = {"#a8c8a6", "#6d8d8a", "#655057", "#cb8175", "#e2a97e", "#f0cf8e", "#f6edcd"};
    
    auto pen = QPen(QColor(lineColors.value(m_plotNames.count())), 1.5);
    // scatter style
    // m_CustomPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, pen, QBrush(Qt::white), 9));
    // m_CustomPlot->graph()->setPen(QPen(QColor(120, 120, 120), 2));
    // line style
    m_CustomPlot->graph()->setPen(pen);
    m_CustomPlot->graph()->setLineStyle(QCPGraph::LineStyle::lsLine);
    m_CustomPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, pen, QBrush(Qt::white), 7));

    m_CustomPlot->graph()->setAdaptiveSampling(true);
    m_CustomPlot->graph()->setName(name);
    m_plotNames << name;

}

void CustomPlotItem::setCustomLabel(const QString &label){ // remade it from DataCollection pointer?
    m_CustomPlot->yAxis->setLabel(label);
}

void CustomPlotItem::paint(QPainter *painter) {
  if (m_CustomPlot) {
    QPixmap picture(boundingRect().size().toSize());
    QCPPainter qcpPainter(&picture);

    m_CustomPlot->toPainter(&qcpPainter);

    painter->drawPixmap(QPoint(), picture);
  }
}

void CustomPlotItem::mousePressEvent(QMouseEvent *event) {
    //qDebug() << Q_FUNC_INFO;
    routeMouseEvents(event);
}

void CustomPlotItem::mouseReleaseEvent(QMouseEvent *event) {
    //qDebug() << Q_FUNC_INFO;
    routeMouseEvents(event);
    //QQuickPaintedItem::mouseReleaseEvent(event);
}

void CustomPlotItem::mouseMoveEvent(QMouseEvent *event) {
    rescalingON = false;
    routeMouseEvents(event);
}

void CustomPlotItem::mouseDoubleClickEvent(QMouseEvent *event) {
    qDebug() << Q_FUNC_INFO;
    rescalingON = true;
    routeMouseEvents(event);
}

void CustomPlotItem::wheelEvent(QWheelEvent *event) { 
    rescalingON = false;
    routeWheelEvents(event); 
}

void CustomPlotItem::updatePlot(){
    if(m_sensors.isEmpty()){
        return;
    }
    // static double lastPointKey = 0; // making problems being static
    qreal lastPointKey = 0;
    for(auto i = 0; i < m_CustomPlot->graphCount(); ++i){
        m_CustomPlot->graph(i)->setData(m_sensors[i]->getTime(), m_sensors[i]->getValue());
        if(lastPointKey < m_sensors[i]->getCurTime())
            lastPointKey = m_sensors[i]->getCurTime();
    }
    if(rescalingON){
        m_CustomPlot->xAxis->setRange(lastPointKey, 10, Qt::AlignRight); // means there a 10 sec
        m_CustomPlot->yAxis->rescale();
        // if(m_sensors[0]->getValue().last() != 0)
        //     m_CustomPlot->yAxis->scaleRange(1.1);
    }
    m_CustomPlot->replot();
}

void CustomPlotItem::backendData(const QString &name, const QList<double> &x, const QList<double> &y){

    if(m_sensors.isEmpty()){
        return;
    }
    
    qreal lastPointKey = 0;

    auto index = m_plotNames.indexOf(name);

    for(auto i = 0; i < m_CustomPlot->graphCount(); ++i){
        m_CustomPlot->graph(i)->setData(m_sensors[i]->getTime(), m_sensors[i]->getValue());
        if(lastPointKey < m_sensors[i]->getCurTime())
            lastPointKey = m_sensors[i]->getCurTime();
    }

    if(rescalingON){
        m_CustomPlot->xAxis->setRange(lastPointKey, 10, Qt::AlignRight); // means there a 10 sec
        m_CustomPlot->yAxis->rescale();
        // if(m_sensors[0]->getValue().last() != 0)
        //     m_CustomPlot->yAxis->scaleRange(1.1);
    }
    m_CustomPlot->replot();
}

void CustomPlotItem::graphClicked(QCPAbstractPlottable *plottable) {
    qDebug() << Q_FUNC_INFO
            << QString("Clicked on graph '%1 ").arg(plottable->name());
}

void CustomPlotItem::resetPos(){
    rescalingON = true;
}

void CustomPlotItem::routeMouseEvents(QMouseEvent *event) {
    if (m_CustomPlot) {
        QMouseEvent *newEvent =
            new QMouseEvent(event->type(), event->localPos(), event->button(),
                            event->buttons(), event->modifiers());
        QCoreApplication::postEvent(m_CustomPlot, newEvent);
    }
}

void CustomPlotItem::routeWheelEvents(QWheelEvent *event) {
    if (m_CustomPlot) {
        QWheelEvent *newEvent = new QWheelEvent(
            event->position(), event->globalPosition(), event->pixelDelta(),
            event->angleDelta(), event->buttons(), event->modifiers(),
            event->phase(), event->inverted());
        QCoreApplication::postEvent(m_CustomPlot, newEvent);
        m_CustomPlot->yAxis->rescale(); //?
    }
}

void CustomPlotItem::updateCustomPlotSize() {
    if (m_CustomPlot) {
        m_CustomPlot->setGeometry(0, 0, (int)width(), (int)height());
        m_CustomPlot->setViewport(QRect(0, 0, (int)width(), (int)height()));
    }
}

void CustomPlotItem::onCustomReplot() {
    update();
}
