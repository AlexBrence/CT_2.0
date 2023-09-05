#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  // On click browse files
  QPushButton* pPbBrowseCTImage = ui->pbBrowseCTImage;
  QPushButton* pPbBrowseColor   = ui->pbBrowseColor;
  connect(pPbBrowseCTImage, SIGNAL(clicked()), this, SLOT(browseCTImage()));
  connect(pPbBrowseColor,   SIGNAL(clicked()), this, SLOT(browseColorPalette()));
}

MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::browseCTImage()
{
  const QString fileName = m_fhCTScan.browseFile("*.img", FileHandler::FileType::CT_SCAN);
  ui->leCTImagePath->setText(fileName);

  // Enable 'Generate image' button if both files are valid
  if (m_fhCTScan.isFileValid() && m_fhColorPalette.isFileValid())
  {
    ui->pbGenerate->setEnabled(true);
  }
}

void MainWindow::browseColorPalette()
{
  const QString fileName = m_fhColorPalette.browseFile("*.lut", FileHandler::FileType::COLOR_PALETTE);
  ui->leColorPalettePath->setText(fileName);

  // Enable 'Generate image' button if both files are valid
  if (m_fhCTScan.isFileValid() && m_fhColorPalette.isFileValid())
  {
    ui->pbGenerate->setEnabled(true);
  }
}
