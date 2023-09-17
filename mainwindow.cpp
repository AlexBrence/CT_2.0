#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas.h"

#include <QDebug>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  // On click browse files
  QPushButton* pPbBrowseCTImage = ui->pbBrowseCTImage;
  QPushButton* pPbBrowseColor   = ui->pbBrowseColor;
  QPushButton* pPbGenerateImg   = ui->pbGenerate;
  connect(pPbBrowseCTImage, SIGNAL(clicked()), this, SLOT(browseCTImage()));
  connect(pPbBrowseColor,   SIGNAL(clicked()), this, SLOT(browseColorPalette()));
  connect(pPbGenerateImg,   SIGNAL(clicked()), this, SLOT(renderImage()));
}

MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::browseCTImage()
{
  const QString fileName = m_fileHandler.browseFile("*.img", CFileHandler::EFileType::CT_SCAN);
  if (fileName.isEmpty())
  {
    QMessageBox::warning(this, QObject::tr("Invalid file"), QObject::tr("Could not parse the file!"));
    return;
  }
  ui->leCTImagePath->setText(fileName);

  // Enable 'Generate image' button if both files are valid
  if (m_fileHandler.areFilesReady())
  {
    ui->pbGenerate->setEnabled(true);
  }
}

void MainWindow::browseColorPalette()
{
  const QString fileName = m_fileHandler.browseFile("*.lut", CFileHandler::EFileType::COLOR_PALETTE);
  if (fileName.isEmpty())
  {
    QMessageBox::warning(this, QObject::tr("Invalid file"), QObject::tr("Could not parse the file!"));
    return;
  }
  ui->leColorPalettePath->setText(fileName);

  // Enable 'Generate image' button if both files are valid
  if (m_fileHandler.areFilesReady())
  {
    ui->pbGenerate->setEnabled(true);
  }
}

void MainWindow::renderImage()
{
  QImage image = m_fileHandler.generateImage();
  ui->canvas_image->renderImage(std::move(image));
}
