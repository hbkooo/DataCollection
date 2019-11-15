#include "iteminfodialog.h"

ItemInfoDialog::ItemInfoDialog(QString subject, QList<QString>titles, QMap<int, QList<QString>> choices,
                               int attachmentIndex, QWidget *parent)
    : QDialog(parent), subject(subject), titles(titles), choices(choices),
      attachmentIndex(attachmentIndex)
{
    this->parent_widget = parent;
    this->setWindowTitle(subject);
    this->setWindowFlags(this->windowFlags()
                         &~Qt::WindowContextHelpButtonHint);
    this->setStyleSheet("background-color:#ffffff;");
    per_column_rows = 13;
    widget_height = 700;
    label_width = 150;
    contruct_dialog();
}

ItemInfoDialog::~ItemInfoDialog()
{
    qDebug() << "free space of item info dialog ...";
    sqlHelper = nullptr;
}

// 更新界面数据
void ItemInfoDialog::update_dialog(data_struct item)
{
    this->item = item;
    QList<QString> item_info = item.item_info;
    if(item_info.size() != titles.size()) {
        qDebug() << "数据维度不一致!! ===> "
                 << "items.size() : " << item_info.size()
                 << " != titles.size : " << titles.size();
        //        return;
    }

    if(current_tab==5){
        box_changed(item_info.at(4));
    } else if(current_tab == 6) {
        box_changed(item_info.at(1));
    }

    for(int i = 0; i < item_info.size(); i++) {
        if (i >= titles.size()) break;
        if(line_edits.keys().contains(i)) {
            line_edits.value(i)->setText(item_info.at(i));
        } else if(comboxes.keys().contains(i)) {
            comboxes.value(i)->setCurrentText(item_info.at(i));
        } else if(i == attachmentIndex) {
            attachment_et->setText(item_info.at(i));
        }

    }

}

void ItemInfoDialog::updateCombox()
{
    qDebug() << "update combox..." << current_tab;
    if(current_tab == 1) {
        QList<QString> items = sqlHelper->getNamesInfo(sqlHelper->getTableNames().at(0),"firm_name");
        //        qDebug() << items;
        if(!comboxes.keys().contains(1)) return;
        QComboBox *box = comboxes.value(1);
        foreach(QString item, items) {
            box->addItem(item);
        }
    } else if(current_tab == 2 || current_tab == 3) {
        QList<QString> items = sqlHelper->getNamesInfo(sqlHelper->getTableNames().at(1),"device_name");
        //        qDebug() << items;
        if(!comboxes.keys().contains(1)) return;
        QComboBox *box = comboxes.value(1);
        foreach(QString item, items) {
            box->addItem(item);
        }
    } else if(current_tab == 5) {
        if(!comboxes.keys().contains(4)) return;
        QComboBox *box_belong = comboxes.value(4);
        connect(box_belong, SIGNAL(activated(const QString &)),
                this, SLOT(box_changed(const QString &)));
        box_changed("设备");
    } else if(current_tab == 6) {
        QList<QString> items = sqlHelper->getNamesInfo(sqlHelper->getTableNames().at(1),"device_name");
        if(!comboxes.keys().contains(1)) return;
        QComboBox *box = comboxes.value(1);
        foreach(QString item, items) {
            box->addItem(item);
        }
        connect(box, SIGNAL(activated(const QString &)),
                this, SLOT(box_changed(const QString &)));
        if(items.size() > 0)
            box_changed(items.at(0));
    }

}

void ItemInfoDialog::updateParams(QMap<int, QList<QString>> params)
{
    QList<QString> all_params = params.value(current_tab);

    qDebug() << "update params all_param : " << all_params
             << ", params : " << params
             <<", current_tab : " << current_tab;

    int offset = 15;
    if(current_tab == 1) offset = 15;
    else if(current_tab == 2) offset = 18;
    else if(current_tab == 3) offset = 20;
    else if(current_tab == 4) offset = 6;


    for(int i = 0; i < all_params.size(); i++) {
        line_edits.value(i+offset)->setPlaceholderText(all_params.at(i));

    }


}

