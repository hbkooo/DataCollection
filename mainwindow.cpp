#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget (parent)
{
    this->resize(1280,720);

    // 初始化表数据
    QString message = initTableData();
    if(message != ""){
        QMessageBox::warning(this, "错误", message);
    }

    if (sqlHelper == nullptr) {
        sqlHelper = new SQLiteHelper();
    }

    setStyleSheet("background-color:#f5f5f5;");

    InitHead();
    InitCenter();
    setMainWindowLayout();
}

MainWindow::~MainWindow()
{
    // TODO delete table widgets
    qDebug() << "free mainwindow space ...";

    // 释放数据库操作对象的资源
    if (sqlHelper != nullptr) {
        delete sqlHelper;
        sqlHelper = nullptr;
    }

}

// height 66
void MainWindow::InitHead()
{
    QPixmap logo_map(":/image/logo_icon");
    logo_head_lb = new QLabel();
    logo_head_lb->setPixmap(logo_map);
    logo_head_lb->setFixedWidth(80);
    logo_head_lb->setFixedHeight(60);

    QLabel *info_manage_head_lb = new QLabel("信息管理");
    QPixmap info_manage_map(":/image/info_manage");
    QLabel *info_manage_lb = new QLabel();
    info_manage_map = info_manage_map.scaled(24,24);
    info_manage_lb->setPixmap(info_manage_map);
    QVBoxLayout *info_manage_head_vlayout = new QVBoxLayout();
    info_manage_head_vlayout->setMargin(0);
    info_manage_head_vlayout->setSpacing(0);
    info_manage_head_vlayout->setContentsMargins(0,0,0,0);
    info_manage_head_vlayout->addStretch();
    info_manage_head_vlayout->addWidget(info_manage_lb,0,Qt::AlignHCenter);
    info_manage_head_vlayout->addSpacing(6);
    info_manage_head_vlayout->addWidget(info_manage_head_lb,0,Qt::AlignHCenter);
    info_manage_head_vlayout->addStretch();

    info_manage_head_wg = new QWidget();
    info_manage_head_wg->setLayout(info_manage_head_vlayout);
    info_manage_head_wg->setStyleSheet("background-color:#e1e1e1;");
    info_manage_head_wg->setFixedSize(100,logo_head_lb->height());

    class_label_ = new QLabel("设备大类:");
    class_label_->setStyleSheet("font-size:18px;"
                                "color:#666;");
    class_label = new QLabel();
    if(system_class_params.keys().size() != 0)
        class_label->setText(system_class_params.keys().at(0));
    class_label->setStyleSheet("font-size:18px;"
                               "color:#666;");

    QIcon add_data_icon(":/image/add_data");
    add_data_btn = new QPushButton();
    add_data_btn->setObjectName("add_data");
    add_data_btn->setFlat(true);
    add_data_btn->setIconSize(QSize(24,24));
    add_data_btn->setIcon(add_data_icon);
    add_data_btn->setToolTip("新增数据库");
    connect(add_data_btn, SIGNAL(clicked()), this, SLOT(button_clicked()));

    QIcon open_data_icon(":/image/file");
    open_data_btn = new QPushButton();
    open_data_btn->setObjectName("open_data");
    open_data_btn->setFlat(true);
    open_data_btn->setIconSize(QSize(24,24));
    open_data_btn->setIcon(open_data_icon);
    open_data_btn->setToolTip("打开数据库");
    connect(open_data_btn, SIGNAL(clicked()), this, SLOT(button_clicked()));

    QIcon save_data_icon(":/image/save");
    save_data_btn = new QPushButton();
    save_data_btn->setObjectName("save_data");
    save_data_btn->setFlat(true);
    save_data_btn->setIconSize(QSize(22,22));
    save_data_btn->setIcon(save_data_icon);
    save_data_btn->setToolTip("保存数据");
    connect(save_data_btn, SIGNAL(clicked()), this, SLOT(button_clicked()));

}

