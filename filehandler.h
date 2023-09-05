#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QWidget>

/**
 * Class that handles browsing and loading files. It also handles
 * merging image and color palette to generate a single image.
 */
class FileHandler
{
public:
  enum class FileType {
    CT_SCAN,
    COLOR_PALETTE
  };

  FileHandler();

  /**
   * @brief  Opens a file dialog and returns file path of the chosen file
   * @param  filter  filter files to specific needs
   * @return QString file name with absolute path
   */
  QString browseFile(const QString& filter, const FileType fileType);

  /**
   * @brief Returns whether a file has been successfully parsed
   * @return bool true if file was parsed successfully, false otherwise
   */
  bool isFileValid() const;


private:
  QString m_fileName;
  FileType m_fileType;
  bool m_bParsed = false;

  bool _parseCTScan();
  bool _parseColorPalette();
};

#endif // FILEHANDLER_H
