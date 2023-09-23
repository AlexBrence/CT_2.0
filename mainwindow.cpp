#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas.h"

#include <QDebug>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  CLineEditDragDrop* const pleCTImage      = ui->leCTImagePath;
  CLineEditDragDrop* const pleColorPalette = ui->leColorPalettePath;

  // Set file type
  pleCTImage->setFileType(CFileHandler::EFileType::CT_SCAN);
  pleColorPalette->setFileType(CFileHandler::EFileType::COLOR_PALETTE);

  connect(pleCTImage, &CLineEditDragDrop::textChanged, this, [=]() {
    fileDropped(pleCTImage->getFileName(), pleCTImage->getFileType());
  });
  connect(pleColorPalette, &CLineEditDragDrop::textChanged, this, [=]() {
    fileDropped(pleColorPalette->getFileName(), pleColorPalette->getFileType());
  });

  // On click browse files
  connect(ui->pbBrowseCTImage, SIGNAL(clicked()), this, SLOT(browseCTImage()));
  connect(ui->pbBrowseColor,   SIGNAL(clicked()), this, SLOT(browseColorPalette()));
  connect(ui->pbSave,          SIGNAL(clicked()), this, SLOT(saveImage()));

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

  if (m_fileHandler.areFilesReady())
  {
    renderImage();
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

  if (m_fileHandler.areFilesReady())
  {
    renderImage();
  }
}


void MainWindow::fileDropped(const QString& strFileName, const CFileHandler::EFileType fileType)
{
  m_fileHandler.setFile(strFileName, fileType);

  if (m_fileHandler.areFilesReady())
  {
    renderImage();
  }
}


void MainWindow::renderImage()
{
  QImage image = m_fileHandler.generateImage();
  ui->canvas_image->renderImage(std::move(image));

  // Enable saving
  ui->pbSave->setEnabled(true);
}


void MainWindow::saveImage()
{
  bool bSaved = m_fileHandler.saveImage();
  if (bSaved)
  {
    QMessageBox::information(this, QObject::tr("Save image"), QObject::tr("Saved successfully."));
  }
  else
  {
    QMessageBox::warning(this, QObject::tr("Saving unsuccessful"), QObject::tr("Could not save the image."));
  }
}