void MainWindow::InitCenter()
{
    firm_info_center_head_lb = new ClickableLabel(table_subjects.at(0));
    firm_info_center_head_lb->setObjectName("0");
    equipment_info_center_head_lb = new ClickableLabel(table_subjects.at(1));
    equipment_info_center_head_lb->setObjectName("1");
    component_info_center_head_lb = new ClickableLabel(table_subjects.at(2));
    component_info_center_head_lb->setObjectName("2");
    sparepart_info_center_head_lb = new ClickableLabel(table_subjects.at(3));
    sparepart_info_center_head_lb->setObjectName("3");
    tool_center_head_lb = new ClickableLabel(table_subjects.at(4));
    tool_center_head_lb->setObjectName("4");
    technical_data_center_head_lb = new ClickableLabel(table_subjects.at(5));
    technical_data_center_head_lb->setObjectName("5");
    repair_center_head_lb = new ClickableLabel(table_subjects.at(6));
    repair_center_head_lb->setObjectName("6");
    center_label_list.append(firm_info_center_head_lb);
    center_label_list.append(equipment_info_center_head_lb);
    center_label_list.append(component_info_center_head_lb);
    center_label_list.append(sparepart_info_center_head_lb);
    center_label_list.append(tool_center_head_lb);
    center_label_list.append(technical_data_center_head_lb);
    center_label_list.append(repair_center_head_lb);
    QHBoxLayout *menu_head_layout = new QHBoxLayout();
    menu_head_layout->setMargin(0);
    menu_head_layout->setSpacing(0);
    foreach(ClickableLabel * label, center_label_list){
        label->setFixedHeight(40);
        label->setAlignment(Qt::AlignCenter);
        menu_head_layout->addWidget(label);
        connect(label, SIGNAL(clicked()),this, SLOT(center_label_clicked()));

        if(center_label_list.indexOf(label) != (center_label_list.size()-1)) {
            QWidget *split = new QWidget();
            split->setFixedSize(1,label->height());
            split->setStyleSheet("background-color:#a3a3a3");
            menu_head_layout->addWidget(split);
        }
    }
    InitCenterLables();
    setClickedLabel(0);

    QPixmap add_map(":/image/add");
    add_map = add_map.scaled(18,18);
    ClickableLabel *add_lb = new ClickableLabel();
    add_lb->setObjectName("add_btn");
    add_lb->setFixedSize(QSize(38,26));
    add_lb->setStyleSheet("border:2px solid #ebebeb;");
    add_lb->setAlignment(Qt::AlignCenter);
    add_lb->setPixmap(add_map);
    add_lb->setToolTip("添加新的记录");
    connect(add_lb, SIGNAL(clicked()), this, SLOT(button_clicked()));

    QPixmap delete_map(":/image/delete");
    delete_map = delete_map.scaled(18,18);
    ClickableLabel *delete_lb = new ClickableLabel();
    delete_lb->setObjectName("delete_btn");
    delete_lb->setFixedSize(QSize(38,26));
    delete_lb->setStyleSheet("border:2px none #ebebeb;"
                             "border-bottom-style:solid;"
                             "border-top-style:solid;"
                             "border-right-style:solid;");
    delete_lb->setAlignment(Qt::AlignCenter);
    delete_lb->setPixmap(delete_map);
    delete_lb->setToolTip("删除选中的记录");
    connect(delete_lb, SIGNAL(clicked()), this, SLOT(button_clicked()));

    QHBoxLayout *button_layout = new QHBoxLayout();
    button_layout->setMargin(0);
    button_layout->setSpacing(0);
    button_layout->addWidget(add_lb);
    button_layout->addWidget(delete_lb);
    button_layout->addSpacing(40);
    QWidget* button_wg = new QWidget();
    button_wg->setFixedHeight(add_lb->height());
    button_wg->setLayout(button_layout);

    MyTable * info_widget = new MyTable(tables_titles.at(current_index), sqlHelper, current_index);
    info_widget->setDialogTitles(dialog_tables_titles.at(current_index));
    info_widget->setSubject(table_subjects.at(current_index));
    info_widget->setMustIndex(must_tables_index.at(current_index));
    info_widget->setDialogChoice(table_choices.at(current_index));
    info_widget->setAttachmentIndex(attachment_index.value(current_index));
    info_widget->setClassName(class_label->text());
    info_widget->setParams(system_class_params.value(class_label->text()));

    center_table_list.insert(current_index, info_widget);

    center_ = new QVBoxLayout();
    center_->setMargin(0);
    center_->setSpacing(0);
    center_->addLayout(menu_head_layout);
    center_->addSpacing(7);
    center_->addWidget(button_wg,0, Qt::AlignRight);
    center_->addSpacing(7);
    center_->addWidget(info_widget);


    QWidget *center_wg = new QWidget();
    center_wg->setLayout(center_);
    center_wg->setStyleSheet("background-color:#ffffff");

    center_layout = new QHBoxLayout();
    center_layout->setMargin(0);
    center_layout->setSpacing(0);
    center_layout->addSpacing(20);
    center_layout->addWidget(center_wg);
    center_layout->addSpacing(20);

}

