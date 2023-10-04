#include "canvas.h"
#include "filehandler.h"

#include <QHBoxLayout>
#include <QPainter>


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
  QPainter painter(this);

  if (m_image.isNull())
  {
    // Draw a border
    const QPen pen(Qt::gray, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawRect(0, 0, CFileHandler::CT_WIDTH, CFileHandler::CT_HEIGHT);

    QWidget::paintEvent(paintEvent);
    return;
  }
  // Draw the image
  painter.drawImage(0, 0, m_image);
}
