#pragma once

#include <QtQuick>
#include "datacollection.h"

class QCustomPlot;
class QCPAbstractPlottable;

class CustomPlotItem : public QQuickPaintedItem {
  Q_OBJECT

public:
  CustomPlotItem(QQuickItem *parent = 0);
  virtual ~CustomPlotItem();

  void paint(QPainter *painter);

  Q_INVOKABLE void initCustomPlot(int index);
  Q_INVOKABLE void placePointerGraph(QList<QSharedPointer<DataCollection>> sensor_ptr, bool exp); // additional paramters
  Q_INVOKABLE void setCustomLabel(const QString &label); // additional paramters
  Q_INVOKABLE void updatePlot();
  Q_INVOKABLE void resetPos();
protected:
  void routeMouseEvents(QMouseEvent *event);
  void routeWheelEvents(QWheelEvent *event);
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mouseDoubleClickEvent(QMouseEvent *event);
  virtual void wheelEvent(QWheelEvent *event);

private:
  QCustomPlot *m_CustomPlot;
  unsigned int m_index;

  bool rescalingON;
  QStringList m_plotNames; // as array of str and graph position
  QSharedPointer<DataCollection> m_time;
  QList<QSharedPointer<DataCollection>> m_sensors;

// public slots:
//   void backendData(const QString &name, const QList<double> &x, const QList<double> &y);

private slots:
  void graphClicked(QCPAbstractPlottable *plottable);
  void onCustomReplot();
  void updateCustomPlotSize();
};
