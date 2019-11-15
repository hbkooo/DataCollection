#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QtXml>
#include <QMap>
#include <QDebug>

// 显示的桌面 table 的标题
static QList<QString> firm_info_titles;
static QList<QString> equipment_info_titles;
static QList<QString> component_info_titles;
static QList<QString> sparepart_info_titles;
static QList<QString> tool_info_titles;
static QList<QString> technical_data_titles;
static QList<QString> repair_info_titles;

// 每个 table 必填项
static QList<int> must_firm_info_index;
static QList<int> must_equipment_info_index;
static QList<int> must_component_info_index;
static QList<int> must_sparepart_info_index;
static QList<int> must_tool_info_index;
static QList<int> must_technical_data_index;
static QList<int> must_repair_info_index;

// 显示的弹窗的标题
static QList<QString> dialog_firm_info_titles;
static QList<QString> dialog_equipment_info_titles;
static QList<QString> dialog_component_info_titles;
static QList<QString> dialog_sparepart_info_titles;
static QList<QString> dialog_tool_titles;
static QList<QString> dialog_technical_data_titles;
static QList<QString> dialog_repair_titles;

static QMap <int, QList<QString>> firm_choices;
static QMap <int, QList<QString>> equipment_choices;
static QMap <int, QList<QString>> component_choices;
static QMap <int, QList<QString>> sparepart_choices;
static QMap <int, QList<QString>> tool_choices;
static QMap <int, QList<QString>> technical_data_choices;
static QMap <int, QList<QString>> repair_choices;

static QList<QList<QString>> tables_titles;         // 桌面 table 标题集合
static QList<QList<int>> must_tables_index;         // 所有表的必填项集合
static QList<QList<QString>> dialog_tables_titles;  // 弹窗标题集合
static QList<QString> table_subjects;               // table 的表名集合
static QList<QMap<int, QList<QString>>> table_choices; // table 的各个表中选项集合

static QMap<int, int> attachment_index;     // 各个表格的附件所在的 index 编号

static QMap<QString, QMap<int, QList<QString>>> system_class_params;

static QList<QString> nullList;    // 空集合
static QMap <int, QList<QString>> nullMap;    // 空集合


/**
 * @brief initFirmInfoTitle
 * 注意：所有的标题添加(append)顺序必须与 sqlitehelper.cpp 文件中创建表时字段的顺序一致
 */

// 厂商
static void initFirmInfoTitle() {
    firm_info_titles.append("厂商名称");
    firm_info_titles.append("国家");
    firm_info_titles.append("地址");
    firm_info_titles.append("税务登记号");
    firm_info_titles.append("联系人");
    firm_info_titles.append("联系电话");
    firm_info_titles.append("电子邮件");
    firm_info_titles.append("QQ/MSN");
    firm_info_titles.append("厂商资质");
    firm_info_titles.append("经营范围");

    foreach(QString title, firm_info_titles) {
        dialog_firm_info_titles.append(title);
    }
    dialog_firm_info_titles.append("邮政编码");
    dialog_firm_info_titles.append("传真");
    dialog_firm_info_titles.append("备注");
    must_firm_info_index.append(0);
    must_firm_info_index.append(2);

//    QList<QString> country;
//    country.append("中国");

//    firm_choices.insert(1, country);

    attachment_index.insert(0, -1);

}

// 设备
static void initEquipmentInfoTitle() {
    equipment_info_titles.append("设备名称");
    equipment_info_titles.append("厂商名称");
    equipment_info_titles.append("设备类型");
    equipment_info_titles.append("型号/标准号");
    equipment_info_titles.append("MTBF(h)");
    equipment_info_titles.append("MTTR(h)");
    equipment_info_titles.append("重量(Kg)");
    equipment_info_titles.append("是否寿命件");
    equipment_info_titles.append("寿命期");
    equipment_info_titles.append("寿命单位");

    foreach(QString title, equipment_info_titles) {
        dialog_equipment_info_titles.append(title);
    }
    dialog_equipment_info_titles.append("尺寸/长(cm)");
    dialog_equipment_info_titles.append("尺寸/宽cm)");
    dialog_equipment_info_titles.append("尺寸/高(cm)");
    dialog_equipment_info_titles.append("装机数量");
    dialog_equipment_info_titles.append("装机计量单位");
    dialog_equipment_info_titles.append("参数1");
    dialog_equipment_info_titles.append("参数2");
    dialog_equipment_info_titles.append("参数3");
    dialog_equipment_info_titles.append("参数4");
    dialog_equipment_info_titles.append("参数5");
    dialog_equipment_info_titles.append("参数6");
    dialog_equipment_info_titles.append("保养要求");
    dialog_equipment_info_titles.append("附件");
    dialog_equipment_info_titles.append("备注");

    must_equipment_info_index.append(0);
    must_equipment_info_index.append(1);
    must_equipment_info_index.append(2);
    must_equipment_info_index.append(3);
    must_equipment_info_index.append(6);
    must_equipment_info_index.append(7);
    must_equipment_info_index.append(8);
    must_equipment_info_index.append(9);

//    QList<QString> type;
//    type.append("容器类");
//    type.append("旋转机械类");
//    type.append("热换器类");
//    type.append("集成类");
//    type.append("电子类");
//    QList<QString> yes_no;
//    yes_no.append("是");
//    yes_no.append("否");
//    QList<QString> life_util;
//    life_util.append("分钟");
//    life_util.append("小时");
//    life_util.append("天");
//    life_util.append("周");
//    life_util.append("月");
//    life_util.append("年");

//    equipment_choices.insert(2, type);
//    equipment_choices.insert(7, yes_no);
//    equipment_choices.insert(9, life_util);

    equipment_choices.insert(1, nullList);

    attachment_index.insert(1, 22);

}

