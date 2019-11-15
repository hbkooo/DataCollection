#include "mytable.h"

MyTable::MyTable(QList<QString> titles, SQLiteHelper *sqlHelper, int current_tab, QWidget *parent)
    : QWidget(parent), titles(titles)
{
    this->current_tab = current_tab;
    this->sqlHelper = sqlHelper;

    per_page_items = 15;

    // table view 的数据适配器
    info_model = new QStandardItemModel();
    info_model->setColumnCount(titles.size() + 2);
    info_model->setHeaderData(0,Qt::Horizontal, "选择");
    for(int i = 0;i < titles.size(); i++) {
        info_model->setHeaderData(i+1,Qt::Horizontal, titles.at(i));
    }
    info_model->setHeaderData(titles.size()+1,Qt::Horizontal, "操作");
    InitTableView();   // 初始化tableview

    initDataFromDB();  // 从数据库中获取该表的数据信息
    InitBottomPage();  // 初始化底部的控件
    updatePageAt(1);   // 更新第一页的数据
    change_row_background(0);

    table_layout = new QVBoxLayout();
    table_layout->setMargin(0);
    table_layout->setSpacing(0);
    table_layout->setContentsMargins(20,0,20,0);
    table_layout->addWidget(table_view);
    table_layout->addSpacing(25);
    table_layout->addWidget(page_widget,0, Qt::AlignHCenter);
    table_layout->addSpacing(25);
    this->setLayout(table_layout);
}

MyTable::~MyTable()
{
    qDebug() << "free space in MyTable ...";
}

void MyTable::deleteSelectRows()
{
//    qDebug() << "delete rows : " << select_rows;
//    qSort(select_rows.begin(),select_rows.end());
//    qDebug() << "delete rows : " << select_rows;
//    qDebug() << "before delete : " << content_data.size();
//    for(int i = select_rows.size()-1; i >= 0; i--) {
//        content_data.removeAt((current_page-1)*per_page_items + i);
//    }
//    qDebug() << "after delete : " << content_data.size();

    if(sqlHelper != nullptr) {
        if(sqlHelper->deleteItemsByIDs(sqlHelper->getTableNames().at(current_tab),
                                       select_ids)) {
            qDebug() << "删除成功...";
        }
    }

    select_ids.clear();
    primary_key_condations.clear();
    updateAllTable();
}

// 数据改变，重新更变数据
void MyTable::updateAllTable()
{
    primary_key_condations_old.clear();
    foreach(QString condation, primary_key_condations){
        // 上次选中的条目
        primary_key_condations_old.append(condation);
    }
    initCondation();
    initDataFromDB();
    QWidget *last_page = page_widget;
    page_lbs.clear();  // 页码按钮清空
    InitBottomPage();
    table_layout->replaceWidget(last_page, page_widget);
    if (last_page != nullptr) {
        delete last_page;
        last_page = nullptr;
        qDebug() << "delete last_page ...";
    }
    updatePageAt(current_page);
    change_row_background(0);
}

void MyTable::addOneItem(data_struct data)
{
    //    qDebug() << "add one item ...";
    // row : 0, 1, 2, 3 ...

    QList<QString> items = data.item_info;
    QString background_color;
    int row = info_model->rowCount();

    if(row % 2 == 0) {
        background_color = "background-color:#ffffff;";// fcfcfc
    } else {
        background_color = "background-color:" + alternal_bg + ";";// ffffff
    }

    QCheckBox * check_box = new QCheckBox();
    check_box->setObjectName(QString::number(row));
    check_box->setStyleSheet(background_color);
    if(primary_key_condations_old.size() != 0) {
        if(primary_key_condations_old.contains(items.at(0))) {
            check_box->setChecked(true);
            primary_key_condations.append(items.at(0));
            select_ids.append(data.id);
        }
    }
    connect(check_box, SIGNAL(pressed()),this,SLOT(check_box_pressed()));
    connect(check_box, SIGNAL(stateChanged(int)), this, SLOT(check_box_state_changed(int)));
    info_model->setItem(row,0,new QStandardItem(""));  // 必须加上这句话，不然下面的复选框不显示
    table_view->setIndexWidget(info_model->index(row,0),check_box);

    for(int i = 0; i < info_model->columnCount()-2; i++) {
        info_model->setItem(row,i+1, new QStandardItem(items.at(i)));
        info_model->item(row,i+1)->setTextAlignment(Qt::AlignCenter);
    }
    QPixmap map(":/image/change");
    map = map.scaled(24,24);
    ClickableLabel *change_lb = new ClickableLabel();
    change_lb->setObjectName(QString::number(row));
    change_lb->setAlignment(Qt::AlignCenter);
    change_lb->setPixmap(map);
    change_lb->setStyleSheet(background_color);
    table_view->setIndexWidget(info_model->index(row,info_model->columnCount()-1),change_lb);
    connect(change_lb, SIGNAL(clicked()), this, SLOT(change_clicked()));

    table_view->update();
}

