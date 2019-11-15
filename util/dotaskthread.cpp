#include "dotaskthread.h"

DoTaskThread::DoTaskThread(QObject *parent,QProgressDialog *progress) : QThread (parent)
{
    this->progress_dialog = progress;
    currentValue = 0;
    if(timer == nullptr) {
        timer = new QTimer(this);
        connect(timer,SIGNAL(timeout()),this,SLOT(valueChanged()));
    }
    connect(this,SIGNAL(timerStart()),this,SLOT(startTimer()));
    connect(this,SIGNAL(convert_over()),this,SLOT(convert_data_over()));

}

DoTaskThread::~DoTaskThread()
{
    qDebug() << "free doTaskThread ...";
}

void DoTaskThread::run()
{
    emit timerStart();
    switch (type) {
    case SaveExcel:
        saveExcel();
        break;
    case SaveAttachment:
        saveAttachment();
        break;
    default:
        break;
    }
    emit convert_over();
}

void DoTaskThread::saveExcel()
{
    QAxObject *excel = new QAxObject("Excel.Application");
    if (excel->isNull()) {
        if (excel != nullptr) {
            //网络中很多使用excel==NULL判断，是错误的
            excel->dynamicCall("Quit()");
            delete excel;
        }
        emit save_error("没有找到EXCEL应用程序");
        return;
    }
    QAxObject *workbooks = nullptr;
    QAxObject *workbook = nullptr;
    QAxObject *worksheets = nullptr;
    QAxObject *worksheet = nullptr;
    QAxObject *cell = nullptr;
    QString str;

    excel->dynamicCall("SetVisible (bool)", false); //不显示窗体
    //不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示
    excel->setProperty("DisplayAlerts", false);
    workbooks = excel->querySubObject("WorkBooks");//获取工作簿集合

    if (QFile::exists(fileName)) {
        workbook = workbooks->querySubObject("Open(const QString &)", fileName);
    } else {
        workbooks->dynamicCall("Add");//新建一个工作簿
        workbook = excel->querySubObject("ActiveWorkBook");//获取当前工作簿
    }

    worksheets = workbook->querySubObject("Sheets");//获取工作表集合

    // 第一张工作表,填写说明
    QAxObject *first_sheet = worksheets->querySubObject("Item(int)", 1);
    first_sheet->setProperty("Name", "填写说明");
    cell = first_sheet->querySubObject("Cells(int,int)", 1, 1);
    cell->dynamicCall("SetValue(const QString&)", "填写说明");
    cell->querySubObject("Font")->setProperty("Size", 16);
    cell->querySubObject("Font")->setProperty("Bold", true);
    cell->setProperty("HorizontalAlignment", -4108);
    cell->setProperty("RowHeight", 24);
    cell->setProperty("ColumnWidth", 68);

    cell = first_sheet->querySubObject("Cells(int,int)", 2, 1);
    cell->setProperty("RowHeight", 24);
    cell->dynamicCall("SetValue(const QString&)", "该表所属的系统大类为：" + class_name);
    cell = first_sheet->querySubObject("Cells(int,int)", 3, 1);
    cell->setProperty("RowHeight", 24);
    cell->dynamicCall("SetValue(const QString&)", "红色字体为必输项");

    // 依次创建每一张表
    for(int i = 0; i < table_subjects.size(); i++) {
        // qDebug() << "test5 ...";
        //插入工作表（插入至最后一行）
        QAxObject *last_sheet = worksheets->querySubObject("Item(int)", i+1);//获取工作表集合的工作表1，即sheet1
        worksheet = worksheets->querySubObject("Add(QVariant)", last_sheet->asVariant());
        last_sheet->dynamicCall("Move(QVariant)", worksheet->asVariant());
        worksheet->setProperty("Name", QString::number(i+1)+","+table_subjects.at(i));  //设置工作表名称

        // 行数加一是因为加上标题行
        int rowCount = sqlHelper->getInfoNumber(sqlHelper->getTableNames().at(i)) + 1;
        int colCount = sqlHelper->getColumnNamesFromTable(sqlHelper->getTableNames().at(i))
                .size();

        //            qDebug() << "rowCount : " << rowCount
        //                     << ", colCount : " << colCount;

        //获取该sheet的使用范围对象
        QAxObject * usedrange = worksheet->querySubObject("UsedRange");
        int intRowStart = usedrange->property("Row").toInt();
        int intColStart = usedrange->property("Column").toInt();

//            qDebug() << "intRowStart : " << intRowStart
//                     << ", intColStart : " << intColStart;

        // 插入标题
        cell = worksheet->querySubObject("Cells(int,int)", intRowStart, intColStart);
        cell->setProperty("Value", "序号");
        cell->setProperty("HorizontalAlignment", -4108);
        cell->querySubObject("Font")->setProperty("Size", 11);
        cell->querySubObject("Font")->setProperty("Bold", true);
        cell->setProperty("RowHeight", 22);
        cell->setProperty("ColumnWidth", 20);
        for (int m = 1; m < colCount; m++) {
            cell = worksheet->querySubObject("Cells(int,int)", intRowStart, intColStart + m);//获取单元格
            cell->setProperty("Value", dialog_tables_titles.at(i).at(m-1));
            cell->setProperty("HorizontalAlignment", -4108);
            cell->querySubObject("Font")->setProperty("Size", 11);
            cell->querySubObject("Font")->setProperty("Bold", true);
            cell->setProperty("RowHeight", 22);
            cell->setProperty("ColumnWidth", 20);
            if(must_tables_index.at(i).contains(m-1)) {
                cell->querySubObject("Font")->setProperty("Color",QColor(255, 0, 0));
            }
        }

        QList<data_struct> infos = sqlHelper->getAllInfos(
                    sqlHelper->getTableNames().at(i));

        //            qDebug() << "test9 ... info.size() : " << infos.size()
        //                     << ", data.size : " << infos.at(0).item_info.size();

        // 插入数据
        for (int m = intRowStart + 1; m < intRowStart + rowCount; m++) {
            for(int n = intColStart; n < intColStart + colCount; n++) {
                if(n == intColStart) str = QString::number(infos.at(m-1-intRowStart).id);
                else str = infos.at(m-1-intRowStart).item_info.at(n-1-intColStart);
                cell = worksheet->querySubObject("Cells(int,int)", m, n);//获取单元格
                cell->setProperty("Value", str);
                cell->setProperty("HorizontalAlignment", -4108);
                cell->setProperty("RowHeight", 16);
            }
        }

    }

    workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(fileName));//保存至filepath，注意一定要用QDir::toNativeSeparators将路径中的"/"转换为"\"，不然一定保存不了。
    workbook->dynamicCall("Close()");//关闭工作簿
    excel->dynamicCall("Quit()");//关闭excel
    delete workbook;
    delete workbooks;
    delete excel;
    qDebug() << "save excel over ...";

    emit save_success("导出Excel成功");

}

