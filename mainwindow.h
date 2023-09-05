#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "filehandler.h"


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


private:
  Ui::MainWindow *ui;

  /// Image that will be generated and shown later
  QImage m_image;

  /// File handler for CT scan
  FileHandler m_fhCTScan;
  /// File handler for color palette
  FileHandler m_fhColorPalette;
};

#endif // MAINWINDOW_H