// 部件
static void initComponentInfoTitle() {
    component_info_titles.append("部件名称");
    component_info_titles.append("设备名称");
    component_info_titles.append("型号/标准号");
    component_info_titles.append("MTBF(h)");
    component_info_titles.append("MTTR(h)");
    component_info_titles.append("重量(Kg)");
    component_info_titles.append("是否寿命件");
    component_info_titles.append("寿命期");
    component_info_titles.append("寿命单位");
    component_info_titles.append("是否备件");
    component_info_titles.append("是否标准件");
    component_info_titles.append("可修复");

    foreach(QString title, component_info_titles) {
        dialog_component_info_titles.append(title);
    }
    dialog_component_info_titles.append("尺寸/长(cm)");
    dialog_component_info_titles.append("尺寸/宽cm)");
    dialog_component_info_titles.append("尺寸/高(cm)");
    dialog_component_info_titles.append("数量每设备");
    dialog_component_info_titles.append("计量单位");
    dialog_component_info_titles.append("单价");
    dialog_component_info_titles.append("主参数1");
    dialog_component_info_titles.append("主参数2");
    dialog_component_info_titles.append("制造商/供应商名称(中文)");
    dialog_component_info_titles.append("附件");
    dialog_component_info_titles.append("备注");

    must_component_info_index.append(0);  // 部件名称
    must_component_info_index.append(1);  // 设备名称
    must_component_info_index.append(2);  // 型号/标准号
    must_component_info_index.append(6);  // 是否寿命件
    must_component_info_index.append(7);  // 寿命期
    must_component_info_index.append(8);  // 寿命单位
    must_component_info_index.append(9);  // 是否备件
    must_component_info_index.append(10); // 是否标准件

//    QList<QString> yes_no;
//    yes_no.append("是");
//    yes_no.append("否");
//    QList<QString> life_util;
//    life_util.append("分钟");
//    life_util.append("小时");
//    life_util.append("天");
//    life_util.append("周");
//    life_util.append("月");
//    life_util.append("年");

//    component_choices.insert(6, yes_no);     // 是否寿命件
//    component_choices.insert(8, life_util);  // 寿命单位
//    component_choices.insert(9, yes_no);     // 是否备件
//    component_choices.insert(10, yes_no);     // 是否标准件
//    component_choices.insert(11, yes_no);     // 可修复

    component_choices.insert(1, nullList);

    attachment_index.insert(2, 21);
}

