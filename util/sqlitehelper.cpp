#include "sqlitehelper.h"

SQLiteHelper::SQLiteHelper()
{
    table_names.append("firm_info");
    table_names.append("equipment_info");
    table_names.append("component_info");
    table_names.append("sparepart_info");
    table_names.append("tool_info");
    table_names.append("technical_data");
    table_names.append("repair_info");

    QString create_firm_info_sql = "create table firm_info (id int primary key,"
                                   " firm_name varchar(30) NOT NULL unique, "  // 厂商名称
            "country varchar(30) NOT NULL, "      // 国家
            "address varchar(50), "               // 地址
            "tax_code varchar(20), "              // 税务登记号
            "contact varchar(10), "               // 联系人
            "telephone varchar(11), "             // 联系电话
            "email varchar(20), "                 // 电子邮件
            "qq_msn varchar(20), "                // QQ/MSN
            "firm_qualification varchar(100), "   // 厂商资质
            "business_scope varchar(100), "       // 经营范围
            "post_code varchar(6), "              // 邮政编码
            "fax varchar(10), "                   // 传真
            "notes varchar(300) )";               // 备注


    QString create_equipment_info_sql = "create table equipment_info (id int primary key,"
                                        "device_name varchar(30) NOT NULL unique, " // 设备名称
            "firm_name varchar(30), "    // 厂商名称
            "device_type varchar(30) NOT NULL, "  // 设备类型
            "model_type varchar(50), "            // 型号/标准号
            "mtbf varchar(10), "                  // MTBF(h)
            "mttr varchar(10), "                  // MTTR(h)
            "weight varchar(10), "                // 重量(Kg)
            "is_life varchar(2), "                // 是否寿命件
            "life_time varchar(10), "             // 寿命期
            "lift_unit varchar(8), "              // 寿命单位
            "length varchar(10), "                // 尺寸/长(cm)
            "width varchar(10), "                 // 尺寸/宽(cm)
            "height varchar(10), "                // 尺寸/高(cm)
            "install_number varchar(10), "        // 装机数量
            "install_unit varchar(10), "          // 装机计量单位
            "parameter1 varchar(10), "            // 参数1
            "parameter2 varchar(10), "            // 参数2
            "parameter3 varchar(10), "            // 参数3
            "parameter4 varchar(10), "            // 参数4
            "parameter5 varchar(10), "            // 参数5
            "parameter6 varchar(10), "            // 参数6
            "keep_requirement varchar(50), "      // 保养要求
            "attachment varchar(100), "            // 附件
            "notes varchar(300), "                 // 备注
            "foreign key(firm_name) references firm_info(firm_name) )";

    QString create_component_info_sql = "create table component_info (id int primary key, "
                                        "component_name varchar(30) NOT NULL unique, "  // 部件名称
            "device_name varchar(30), "  // 设备名称
            "model_type varchar(50), "        // 型号/标准号
            "mtbf varchar(10), "              // MTBF(h)
            "mttr varchar(10), "              // MTTR(h)
            "weight varchar(10), "            // 重量(Kg)
            "is_life varchar(2), "            // 是否寿命件
            "life_time varchar(10), "         // 寿命期
            "lift_unit varchar(8), "          // 寿命单位
            "is_spare_part varchar(2), "      // 是否备件
            "is_standard_part varchar(2), "   // 是否标准件
            "is_repairable varchar(2), "      // 可修复
            "length varchar(10), "            // 尺寸/长(cm)
            "width varchar(10), "             // 尺寸/宽(cm)
            "height varchar(10), "            // 尺寸/高(cm)
            "per_device_number varchar(10), " // 数量每设备
            "component_unit varchar(8), "     // 计量单位
            "price varchar(10), "             // 单价
            "parameter1 varchar(10), "        // 主参数1
            "parameter2 varchar(10), "        // 主参数2
            "provider varchar(50), "          // 供应商
            "attachment varchar(100), "        // 附件
            "notes varchar(300), "            // 备注
            "foreign key(device_name) references equipment_info(device_name) )";

    QString create_sparepart_info_sql = "create table sparepart_info (id int primary key, "
                                        "sparepart_name varchar(30) NOT NULL, "  // 零配件名称
            "device_name varchar(30), "  // 设备名称
            "model_type varchar(50), "        // 型号/标准号
            "mtbf varchar(10), "              // MTBF(h)
            "mttr varchar(10), "              // MTTR(h)
            "weight varchar(10), "            // 重量(Kg)
            "is_life varchar(2), "            // 是否寿命件
            "life_time varchar(10), "         // 寿命期
            "lift_unit varchar(8), "          // 寿命单位
            "is_spare_part varchar(2), "      // 是否备件
            "is_standard_part varchar(2), "   // 是否标准件
            "is_rapid_wear varchar(2), "      // 是否易损件
            "is_repairable varchar(2), "      // 可修复
            "length varchar(10), "            // 尺寸/长(cm)
            "width varchar(10), "             // 尺寸/宽(cm)
            "height varchar(10), "            // 尺寸/高(cm)
            "sandard_equipment varchar(10), " // 随艇配备标准
            "per_device_number varchar(10), " // 数量每设备
            "sparepart_unit varchar(8), "     // 计量单位
            "price varchar(10), "             // 单价
            "parameter1 varchar(10), "        // 主参数1
            "parameter2 varchar(10), "        // 主参数2
            "provider varchar(50), "          // 供应商
            "attachment varchar(100), "        // 附件
            "notes varchar(300), "            // 备注
            "foreign key(device_name) references equipment_info(device_name) )";


    QString create_tool_info_sql = "create table tool_info (id int primary key, "
                                   "object_name varchar(30) NOT NULL, "  // 物品名称
            "object_type varchar(30), "           // 物品类型
            "unit varchar(8), "                   // 计量单位
            "number varchar(10), "                // 数量
            "price varchar(10), "                 // 单价(元)
            "provider varchar(50), "              // 供应商
            "parameter1 varchar(10), "            // 主参数1
            "parameter2 varchar(10), "            // 主参数2
            "parameter3 varchar(10), "            // 主参数2
            "notes varchar(300) )";               // 备注

    QString create_technical_data_sql = "create table technical_data (id int primary key, "
                                        "information_id varchar(10) NOT NULL, "  // 原始资料编号
            "information_name varchar(30), "           // 资料名称
            "information_type varchar(10), "           // 资料类型
            "per_file_pages varchar(10), "             // 每份文档总页数
            "info_belong varchar(10), "                // 资料所属
            "belong_parts varchar(30), "               // 所属设备/部件/零配件
            "similar_file_number varchar(10), "        // 同种文档数量
            "drawing_size varchar(10), "               // 图纸规格
            "publication_date varchar(10), "           // 出版日期
            "provider varchar(50), "                   // 出版商
            "storage_medium varchar(50), "             // 存储介质
            "unit varchar(8), "                        // 单位
            "attachment varchar(100), "                // 附件
            "notes varchar(300) )";                    // 备注

    QString create_repair_info_sql = "create table repair_info (id int primary key, "
                                     "repair_level varchar(10) NOT NULL, "  // 维修级别
            "device_name varchar(30), "              // 设备名称
            "component_name varchar(10), "           // 部件
            "repair_item_name varchar(30), "         // 维修项名称
            "time_consum varchar(10), "              // 消耗时间
            "time_unit varchar(10), "                // 时间单位
            "repair_big_class varchar(10), "         // 维修大类
            "repair_samll_class varchar(10), "       // 维修小类
            "fault_analysis varchar(50), "           // 故障分析
            "attachment varchar(100), "               // 附件
            "safety_precaution varchar(50), "        // 安全注意事项
            "repair_procedure varchar(8), "          // 维修工作步骤
            "notes varchar(300), "                   // 备注
            "foreign key(device_name) references equipment_info(device_name), "
            "foreign key(component_name) references component_info(component_name) )";

    create_table_sqls.insert(table_names.at(0), create_firm_info_sql);
    create_table_sqls.insert(table_names.at(1), create_equipment_info_sql);
    create_table_sqls.insert(table_names.at(2), create_component_info_sql);
    create_table_sqls.insert(table_names.at(3), create_sparepart_info_sql);
    create_table_sqls.insert(table_names.at(4), create_tool_info_sql);
    create_table_sqls.insert(table_names.at(5), create_technical_data_sql);
    create_table_sqls.insert(table_names.at(6), create_repair_info_sql);

    //    qDebug() << create_table_sqls;

    initDB();
}