// 初始化表格
void MyTable::InitTableView()
{
    table_view = new QTableView();
    table_view->setModel(info_model);
    table_view->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);      //表头信息显示居中
//    table_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//所有列都扩展自适应宽度，填充充满整个屏幕宽度
//    table_view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );//根据列内容来定列宽
    //    table_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    table_view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);//对第0列单独设置固定宽度
    table_view->horizontalHeader()->setSectionResizeMode(titles.size()+1,QHeaderView::Fixed);
    table_view->setColumnWidth(0,40);
    table_view->setColumnWidth(titles.size()+1,100);
    table_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_view->horizontalHeader()->setStretchLastSection(true);
    table_view->horizontalHeader()->setHighlightSections(false);
    table_view->horizontalHeader()->setMinimumHeight(40);     // 表头行高
    table_view->verticalHeader()->hide();                     // 列表头不可见，即行号
    table_view->verticalHeader()->setDefaultSectionSize(39);  // 设置表格中每行的行高
    table_view->setShowGrid(false);                           // 网格不可见
    table_view->setAlternatingRowColors(true);                // 交替出现颜色
    table_view->setFrameShape(QFrame::NoFrame);
    table_view->setSelectionMode(QAbstractItemView::SingleSelection);
    table_view->setFocusPolicy(Qt::NoFocus);
    table_view->setStyleSheet("QTableView { "
                              "border: none;"
//                              "font-size:12px;"
                              "color: #666;"
                              "background-color:#ffffff;"   // fcfcfc
                              "alternate-background-color:" + alternal_bg + "; }"  // ffffff
                              "QTableView::item {"
                              "selection-background-color:" + select_bg + ";"
                              "selection-color: #666; }");
    table_view->horizontalHeader()->setStyleSheet("QHeaderView::section {"
                                                  "background-color:#ffffff;"
//                                                  "font-size:14px;"
                                                  "color:#333;"
//                                                  "border:1px none #e5e5e5;"
                                                  "border:1px solid #e5e5e5;"
                                                  "border-bottom-style:solid;}");

    connect(table_view, SIGNAL(pressed(const QModelIndex &)), this, SLOT(onTablePressed(const QModelIndex &)));

}

// 初始化表底部的翻页部件
void MyTable::InitBottomPage()
{
    ClickableLabel *next_page_lb = new ClickableLabel("下一页");
    next_page_lb->setObjectName("next_page");
    next_page_lb->setAlignment(Qt::AlignCenter);
    next_page_lb->setStyleSheet("border: 2px solid #ebebeb;");
    next_page_lb->setFixedSize(60,30);
    connect(next_page_lb, SIGNAL(clicked()),this,SLOT(page_change_clicked()));

    ClickableLabel *last_page_lb = new ClickableLabel("上一页");
    last_page_lb->setObjectName("last_page");
    last_page_lb->setAlignment(Qt::AlignCenter);
    last_page_lb->setStyleSheet("border: 2px solid #ebebeb;");
    last_page_lb->setFixedSize(60,30);
    connect(last_page_lb, SIGNAL(clicked()),this,SLOT(page_change_clicked()));

    QHBoxLayout *page_layout = new QHBoxLayout();
    page_layout->setMargin(0);
    page_layout->setSpacing(0);
    page_layout->addWidget(last_page_lb,0,Qt::AlignVCenter);

    for(int i = 1; i <= max_pages; i++) {
        ClickableLabel* page_lb = new ClickableLabel(QString::number(i));
        page_lb->setObjectName(QString::number(i));
        page_lb->setAlignment(Qt::AlignCenter);
        page_lb->setStyleSheet("border: 2px solid #ebebeb;");
        page_lb->setFixedSize(30,30);
        page_layout->addSpacing(8);
        page_layout->addWidget(page_lb,0,Qt::AlignVCenter);
        page_lbs.append(page_lb);
        connect(page_lb, SIGNAL(clicked()), this, SLOT(page_change_clicked()));
    }
    //    qDebug() << "test2 ...";
    if(current_page > max_pages) current_page = max_pages;
    if(max_pages != 0) {
        if (current_page == 0)
            current_page = 1;
        page_lbs.at(current_page-1)->setStyleSheet("background-color:#e4e4e4;");
    }

    //    qDebug() << "test3 ...";

    page_layout->addSpacing(8);
    page_layout->addWidget(next_page_lb,0,Qt::AlignVCenter);

    page_widget = new QWidget();
    page_widget->setLayout(page_layout);

    qDebug() << "Init new BottomPage ...";

}