QString ItemInfoDialog::copyAttachment(QString filePath)
{
    //    qDebug() << filePath.split('/');
    if(filePath.split("/").size() == 1) {
        // 该处表明是修改附件，附件文件夹中已经存在该附件，所以直接返回该附件的名字
        QFile temp(QDir::currentPath() + "/" + class_name+"/附件/"+filePath);
        if(temp.exists())
            return filePath;
        else
            return "";
    }
    QString file_name = filePath.split('/').last();
    QDir dir(QDir::currentPath() + "/" + class_name);  // 运行路径
    if(!dir.exists("附件")) {
        dir.mkdir("附件");
    }
    QString save_file_path = QDir::currentPath() + "/" + class_name + "/附件/";
    QString save_file_name = save_file_path + file_name;
    for(int i = 0; ;i++) {
        QFile save_file(save_file_name);
        if(!save_file.exists())  break;
        // 该文件已经存在，重新构造文件名
        save_file_name = save_file_path + file_name.split(".").first() + QString::number(i)
                + "." + file_name.split(".").last();
    }

    // qDebug() << "保存路径 : " << save_file_name;

    QFile file(filePath);
    if (file.copy(save_file_name)) {
        return save_file_name;
    } else {
        return "";
    }
}

void ItemInfoDialog::contruct_dialog()
{
    if(titles.size()>per_column_rows) {
        contruce_two_column_dialog();
    }else {
        contruct_one_column_dialog();
    }
}

void ItemInfoDialog::contruct_one_column_dialog()
{
    this->setFixedSize(600, widget_height);
    QLabel* info_title = new QLabel(subject);
    info_title->setStyleSheet("font-size:16px;color:#333;");

    QPixmap map(":/image/close");
    map = map.scaled(30,30);
    ClickableLabel* close_lb = new ClickableLabel();
    close_lb->setObjectName("close");
    close_lb->setPixmap(map);
    connect(close_lb,SIGNAL(clicked()),this,SLOT(btn_clicked()));

    QHBoxLayout *head_layout = new QHBoxLayout();
    head_layout->setMargin(0);
    head_layout->setSpacing(0);
    head_layout->addSpacing(20);
    head_layout->addWidget(info_title);
    head_layout->addStretch();
    head_layout->addWidget(close_lb,0,Qt::AlignVCenter);
    head_layout->addSpacing(20);
    QWidget *head_widget = new QWidget();
    head_widget->setFixedHeight(50);
    head_widget->setLayout(head_layout);

    QWidget *split = new QWidget();
    split->setFixedHeight(1);
    split->setStyleSheet("background-color:#e5e5e5;");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(0);
    //    layout->addWidget(head_widget);
    layout->addSpacing(10);
    layout->addWidget(split);
    layout->addSpacing(20);
    layout->addSpacing(20);

    for(int i = 0; i < titles.size(); i++) {
        QHBoxLayout *item_layout = new QHBoxLayout();
        item_layout->setMargin(0);
        item_layout->setSpacing(0);

        QLabel *label = new QLabel(titles.at(i));
        label->setFixedSize(label_width, 32);
        label->setAlignment(Qt::AlignVCenter);
        label->setStyleSheet("font-size:14;");


        item_layout->addStretch();
        item_layout->addWidget(label);

        if(choices.keys().contains(i)){
            QComboBox * combox = new QComboBox();
            combox->setFixedSize(300,32);
            QList<QString> choice = choices.value(i);
            foreach(QString item, choice){
                combox->addItem(item);
            }
            comboxes.insert(i, combox);
            item_layout->addWidget(combox);
        } else if(i != attachmentIndex) {
            QLineEdit *edit = new QLineEdit();
            edit->setFixedSize(300,32);
            edit->setStyleSheet("background-color:#f9f9f9;");// #f9f9f9
            line_edits.insert(i, edit);
            item_layout->addWidget(edit);
        } else {
            attachment_et = new QLineEdit();
            attachment_et->setPlaceholderText("选择附件(多个附件之间用','隔开)");
            attachment_et->setObjectName("attachment_lb");
            attachment_et->setFixedSize(270,32);
            attachment_et->setStyleSheet("border:1px solid black;"
                                         "background-color:#f9f9f9;");// #f9f9f9
            QPushButton *file_btn = new QPushButton();
            QIcon icon(":/image/dot");
            file_btn->setObjectName("file_btn");
            file_btn->setFlat(true);
            file_btn->setIconSize(QSize(20,20));
            file_btn->setIcon(icon);
            file_btn->setStyleSheet("border:1px none black;");

            connect(file_btn, SIGNAL(clicked()), this, SLOT(choose_attachment()));
            item_layout->addWidget(attachment_et);
            item_layout->addSpacing(6);
            item_layout->addWidget(file_btn);
        }

        item_layout->addStretch();
        layout->addLayout(item_layout);
        layout->addSpacing(10);
    }

    ClickableLabel *cancle_btn = new ClickableLabel("取消");
    cancle_btn->setObjectName("cancel");
    cancle_btn->setAlignment(Qt::AlignCenter);
    cancle_btn->setFixedSize(80,36);
    cancle_btn->setStyleSheet("font-size:14;"
                              "border:1px solid #ebebeb;");
    connect(cancle_btn,SIGNAL(clicked()),this,SLOT(btn_clicked()));

    ClickableLabel *ok_btn = new ClickableLabel("确定");
    ok_btn->setObjectName("ok");
    ok_btn->setAlignment(Qt::AlignCenter);
    ok_btn->setFixedSize(80,36);
    ok_btn->setStyleSheet("font-size:14;"
                          "border:1px solid #343434;");
    connect(ok_btn,SIGNAL(clicked()),this,SLOT(btn_clicked()));

    QHBoxLayout *btn_layout = new QHBoxLayout();
    btn_layout->addStretch();
    btn_layout->addWidget(cancle_btn);
    btn_layout->addSpacing(10);
    btn_layout->addWidget(ok_btn);
    btn_layout->addSpacing(60);

    layout->addStretch();
    layout->addLayout(btn_layout);
    layout->addSpacing(40);
    setLayout(layout);
}