void SQLiteHelper::initDB(QString db_name)
{
    qDebug() << "in initDB() ...";
    QMutexLocker lock(&m_lockdb);

    // "mysqlit_data_collection_connection" 这个名字任意取，判断指定连接名称的连接是否已经存在，如果存在则返回true
    if(QSqlDatabase::contains("mysqlit_data_collection_connection")) {
        m_database = QSqlDatabase::database("mysqlit_data_collection_connection");
    } else {
        m_database = QSqlDatabase::addDatabase("QSQLITE","mysqlit_data_collection_connection");
    }
    m_database.setDatabaseName(db_name);
    m_database.setUserName("root");
    m_database.setPassword("123456");
    if(!openDB())
        return;

//    deleteTable(m_database, table_names.at(5));
    createTable(m_database, table_names.at(0), create_table_sqls.value(table_names.at(0)));
    createTable(m_database, table_names.at(1), create_table_sqls.value(table_names.at(1)));
    createTable(m_database, table_names.at(2), create_table_sqls.value(table_names.at(2)));
    createTable(m_database, table_names.at(3), create_table_sqls.value(table_names.at(3)));
    createTable(m_database, table_names.at(4), create_table_sqls.value(table_names.at(4)));
    createTable(m_database, table_names.at(5), create_table_sqls.value(table_names.at(5)));
    createTable(m_database, table_names.at(6), create_table_sqls.value(table_names.at(6)));


    closeDB();
    lock.unlock();

}

