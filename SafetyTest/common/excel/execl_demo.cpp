/*
 * execl_demo.cpp
 * Execl表操作 参考方法
 * 提供二种操作Execl表类
 * 1、BasicExcel类，
 * 2、QExcel类，代码简短
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "qexcel.h"
#include <BasicExcel.hpp>
using namespace YExcel;

int BasicExcel_demo(void)
{
    BasicExcel e;

    // Load a workbook with one sheet, display its contents and save into another file.
    e.Load("example1.xls");
    BasicExcelWorksheet* sheet1 = e.GetWorksheet("Sheet1");
    if (sheet1)
    {
        size_t maxRows = sheet1->GetTotalRows();
        size_t maxCols = sheet1->GetTotalCols();
        cout << "Dimension of " << sheet1->GetAnsiSheetName() << " (" << maxRows << ", " << maxCols << ")" << endl;

        printf("          ");
        for (size_t c=0; c<maxCols; ++c) printf("%10d", c+1);
        cout << endl;

        for (size_t r=0; r<maxRows; ++r)
        {
            printf("%10d", r+1);
            for (size_t c=0; c<maxCols; ++c)
            {
                BasicExcelCell* cell = sheet1->Cell(r,c);
                switch (cell->Type())
                {
                    case BasicExcelCell::UNDEFINED:
                        printf("          ");
                        break;

                    case BasicExcelCell::INT:
                        printf("%10d", cell->GetInteger());
                        break;

                    case BasicExcelCell::DOUBLE:
                        printf("%10.6lf", cell->GetDouble());
                        break;

                    case BasicExcelCell::STRING:
                        printf("%10s", cell->GetString());
                        break;

                    case BasicExcelCell::WSTRING:
                        wprintf(L"%10s", cell->GetWString());
                        break;
                }
            }
            cout << endl;
        }
    }
    cout << endl;
    e.SaveAs("example2.xls");

    // Create a new workbook with 2 worksheets and write some contents.
    e.New(2);
    e.RenameWorksheet("Sheet1", "Test1");
    BasicExcelWorksheet* sheet = e.GetWorksheet("Test1");
    BasicExcelCell* cell;
    if (sheet)
    {
        for (size_t c=0; c<4; ++c)
        {
            cell = sheet->Cell(0,c);
            cell->Set((int)c);
        }

        cell = sheet->Cell(1,3);
        cell->SetDouble(3.141592654);

        sheet->Cell(1,4)->SetString("Test str1");
        sheet->Cell(2,0)->SetString("Test str2");
        sheet->Cell(2,5)->SetString("Test str1");

        sheet->Cell(4,0)->SetDouble(1.1);
        sheet->Cell(4,1)->SetDouble(2.2);
        sheet->Cell(4,2)->SetDouble(3.3);
        sheet->Cell(4,3)->SetDouble(4.4);
        sheet->Cell(4,4)->SetDouble(5.5);

        sheet->Cell(4,4)->EraseContents();
    }

    sheet = e.AddWorksheet("Test2", 1);
    sheet = e.GetWorksheet(1);
    if (sheet)
    {
        sheet->Cell(1,1)->SetDouble(1.1);
        sheet->Cell(2,2)->SetDouble(2.2);
        sheet->Cell(3,3)->SetDouble(3.3);
        sheet->Cell(4,4)->SetDouble(4.4);
        sheet->Cell(70,2)->SetDouble(5.5);
    }
    e.SaveAs("example3.xls");

    // Load the newly created sheet and display its contents
    e.Load("example3.xls");

    size_t maxSheets = e.GetTotalWorkSheets();
    cout << "Total number of worksheets: " << e.GetTotalWorkSheets() << endl;
    for (size_t i=0; i<maxSheets; ++i)
    {
        BasicExcelWorksheet* sheet = e.GetWorksheet(i);
        if (sheet)
        {
            size_t maxRows = sheet->GetTotalRows();
            size_t maxCols = sheet->GetTotalCols();
            cout << "Dimension of " << sheet->GetAnsiSheetName() << " (" << maxRows << ", " << maxCols << ")" << endl;

            if (maxRows>0)
            {
                printf("          ");
                for (size_t c=0; c<maxCols; ++c) printf("%10d", c+1);
                cout << endl;
            }

            for (size_t r=0; r<maxRows; ++r)
            {
                printf("%10d", r+1);
                for (size_t c=0; c<maxCols; ++c)
                {
                    cout << setw(10) << *(sheet->Cell(r,c));	// Another way of printing a cell content.
                }
                cout << endl;
            }
            if (i==0)
            {
                ofstream f("example4.csv");
                sheet->Print(f, ',', '\"');	// Save the first sheet as a CSV file.
                f.close();
            }
        }
        cout << endl;
    }
    return 0;
}


#if defined(Q_OS_WIN32)
void QExcel_demo(void)
{

    //打开文件，取得工作簿
//    QExcel j("F:/pro/QtExcel/c.xlsx");
//    QExcel j;
//    j.createNewExcel("F:/pro/QtExcel/cc.xlsx");

//    j.insertSheet("表5");
//    j.selectSheet("表5");
//    j.setCellString(1, 1, "addString");

//    j.insertSheet("表4");
//    j.selectSheet("表4");

//    j.insertSheet("表3");
//    j.selectSheet("表3");

//    j.insertSheet("表2");
//    j.selectSheet("表2");

//    j.insertSheet("表1");
//    j.selectSheet("表1");

//    //添加完表后在删除，不能直接删除Excel里面的全部表
//    j.selectSheet("Sheet1");
//    j.deleteSheet("Sheet1");
//    j.selectSheet("Sheet2");
//    j.deleteSheet("Sheet2");
//    j.selectSheet("Sheet3");
//    j.deleteSheet("Sheet3");

//    j.save();
//    j.saveAs();
//    j.close();

    //打开文件，取得工作簿
    QExcel j("E:/test.xls");
    //取得工作表数量
    //qDebug()<<"SheetCount"<<j.getSheetsCount();
    //激活一张工作表
    //j.selectSheet(1);
    //j.selectSheet("JSheet2");
    //取得工作表名称
    //j.selectSheet(1);
    //j.getSheetName();
    //qDebug()<<"SheetName 1"<<j.getSheetName(1);
    //取得工作表已使用范围
    //int topLeftRow, topLeftColumn, bottomRightRow, bottomRightColumn;
    //j.getUsedRange(&topLeftRow, &topLeftColumn, &bottomRightRow, &bottomRightColumn);
    //读值
    //j.getCellValue(2, 2).toString();
    //删除工作表
    //j.selectSheet("Sheet1");
    //j.selectSheet(1);
    //j.deleteSheet();
    //j.save();
    //插入数据
    j.selectSheet("Sheet1");
    j.setCellString(1, 7, "addString");
    j.setCellString("A3", "abc");
    j.save();
    //合并单元格
    //j.selectSheet(2);
    //j.mergeCells("G1:H2");
    //j.mergeCells(4, 7, 5 ,8);
    //j.save();
    //设置列宽
    //j.selectSheet(1);
    //j.setColumnWidth(1, 20);
    //j.save();
    //设置粗体
    //j.selectSheet(1);
    //j.setCellFontBold(2, 2, true);
    //j.setCellFontBold("A2", true);
    //j.save();
    //设置文字大小
    //j.selectSheet(1);
    //j.setCellFontSize("B3", 20);
    //j.setCellFontSize(1, 2, 20);
    //j.save();
    //设置单元格文字居中
    //j.selectSheet(2);
    //j.setCellTextCenter(1, 2);
    //j.setCellTextCenter("A2");
    //j.save();
    //设置单元格文字自动折行
    //j.selectSheet(1);
    //j.setCellTextWrap(2,2,true);
    //j.setCellTextWrap("A2", true);
    //j.save();
    //设置一行自适应行高
    //j.selectSheet(1);
    //j.setAutoFitRow(2);
    //j.save();
    //新建工作表
    //j.insertSheet("abc");
    //j.save();
    //清除单元格内容
    //j.selectSheet(4);
    //j.clearCell(1,1);
    //j.clearCell("A2");
    //j.save();
    //合并一列中相同连续的单元格
    //j.selectSheet(1);
    //j.mergeSerialSameCellsInColumn(1, 2);
    //j.save();
    //获取一张工作表已用行数
    //j.selectSheet(1);
    //qDebug()<<j.getUsedRowsCount();
    //设置行高
    //j.selectSheet(1);
    //j.setRowHeight(2, 30);
    //j.save();

    j.close();
//    qDebug()<<"App End";
}
#endif
