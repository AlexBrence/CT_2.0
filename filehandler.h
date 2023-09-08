#pragma once


#include <QWidget>
#include <QFile>
#include <QVector>
#include <QByteArray>

/**
 * Class that handles browsing and loading files. It also handles
 * merging image and color palette to generate a single image.
 */
class CFileHandler
{
public:
  enum class EFileType {
    CT_SCAN,
    COLOR_PALETTE
  };

  CFileHandler();

  /**
   * @brief  Opens a file dialog and returns file path of the chosen file
   * @param  filter  filter files to specific needs
   * @return QString file name with absolute path
   */
  QString browseFile(const QString& filter, const EFileType fileType);

  /**
   * @brief Returns whether a file has been successfully parsed
   * @return bool true if file was parsed successfully, false otherwise
   */
  bool areFilesReady() const;

  QImage& generateImage();


private:
  QString m_strFileNameCT;
  QString m_strFileNameColor;
  // TODO: reserve space in the constructor
  QVector<QVector<short>> m_v2CTScan;        // 256 * 3
  QVector<QByteArray>     m_vbyColorPalette; // 512 * 512
  bool m_bParsed = false;


  bool _parseCTScan(const QString& strFileName);
  bool _parseColorPalette(const QString& strFileName);
};

