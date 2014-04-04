#ifndef CSVREADER_H
#define CSVREADER_H

#include <QFile>
#include <QStringList>

class CsvFile : public QFile
{
    private:
        QStringList firstRow, currentRow;
        QFile file;

    public:
        CsvFile(const QString& name = "");

        void setFileName(const QString &name) { file.setFileName(name); }
        bool openForRead();
        const QStringList& headerLabels();
        bool readRow();
        const QString& header(int column) { return headerLabels()[column]; }
        const QString& row(int column)    { return currentRow[column]; }
        int columnsCount()                { return currentRow.count(); }
        void close();

};

#endif // CSVREADER_H
