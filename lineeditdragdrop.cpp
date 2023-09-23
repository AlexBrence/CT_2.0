#include "lineeditdragdrop.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>


CLineEditDragDrop::CLineEditDragDrop(QWidget* parent)
  : QLineEdit(parent)
{
  setAcceptDrops(true);
}


QString CLineEditDragDrop::getFileName() const
{
  return m_strFileName;
}


CFileHandler::EFileType CLineEditDragDrop::getFileType() const
{
  return m_fileType;
}


void CLineEditDragDrop::setFileType(const CFileHandler::EFileType fileType)
{
  m_fileType = fileType;
}


void CLineEditDragDrop::dragEnterEvent(QDragEnterEvent* event)
{
  if (event->mimeData()->hasText())
  {
    QString text = event->mimeData()->text().trimmed();
    bool bIsImg  = text.endsWith(".img") && m_fileType == CFileHandler::EFileType::CT_SCAN;
    bool bIsLut  = text.endsWith(".lut") && m_fileType == CFileHandler::EFileType::COLOR_PALETTE;

    if (bIsImg || bIsLut)
    {
      event->accept();
      return;
    }
  }
  event->ignore();
}


void CLineEditDragDrop::dropEvent(QDropEvent* event)
{
  if (event->mimeData()->hasText())
  {
    QUrl url(event->mimeData()->text().trimmed());
    if (url.isLocalFile())
    {
      m_strFileName = url.toLocalFile();
      setText(m_strFileName);
    }
  }
}

