#include "filehandler.h"

#include <QFileDialog>
#include <QDebug>

#include <cstdint>


constexpr uint16_t CT_WIDTH  = 512;
constexpr uint16_t CT_HEIGHT = 512;
constexpr uint16_t COLOR_PALETTE_WIDTH  = 256;
constexpr uint16_t COLOR_PALETTE_HEIGHT = 3;


CFileHandler::CFileHandler()
{}


bool CFileHandler::_parseCTScan(const QString& strFileName)
{
  // Open a file
  QFile file(strFileName);
  // TODO: should you clear the vector first?

  if (!file.open(QIODevice::ReadOnly))
  {
    qDebug() << "Could not read the file in: " + QString(__FUNCTION__);
    return false;
  }

  // Reserve space
  m_v2CTScan.reserve(CT_WIDTH);
  for (QVector<short>& v : m_v2CTScan)
  {
    v.reserve(CT_HEIGHT);
  }
  QByteArray aBytes;
  aBytes.reserve(CT_HEIGHT);

  // Read content into a vector
  for (size_t i = 0; i < CT_WIDTH; i++)
  {
    aBytes = file.read(CT_HEIGHT);
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
  // Open a file
  QFile file(strFileName);

  if (!file.open(QIODevice::ReadOnly))
  {
    qDebug() << "Could not read the file in: " + QString(__FUNCTION__);
    return false;
  }

  // Reserve space
  m_vbyColorPalette.reserve(COLOR_PALETTE_WIDTH);
  for (QByteArray& aBytes : m_vbyColorPalette)
  {
    aBytes.reserve(COLOR_PALETTE_HEIGHT);
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
        break;
    }
  }

  return strFileName;
}


bool CFileHandler::areFilesReady() const
{
  return m_bParsed;
}

QImage& CFileHandler::generateImage()
{
  throw std::logic_error("Not implemented yet");
}