void MainWindow::setMainWindowLayout()
{
    QHBoxLayout* head_layout = new QHBoxLayout();
    head_layout->setMargin(0);
    head_layout->setSpacing(0);
    head_layout->addSpacing(20);
    head_layout->addWidget(logo_head_lb);
    //    head_layout->addSpacing(30);
    head_layout->addWidget(info_manage_head_wg);
    head_layout->addStretch();
    head_layout->addWidget(class_label_);
    head_layout->addWidget(class_label);
    head_layout->addStretch();
    head_layout->addWidget(add_data_btn);
    head_layout->addWidget(open_data_btn);
    head_layout->addWidget(save_data_btn);
    head_layout->addSpacing(30);
    QWidget *head_wg = new QWidget();
    head_wg->setStyleSheet("background-color:#ffffff;");
    head_wg->setFixedHeight(logo_head_lb->height());
    head_wg->setLayout(head_layout);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(head_wg);
    layout->addSpacing(20);
    layout->addLayout(center_layout);
    layout->addSpacing(20);

    setLayout(layout);
}

// 初始化原始的菜单按钮
void MainWindow::InitCenterLables()
{
    foreach(ClickableLabel * label, center_label_list) {
        label->setStyleSheet("background-color:#e1e1e1;"
                             "font-size:14px;"
                             "color:#666;");
    }
}

// 选中某一个菜单后
void MainWindow::setClickedLabel(int index)
{
    InitCenterLables();
    QLabel *label = center_label_list.at(index);
    label->setStyleSheet("background-color:#ffffff;"
                         "font-size:14px;"
                         "color:#333;");
}

void MainWindow::showDialogClassName()
{
    if(class_data_dialog == nullptr) {
        class_data_dialog = new QDialog(this);
        class_data_dialog->setFixedSize(400, 300);
        class_data_dialog->setWindowFlags(class_data_dialog->windowFlags() & ~Qt::WindowType::WindowContextHelpButtonHint);
        QVBoxLayout *content_layout = new QVBoxLayout();

        QLabel *title = new QLabel("请选择该收集数据的系统大类类别");
        title->setStyleSheet("font-size:18px;"
                             "color:#000;");
        title->setFixedHeight(30);

        class_combobox = new QComboBox();
        class_combobox->setFixedSize(200,36);
        foreach(QString class_name, system_class_params.keys()) {
            class_combobox->addItem(class_name);
        }

        QPushButton *confirm_btn = new QPushButton("确认");
        confirm_btn->setStyleSheet("border:1px solid #666");
        confirm_btn->setFixedSize(200,36);
        connect(confirm_btn, SIGNAL(clicked()), this, SLOT(dialog_class_confirm()));

        content_layout->addStretch();
        content_layout->addWidget(title, 0, Qt::AlignHCenter);
        content_layout->addSpacing(40);
        content_layout->addWidget(class_combobox, 0, Qt::AlignHCenter);
        content_layout->addSpacing(20);
        content_layout->addWidget(confirm_btn, 0, Qt::AlignHCenter);
        content_layout->addStretch();

        QWidget *widget = new QWidget();
        widget->setStyleSheet("background-color:#ffffff;");
        widget->setLayout(content_layout);

        QVBoxLayout *layout = new QVBoxLayout();
        layout->setMargin(0);
        layout->addWidget(widget);
        class_data_dialog->setLayout(layout);
        class_data_dialog->setModal(true);
    }

    if(isAddDataBase) {
        class_data_dialog->setWindowTitle("新增数据库选择大类");
    } else {
        class_data_dialog->setWindowTitle("打开数据库选择大类");
    }

    class_data_dialog->show();
}

void MainWindow::chooseDataBase()
{

    QFileDialog *file_dialog = new QFileDialog(this,"请选择要打开的数据库",
                                               "D:/tools/QT/work/release/",
                                               "DataBase(*.db)");
    if(file_dialog->exec() == QFileDialog::Accepted) {
        database_path = file_dialog->selectedFiles()[0];
        qDebug() << database_path;
        if(sqlHelper->openDB(database_path,"root","123456")) {
            center_table_list.value(current_index)->updateAllTable();
        } else {
            QMessageBox::warning(this,"数据库打开失败","数据库打开失败");
        }
    } else {
        file_dialog->close();
    }
}