// 从数据库中获取该表的数据信息
void MyTable::initDataFromDB()
{
    num_info_in_table = sqlHelper->getInfoNumber(sqlHelper->getTableNames().at(current_tab)
                                                 ,condation_sql);
    max_pages = num_info_in_table/per_page_items;
    if(num_info_in_table % per_page_items != 0) {
        max_pages += 1;
    }
    view_max_pages = 5;
    qDebug() << "all counts in table : " << num_info_in_table
             << ", max_pages : " << max_pages;
}

// 获取 page 页的数据 page : 1, 2, 3, 4 ...
void MyTable::initContentDataByPage(int page)
{
    content_data.clear();
    int start = (page - 1) * per_page_items;
    int end = page * per_page_items;
    if (page == max_pages) {
        end = num_info_in_table;
    }
    if (sqlHelper != nullptr) {
        content_data = sqlHelper->getInfosByRange(sqlHelper->getTableNames().at(current_tab),
                                                  start,end,condation_sql);
    }

}

// 更新page页的界面 page : 1, 2, 3, 4 ...
void MyTable::updatePageAt(int page)
{
    if(max_pages == 0) {
        select_ids.clear();
        primary_key_condations.clear();
        last_clicked_row = -1;
        info_model->removeRows(0, info_model->rowCount());
        table_view->update();
        return;
    }
    qDebug() << "process the page of " << page;
    select_ids.clear();
    primary_key_condations.clear();
    last_clicked_row = -1;
    info_model->removeRows(0, info_model->rowCount());

    initContentDataByPage(page);

    for(int i = 0; i < content_data.size(); i++) {
        addOneItem(content_data.at(i));
    }
    table_view->update();
}

// 修改显示信息界面, row : 0, 1, 2, 3 ...
void MyTable::show_info_dialog(int row)
{
//    qDebug() << "test " << current_page << " " << row;
    data_struct items = content_data.at(row);
//    qDebug() << "test2 " << current_tab;
    ItemInfoDialog *dialog = new ItemInfoDialog("修改"+ subject ,dialog_titles,dialog_choices,
                                                attachmentIndex, this);
    connect(dialog, SIGNAL(change_item_success()),this,SLOT(change_item_success_slot()));
    dialog->setAttribute(Qt::WA_DeleteOnClose);  // 关闭的时候释放资源
    dialog->setMustIndex(must_index);
    dialog->setSQLiteHelper(sqlHelper);
    dialog->setCurrentTab(current_tab);
    dialog->updateCombox();
    dialog->setModal(true);
    dialog->setIsAdd(false);
    dialog->update_dialog(items);
    dialog->setClassName(class_name);
    dialog->updateParams(firm_params);
    dialog->show();

}

// 修改信息按钮事件处理
void MyTable::change_clicked()
{
    qDebug() << "clicked change ...";
    ClickableLabel* label = (ClickableLabel*) sender();
    int row = label->objectName().toInt();
    change_row_background(row);
    show_info_dialog(row);
}

// 翻页点击事件处理
void MyTable::page_change_clicked()
{
    ClickableLabel* label = (ClickableLabel*) sender();
    QString object_name = label->objectName();
    qDebug() << object_name;

    if(max_pages == 0) return;
    if(current_page == 0) current_page = 1;

    if (object_name == "next_page") {
        if(current_page == max_pages) {
            return;
        } else {
            page_lbs.at(current_page-1)->setStyleSheet("border: 2px solid #ebebeb;"
                                                       "background-color: #ffffff;");
            current_page++;
            page_lbs.at(current_page-1)->setStyleSheet("border: 2px solid #ebebeb;"
                                                       "background-color: #e4e4e4;");
            updatePageAt(current_page);
        }
    } else if (object_name == "last_page") {
        if(current_page == 1) {
            return;
        } else {
            page_lbs.at(current_page-1)->setStyleSheet("border: 2px solid #ebebeb;"
                                                       "background-color: #ffffff;");
            current_page--;
            page_lbs.at(current_page-1)->setStyleSheet("border: 2px solid #ebebeb;"
                                                       "background-color: #e4e4e4;");
            updatePageAt(current_page);
        }
    } else {
        if(current_page == object_name.toInt()) return;
        page_lbs.at(current_page-1)->setStyleSheet("border: 2px solid #ebebeb;"
                                                   "background-color: #ffffff;");
        current_page = object_name.toInt();
        page_lbs.at(current_page-1)->setStyleSheet("border: 2px solid #ebebeb;"
                                                   "background-color: #e4e4e4;");
        updatePageAt(current_page);
    }

}

