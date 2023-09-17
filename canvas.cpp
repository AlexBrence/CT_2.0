#include "canvas.h"
#include "filehandler.h"

#include <QPainter>
#include <QHBoxLayout>


CCanvas::CCanvas(QWidget *parent)
    : QWidget{parent}
{}


void CCanvas::renderImage(QImage&& image)
{
  m_image = std::move(image);
  update();
}


void CCanvas::paintEvent(QPaintEvent* paintEvent)
{
  if (m_image.isNull())
  {
    QWidget::paintEvent(paintEvent);
    return;
  }
  QPainter painter(this);
  painter.drawImage(0, 0, m_image);
}
