#ifndef MYTABLE_H
#define MYTABLE_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QScrollBar>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QLabel>
#include <QPixmap>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QDebug>

#include <clickablelabel.h>
#include <iteminfodialog.h>
#include <util/sqlitehelper.h>

class MyTable : public QWidget
{
    Q_OBJECT
public:
    explicit MyTable(QList<QString> titles, SQLiteHelper *sqlHelper, int current_tab, QWidget *parent = nullptr);

    ~MyTable();

    void setSQLHelper(SQLiteHelper *sqlHelper) {this->sqlHelper = sqlHelper;}
    void setSubject(QString subject) {this->subject = subject;}
    void setDialogTitles(QList<QString> dialog_titles) {this->dialog_titles = dialog_titles;}
    void setDialogChoice(QMap<int, QList<QString>> dialog_choices){this->dialog_choices=dialog_choices;}
    void setMustIndex(QList<int> must_index) { this->must_index = must_index; }
    QList<data_struct> getContentData() {return this->content_data;}
    QList<QString> getKeyCondations() {return this->primary_key_condations;}
    void setCondations(QList<QString> condataions) {this->condations = condataions;}
    void setAttachmentIndex(int attachment) {this->attachmentIndex = attachment;}
    void setClassName(QString class_name) {this->class_name = class_name;}
    void setParams(QMap<int, QList<QString>> firm_params) {this->firm_params = firm_params;}

    int getCurrentPage() {return this->current_page;}
    int getMaxPages() {return this->max_pages;}
    int getPerPageItems() {return this->per_page_items;}

    QList<QString> getTitles(){qDebug() << titles;return titles;}
    QList<int> getSelectRows() {return this->select_ids;}

    void deleteSelectRows();                 // 删除选中的行数据
    void updateAllTable();                   // 更新整个 table 控件

    void initCondation();

private:

    QString alternal_bg = "#f2f2f2"; // efefef bbbcbd
    QString select_bg = "#d8d8d8"; // bbbcbd

    SQLiteHelper *sqlHelper = nullptr;       // 数据库操作类

    int current_tab = 0;                     // 当前显示的 table 编号  0, 1, 2, 3, 4 ...

    int per_page_items = 15;                 // 每页显示的条目数
    int max_pages = 0;                       // 总共的页数
    int num_info_in_table = 0;               // 该表中数据的总条目
    int view_max_pages = 0;                  // 显示的最多页数
    int current_page = 1;                    // 当前显示的页数  1, 2, 3, 4 ...

    int last_clicked_row = -1;               // 上次点击的行号

    QList<ClickableLabel*> page_lbs;         // 所有的页码按钮

    QString subject;                         // 该界面显示的主题,即选项卡的名字
    QList<QString> titles;                   // 表格的标题
    QList<QString> dialog_titles;            // 弹出的对话框的标题
    QMap<int, QList<QString>> dialog_choices;// 弹出对话框中的一些固定的选择
    QList<int> must_index;                   // 必填项序号
    QList<data_struct> content_data;         // 当前页显示的表格数据
    QList<int> select_ids;                   // 选中的行数据的 id

    QList<QString> primary_key_condations;   // 该 table 的 primary key 作为其他 table 的condations
    QList<QString> primary_key_condations_old;// 上一次的该页选中的 primary
    QString condation_sql = "";              // 要添加的查询sql条件
    QList<QString> condations;               // 条件的所有项

    int attachmentIndex;                     // 附件所在的空间集合编号

    QString class_name;                      // 现在打开的大类名字  : EHL系统, HAQ与FSPB防护系统, ...
    QMap<int, QList<QString>> firm_params;
//    QList<QString> firm_params;                   // 厂商信息的参数提示信息

private:
    QVBoxLayout *table_layout;               // 整个界面布局

    QTableView * table_view;                 // table view
    QStandardItemModel *info_model;          // 为table view 设置数据模式
    QWidget *page_widget;                    // 底部页码控件


private:
    void addOneItem(data_struct data);   // 向 tableview 中添加一个条目数据
    void InitTableView();                    // 初始化 TableView 控件
    void InitBottomPage();                   // 初始化底部的页码控件

    void initDataFromDB();                   // 从数据库中获取该表的基本信息，总条目数
    void initContentDataByPage(int page);    // 初始化 page 页码的数据 : 1, 2, 3, 4 ...
    void updatePageAt(int page);             // 更新 page 页的信息 : 1, 2, 3, 4 ...

    void change_row_background(int row);     // 修改第 row 行的背景颜色


    QString conductCondationSQL(QString name, QList<QString> condations);

private:
    void show_info_dialog(int row);          // 弹窗显示第 row 行的数据信息

signals:

public slots:
    void change_clicked();                        // 修改按钮事件监听
    void page_change_clicked();                   // 页码按钮事件监听

    void check_box_pressed();                     // 复选框按压事件
    void check_box_state_changed(int checkState); // 复选框状态改变事件
    void onTablePressed(const QModelIndex &);     // table 行点击事件

    void change_item_success_slot();              // 修改信息成功事件监听
};

#endif // MYTABLE_H