void DoTaskThread::saveAttachment()
{
    QDir temp(QDir::currentPath());
    if(!temp.exists("temp")) {
        temp.mkdir("temp");
    }
    if(!temp.exists("temp/附件")) {
        temp.mkdir("temp/附件");
    }
    QString temp_path = QDir::currentPath() + "/temp/附件/";

    QList<QString> equipment_attachments = sqlHelper->getNamesInfo(
                sqlHelper->getTableNames().at(1), "attachment");
    QList<QString> component_attachments = sqlHelper->getNamesInfo(
                sqlHelper->getTableNames().at(2), "attachment");
    QList<QString> sparepart_attachments = sqlHelper->getNamesInfo(
                sqlHelper->getTableNames().at(3), "attachment");
    QList<QString> technical_attachments = sqlHelper->getNamesInfo(
                sqlHelper->getTableNames().at(5), "attachment");
    QList<QString> repair_attachments = sqlHelper->getNamesInfo(
                sqlHelper->getTableNames().at(6), "attachment");

    QList<QString> attachments;
    attachments.append(equipment_attachments);
    attachments.append(component_attachments);
    attachments.append(sparepart_attachments);
    attachments.append(technical_attachments);
    attachments.append(repair_attachments);

    qDebug() << "equipment : " << equipment_attachments
             << "\ncomponent : " << component_attachments
             << "\nsparepart : " << sparepart_attachments
             << "\ntechnical : " << technical_attachments
             << "\nrepair : " << repair_attachments;

    foreach(QString attachment, attachments) {
        QList<QString> files = attachment.split(",");
        // 复制每一个文件
        foreach(QString file, files) {
            QString original_file_name = QDir::currentPath() + "/" +
                    class_name + "/附件/" + file;
            QFile original_file(original_file_name);
            if(!original_file.exists()){
                qDebug() << "文件不存在 ： " << original_file_name;
                continue;
            }
            QFile save_file(temp_path + file);
            if(save_file.exists()) {
                // 文件已经存在，则删除原始文件
                save_file.remove();
            }
            if (original_file.copy(temp_path + file)) {
                qDebug() << "复制成功 : " << temp_path + file;
            } else {
                qDebug() << "复制失败 : " << temp_path + file;
            }
        }
    }

    if(JlCompress::compressDir(fileName, temp_path)) {
        emit save_success("附件压缩保存成功");
    } else {
        emit save_error("附件压缩保存失败");
    }

    // 清空附件缓存
    QDir dir(temp_path);
    QStringList lists = dir.entryList();
    for(int i=0;i<lists.size();i++) {
        QFile tmp_file(temp_path + lists.at(i));
        tmp_file.remove();
    }
}

void DoTaskThread::startTimer()
{
    timer->start(500);
    progress_dialog->show();
    currentValue = 0;
}

void DoTaskThread::convert_data_over()
{
    currentValue = 100;
    progress_dialog->setValue(currentValue);
    progress_dialog->hide();
    timer->stop();
}

void DoTaskThread::valueChanged()
{
    currentValue++;
    if(currentValue == 100){
        currentValue = 50;
    }
    progress_dialog->setValue(currentValue);
}

void DoTaskThread::cancel()
{

}
