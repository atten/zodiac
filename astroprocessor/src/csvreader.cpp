#include "csvreader.h"

CsvFile :: CsvFile(const QString& name)
 {
  file.setFileName(name);
 }

bool CsvFile :: openForRead()
 {
  bool ret = file.open(QIODevice::ReadOnly);
  headerLabels();
  return ret;
 }

const QStringList& CsvFile :: headerLabels()
 {
  if (!firstRow.count())
   {
    readRow();
    firstRow = currentRow;
   }

  return firstRow;
 }

bool CsvFile :: readRow()
 {
  if (!file.isOpen()) openForRead();
  while (1)
   {
    if (file.atEnd())
     {
      currentRow.clear();
      return false;
     }
    QString str = file.readLine().trimmed();
    currentRow = str.split(';');
    if (!str.isEmpty()) return true;
   }
 }

void CsvFile :: close()
 {
  file.close();
  firstRow.clear();
  currentRow.clear();
 }