bool SQLiteHelper::openDB(QString db_name, QString username, QString pwd)
{
    qDebug() << "in openDB() ...";
    QMutexLocker lock(&m_lockdb);

    // "new_data_collection_connection" 这个名字任意取，判断指定连接名称的连接是否已经存在，如果存在则返回true
    if(QSqlDatabase::contains("new_data_collection_connection")) {
        m_database = QSqlDatabase::database("mysqlit_data_collection_connection");
    } else {
        m_database = QSqlDatabase::addDatabase("QSQLITE","new_data_collection_connection");
    }
    m_database.setDatabaseName(db_name);
    m_database.setUserName(username);
    m_database.setPassword(pwd);

    if(!openDB())
        return false;

    // 更新数据库中的第五张表
//    deleteTable(m_database, table_names.at(5));
//    createTable(m_database, table_names.at(5), create_table_sqls.value(table_names.at(5)));

    closeDB();
    lock.unlock();
    return true;
}

// 获取表的所有字段名
QList<QString> SQLiteHelper::getColumnNamesFromTable(QString table_name)
{
    QList<QString> columnNames;
    QMutexLocker lock(&m_lockdb);
    if (!openDB())
        return columnNames;

    QSqlQuery query(m_database);
    QString strTableNmae = table_name;
    QString str = "PRAGMA table_info(" + table_name + ")";

    query.prepare(str);
    if (query.exec()) {
        while (query.next()) {
            columnNames.append(query.value(1).toString());
        }
    }
    else {
        qDebug() << "Error : " << query.lastError();
        return columnNames;
    }
    closeDB();
    lock.unlock();
    return columnNames;
}

int SQLiteHelper::getMaxID(QString table_name)
{
    QMutexLocker lock(&m_lockdb);
    if (!openDB())
        return 0;

    int max_id = 0;

    QSqlQuery sql_query(m_database);
    QString select_max_id = "select max(id) from " + table_name;

    sql_query.prepare(select_max_id);
    if (!sql_query.exec()) {
        qDebug() << sql_query.lastError();
    } else {
        while(sql_query.next()) {
            max_id = sql_query.value(0).toInt();
//            qDebug()<<QString("max id:%1").arg(max_id);
        }
    }

    closeDB();
    lock.unlock();
    return max_id;
}

bool SQLiteHelper::insertItemInfo(QString table_name, QList<QString> item_info)
{
    int max_id = getMaxID(table_name) + 1;

    QMutexLocker lock(&m_lockdb);
    if (!openDB())
        return false;

    bool flag = false;
    QSqlQuery sql_query(m_database);

    QString foreign_sql = "PRAGMA foreign_keys = ON;";
    sql_query.prepare(foreign_sql);
    if(!sql_query.exec()) {
        qDebug() << "Error : foreign_sql faile : " << sql_query.lastError();
    }
//    qDebug() << item_info;

    QString insert_sql = "insert into " + table_name + " values ( ?";

    for(int i = 0; i < item_info.size(); i++) {
        insert_sql += ", ?";
    }
    insert_sql += ")";

//    qDebug() << "insert_sql : " << insert_sql;

    sql_query.prepare(insert_sql);
    sql_query.addBindValue(max_id);

    foreach(QString item, item_info) {
        sql_query.addBindValue(item);
    }

    if (!sql_query.exec()) {
        qDebug() << "Error : insert faile : " << sql_query.lastError();
    } else {
        flag = true;
        //        qDebug() << "insert success ...";
    }
    closeDB();
    return flag;
}

