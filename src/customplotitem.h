#pragma once

#include <QtQuick>

class QCustomPlot;
class QCPAbstractPlottable;

class CustomPlotItem : public QQuickPaintedItem {
  Q_OBJECT

public:
  CustomPlotItem(QQuickItem *parent = 0);
  virtual ~CustomPlotItem();

  void paint(QPainter *painter);

  Q_INVOKABLE void initCustomPlot(int index);
  Q_INVOKABLE void placeGraph(const QString &name);
  Q_INVOKABLE void resetPos();
  // QStringList as PROPERTY?

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
  int m_timerId;
  int testTimer;
  bool rescalingON;
  QStringList m_plotNames; // as array of str and graph position

public slots:
  void backendData(const QString &name, const QList<double> &x, const QList<double> &y);

private slots:
  void graphClicked(QCPAbstractPlottable *plottable);
  void onCustomReplot();
  void updateCustomPlotSize();
};
