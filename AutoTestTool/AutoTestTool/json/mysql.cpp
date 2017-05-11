#include "mysql.h"

MySql::MySql()
{
    create_sql = "create table student (id int primary key, name varchar(30), age int)";
    select_max_sql = "select max(id) from student";
    insert_sql = "insert into student values (?, ?, ?)";
    update_sql = "update student set name = :name where id = :id";
    select_sql = "select id, name from student";
    select_one_sql = "select from student where id = ?";
    select_all_sql = "select * from student";
    delete_sql = "delete from student where id = ?";
    clear_sql = "delete from student";

}

bool MySql::createSqlite()
{
    database = QSqlDatabase::addDatabase("QSQLITE");  //添加数据库驱动
    database.setDatabaseName("D:/database.db");  //设置数据库名称，同时包括其路径
    database.setUserName("root");  //设置登录用户名
    database.setPassword("123456"); //设置登录密码
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

void MySql::insertData(QString name,int age)
{
    int max_id = serachMaxId();  //先获取最大ID
    //     QSqlQuery sql_query(database);
    sql_query.prepare(insert_sql);
    sql_query.addBindValue(max_id+1);
    sql_query.addBindValue(name);
    sql_query.addBindValue(age);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        qDebug()<<"inserted!";
    }
}

void MySql::updateDatabase()
{
    //     QSqlQuery sql_query(database);
    sql_query.prepare(update_sql);
    sql_query.bindValue(":name", "Qt");
    sql_query.bindValue(":id", 1);
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
            QString name = sql_query.value("name").toString();

            qDebug()<<QString("id:%1    name:%2").arg(id).arg(name);
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
            QString name = sql_query.value(1).toString();
            int age = sql_query.value(2).toInt();

            qDebug()<<QString("id:%1    name:%2    age:%3").arg(id).arg(name).arg(age);
        }
    }
}

bool MySql::searchOneData(int id)
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
            QString name = sql_query.value(1).toString();
            int age = sql_query.value(2).toInt();

            if(num == id)
            {
                qDebug()<<QString("id:%1    name:%2    age:%3").arg(id).arg(name).arg(age);
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

