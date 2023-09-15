#include "canvas.h"
#include "filehandler.h"

#include <QPainter>
#include <QHBoxLayout>


CCanvas::CCanvas(QWidget *parent)
    : QWidget{parent}, m_label(new QLabel(this))
{
  // TODO: do you need this?
  // QHBoxLayout* layout = new QHBoxLayout(this);

  m_label->setFixedSize(CFileHandler::CT_WIDTH, CFileHandler::CT_HEIGHT);

  // delete layout;
}

CCanvas::~CCanvas()
{
  delete m_label;
}

void CCanvas::renderImage(QImage&& image)
{
  m_image = std::move(image);
  update();
}


void CCanvas::paintEvent(QPaintEvent* paintEvent)
{
  if (m_image.isNull())
  {
    qDebug() << "Image is null, calling the default method";
    QWidget::paintEvent(paintEvent);
    return;
  }
  QPixmap pixmap = QPixmap::fromImage(m_image);
  m_label->setPixmap(pixmap);
}
