#include <QFile>
#include <QStringList>
#include <QDebug>

#if defined(Q_OS_WIN32)
#include <QAxObject>
#include "qexcel.h"

QExcel::QExcel(QString xlsFilePath, QObject *parent)
{
    excel = 0;
    workBooks = 0;
    workBook = 0;
    sheets = 0;
    sheet = 0;

    excel = new QAxObject("Excel.Application", parent);
    workBooks = excel->querySubObject("Workbooks");
    QFile file(xlsFilePath);
    if (file.exists())
    {
        workBooks->dynamicCall("Open(const QString&)", xlsFilePath);
        workBook = excel->querySubObject("ActiveWorkBook");
        sheets = workBook->querySubObject("WorkSheets");
    }
}

QExcel::QExcel(QObject *parent)
{
    excel = 0;
    workBooks = 0;
    workBook = 0;
    sheets = 0;
    sheet = 0;
}

QExcel::~QExcel()
{
    close();
}

void QExcel::close()
{
    excel->dynamicCall("Quit()");

    delete sheet;
    delete sheets;
    delete workBook;
    delete workBooks;
    delete excel;

    excel = 0;
    workBooks = 0;
    workBook = 0;
    sheets = 0;
    sheet = 0;
}

QAxObject *QExcel::getWorkBooks()
{
    return workBooks;
}

QAxObject *QExcel::getWorkBook()
{
    return workBook;
}

QAxObject *QExcel::getWorkSheets()
{
    return sheets;
}

QAxObject *QExcel::getWorkSheet()
{
    return sheet;
}

void QExcel::selectSheet(const QString& sheetName)
{
    sheet = sheets->querySubObject("Item(const QString&)", sheetName);
}

void QExcel::deleteSheet(const QString& sheetName)
{
    QAxObject * a = sheets->querySubObject("Item(const QString&)", sheetName);
    a->dynamicCall("delete");
}

void QExcel::deleteSheet(int sheetIndex)
{
    QAxObject * a = sheets->querySubObject("Item(int)", sheetIndex);
    a->dynamicCall("delete");
}

void QExcel::selectSheet(int sheetIndex)
{
    sheet = sheets->querySubObject("Item(int)", sheetIndex);
}

void QExcel::setCellString(int row, int column, const QString& value)
{
    QAxObject *range = sheet->querySubObject("Cells(int,int)", row, column);
    range->dynamicCall("SetValue(const QString&)", value);
}

void QExcel::setCellFontBold(int row, int column, bool isBold)
{
    QString cell;
    cell.append(QChar(column - 1 + 'A'));
    cell.append(QString::number(row));

    QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
    range = range->querySubObject("Font");
    range->setProperty("Bold", isBold);
}

void QExcel::setCellFontSize(int row, int column, int size)
{
    QString cell;
    cell.append(QChar(column - 1 + 'A'));
    cell.append(QString::number(row));

    QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
    range = range->querySubObject("Font");
    range->setProperty("Size", size);
}

void QExcel::mergeCells(const QString& cell)
{
    QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
    range->setProperty("VerticalAlignment", -4108);//xlCenter
    range->setProperty("WrapText", true);
    range->setProperty("MergeCells", true);
}

void QExcel::mergeCells(int topLeftRow, int topLeftColumn, int bottomRightRow, int bottomRightColumn)
{
    QString cell;
    cell.append(QChar(topLeftColumn - 1 + 'A'));
    cell.append(QString::number(topLeftRow));
    cell.append(":");
    cell.append(QChar(bottomRightColumn - 1 + 'A'));
    cell.append(QString::number(bottomRightRow));

    QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
    range->setProperty("VerticalAlignment", -4108);//xlCenter
    range->setProperty("WrapText", true);
    range->setProperty("MergeCells", true);
}

QVariant QExcel::getCellValue(int row, int column)
{
    QAxObject *range = sheet->querySubObject("Cells(int,int)", row, column);
    return range->property("Value");
}

void QExcel::save()
{
    workBook->dynamicCall("Save()");
}

int QExcel::getSheetsCount()
{
    return sheets->property("Count").toInt();
}

QString QExcel::getSheetName()
{
    return sheet->property("Name").toString();
}

QString QExcel::getSheetName(int sheetIndex)
{
    QAxObject * a = sheets->querySubObject("Item(int)", sheetIndex);
    return a->property("Name").toString();
}

bool QExcel::createNewExcel(const QString& filePath)
{
    m_saveName = filePath;
    QFile file(m_saveName);
    if(file.exists())
    {       
        qDebug() << "错误：目标文件已存在!";
        return false;
    }

    if(!m_saveName.isEmpty())
    {
//        if(!m_word->setControl("Word.Application"))
//        {
//            m_strError += "错误：获取word组件失败，请确定是否安装了word!\n";
//            return false;
//        }
//        m_word->setProperty("Visible",false);
//        m_word->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示
//        m_documents = m_word->querySubObject("Documents");
//        if(!m_documents)
//        {
//            m_strError += "获取文档失败！\n";
//            return false;
//        }
//        m_documents->dynamicCall("Add (void)");
//        m_document = m_word->querySubObject("ActiveDocument");//获取当前激活的文档
//        return true;


//        qDebug() << "new file";
        excel = new QAxObject("Excel.Application", this);        
        workBooks = excel->querySubObject("Workbooks");
        workBooks->dynamicCall("Add (void)");
        workBook = excel->querySubObject("ActiveWorkBook");
        sheets = workBook->querySubObject("WorkSheets");

        return true;
    }
    else
    {
        qDebug() << "错误：文件名为空";
        return false;
    }
}

