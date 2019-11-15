#ifndef SQLITEHELPER_H
#define SQLITEHELPER_H

#include <QObject>
#include <QDebug>

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>

#include <QMutex>

struct data_struct {
    int id;                    // id
    QList<QString> item_info;  // 数据信息
};

class SQLiteHelper
{
public:
    SQLiteHelper();

    void initDB(QString db_name = "DataCollection.db");   // 初始化数据库

    bool openDB(QString db_name, QString username="root", QString pwd="123456");   // 打开数据库

    QList<QString> getColumnNamesFromTable(QString table_name);        // 获取表的所有字段名
    int getMaxID(QString table_name);                                  // 获取最大 ID
    bool insertItemInfo(QString table_name, QList<QString> item_info); // 插入信息
    bool updateItemInfo(QString table_name, data_struct item);         // 更新信息
    bool deleteItemsByIDs(QString table_name, QList<int> ids);         // 删除信息

    bool isExit(QString table_name, QString key, QString value);       // 判断表中是否存在该字段

    int getInfoNumber(QString table_name, QString condation="");       // 获取表中信息数目
    QList<data_struct> getAllInfos(QString table_name,
                                   QString condation="");              // 获取所有的信息
    // 获取一定范围内的信息
    QList<data_struct> getInfosByRange(QString table_name, int start,
                                       int end, QString condation="");

    // 从列名 col_name 获取该列的所有数据
    QList<QString> getNamesInfo(QString table_name, QString col_name, QString condation="");

    // 获取附件列表
    QList<QString> getAttachment(QString table_name);

public:
    QList<QString> getTableNames() {return this->table_names;}


private:
    // 创建表
    bool createTable(QSqlDatabase m_database, QString table_name, QString create_table_sql);
    bool deleteTable(QSqlDatabase m_database, QString table_name);
    bool openDB();                // 打开数据库
    void closeDB();               // 关闭数据库

private:

    QSqlDatabase m_database;      // 数据库实例
    QMutex m_lockdb;              // 锁

private:
    QMap<QString, QString> create_table_sqls;  // 表名对应的创建表的 SQL 语句
    QList<QString> table_names;                // 所有的表名集合

};

#endif // SQLITEHELPER_H