// 零配件
static void initSparepartInfoTitle() {
    sparepart_info_titles.append("零配件名称");
    sparepart_info_titles.append("设备名称");
    sparepart_info_titles.append("型号/标准号");
    sparepart_info_titles.append("MTBF(h)");
    sparepart_info_titles.append("MTTR(h)");
    sparepart_info_titles.append("重量(Kg)");
    sparepart_info_titles.append("是否寿命件");
    sparepart_info_titles.append("寿命期");
    sparepart_info_titles.append("寿命单位");
    sparepart_info_titles.append("是否备件");
    sparepart_info_titles.append("是否标准件");
    sparepart_info_titles.append("是否易损件");
    sparepart_info_titles.append("可修复");

    foreach(QString title, sparepart_info_titles) {
        dialog_sparepart_info_titles.append(title);
    }
    dialog_sparepart_info_titles.append("尺寸/长(cm)");
    dialog_sparepart_info_titles.append("尺寸/宽cm)");
    dialog_sparepart_info_titles.append("尺寸/高(cm)");
    dialog_sparepart_info_titles.append("随艇配备标准");
    dialog_sparepart_info_titles.append("数量每设备");
    dialog_sparepart_info_titles.append("计量单位");
    dialog_sparepart_info_titles.append("单价");
    dialog_sparepart_info_titles.append("主参数1");
    dialog_sparepart_info_titles.append("主参数2");
    dialog_sparepart_info_titles.append("制造商/供应商名称(中文)");
    dialog_sparepart_info_titles.append("附件");
    dialog_sparepart_info_titles.append("备注");

    must_sparepart_info_index.append(0);
    must_sparepart_info_index.append(1);
    must_sparepart_info_index.append(2);
    must_sparepart_info_index.append(6);
    must_sparepart_info_index.append(7);
    must_sparepart_info_index.append(8);
    must_sparepart_info_index.append(9);
    must_sparepart_info_index.append(10);
    must_sparepart_info_index.append(11);
    must_sparepart_info_index.append(12);

//    QList<QString> yes_no;
//    yes_no.append("是");
//    yes_no.append("否");
//    QList<QString> life_util;
//    life_util.append("分钟");
//    life_util.append("小时");
//    life_util.append("天");
//    life_util.append("周");
//    life_util.append("月");
//    life_util.append("年");

//    sparepart_choices.insert(6, yes_no);      // 是否寿命件
//    sparepart_choices.insert(8, life_util);   // 寿命单位
//    sparepart_choices.insert(9, yes_no);      // 是否备件
//    sparepart_choices.insert(10, yes_no);     // 是否标准件
//    sparepart_choices.insert(11, yes_no);     // 是否易损件
//    sparepart_choices.insert(12, yes_no);     // 可修复

    sparepart_choices.insert(1, nullList);

    attachment_index.insert(3, 23);
}

// 工具
static void initToolTitle() {
    tool_info_titles.append("物品名称");
    tool_info_titles.append("物品类型");
    tool_info_titles.append("计量单位");
    tool_info_titles.append("数量");
    tool_info_titles.append("单价(元)");
    tool_info_titles.append("制造商");

    foreach(QString title, tool_info_titles) {
        dialog_tool_titles.append(title);
    }
    dialog_tool_titles.append("主参数1");
    dialog_tool_titles.append("主参数2");
    dialog_tool_titles.append("主参数3");
    dialog_tool_titles.append("备注");

    must_tool_info_index.append(0);
    must_tool_info_index.append(1);
    must_tool_info_index.append(2);

    QList<QString> object_type;
    object_type.append("专用工具");
    object_type.append("通用工具");
    object_type.append("专用测试设备");
    object_type.append("通用测试设备");
    object_type.append("便携式电子测试设备");
    object_type.append("消耗品");
    object_type.append("其他");

    tool_choices.insert(1, object_type);

    attachment_index.insert(4,-1);

}

// 技术资料
static void initTechnicalDataTitle() {
    technical_data_titles.append("原始资料编号");
    technical_data_titles.append("资料名称");
    technical_data_titles.append("资料类型");
    technical_data_titles.append("每份文档总页数");
    technical_data_titles.append("资料所属");
    technical_data_titles.append("所属设备/部件/零配件");
    technical_data_titles.append("同种文档数量");
    technical_data_titles.append("图纸规格");
    technical_data_titles.append("出版日期");
    technical_data_titles.append("出版商");
    technical_data_titles.append("存储介质");

    foreach(QString title, technical_data_titles) {
        dialog_technical_data_titles.append(title);
    }
    dialog_technical_data_titles.append("单位");
    dialog_technical_data_titles.append("附件");
    dialog_technical_data_titles.append("备注");

    must_technical_data_index.append(0);
    must_technical_data_index.append(1);
    must_technical_data_index.append(2);
    must_technical_data_index.append(4);
    must_technical_data_index.append(5);

//    QList<QString> data_type;
//    data_type.append("技术手册");
//    data_type.append("操作手册");
//    data_type.append("零件分解图册");
//    data_type.append("培训手册/教材");
//    data_type.append("图纸");
//    data_type.append("保障物品清单");
//    data_type.append("实验报告");
//    data_type.append("其他");

//    QList<QString> data_belong;
//    data_belong.append("设备");
//    data_belong.append("部件");
//    data_belong.append("零配件");

//    QList<QString> paper_type;
//    paper_type.append("4A0");
//    paper_type.append("2A0");
//    paper_type.append("A0");
//    paper_type.append("A1");
//    paper_type.append("A2");
//    paper_type.append("A3");
//    paper_type.append("A4");
//    paper_type.append("A5");
//    paper_type.append("A6");
//    paper_type.append("A7");
//    paper_type.append("A8");
//    paper_type.append("A9");
//    paper_type.append("A10");
//    paper_type.append("其他");

//    technical_data_choices.insert(2, data_type);   // 资料类型
//    technical_data_choices.insert(4, data_belong); // 资料所属
//    technical_data_choices.insert(7, paper_type);  // 图纸规格

    technical_data_choices.insert(5,nullList);

    attachment_index.insert(5,12);
}

