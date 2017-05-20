#include "mysql.h"

MySql::MySql()
{
    create_sql = "create table if not exists deviceinfo(id int primary key, ip varchar(30), time TEXT )";
    select_max_sql = "select max(id) from deviceinfo";
    insert_sql = "insert into deviceinfo values (?, ?, ?)";
    update_sql = "update deviceinfo set ip = :ip where id = :id"; //根据id来修改ip
    select_sql = "select id, ip from deviceinfo";
//    select_one_sql = "select from student where id = ?";
    select_all_sql = "select * from deviceinfo";
    delete_sql = "delete from deviceinfo where id = ?";  //根据id删除，当然也可以根据ip或者时间删除
    clear_sql = "delete from deviceinfo";

}

bool MySql::createSqlite()
{
//    database = QSqlDatabase::addDatabase("QSQLITE");  //添加数据库驱动
    database = QSqlDatabase::addDatabase("QMYSQL");  //添加数据库驱动
//    database.setDatabaseName("D:/database.db");  //设置数据库名称，同时包括其路径
    database.setDatabaseName("MySQL");  //设置数据库名称，同时包括其路径
    database.setUserName("root");  //设置登录用户名
    database.setPassword("557326"); //设置登录密码
    sql_query = QSqlQuery(database);

}

void MySql::openDataBase()
{
    if(database.open())
    {
        qDebug() << "数据库打开成功！";
    }else
        qDebug()<<"";
    //        qDebug() << database.lastError();
}

bool MySql::createTable()
{
#if 0
    QSqlQuery sql_query;
    sql_query.prepare(create_sql);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
        return false;
    }
    else
    {
        qDebug()<<"table created!";
    }

    return true;
#else
    //    QSqlQuery sql_query(database);

    bool success = sql_query.exec(create_sql);
    if(!success)
        return false;

    return true;
#endif
}

int MySql::serachMaxId()
{
    int max_id;
    //     QSqlQuery sql_query(database);
    sql_query.prepare(select_max_sql);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        while(sql_query.next())
        {
            max_id = sql_query.value(0).toInt();
            qDebug()<<QString("max id:%1").arg(max_id);
        }
    }

    return max_id;
}

void MySql::insertData(QString ip,QString time)
{
    int max_id = serachMaxId();  //先获取最大ID
    //     QSqlQuery sql_query(database);
    sql_query.prepare(insert_sql);
    sql_query.addBindValue(max_id+1);
    sql_query.addBindValue(ip);
    sql_query.addBindValue(time);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        qDebug()<<"inserted!";
    }
}

void MySql::updateDatabase(QString ip ,int id)  //把某一项修改成指定ip
{
    //     QSqlQuery sql_query(database);
    sql_query.prepare(update_sql);
    sql_query.bindValue(":ip", "ip");
    sql_query.bindValue(":id", id);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        qDebug()<<"updated!";
    }
}

void MySql::searchDatabase()
{
    //     QSqlQuery sql_query(database);
    if(!sql_query.exec(select_sql))
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        while(sql_query.next())
        {
            int id = sql_query.value("id").toInt();
            QString ip = sql_query.value("ip").toString();

            qDebug()<<QString("id:%1    ip:%2").arg(id).arg(ip);
        }
    }

}

void MySql::searchAllDatabase()
{
    //     QSqlQuery sql_query(database);
    sql_query.prepare(select_all_sql);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        while(sql_query.next())
        {
            int id = sql_query.value(0).toInt();
            QString ip = sql_query.value(1).toString();
            QString time = sql_query.value(2).toString();

            qDebug()<<QString("id:%1    ip:%2    time:%3").arg(id).arg(ip).arg(time);
        }
    }
}

bool MySql::searchOneData(int id) //根据id查询，可换成根据IP查询或者时间查询
{
    sql_query.prepare(select_all_sql);
    sql_query.bindValue(":id", id);
    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
    }else
    {
        while(sql_query.next())
        {
            int num = sql_query.value(0).toInt();
            QString ip = sql_query.value(1).toString();
            QString time = sql_query.value(2).toString();

            if(num == id)
            {
                qDebug()<<QString("id:%1    ip:%2    time:%3").arg(id).arg(ip).arg(time);
                return true;
            }
        }
    }

    return false;
}

void MySql::delateData(int id)
{
    int max_id = serachMaxId();
    //     QSqlQuery sql_query(database);
    sql_query.prepare(delete_sql);
    sql_query.addBindValue(id);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        qDebug()<<"deleted!";
    }
}

void MySql::clearTable()
{
    //     QSqlQuery sql_query(database);
    sql_query.prepare(clear_sql);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        qDebug()<<"cleared";
    }
}


void MySql::closeDatabase()
{
    database.close();
}

void MySql::removeDatabase()
{
    QFile::remove("database.db");
}