void ItemInfoDialog::contruce_two_column_dialog()
{
    this->setFixedSize(1100,widget_height);
    QLabel* info_title = new QLabel(subject);
    info_title->setStyleSheet("font-size:16px;color:#333;");

    QPixmap map(":/image/close");
    map = map.scaled(30,30);
    ClickableLabel* close_lb = new ClickableLabel();
    close_lb->setObjectName("close");
    close_lb->setPixmap(map);
    connect(close_lb,SIGNAL(clicked()),this,SLOT(btn_clicked()));

    QHBoxLayout *head_layout = new QHBoxLayout();
    head_layout->setMargin(0);
    head_layout->setSpacing(0);
    head_layout->addSpacing(20);
    head_layout->addWidget(info_title);
    head_layout->addStretch();
    head_layout->addWidget(close_lb,0,Qt::AlignVCenter);
    head_layout->addSpacing(20);
    QWidget *head_widget = new QWidget();
    head_widget->setFixedHeight(50);
    head_widget->setLayout(head_layout);

    QWidget *split = new QWidget();
    split->setFixedHeight(1);
    split->setStyleSheet("background-color:#e5e5e5;");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(0);
    //    layout->addWidget(head_widget);
    layout->addSpacing(10);
    layout->addWidget(split);
    layout->addSpacing(20);
    layout->addSpacing(20);

    int index = 0;
    QVBoxLayout *col1_layout = new QVBoxLayout();
    col1_layout->setMargin(0);
    col1_layout->setSpacing(0);
    for(; index < per_column_rows; index++) {
        QHBoxLayout *item_layout = new QHBoxLayout();
        item_layout->setMargin(0);
        item_layout->setSpacing(0);

        QLabel *label = new QLabel(titles.at(index));
        label->setFixedSize(label_width, 32);
        label->setAlignment(Qt::AlignVCenter);
        label->setStyleSheet("font-size:14;");

        item_layout->addStretch();
        item_layout->addWidget(label);

        if(choices.keys().contains(index)){
            QComboBox * combox = new QComboBox();
            combox->setFixedSize(300,32);
            QList<QString> choice = choices.value(index);
            foreach(QString item, choice){
                combox->addItem(item);
            }
            comboxes.insert(index, combox);
            item_layout->addWidget(combox);
        } else if(index != attachmentIndex) {
            QLineEdit *edit = new QLineEdit();
            edit->setFixedSize(300,32);
            edit->setStyleSheet("background-color:#f9f9f9;");// #f9f9f9
            line_edits.insert(index, edit);
            item_layout->addWidget(edit);
        } else {
            attachment_et = new QLineEdit();
            attachment_et->setPlaceholderText("选择附件(多个附件之间用','隔开)");
            attachment_et->setObjectName("attachment_lb");
            attachment_et->setFixedSize(270,32);
            attachment_et->setStyleSheet("border:1px solid black;"
                                         "background-color:#f9f9f9;");// #f9f9f9
            QPushButton *file_btn = new QPushButton();
            QIcon icon(":/image/dot");
            file_btn->setObjectName("file_btn");
            file_btn->setFlat(true);
            file_btn->setIconSize(QSize(20,20));
            file_btn->setIcon(icon);
            file_btn->setStyleSheet("border:1px none black;");

            connect(file_btn, SIGNAL(clicked()), this, SLOT(choose_attachment()));
            item_layout->addWidget(attachment_et);
            item_layout->addSpacing(6);
            item_layout->addWidget(file_btn);
        }
        item_layout->addStretch();

        col1_layout->addLayout(item_layout);
        col1_layout->addSpacing(10);
    }

    QVBoxLayout *col2_layout = new QVBoxLayout();
    col2_layout->setMargin(0);
    col2_layout->setSpacing(0);
    for(; index < titles.size(); index++) {
        QHBoxLayout *item_layout = new QHBoxLayout();
        item_layout->setMargin(0);
        item_layout->setSpacing(0);

        QLabel *label = new QLabel(titles.at(index));
        label->setFixedSize(100, 32);
        label->setAlignment(Qt::AlignVCenter);
        label->setStyleSheet("font-size:14;");

        item_layout->addStretch();
        item_layout->addWidget(label);

        if(choices.keys().contains(index)){
            QComboBox * combox = new QComboBox();
            combox->setFixedSize(300,32);
            QList<QString> choice = choices.value(index);
            foreach(QString item, choice){
                combox->addItem(item);
            }
            comboxes.insert(index, combox);
            item_layout->addWidget(combox);
        } else if (index != attachmentIndex) {
            QLineEdit *edit = new QLineEdit();
            edit->setFixedSize(300,32);
            edit->setStyleSheet("background-color:#f9f9f9;");// #f9f9f9
            line_edits.insert(index, edit);
            item_layout->addWidget(edit);
        } else {
            attachment_et = new QLineEdit();
            attachment_et->setPlaceholderText("选择附件(多个附件之间用','隔开)");
            attachment_et->setObjectName("attachment_lb");
            attachment_et->setFixedSize(270,32);
            attachment_et->setStyleSheet("border:1px solid black;"
                                         "background-color:#f9f9f9;");// #f9f9f9
            QPushButton *file_btn = new QPushButton();
            QIcon icon(":/image/dot");
            file_btn->setObjectName("file_btn");
            file_btn->setFlat(true);
            file_btn->setIconSize(QSize(20,20));
            file_btn->setIcon(icon);
            file_btn->setStyleSheet("border:1px none black;");

            connect(file_btn, SIGNAL(clicked()), this, SLOT(choose_attachment()));
            item_layout->addWidget(attachment_et);
            item_layout->addSpacing(6);
            item_layout->addWidget(file_btn);
        }
        item_layout->addStretch();

        col2_layout->addLayout(item_layout);
        col2_layout->addSpacing(10);
    }
    col2_layout->addStretch();

    QHBoxLayout *center_layout = new QHBoxLayout();
    center_layout->setMargin(0);
    center_layout->setSpacing(0);
    center_layout->addStretch();
    center_layout->addLayout(col1_layout);
    center_layout->addSpacing(70);
    center_layout->addLayout(col2_layout);
    center_layout->addStretch();

    layout->addLayout(center_layout);


    ClickableLabel *cancle_btn = new ClickableLabel("取消");
    cancle_btn->setObjectName("cancel");
    cancle_btn->setAlignment(Qt::AlignCenter);
    cancle_btn->setFixedSize(80,36);
    cancle_btn->setStyleSheet("font-size:14;"
                              "border:1px solid #ebebeb;");
    connect(cancle_btn,SIGNAL(clicked()),this,SLOT(btn_clicked()));

    ClickableLabel *ok_btn = new ClickableLabel("确定");
    ok_btn->setObjectName("ok");
    ok_btn->setAlignment(Qt::AlignCenter);
    ok_btn->setFixedSize(80,36);
    ok_btn->setStyleSheet("font-size:14;"
                          "border:1px solid #343434;");
    connect(ok_btn,SIGNAL(clicked()),this,SLOT(btn_clicked()));

    QHBoxLayout *btn_layout = new QHBoxLayout();
    btn_layout->addStretch();
    btn_layout->addWidget(cancle_btn);
    btn_layout->addSpacing(10);
    btn_layout->addWidget(ok_btn);
    btn_layout->addSpacing(60);

    layout->addStretch();
    layout->addLayout(btn_layout);
    layout->addSpacing(40);
    setLayout(layout);
}

