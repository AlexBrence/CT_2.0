#pragma once

#include <QWidget>
#include <QFile>
#include <QVector>
#include <QByteArray>

#include <cstdint>

/**
 * Class that handles browsing and loading files. It also handles
 * merging image and color palette to generate a single image.
 */
class CFileHandler
{
public:
  static constexpr uint16_t CT_WIDTH  = 512;
  static constexpr uint16_t CT_HEIGHT = 512;
  static constexpr uint16_t COLOR_PALETTE_WIDTH  = 256;
  static constexpr uint16_t COLOR_PALETTE_HEIGHT =   3;

  enum class EFileType {
    CT_SCAN,
    COLOR_PALETTE
  };

  CFileHandler();

  /**
   * @brief  Opens a file dialog and returns file path of the chosen file
   * @param  strFilter    filter files to specific needs
   * @param  fileType  type of the file
   * @return QString   file name with absolute path
   */
  QString browseFile(const QString& strFilter, const EFileType fileType);

  /**
   * @brief Sets the file if user has used drag and drop, parses the file
   * @param strFileName full path of the file
   * @param fileType    type of the file
   */
  void setFile(const QString& strFileName, const EFileType fileType);

  /**
   * @brief Returns whether a file has been successfully parsed
   * @return bool true if file was parsed successfully, false otherwise
   */
  bool areFilesReady() const;

  /**
   * @brief Generates the image from CT scan and color palette
   * @return QImage generated image
   */
  QImage generateImage();

  /**
   * @brief Saves the image that is currently being displayed
   * @return bool true if saved successfully
   */
  bool saveImage() const;


private:
  QString                 m_strFileNameCT;
  QString                 m_strFileNameColor;
  QVector<QVector<short>> m_v2CTScan;        // 512 * 512
  QVector<QByteArray>     m_vbyColorPalette; // 256 * 3
  QImage                  m_image   = QImage(CFileHandler::CT_WIDTH, CFileHandler::CT_HEIGHT, QImage::Format_RGB888);
  bool                    m_bParsed = false;


  bool _parseCTScan(const QString& strFileName);
  bool _parseColorPalette(const QString& strFileName);
  void _clearV2CTScan();
  void _clearVbyColorPalette();
};

