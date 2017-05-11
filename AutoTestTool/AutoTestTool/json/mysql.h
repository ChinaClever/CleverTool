#ifndef MYSQL_H
#define MYSQL_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QFile>
class MySql
{
public:
    MySql();

    bool createSqlite();
    void openDataBase();
    bool createTable();
    int serachMaxId();
    void insertData(QString name, int age);
    void updateDatabase();
    void searchDatabase();
    void searchAllDatabase();
    bool searchOneData(int id);
    void delateData(int id);
    void clearTable();
    void closeDatabase();
    void removeDatabase();

private:
    QSqlDatabase database;

    QString create_sql ;
    QString select_max_sql ;
    QString insert_sql ;
    QString update_sql ;
    QString select_sql ;
    QString select_one_sql;
    QString select_all_sql ;
    QString delete_sql ;
    QString clear_sql ;

    QSqlQuery sql_query;

//    int max_id;

};

#endif // MYSQL_H