void MainWindow::saveDataBase()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, tr("导出数据库.db另存为"),
                                            "D:/" + class_label->text() + ".db", tr("data(*.db)"));
    if (!fileName.isNull())
    {
        //fileName是文件名
        qDebug() << "save file path : " << fileName;

        QFile save_file(fileName);
        if(save_file.exists()) {
            // 文件已经存在，则删除原始文件
            save_file.remove();
        }

        QFile file(database_path);
        if (file.copy(fileName)) {
            QMessageBox::information(this, "提示", "保存成功!");
        } else {
            QMessageBox::critical(this, "错误", "保存失败!");
        }
    }
}

void MainWindow::saveExcel()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, tr("导出Excel另存为"),
                                            "D:/" + class_label->text() + ".xlsx", tr("excel(*.xlsx)"));
    if (!fileName.isNull())
    {
        //fileName是文件名
        qDebug() << "save file path : " << fileName;

        QFile save_file(fileName);
        if(save_file.exists()) {
            // 文件已经存在，则删除原始文件
            save_file.remove();
        }

        initDoTaskThread();
        doTaskThread->setFileName(fileName);
        doTaskThread->setType(DoTaskThread::SaveExcel);
        doTaskThread->setClassName(class_label->text());
        doTaskThread->start();

    }
}

void MainWindow::saveAttachment()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, tr("导出附件"),
                                            "D:/附件.zip", tr("zip(*.zip)"));
    if (!fileName.isNull())
    {
        //fileName是文件名
        // qDebug() << "save file path : " << fileName;

        QFile save_file(fileName);
        if(save_file.exists()) {
            // 文件已经存在，则删除原始文件
            save_file.remove();
        }

        initDoTaskThread();
        doTaskThread->setFileName(fileName);
        doTaskThread->setType(DoTaskThread::SaveAttachment);
        doTaskThread->setClassName(class_label->text());
        doTaskThread->start();

    }
}

void MainWindow::saveAs()
{
    if(save_as_dialog == nullptr) {
        save_as_dialog = new QDialog(this);
        save_as_dialog->setWindowFlags(save_as_dialog->windowFlags() & ~Qt::WindowType::WindowContextHelpButtonHint);
        save_as_dialog->setWindowTitle("数据另存为");
        save_as_dialog->setFixedSize(400, 300);
        QVBoxLayout *content_layout = new QVBoxLayout();
        content_layout->setMargin(0);
        content_layout->setSpacing(0);

        ClickableLabel *excel_lb = new ClickableLabel("导出为Excel");
        excel_lb->setObjectName("excel");
        excel_lb->setStyleSheet("background-color:#efefef;"  // fcfcfc
                                "font-size:18px;"
                                "color:#000;");
        excel_lb->setAlignment(Qt::AlignCenter);
        excel_lb->setFixedSize(400,50);

        ClickableLabel *database_lb = new ClickableLabel("导出为数据库.db");
        database_lb->setObjectName("database");
        database_lb->setStyleSheet("background-color:#fcfcfc;"  // fcfcfc
                                   "font-size:18px;"
                                   "color:#000;");
        database_lb->setAlignment(Qt::AlignCenter);
        database_lb->setFixedSize(400,50);

        ClickableLabel *attachment_lb = new ClickableLabel("导出该表中所有附件");
        attachment_lb->setObjectName("attachment");
        attachment_lb->setStyleSheet("background-color:#efefef;"  // fcfcfc
                                     "font-size:18px;"
                                     "color:#000;");
        attachment_lb->setAlignment(Qt::AlignCenter);
        attachment_lb->setFixedSize(400,50);

        connect(excel_lb, SIGNAL(clicked()), this, SLOT(dialog_save_as()));
        connect(database_lb, SIGNAL(clicked()), this, SLOT(dialog_save_as()));
        connect(attachment_lb, SIGNAL(clicked()), this, SLOT(dialog_save_as()));

        content_layout->addStretch();
        content_layout->addWidget(excel_lb, 0, Qt::AlignHCenter);
        //        content_layout->addSpacing(20);
        content_layout->addWidget(database_lb, 0, Qt::AlignHCenter);
        //        content_layout->addSpacing(20);
        content_layout->addWidget(attachment_lb, 0, Qt::AlignHCenter);
        content_layout->addStretch();

        QWidget *widget = new QWidget();
        widget->setStyleSheet("background-color:#ffffff;");
        widget->setLayout(content_layout);

        QVBoxLayout *layout = new QVBoxLayout();
        layout->setMargin(0);
        layout->addWidget(widget);
        save_as_dialog->setLayout(layout);
        save_as_dialog->setModal(true);
    }

    save_as_dialog->show();
}