void MyTable::check_box_pressed()
{
    QCheckBox * box = (QCheckBox*) sender();
    int row = box->objectName().toInt();
    qDebug() << "checkbox press " << row;
    change_row_background(row);
}

void MyTable::check_box_state_changed(int checkState)
{
    QCheckBox * box = (QCheckBox*) sender();
    int row = box->objectName().toInt();
    if(checkState == Qt::Checked) {
        select_ids.append(content_data.at(row).id);
        primary_key_condations.append(content_data.at(row).item_info.at(0));
        qDebug() << "select rows : " << select_ids;
    } else {
        select_ids.removeOne(content_data.at(row).id);
        primary_key_condations.removeOne(content_data.at(row).item_info.at(0));
        qDebug() << "select rows : " << select_ids;
    }
    qDebug() << primary_key_condations;
}

void MyTable::onTablePressed(const QModelIndex &index)
{
    qDebug() << "press row table..." << index.row();
    if (index.isValid()) {
        int row = index.row();
        change_row_background(row);
    }
}

void MyTable::change_item_success_slot()
{
    qDebug() << "change_item_success_slot ...";
    updateAllTable();
    QMessageBox::information(this, "提示", "更新成功");
}

// 修改row行的背景
void MyTable::change_row_background(int row)
{
    if (last_clicked_row == row || max_pages == 0)
        return;
    QString background_color;
    QString click_bg = "background-color:" + select_bg + ";";  // efefef

    if(last_clicked_row % 2 == 0) {
        background_color = "background-color:#ffffff;";  // fcfcfc
    } else {
        background_color = "background-color:" + alternal_bg + ";";  // ffffff
    }

    //    qDebug() << "test2_1";

    // 按压后设置背景颜色
    QCheckBox* click_check_box = (QCheckBox*)
            table_view->indexWidget(info_model->index(row, 0));
    ClickableLabel* click_change_lb = (ClickableLabel*)
            table_view->indexWidget(info_model->index(row, info_model->columnCount()-1));
    click_check_box->setStyleSheet(click_bg);
    click_change_lb->setStyleSheet(click_bg);

    //    qDebug() << "test2_2";
    if (last_clicked_row != -1) {
        // 恢复上一次选中的行的背景颜色
        QCheckBox* last_check_box = (QCheckBox*)
                table_view->indexWidget(info_model->index(last_clicked_row, 0));
        ClickableLabel* last_change_lb = (ClickableLabel*)
                table_view->indexWidget(info_model->index(last_clicked_row, info_model->columnCount()-1));
        last_check_box->setStyleSheet(background_color);
        last_change_lb->setStyleSheet(background_color);
    }
    //    qDebug() << "test2_3";
    last_clicked_row = row;
    table_view->setCurrentIndex(info_model->index(row, 0));
}

void MyTable::initCondation()
{
    condation_sql = "";
    if(current_tab == 1 && condations.size()!=0) {
        // 设备名称
        condation_sql = conductCondationSQL("firm_name", condations);
    } else if(current_tab == 2 && condations.size()!=0) {
        // 设备名称
        condation_sql = conductCondationSQL("device_name", condations);
    } else if(current_tab == 3 && condations.size()!=0) {
        // 设备名称
        condation_sql = conductCondationSQL("device_name", condations);
    } else if(current_tab == 5 && condations.size()!=0) {
        // 所属设备/部件/零配件
        condation_sql = conductCondationSQL("belong_parts", condations);
    } else if(current_tab == 6 && condations.size()!=0) {
        // 设备名称
        condation_sql = conductCondationSQL("device_name", condations);
        condation_sql += " or ";
        condation_sql += conductCondationSQL("component_name", condations);
    }
    qDebug() << condation_sql;
}

QString MyTable::conductCondationSQL(QString name, QList<QString> condations)
{
    qDebug() << condations;
    QString condation_sql = "";
    if(condations.size() == 0) return condation_sql;
    if(condations.size() >= 1) condation_sql = name + "='" + condations.at(0) + "'";
    for(int i = 1; i < condations.size(); i++) {
        condation_sql +=  " or " + name + "='" + condations.at(i)+"'";
    }
    return condation_sql;
}



