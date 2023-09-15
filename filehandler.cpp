#include "filehandler.h"

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


bool CFileHandler::_parseCTScan(const QString& strFileName)
{
  // Clear the vector
  for (QVector<short> inner : m_v2CTScan)
  {
    inner.clear();
  }
  m_v2CTScan.clear();

  // Open a file
  QFile file(strFileName);

  if (!file.open(QIODevice::ReadOnly))
  {
    qDebug() << "Could not read the file in: " + QString(__FUNCTION__);
    return false;
  }

  QByteArray aBytes;
  aBytes.reserve(CT_HEIGHT);

  // Read content into a vector
  for (size_t i = 0; i < CT_WIDTH; i++)
  {
    aBytes = file.read(CT_HEIGHT * 2);
    assert(aBytes.size() != 0);

    QVector<short> vBytes;
    vBytes.reserve(aBytes.size() / sizeof(short));

    // Convert bytes to short and push it into vector
    const char* pData = aBytes.constData();

    for (qsizetype i = 0; i < aBytes.size(); i += sizeof(short))
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
  for (QByteArray byArr : m_vbyColorPalette)
  {
    byArr.clear();
  }
  m_vbyColorPalette.clear();

  // Open a file
  QFile file(strFileName);

  if (!file.open(QIODevice::ReadOnly))
  {
    qDebug() << "Could not read the file in: " + QString(__FUNCTION__);
    return false;
  }

  // Read content into vector
  for (size_t i = 0; i < COLOR_PALETTE_WIDTH; i++)
  { // TODO: check if correct number of bytes were read
    m_vbyColorPalette.push_back(file.read(COLOR_PALETTE_HEIGHT));
  }

  return true;
}


QString CFileHandler::browseFile(const QString& filter, const EFileType fileType)
{
  m_bParsed = false;
  QString strFileName = QFileDialog::getOpenFileName(
      nullptr,
      "Select a file",
      "",
      filter
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

  return strFileName;
}


bool CFileHandler::areFilesReady() const
{ // TODO: make this better
  return (!m_v2CTScan.empty() && !m_vbyColorPalette.empty());
}


QImage CFileHandler::generateImage()
{
  QImage img(CFileHandler::CT_WIDTH, CFileHandler::CT_HEIGHT, QImage::Format_RGB888);

  // Convert every pixel from CT image to RGB value
  for (size_t i = 0; i < CFileHandler::CT_WIDTH; i++)
  {
    for (size_t j = 0; j < CFileHandler::CT_HEIGHT; j++)
    {
      quint8 colorIdx = (static_cast<float>(m_v2CTScan[i][j] + 2048) / 4095) * 255;
      QRgb   color    = qRgb(m_vbyColorPalette[colorIdx][0], m_vbyColorPalette[colorIdx][1], m_vbyColorPalette[colorIdx][2]);
      img.setPixel(i, j, color);
    }
  }

  return img;
}