void MainWindow::initDoTaskThread()
{
    if(doTaskThread == nullptr) {
        if(progress_dialog == nullptr) {
            progress_dialog = new QProgressDialog("正在导出...", "取消",0,100,this);
            progress_dialog->setFixedWidth(300);
            progress_dialog->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
            progress_dialog->setWindowTitle("导出成Excel");
            progress_dialog->setModal(true);
        }
        doTaskThread = new DoTaskThread(this, progress_dialog);
        doTaskThread->setSQLHelper(sqlHelper);
        doTaskThread->setTableSubject(table_subjects);
        doTaskThread->setDialogTableTitle(dialog_tables_titles);
        doTaskThread->setMustTableIndex(must_tables_index);
        connect(progress_dialog,SIGNAL(canceled()),doTaskThread,SLOT(cancel()));
        connect(doTaskThread, SIGNAL(save_success(QString)),this,SLOT(saveSuccess(QString)));
        connect(doTaskThread, SIGNAL(save_error(QString)),this,SLOT(saveError(QString)));
    }
}

QList<QString> MainWindow::getTableCondations(int current_index)
{
    QList<QString> list;
    if(current_index == 1) {
        // 获取 table0 中选中的厂商名称
        return center_table_list.value(0)->getKeyCondations();
    } else if (current_index == 2) {
        // 获取 table1 中选中的设备名称
        if(center_table_list.keys().contains(1))
            return center_table_list.value(1)->getKeyCondations();
        else
            return list;
    } else if (current_index == 3) {
        // 获取 table1 中选中的设备名称
        if(center_table_list.keys().contains(1))
            return center_table_list.value(1)->getKeyCondations();
        else
            return list;
    } else if (current_index == 5) {
        // 获取 table1 中选中的设备名称
        if(center_table_list.keys().contains(1)) {
            foreach(QString condation, center_table_list.value(1)->getKeyCondations()) {
                list.append(condation);
            }
        }

        // 获取 table2 中选中的部件名称
        if(center_table_list.keys().contains(2)) {
            foreach(QString condation, center_table_list.value(2)->getKeyCondations()) {
                list.append(condation);
            }
        }
        // 获取 table3 中选中的零配件名称
        if(center_table_list.keys().contains(3)) {
            foreach(QString condation, center_table_list.value(3)->getKeyCondations()) {
                list.append(condation);
            }
        }
        return list;
    } else if (current_index == 6) {

        // 获取 table1 中选中的设备名称
        if(center_table_list.keys().contains(1)) {
            foreach(QString condation, center_table_list.value(1)->getKeyCondations()) {
                list.append(condation);
            }
        }

        // 获取 table2 中选中的部件名称
        if(center_table_list.keys().contains(2)) {
            foreach(QString condation, center_table_list.value(2)->getKeyCondations()) {
                list.append(condation);
            }
        }
        return list;
    }
    return list;
}

void MainWindow::center_label_clicked()
{
    ClickableLabel *label = (ClickableLabel*) sender();
    last_index = current_index;
    current_index = label->objectName().toInt();
    if(last_index == current_index) return;
    setClickedLabel(current_index);

    qDebug() << "click label at " << current_index;
    QList<QString> condations = getTableCondations(current_index);

    qDebug() << "condations : " << condations;

    if(center_table_list.keys().contains(current_index)) {
        qDebug() << "contain ..." << current_index << ", last_index : " << last_index;
        center_table_list.value(last_index)->setVisible(false);
        center_table_list.value(current_index)->setVisible(true);
        center_table_list.value(current_index)->setCondations(condations);
        center_table_list.value(current_index)->updateAllTable();
        center_->replaceWidget(center_table_list.value(last_index),
                               center_table_list.value(current_index));
    } else {
        qDebug() << "not contain ..." << current_index;
        MyTable * info_widget = new MyTable(tables_titles.at(current_index), sqlHelper, current_index);
        info_widget->setDialogTitles(dialog_tables_titles.at(current_index));
        info_widget->setSubject(table_subjects.at(current_index));
        info_widget->setDialogChoice(table_choices.at(current_index));
        info_widget->setMustIndex(must_tables_index.at(current_index));
        info_widget->setCondations(condations);
        info_widget->setAttachmentIndex(attachment_index.value(current_index));
        info_widget->updateAllTable();
        info_widget->setClassName(class_label->text());
        info_widget->setParams(system_class_params.value(class_label->text()));
        center_table_list.insert(current_index, info_widget);
        center_table_list.value(last_index)->setVisible(false);
        center_->replaceWidget(center_table_list.value(last_index),
                               info_widget);
    }

}

