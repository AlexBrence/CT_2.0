#include "filehandler.h"

#include <QFileDialog>
#include <QDebug>


FileHandler::FileHandler()
{}


bool FileHandler::_parseCTScan()
{
  throw std::logic_error(std::string(__FUNCTION__) + " not implemented yet.");
}

bool FileHandler::_parseColorPalette()
{
  throw std::logic_error(std::string(__FUNCTION__) + " not implemented yet.");
}


QString FileHandler::browseFile(const QString& filter, const FileType fileType)
{
  m_bParsed = false;
  m_fileName = QFileDialog::getOpenFileName(
      nullptr,
      "Select a file",
      "",
      filter
  );

  if (!m_fileName.isEmpty())
  {
    switch (fileType)
    {
      case FileType::CT_SCAN:
        m_bParsed = _parseCTScan();
        break;

      case FileType::COLOR_PALETTE:
        m_bParsed = _parseColorPalette();
        break;

      default:
        qDebug() << QString(__FUNCTION__) + " default case, shouldn't happened.";
        assert(0);
        break;
    }
  }

  return m_fileName;
}


bool FileHandler::isFileValid() const
{
  return m_bParsed;
}