// 维修
static void initRepairTitle() {
    repair_info_titles.append("维修级别");
    repair_info_titles.append("设备名称");
    repair_info_titles.append("部件");
    repair_info_titles.append("维修项名称");
    repair_info_titles.append("消耗时间");
    repair_info_titles.append("时间单位");
    repair_info_titles.append("维修大类");
    repair_info_titles.append("维修小类");
    repair_info_titles.append("故障分析");
    repair_info_titles.append("附件");

    foreach(QString title, repair_info_titles) {
        dialog_repair_titles.append(title);
    }
    dialog_repair_titles.append("安全注意事项");
    dialog_repair_titles.append("维修工作步骤");
    dialog_repair_titles.append("备注");

    must_repair_info_index.append(1);
    must_repair_info_index.append(2);

//    QList<QString> repair_level;
//    repair_level.append("航员级");
//    repair_level.append("中继级");
//    repair_level.append("基地级");

//    QList<QString> time_util;
//    time_util.append("分钟");
//    time_util.append("小时");
//    time_util.append("天");
//    time_util.append("周");
//    time_util.append("月");
//    time_util.append("年");

//    QList<QString> big_class;
//    big_class.append("预防性维修");
//    big_class.append("修复性维修");
//    big_class.append("视情维修");

//    repair_choices.insert(0, repair_level);   // 维修级别
//    repair_choices.insert(5, time_util);      // 时间单位
//    repair_choices.insert(6, big_class);      // 维修大类

    repair_choices.insert(1, nullList);      // 维修大类
    repair_choices.insert(2, nullList);      // 维修大类

    attachment_index.insert(6, 9);
}

static QString initClassParams(QDomElement root) {

    QString message = "";

    QString root_name = root.tagName();
    qDebug() << "root_name : " << root_name;
    QDomNodeList systems = root.childNodes();
    // 循环遍历各个系统
    for(int i = 0; i < systems.size(); i++) {

        QMap<int, QList<QString>> system_params;

        QDomElement system = systems.at(i).toElement();
        QString system_name = system.tagName();
        qDebug() << "system_name : " << system_name;

        // 注释行
        if(system_name == "") continue;

        // 该系统下的有参数设置的各个表
        QDomNodeList tables = system.childNodes();
        for(int j = 0; j < tables.size(); j++) {
            QDomElement table = tables.at(j).toElement();
            QString table_name = table.tagName();
            if(table_name == "") continue;

            int max_params = table.attribute("max").toInt();
            int table_index = table_subjects.indexOf(table_name);
            qDebug() << "table_name : " << table_name
                     << ", max_params : " << max_params
                     << ", table_index : " << table_index;

            if(table_index == -1) {
                message += "\"系统大类\"节点下的\"" + system_name + "\"下的表名\"" + table_name + "\"与系统定义表名不匹配\n";
                continue;
            }

            QList<QString> params;
            QDomNodeList table_params = table.childNodes();
            // 读取参数值
            for(int n = 0; n < table_params.size(); n++) {
                if(max_params <= n) break;
                QDomElement table_param = table_params.at(n).toElement();
                params.append(table_param.text());
            }
            qDebug() << "params : " << params;
            system_params.insert(table_index, params);
        }

        system_class_params.insert(system_name, system_params);
    }

    qDebug() << "all params : " << system_class_params;

    return message;
}

static QString initTableChoices(QString xmlFile) {

    QString message = "";

    if(xmlFile.isEmpty())
        return "config.xml配置文件不存在";

    QFile file(xmlFile);
    if(!file.exists()) return "config.xml配置文件不存在";
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "open file fail ...";
        return " config.xml 文件打开错误";
    }

    QDomDocument document;
    QString error;
    int row, column;
    if(!document.setContent(&file, false, &error, &row, &column))
    {
        qDebug() << "content fail ..." << error
                 << " at row " << QString::number(row)
                 << " , column " << QString::number(column);
        return "配置文件 config.xml 解析错误：\n" + error + " at " + QString::number(row) + " row, " + QString::number(column) + " column";
    }

    if(document.isNull())
    {
        qDebug() << "document is null ...";
        return "文件为空";
    }

    QDomElement root = document.documentElement();

    //root_tag_name为 "工程"
    QString root_tag_name = root.tagName();
