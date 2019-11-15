#ifndef ITEMINFODIALOG_H
#define ITEMINFODIALOG_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QMap>
#include <QFileDialog>
#include <QDir>

#include <clickablelabel.h>
#include <util/sqlitehelper.h>

#include <QDebug>

class ItemInfoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ItemInfoDialog(QString subject, QList<QString>titles,
                            QMap<int, QList<QString>> choices,
                            int attachmentIndex,
                            QWidget *parent);

    ~ItemInfoDialog();

    void setIsAdd(bool isAdd) {this->isAdd = isAdd;}
    void setCurrentTab(int current_tab) {this->current_tab = current_tab;}
    void set_titles(QList<QString> titles) {this->titles = titles;}
    void set_subject(QString subject) {this->subject = subject;}
    void setSQLiteHelper(SQLiteHelper *sqlHelper) {this->sqlHelper = sqlHelper;}
    void setClassName(QString class_name) {this->class_name = class_name;}
    void setMustIndex(QList<int> must_index) {
        this->must_index = must_index;
        updateBorderOfMustLineEdit();
    }


    void update_dialog(data_struct items);    // 更新界面数据

    void updateCombox();
    void updateParams(QMap<int, QList<QString>> params);  // 更新参数的提示信息
    QString copyAttachment(QString filePath);  // 复制附件，并返回最终的附件名字


private:
    QWidget *parent_widget = nullptr;     // 父容器控件

private:

    bool isAdd = false;                   // 是否是新增, (新增/修改)
    int current_tab = 0;                  // 当前显示的信息所属的 table 编号  0, 1, 2, 3, 4 ...

    SQLiteHelper *sqlHelper = nullptr;    // 数据库操作类

    int per_column_rows = 10;             // 每列最多的条目数
    int widget_height = 600;              // 弹窗的高
    int widget_width = 600;               // 弹窗的宽度
    int label_width = 100;                // label 的宽度


    QString subject;                      // 弹窗的主题信息, 即主界面中的选项卡按钮的信息
    QList<QString> titles;                // 弹窗中所有的标题
    QMap<int, QList<QString>> choices;    // 一些必填的固定选择
    QList<int> must_index;                // 必填项序号
    QMap<int, QLineEdit*> line_edits;     // 多有的编辑框, 对于每一个标题有一个对应的编辑框
    QMap<int, QComboBox*> comboxes;       // 选择框
    QLineEdit *attachment_et;             // 附件的标签
    int attachmentIndex;                  // 附件所在的控件编号

    data_struct item;                     // 待显示的数据信息

    QString class_name;                   // 现在打开的大类名字  : EHL系统, HAQ与FSPB防护系统, ...


private:

    void contruct_dialog();               // 构建弹窗
    void contruct_one_column_dialog();    // 构建只包含一列信息的弹窗
    void contruce_two_column_dialog();    // 构建包含两列信息的弹窗
    void updateBorderOfMustLineEdit();    // 更新所有必填项的边框
    QList<QString> checkMustIndex();      // 检查必填项是否填写，并返回填写结果

    bool checkBelongParts(QString belong_parts);

    bool checkCondation(QList<QString> item_info);

signals:
    void add_item_success();              // 新增信息成功信号
    void change_item_success();           // 修改信息成功信号

public slots:
    void btn_clicked();                   // 弹窗中按钮点击事件监听
    void box_changed(const QString & item);    // 选中某个条目的事件
    void choose_attachment();                  // 选择附件文件
};

#endif // ITEMINFODIALOG_H
