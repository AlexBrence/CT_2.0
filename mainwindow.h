#pragma once

#include "filehandler.h"

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();


public slots:
  /**
   * @brief Envokes the file handler and updates the
   * label with the chosen file path
   */
  void browseCTImage();

  /**
   * @brief Envokes the file handler and updates the
   * label with the chosen file path
   */
  void browseColorPalette();

  /**
   * @brief Calls a function in FileHandler that will
   * generate image and render it
   */
  void renderImage();

  /**
   * @brief Tells filehandler to save image
   */
  void saveImage();


private:
  Ui::MainWindow *ui;

  /// File handler for CT scan and color pallete
  CFileHandler m_fileHandler;
};

