#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <clickablelabel.h>
#include <QLabel>
#include <QIcon>
#include <QPushButton>
#include <QPixmap>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QComboBox>
#include <QDir>
#include <QUuid>
#include <ActiveQt/QAxObject>
#include <QTimer>
#include <QProgressDialog>

#include <infotable.h>
#include <mytable.h>

#include <util/config.h>
#include <util/sqlitehelper.h>
#include <util/dotaskthread.h>

#include "JlCompress.h"

#include <QDebug>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:

    void InitHead();                   // 初始化表头
    void InitCenter();                 // 初始化表的中间部件
    void setMainWindowLayout();        // 主窗口控件拼接

    void InitCenterLables();           // 初始化中间的选项卡
    void setClickedLabel(int index);   // 设置选中选项卡的效果

    void showDialogClassName();        // 显示要选择的数据库的类别名字
    void chooseDataBase();             // 打开文件系统选择要打开的数据库
    void saveDataBase();               // 保存导出数据
    void saveExcel();                  // 保存为Excel
    void saveAttachment();             // 保存附件

    void saveAs();                     // 另存为 选择对话框
    void initDoTaskThread();           // 初始化任务线程

    // 根据要显示的table编号获取该table显示的条件
    QList<QString> getTableCondations(int current_index);


private:
    int last_index = 0;                // 上次点击的选项卡编号
    int current_index = 0;             // 当前显示的选项卡编号 : 0, 1, 2, 3 ...


    bool isAddDataBase = false;        // 是否是创建新的数据库
    QComboBox *class_combobox;         // 大类信息选项

    SQLiteHelper *sqlHelper = nullptr; // 数据库操作对象

    QString database_path = "DataCollection.db";                   // 当前显示的数据库的路径

    DoTaskThread * doTaskThread = nullptr;

private:

    QProgressDialog* progress_dialog = nullptr;

    QDialog *class_data_dialog = nullptr;
    QDialog *save_as_dialog = nullptr;

    QLabel *logo_head_lb;              // 头部 logo
    QWidget *info_manage_head_wg;      // 头部"信息管理"按钮
    QLabel *class_label_;              // "设备大类"文字显示
    QLabel *class_label;               // 设备大类类别
//    QWidget *save_data_head_wg;        // 头部保存数据库按钮
    QPushButton *save_data_btn;      // 头部保存数据库按钮
    QPushButton *open_data_btn;      // 头部打开数据库按钮
    QPushButton *add_data_btn;       // 头部新增数据库按钮

    // 中间部件的选项卡按钮
    ClickableLabel *firm_info_center_head_lb;
    ClickableLabel *equipment_info_center_head_lb;
    ClickableLabel *component_info_center_head_lb;
    ClickableLabel *sparepart_info_center_head_lb;
    ClickableLabel *tool_center_head_lb;
    ClickableLabel *technical_data_center_head_lb;
    ClickableLabel *repair_center_head_lb;

    QHBoxLayout *center_layout;           // 中间所有控件的布局
    QVBoxLayout * center_;                // 中间的表格控件所属的布局

    QList<ClickableLabel *> center_label_list;   // 选项卡按钮集合
    QMap<int, MyTable *> center_table_list;      // 中间表格 table 控件 Map 集合

private slots:
    void center_label_clicked();        // 选项卡按钮点击事件
    void button_clicked();              // 新增、删除按钮点击事件
    void add_item_success_slot();       // 新增信息成功后的监听事件
    void dialog_class_confirm();        // 弹窗选择大类别
    void dialog_save_as();              // 另存为选择对话框

    void saveSuccess(QString message);  // 保存成功
    void saveError(QString message);    // 保存失败

};

#endif // MAINWINDOW_H