void ItemInfoDialog::updateBorderOfMustLineEdit()
{
    foreach(int index, must_index) {
        if(line_edits.keys().contains(index)) {
            line_edits.value(index)->setStyleSheet("border:1px solid red;"
                                                   "background-color:#f9f9f9;");
        } else if(comboxes.keys().contains(index)) {
            comboxes.value(index)->setStyleSheet("border:1px solid red;"
                                                 "background-color:#f9f9f9;");
        }
    }
}

QList<QString> ItemInfoDialog::checkMustIndex()
{
    QList<QString> item_info;
    for(int i = 0; i < titles.size(); i++) {
        QString text = "";
        if(comboxes.keys().contains(i)) {
            text = comboxes.value(i)->currentText();
        } else if(line_edits.keys().contains(i)) {
            text = line_edits.value(i)->text();
        } else if(i == attachmentIndex) {
            text = attachment_et->text();
            QList<QString> files;
            foreach(QString file, text.split(',')) {
                QString file_path = copyAttachment(file);
                if (file_path != "") {
                    files.append(file_path.split('/').last());
                }
            }
            text = "";
            if(files.size() >= 1) {
                text = files.at(0);
            }
            for(int i = 1; i < files.size(); i++) {
                text = text + "," + files.at(i);
            }
        }
        if(must_index.contains(i)) {
            if(text == "") {
                item_info.clear();
                return item_info;
            }
        }
        item_info.append(text);
    }

    qDebug() << item_info.size();

    return item_info;
}

