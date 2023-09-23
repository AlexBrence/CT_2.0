#pragma once

#include "filehandler.h"

#include <QLineEdit>
#include <QObject>


class CLineEditDragDrop : public QLineEdit
{
  Q_OBJECT

public:
  explicit CLineEditDragDrop(QWidget* parent = nullptr);

  /**
   * @brief Returns file name that was dropped in
   * @return QString filename
   */
  QString getFileName() const;

  /**
   * @brief  Returns the type of the file
   * @return CFileHandler::EFileType
   */
  CFileHandler::EFileType getFileType() const;

  /**
   * @brief Sets the file type that is expected to be dropped in
   * @param fileType enum from CFileHandler class
   */
  void setFileType(const CFileHandler::EFileType fileType);


protected:
  void dragEnterEvent(QDragEnterEvent* event) override;
  void dropEvent(QDropEvent* event) override;


private:
  QString m_strFileName;
  CFileHandler::EFileType m_fileType;
};