void QExcel::saveAs()
{
    if(workBook)
        workBook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(m_saveName));
    else
        return;
}

void QExcel::getUsedRange(int *topLeftRow, int *topLeftColumn, int *bottomRightRow, int *bottomRightColumn)
{
    QAxObject *usedRange = sheet->querySubObject("UsedRange");
    *topLeftRow = usedRange->property("Row").toInt();
    *topLeftColumn = usedRange->property("Column").toInt();

    QAxObject *rows = usedRange->querySubObject("Rows");
    *bottomRightRow = *topLeftRow + rows->property("Count").toInt() - 1;

    QAxObject *columns = usedRange->querySubObject("Columns");
    *bottomRightColumn = *topLeftColumn + columns->property("Count").toInt() - 1;
}

void QExcel::setColumnWidth(int column, int width)
{
    QString columnName;
    columnName.append(QChar(column - 1 + 'A'));
    columnName.append(":");
    columnName.append(QChar(column - 1 + 'A'));

    QAxObject * col = sheet->querySubObject("Columns(const QString&)", columnName);
    col->setProperty("ColumnWidth", width);
}

void QExcel::setCellTextCenter(int row, int column)
{
    QString cell;
    cell.append(QChar(column - 1 + 'A'));
    cell.append(QString::number(row));

    QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
    range->setProperty("HorizontalAlignment", -4108);//xlCenter
}

void QExcel::setCellTextWrap(int row, int column, bool isWrap)
{
    QString cell;
    cell.append(QChar(column - 1 + 'A'));
    cell.append(QString::number(row));

    QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
    range->setProperty("WrapText", isWrap);
}

void QExcel::setAutoFitRow(int row)
{
    QString rowsName;
    rowsName.append(QString::number(row));
    rowsName.append(":");
    rowsName.append(QString::number(row));

    QAxObject * rows = sheet->querySubObject("Rows(const QString &)", rowsName);
    rows->dynamicCall("AutoFit()");
}

void QExcel::insertSheet(QString sheetName)
{
    sheets->querySubObject("Add()");
    QAxObject * a = sheets->querySubObject("Item(int)", 1);
    a->setProperty("Name", sheetName);
}

void QExcel::mergeSerialSameCellsInAColumn(int column, int topRow)
{
    int a,b,c,rowsCount;
    getUsedRange(&a, &b, &rowsCount, &c);

    int aMergeStart = topRow, aMergeEnd = topRow + 1;

    QString value;
    while(aMergeEnd <= rowsCount)
    {
        value = getCellValue(aMergeStart, column).toString();
        while(value == getCellValue(aMergeEnd, column).toString())
        {
            clearCell(aMergeEnd, column);
            aMergeEnd++;
        }
        aMergeEnd--;
        mergeCells(aMergeStart, column, aMergeEnd, column);

        aMergeStart = aMergeEnd + 1;
        aMergeEnd = aMergeStart + 1;
    }
}

void QExcel::clearCell(int row, int column)
{
    QString cell;
    cell.append(QChar(column - 1 + 'A'));
    cell.append(QString::number(row));

    QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
    range->dynamicCall("ClearContents()");
}

void QExcel::clearCell(const QString& cell)
{
    QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
    range->dynamicCall("ClearContents()");
}

int QExcel::getUsedRowsCount()
{
    QAxObject *usedRange = sheet->querySubObject("UsedRange");
    int topRow = usedRange->property("Row").toInt();
    QAxObject *rows = usedRange->querySubObject("Rows");
    int bottomRow = topRow + rows->property("Count").toInt() - 1;
    return bottomRow;
}

void QExcel::setCellString(const QString& cell, const QString& value)
{
    QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
    range->dynamicCall("SetValue(const QString&)", value);
}

void QExcel::setCellFontSize(const QString &cell, int size)
{
    QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
    range = range->querySubObject("Font");
    range->setProperty("Size", size);
}

void QExcel::setCellTextCenter(const QString &cell)
{
    QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
    range->setProperty("HorizontalAlignment", -4108);//xlCenter
}

void QExcel::setCellFontBold(const QString &cell, bool isBold)
{
    QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
    range = range->querySubObject("Font");
    range->setProperty("Bold", isBold);
}

void QExcel::setCellTextWrap(const QString &cell, bool isWrap)
{
    QAxObject *range = sheet->querySubObject("Range(const QString&)", cell);
    range->setProperty("WrapText", isWrap);
}

void QExcel::setRowHeight(int row, int height)
{
    QString rowsName;
    rowsName.append(QString::number(row));
    rowsName.append(":");
    rowsName.append(QString::number(row));

    QAxObject * r = sheet->querySubObject("Rows(const QString &)", rowsName);
    r->setProperty("RowHeight", height);
}
#endif