bool ItemInfoDialog::checkBelongParts(QString belong_parts)
{
    bool isSuccess1 = sqlHelper->isExit(sqlHelper->getTableNames().at(1), "device_name", belong_parts);
    bool isSuccess2 = sqlHelper->isExit(sqlHelper->getTableNames().at(2), "component_name", belong_parts);
    bool isSuccess3 = sqlHelper->isExit(sqlHelper->getTableNames().at(3), "sparepart_name", belong_parts);
    return isSuccess1 || isSuccess2 || isSuccess3;
}

bool ItemInfoDialog::checkCondation(QList<QString> item_info)
{
    if(current_tab == 1) {
        // 设备信息的约束条件,厂商信息表中必须存在该信息才可以添加
        QString condation = "firm_name='" + item_info.at(1) + "' ";
        int count = sqlHelper->getInfoNumber(sqlHelper->getTableNames().at(0),
                                             condation);
        if(count == 0) {
            QMessageBox::warning(this, "警告", "所填的厂商名称在厂商信息表中不存在");
            return false;
        } else
            return true;
    }
    if (current_tab == 2 || current_tab == 3) {
        // 部件信息或零配件信息的约束条件,设备信息表中必须存在该信息才可以
        QString condation = "device_name='" + item_info.at(1) + "' ";
        int count = sqlHelper->getInfoNumber(sqlHelper->getTableNames().at(1),
                                             condation);
        if(count == 0) {
            QMessageBox::warning(this, "警告", "所填的设备名称在设备信息表中不存在");
            return false;
        } else
            return true;
    }
    if(current_tab == 5) {
        // TODO 判断 belong_parts(所属设备/部件/零配件) 是否已经存在
        if(!checkBelongParts(item_info.at(5))) {
            QMessageBox::warning(this, "警告", "所填的'所属设备/部件/零配件'不存在");
            return false;
        } else {
            return true;
        }
    }
    if(current_tab == 6) {
        // 维修信息的约束条件,部件信息表中必须存在该信息才可以
        QString condation = "device_name='" + item_info.at(1) + "' and component_name='"
                + item_info.at(2) + "'";
        int count = sqlHelper->getInfoNumber(sqlHelper->getTableNames().at(2),
                                             condation);
        if(count == 0) {
            QMessageBox::warning(this, "警告", "所填的部件和设备名在部件信息表中不存在");
            return false;
        } else {
            return true;
        }
    }
    return true;
}