//    qDebug() << "root name : " << root_tag_name;

    // 依次解析每一张表中的信息
    QDomNodeList tables = root.childNodes();
    for(int m = 0; m < tables.size(); m ++) {
        // 表名
        QDomElement table = tables.item(m).toElement();
        QString subject_name = table.tagName();

        // 如果读的是注释的话
        if(subject_name == "") continue;

        if(subject_name == "系统大类") {
            // 初始化设备大类的各个参数信息
            message += initClassParams(table);
            continue;
        }

        int table_index = table_subjects.indexOf(subject_name);  // 该表所在的序号
        if(table_index == -1) {
            qDebug() << "配置文件中表名书写错误..." << subject_name;
            message += "表名\"" + subject_name + "\"与系统定义表名不匹配\n";
            continue;
        }
        qDebug() << subject_name << " , " << table_index;
        QMap<int, QList<QString>> titles_choices = table_choices.takeAt(table_index);                // 该表对应的title的choices集合
        QList<QString> table_titles = dialog_tables_titles.at(table_index);// 该表所有的标题

        // qDebug() << "subject_name : " << subject_name;
        // 依次解析该表下所有的标题节点
        QDomNodeList titles = table.childNodes();
        for(int n = 0; n < titles.size(); n++) {
            QDomElement title = titles.item(n).toElement();
            QString title_name = title.tagName();

            if(title_name == "") continue;

            // qDebug() << "title_name : " << title_name;

            int title_index = table_titles.indexOf(title_name);

            if(title_index == -1) {
                message += "在表\"" + subject_name + "\"节点下的标题\"" + title_name + "\"与系统定义标题不匹配\n";
                continue;
            }

            QList<QString> items_choice;              // 某一个title的choice集合

            QDomNodeList choices = title.childNodes();
            for(int i = 0; i < choices.size(); i++) {
                QDomElement item = choices.item(i).toElement();
                QString choice = item.text();
                items_choice.append(choice);
                // qDebug() << "item : " << choice;
            }
            qDebug() << items_choice;
            titles_choices.insert(title_index, items_choice);
        }
        table_choices.insert(table_index, titles_choices);
    }

    if(system_class_params.size() == 0) {
        system_class_params.insert("默认类",nullMap);
        message += "没有配置\"系统大类\"选择信息";
    }

    if(message != "")
        message = "配置文件 config.xml 中：\n" + message;

    return message;

}

static QString initTableData() {

    initFirmInfoTitle();
    initEquipmentInfoTitle();
    initComponentInfoTitle();
    initSparepartInfoTitle();
    initToolTitle();
    initTechnicalDataTitle();
    initRepairTitle();
    tables_titles.append(firm_info_titles);
    tables_titles.append(equipment_info_titles);
    tables_titles.append(component_info_titles);
    tables_titles.append(sparepart_info_titles);
    tables_titles.append(tool_info_titles);
    tables_titles.append(technical_data_titles);
    tables_titles.append(repair_info_titles);

    must_tables_index.append(must_firm_info_index);
    must_tables_index.append(must_equipment_info_index);
    must_tables_index.append(must_component_info_index);
    must_tables_index.append(must_sparepart_info_index);
    must_tables_index.append(must_tool_info_index);
    must_tables_index.append(must_technical_data_index);
    must_tables_index.append(must_repair_info_index);

    dialog_tables_titles.append(dialog_firm_info_titles);
    dialog_tables_titles.append(dialog_equipment_info_titles);
    dialog_tables_titles.append(dialog_component_info_titles);
    dialog_tables_titles.append(dialog_sparepart_info_titles);
    dialog_tables_titles.append(dialog_tool_titles);
    dialog_tables_titles.append(dialog_technical_data_titles);
    dialog_tables_titles.append(dialog_repair_titles);

    tables_titles = dialog_tables_titles;

    table_subjects.append("厂商信息");
    table_subjects.append("设备信息");
    table_subjects.append("部件信息");
    table_subjects.append("零配件信息");
    table_subjects.append("工具");
    table_subjects.append("技术资料");
    table_subjects.append("维修");

    table_choices.append(firm_choices);
    table_choices.append(equipment_choices);
    table_choices.append(component_choices);
    table_choices.append(sparepart_choices);
    table_choices.append(tool_choices);
    table_choices.append(technical_data_choices);
    table_choices.append(repair_choices);

    return initTableChoices("config.xml");

}

#endif // CONFIG_H
