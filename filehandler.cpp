#include "filehandler.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QRgb>


CFileHandler::CFileHandler()
{
  // Reserve space
  m_v2CTScan.reserve(CT_WIDTH);
  for (QVector<short>& v : m_v2CTScan)
  {
    v.reserve(CT_HEIGHT);
  }

  m_vbyColorPalette.reserve(COLOR_PALETTE_WIDTH);
  for (QByteArray& aBytes : m_vbyColorPalette)
  {
    aBytes.reserve(COLOR_PALETTE_HEIGHT);
  }
}


void CFileHandler::_clearV2CTScan()
{
  for (QVector<short> inner : m_v2CTScan)
  {
    inner.clear();
  }
  m_v2CTScan.clear();
}


void CFileHandler::_clearVbyColorPalette()
{
  for (QByteArray byArr : m_vbyColorPalette)
  {
    byArr.clear();
  }
  m_vbyColorPalette.clear();
}


bool CFileHandler::_parseCTScan(const QString& strFileName)
{
  // Clear the vector
  _clearV2CTScan();

  // Open a file
  QFile file(strFileName);

  if (!file.open(QIODevice::ReadOnly))
  {
    qWarning() << "Could not read the file (" + strFileName + ") in: " + QString(__FUNCTION__);
    return false;
  }

  QByteArray aBytesRead;
  aBytesRead.reserve(CT_HEIGHT * 2); // short = 2 bytes

  // Read content into a vector
  for (size_t i = 0; i < CT_WIDTH; i++)
  {
    aBytesRead = file.read(CT_HEIGHT * 2);
    if (aBytesRead.size() != CT_HEIGHT * 2)
    {
      qWarning() << "Error while reading from CT scan file!";
      _clearV2CTScan();
      return false;
    }

    QVector<short> vBytes;
    vBytes.reserve(aBytesRead.size() / sizeof(short));

    // Convert bytes to short and push it into vector
    const char* pData = aBytesRead.constData();

    for (qsizetype i = 0; i < aBytesRead.size(); i += sizeof(short))
    {
      short value = *reinterpret_cast<const short*>(pData + i);
      vBytes.append(std::move(value));
    }
    m_v2CTScan.append(vBytes);
  }

  return true;
}


bool CFileHandler::_parseColorPalette(const QString& strFileName)
{
  // Clear the vector
  _clearVbyColorPalette();

  // Open a file
  QFile file(strFileName);

  if (!file.open(QIODevice::ReadOnly))
  {
    qWarning() << "Could not read the file (" + strFileName + ") in: " + QString(__FUNCTION__);
    return false;
  }

  // Read content into vector
  for (size_t i = 0; i < COLOR_PALETTE_WIDTH; i++)
  {
    QByteArray aBytesRead = file.read(COLOR_PALETTE_HEIGHT);
    if (aBytesRead.size() != 3)
    {
      qWarning() << "Error while reading from color palette file!";
      _clearVbyColorPalette();
      return false;
    }

    m_vbyColorPalette.push_back(std::move(aBytesRead));
  }

  return true;
}


QString CFileHandler::browseFile(const QString& strFilter, const EFileType fileType)
{
  m_bParsed = false;
  QString strFileName = QFileDialog::getOpenFileName(
      nullptr,
      "Select a file",
      "",
      strFilter
  );

  if (!strFileName.isEmpty())
  {
    switch (fileType)
    {
      case EFileType::CT_SCAN:
        m_bParsed = _parseCTScan(strFileName);
        break;
      case EFileType::COLOR_PALETTE:
        m_bParsed = _parseColorPalette(strFileName);
        break;
      default:
        qDebug() << QString(__FUNCTION__) + " default case, shouldn't happened.";
        assert(0);
    }
  }

  return m_bParsed ? strFileName : "";
}


void CFileHandler::setFile(const QString& strFileName, const EFileType fileType)
{
  m_bParsed = false;

  if (!strFileName.isEmpty())
  {
    switch (fileType)
    {
      case EFileType::CT_SCAN:
        m_bParsed = _parseCTScan(strFileName);
        break;
      case EFileType::COLOR_PALETTE:
        m_bParsed = _parseColorPalette(strFileName);
        break;
      default:
        qDebug() << QString(__FUNCTION__) + " default case, shouldn't happened.";
        assert(0);
    }
  }
}


bool CFileHandler::areFilesReady() const
{
  return !m_v2CTScan.empty() && !m_vbyColorPalette.empty();
}


QImage CFileHandler::generateImage()
{
  // Convert every pixel from CT image to RGB value
  for (size_t i = 0; i < CFileHandler::CT_WIDTH; i++)
  {
    for (size_t j = 0; j < CFileHandler::CT_HEIGHT; j++)
    {
      quint8 colorIdx = (static_cast<float>(m_v2CTScan[i][j] + 2048) / 4095) * 255;
      QRgb   color    = qRgb(m_vbyColorPalette[colorIdx][0],
                             m_vbyColorPalette[colorIdx][1],
                             m_vbyColorPalette[colorIdx][2]);
      m_image.setPixel(i, j, color);
    }
  }

  return m_image;
}


bool CFileHandler::saveImage() const
{
  QString strFileName = QFileDialog::getSaveFileName(
      nullptr,
      QDialog::tr("Save Image"),
      ""
  );
  return m_image.save(strFileName + ".png", "PNG", 0);
}