void MainWindow::button_clicked()
{
    QWidget * button = (QWidget*)sender();
    QString object_name = button->objectName();
    qDebug() << "object_name : " << object_name;

    if(object_name == "add_btn") {
        //        qDebug() << "click button add_btn ";
        QList<QString> titles = dialog_tables_titles.at(current_index);
        ItemInfoDialog *item_dialog = new ItemInfoDialog("新增"+table_subjects.at(current_index),
                                                         titles,table_choices.at(current_index),
                                                         attachment_index.value(current_index), this);
        connect(item_dialog, SIGNAL(add_item_success()),this,SLOT(add_item_success_slot()));
        item_dialog->setAttribute(Qt::WA_DeleteOnClose);  // 关闭的时候释放资源
        item_dialog->setMustIndex(must_tables_index.at(current_index));
        item_dialog->setSQLiteHelper(sqlHelper);
        item_dialog->setCurrentTab(current_index);
        item_dialog->updateCombox();
        item_dialog->setIsAdd(true);
        item_dialog->setClassName(class_label->text());
        item_dialog->updateParams(system_class_params.value(class_label->text()));
        item_dialog->setModal(true);
        item_dialog->show();
    } else if (object_name == "delete_btn") {
        MyTable *info_table = center_table_list.value(current_index);
        if (info_table->getSelectRows().size() == 0) {
            QMessageBox::warning(this,"警告","请选择要删除的条目！");
            return;
        }
        QMessageBox::StandardButton reply =
                QMessageBox::question(this, tr("询问"),"确认删除吗？",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
            info_table->deleteSelectRows();
    } else if (object_name == "add_data") {
        isAddDataBase = true;
        showDialogClassName();
    }else if (object_name == "open_data") {
        isAddDataBase = false;
        showDialogClassName();
    }else if (object_name == "save_data") {
        saveAs();
    }
}

void MainWindow::add_item_success_slot()
{
    //    qDebug() << "add_item_success_slot ...";
    this->center_table_list.value(current_index)->updateAllTable();
    QMessageBox::information(this, "提示", "新增成功");
}

void MainWindow::dialog_class_confirm() {
    QString select = class_combobox->currentText();
    class_label->setText(select);
    //    qDebug() << select;
    class_data_dialog->close();

    if(!isAddDataBase) {
        chooseDataBase();
        return;
    }

    QDir dir(QDir::currentPath());  // 运行路径
    if(!dir.exists(class_label->text().trimmed())) {
        dir.mkdir(class_label->text().trimmed());
        if(!dir.exists(class_label->text().trimmed() + "/附件"))
            dir.mkdir(class_label->text().trimmed() + "/附件");
    }

    QString data_path = QDir::currentPath() + "/" + class_label->text().trimmed()+"/";
    database_path = data_path + class_label->text().trimmed() + "_" +
            QUuid::createUuid().toString().remove("{").remove("}") + ".db";
    //    qDebug() << "new database path : " << database_path;
    sqlHelper->initDB(database_path);
    center_table_list.value(current_index)->updateAllTable();
}

void MainWindow::dialog_save_as()
{
    QString objectName = ((ClickableLabel*) sender())->objectName();
    qDebug() << "save type ：" << objectName;
    save_as_dialog->close();
    if(objectName == "excel") {
        saveExcel();
    } else if(objectName == "database") {
        saveDataBase();
    } else if(objectName == "attachment") {
        saveAttachment();
    }
}

void MainWindow::saveSuccess(QString message)
{
    QMessageBox::information(this, "提示", message);
}

void MainWindow::saveError(QString message)
{
    QMessageBox::warning(this, "警告", message);
}