bool SQLiteHelper::updateItemInfo(QString table_name, data_struct item)
{
    QList<QString> columnNames = getColumnNamesFromTable(table_name);
    if (columnNames.size() == 0) return false;

//    qDebug() << "update : columnNames.size() : " << columnNames.size()
//             << ", item.item_info.size() : " << item.item_info.size();

    QMutexLocker lock(&m_lockdb);
    if (!openDB())
        return false;
    bool flag = false;
    QSqlQuery sql_query(m_database);

    QString update_sql = "update " + table_name + " set ";

    // 根据字段动态构建SQL语句
    QString name;
    if(columnNames.size() >= 2) {
        update_sql = update_sql + columnNames.at(1) + " = :" + columnNames.at(1);
    }
    for(int i = 2; i < columnNames.size(); i++) {
        name = columnNames.at(i);
        update_sql = update_sql + " ," + name + " = :" + name;
    }
    update_sql = update_sql + " where id = :id";

    //    qDebug() << "update_sql : " << update_sql;

    sql_query.prepare(update_sql);

    for(int i = 1; i < columnNames.size(); i++) {
        name = columnNames.at(i);
        sql_query.bindValue(":"+name, item.item_info.at(i-1));
    }
    sql_query.bindValue(":id",item.id);

    if (!sql_query.exec()) {
        qDebug() << "Error : update faile..." << sql_query.lastError();
    } else {
        flag = true;
        //        qDebug() << "update success ...";
    }
    closeDB();
    return flag;
}

bool SQLiteHelper::deleteItemsByIDs(QString table_name, QList<int> ids)
{
    int max_id = getMaxID(table_name) + 1;

    QMutexLocker lock(&m_lockdb);
    if (!openDB())
        return false;

    bool flag = true;
    QSqlQuery sql_query(m_database);

    QString delete_sql = "delete from " + table_name + " where id=";

    foreach(int id, ids) {
        //        qDebug() << "delete_sql : " << delete_sql + QString::number(id);

        sql_query.prepare(delete_sql + QString::number(id));

        if (!sql_query.exec()) {
            flag = false;
            qDebug() << "Error : delete faile : " << sql_query.lastError();
        } else {
            //            qDebug() << "delete success ...";
        }
    }

    closeDB();
    return flag;
}

bool SQLiteHelper::isExit(QString table_name, QString key, QString value)
{
    int count = 0;
    QMutexLocker lock(&m_lockdb);
    if (!openDB())
        return count!=0;

    QSqlQuery sql_query(m_database);

    QString count_sql = "select count(*) from " + table_name +
            " where " + key + "='" + value + "'";
//    qDebug() << count_sql;
    sql_query.prepare(count_sql);

    if (!sql_query.exec()) {
        qDebug() << "Error : " << sql_query.lastError();
    } else {
        sql_query.next();
        count = sql_query.value(0).toInt();
    }
    closeDB();
    return count!=0;
}

int SQLiteHelper::getInfoNumber(QString table_name, QString condation)
{
    int count = 0;
    QMutexLocker lock(&m_lockdb);
    if (!openDB())
        return count;

    QSqlQuery sql_query(m_database);

    QString count_sql = "select count(*) from " + table_name;
    if(condation != "") {
        count_sql += " where " + condation;
    }
//    qDebug() << count_sql;
    sql_query.prepare(count_sql);

    if (!sql_query.exec()) {
        qDebug() << "Error : " << sql_query.lastError();
    } else {
        sql_query.next();
        count = sql_query.value(0).toInt();
    }
    closeDB();
    return count;
}

QList<data_struct> SQLiteHelper::getAllInfos(QString table_name,QString condation)
{
    QList<data_struct> infos;
    QMutexLocker lock(&m_lockdb);
    if (!openDB())
        return infos;

    QSqlQuery sql_query(m_database);
    // "select * from object_info ORDER BY id DESC"    ASC为升序，DESC为降序 pragma
    QString select_infos = "select * from " + table_name + " ORDER BY id DESC";
    if(condation != "") {
        select_infos += " where " + condation;
    }
    sql_query.prepare(select_infos);

    if (!sql_query.exec()) {
        qDebug() << "Error : " << sql_query.lastError();
    } else {
        while(sql_query.next()) {
            data_struct info;
            info.id = sql_query.value(0).toInt();
            int row = sql_query.at();                    // 该结果的行号， 0, 1, 2, 3 ...
            int columnNum = sql_query.record().count();  // 表的列数
            //            qDebug() << "表 " << table_name << " 的列数为 ： " << columnNum;
            QList<QString> item_info;
            for(int i = 1; i < columnNum; i++) {
                item_info.append(sql_query.value(i).toString().trimmed());
            }
            info.item_info = item_info;
            infos.append(info);
        }
    }
    closeDB();
    return infos;

}

