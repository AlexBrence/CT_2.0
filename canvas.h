#pragma once

#include <QWidget>
#include <QLabel>

class CCanvas : public QWidget
{
  Q_OBJECT

public:
  explicit CCanvas(QWidget *parent = nullptr);

  /**
   * @brief Renders the image
   * @param image image to be rendered
   */
  void renderImage(QImage&& image);

  /**
   * @brief Override paint event
   * @param paintEvent
   */
  void paintEvent(QPaintEvent* paintEvent) override;

private:
  QImage m_image;
};