void ItemInfoDialog::btn_clicked()
{
    ClickableLabel* label = (ClickableLabel*) sender();
    QString object_name = label->objectName();
    if(object_name == "close") {
        this->close();
    } else if (object_name == "cancel") {
        this->close();
    } else if (object_name == "ok") {

        QList<QString> item_info = checkMustIndex();

        if(item_info.size() == 0) {
            QMessageBox::warning(this, "警告", "请检查必填项是否填写");
            return;
        }
        // 判断输入的信息是否满足条件
        if(!checkCondation(item_info)) return;

        if (isAdd) {
            if(sqlHelper == nullptr) return;
            if(current_tab == 1 && sqlHelper->isExit(sqlHelper->getTableNames().at(1), "device_name", item_info.at(0))) {
                QMessageBox::warning(parent_widget, "警告", "设备名称需要唯一，表中已经存在该名称");
                return;
            }
            if(current_tab == 2 && sqlHelper->isExit(sqlHelper->getTableNames().at(2), "component_name", item_info.at(0))) {
                QMessageBox::warning(parent_widget, "警告", "部件名称需要唯一，表中已经存在该名称");
                return;
            }
            if(current_tab == 3 && sqlHelper->isExit(sqlHelper->getTableNames().at(3), "sparepart_name", item_info.at(0))) {
                QMessageBox::warning(parent_widget, "警告", "零配件名称需要唯一，表中已经存在该名称");
                return;
            }
            // 表中物品名称要唯一
            if(current_tab == 4 && sqlHelper->isExit(sqlHelper->getTableNames().at(4), "object_name", item_info.at(0))) {
                QMessageBox::warning(parent_widget, "警告", "物品名称需要唯一，表中已经存在该名称");
                return;
            }

            if(sqlHelper->insertItemInfo(
                        sqlHelper->getTableNames().at(current_tab), item_info)) {
                qDebug() << "新增成功";
                this->close();
                emit add_item_success();
            } else {
                this->close();
                QMessageBox::warning(parent_widget, "警告", "新增失败...");
            }
        } else {
            item.item_info = item_info;
            if(sqlHelper != nullptr && sqlHelper->updateItemInfo(
                        sqlHelper->getTableNames().at(current_tab), item)) {
                qDebug() << "更新成功";
                this->close();
                emit change_item_success();
            } else {
                this->close();
                QMessageBox::warning(parent_widget, "警告", "更新失败...");
            }
        }
    }
}

// 点击级联效果，数据更新
void ItemInfoDialog::box_changed(const QString & item)
{
    qDebug() << "clicked " << item;
    if(current_tab == 5) {
        if(!comboxes.keys().contains(5)) return;
        QComboBox *box = comboxes.value(5);
        box->clear();
        QList<QString> items;
        if(item == "设备")
            items = sqlHelper->getNamesInfo(sqlHelper->getTableNames().at(1), "device_name");
        else if(item == "部件")
            items = sqlHelper->getNamesInfo(sqlHelper->getTableNames().at(2), "component_name");
        else if(item == "零配件")
            items = sqlHelper->getNamesInfo(sqlHelper->getTableNames().at(3), "sparepart_name");
        foreach(QString d, items) {
            box->addItem(d);
        }
    } else if(current_tab == 6) {
        if(!comboxes.keys().contains(2)) return;
        QComboBox *box = comboxes.value(2);
        box->clear();
        QList<QString> items;
        items = sqlHelper->getNamesInfo(sqlHelper->getTableNames().at(2), "component_name",
                                        "device_name='"+item+"'");
        foreach(QString d, items) {
            box->addItem(d);
        }
    }


}

void ItemInfoDialog::choose_attachment()
{
    qDebug() << "click attachment ...";
    QString file_path;
    QFileDialog *file_dialog = new QFileDialog(this,"请选择附件",
                                               "D:/tools/QT/work/release/",
                                               "file(*.*)");
    if(file_dialog->exec() == QFileDialog::Accepted) {
        file_path = file_dialog->selectedFiles()[0];
        QString files = attachment_et->text().trimmed();
        if(files != "") {
            files = files + "," + file_path;
        } else {
            files = file_path;
        }
        attachment_et->setText(files);
    } else {
        file_dialog->close();
    }
}