QList<data_struct> SQLiteHelper::getInfosByRange(QString table_name,
                                                 int start, int end, QString condation)
{
    QList<data_struct> infos;
    QMutexLocker lock(&m_lockdb);
    if (!openDB())
        return infos;

    QSqlQuery sql_query(m_database);
    // "select * from object_info ORDER BY id DESC"    ASC为升序，DESC为降序 pragma
    QString select_infos = "select * from " + table_name;
    if(condation != "") {
        select_infos += " where " + condation;
    }
    select_infos += " ORDER BY id DESC";
//    qDebug() << select_infos;
    sql_query.prepare(select_infos);

    if (!sql_query.exec()) {
        qDebug() << "Error : " << sql_query.lastError();
    } else {
        for(int i = start; i < end; i++) {
            if(sql_query.seek(i)) {
                data_struct info;
                info.id = sql_query.value(0).toInt();
                int columnNum = sql_query.record().count();  // 表的列数
                QList<QString> item_info;
                for(int j = 1; j < columnNum; j++) {
                    item_info.append(sql_query.value(j).toString().trimmed());
                }
                info.item_info = item_info;
                infos.append(info);
            }
        }

    }
//    qDebug() << "end - start : " << end-start
//             << ", infos.size() : " << infos.size();
    closeDB();
    return infos;
}

QList<QString> SQLiteHelper::getNamesInfo(QString table_name, QString col_name,
                                          QString condation)
{
    QList<QString> infos;
    QMutexLocker lock(&m_lockdb);
    if (!openDB())
        return infos;

    QSqlQuery sql_query(m_database);
    // "select * from object_info ORDER BY id DESC"    ASC为升序，DESC为降序 pragma
    QString select_infos = "select " + col_name + " from " + table_name;

    if(condation != "") {
        select_infos = select_infos + " where " + condation;
    }

//    qDebug() << select_infos;

    sql_query.prepare(select_infos);

    if (!sql_query.exec()) {
        qDebug() << "Error : " << sql_query.lastError();
    } else {
        while(sql_query.next()) {
            QString item = sql_query.value(0).toString().trimmed();
            if(item == "") continue;
            infos.append(item);
        }
    }
    closeDB();
    return infos;
}

QList<QString> SQLiteHelper::getAttachment(QString table_name)
{

}

// 在数据库 m_database 中创建表 table_name, 创建数据库的 SQL 语句为 create_table_sql
bool SQLiteHelper::createTable(QSqlDatabase m_database, QString table_name,
                               QString create_table_sql)
{
    //    qDebug() << "create table : " << table_name << ", \n create_sql : " << create_table_sql;

    bool isSuccess = false;

    QSqlQuery sql_query(m_database);
    QString isExist_table = "select count(*) from sqlite_master where type='table' and "
                            "name='" + table_name + "'";
    if(!sql_query.exec(isExist_table)) {
        qDebug() << "Error: Failed to connect database." << sql_query.lastError();
    } else {
        if(sql_query.next()) {
            int num = sql_query.value(0).toInt();
            if (num == 0) {
                if(!sql_query.exec(create_table_sql)) {
                    qDebug() << "Error: Failed to connect database." << sql_query.lastError();
                } else {
                    qDebug() << table_name << " table created!";
                    isSuccess = true;
                }
            } else {
                qDebug() << table_name << " table has exist ...";
                isSuccess = true;
            }
        }
    }
    return isSuccess;
}

// 在数据库 m_database 中删除表 table_name
bool SQLiteHelper::deleteTable(QSqlDatabase m_database, QString table_name)
{
    //    qDebug() << "create table : " << table_name << ", \n create_sql : " << create_table_sql;

    bool isSuccess = false;

    QSqlQuery sql_query(m_database);

    QString delete_table = "drop table " + table_name;
    if(!sql_query.exec(delete_table)) {
        qDebug() << "Error: Failed to connect database." << sql_query.lastError();
    } else {
        qDebug() << "table" << table_name << " delete ...";
        isSuccess = true;
    }
    return isSuccess;
}


bool SQLiteHelper::openDB()
{
    return m_database.open();
}

void SQLiteHelper::closeDB()
{
    m_database.close();
}
