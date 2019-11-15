#ifndef SHOWPROGRESSDIALOG_H
#define SHOWPROGRESSDIALOG_H

#include <QObject>
#include <QThread>
#include <QProgressDialog>
#include <QTimer>
#include <QDir>
#include <QMessageBox>

#include <ActiveQt/QAxObject>
#include "JlCompress.h"

#include <util/sqlitehelper.h>

#include <QDebug>

class DoTaskThread : public QThread
{
    Q_OBJECT

public:
    enum SaveType{
        SaveExcel,
        SaveAttachment
    };

public:
    explicit DoTaskThread(QObject *parent=nullptr,QProgressDialog *progress = nullptr);
    ~DoTaskThread();

    void setType(SaveType type) {this->type = type;}
    void setFileName(QString fileName) {this->fileName = fileName;}
    void setClassName(QString class_name) {this->class_name = class_name;}
    void setSQLHelper(SQLiteHelper *sqlHelper) {this->sqlHelper = sqlHelper;}
    void setTableSubject(QList<QString> table_subjects) {this->table_subjects=table_subjects;}
    void setDialogTableTitle(QList<QList<QString>> dialog_tables_titles){
        this->dialog_tables_titles = dialog_tables_titles;
    }
    void setMustTableIndex(QList<QList<int>> must_tables_index){
        this->must_tables_index = must_tables_index;
    }



protected:
    void run();

private:
    void saveExcel();
    void saveAttachment();

private:
    int currentValue = 0;

    SaveType type;

    QString fileName;
    QString class_name;

    SQLiteHelper *sqlHelper = nullptr;

    QList<QString> table_subjects;
    QList<QList<QString>> dialog_tables_titles;
    QList<QList<int>> must_tables_index;


private:
    QTimer *timer = nullptr;
    QProgressDialog *progress_dialog = nullptr;

signals:
    void timerStart();       // 开始进度条
    void updateProgress();   // 更新进度条
    void convert_over();     // 转换结束

    void save_success(QString message); // 保存成功
    void save_error(QString message);   // 保存失败

private slots:
    void startTimer();
    void convert_data_over();  // 导出数据结束
    void valueChanged();       // 更新进度条的数据
    void cancel();             // 进度条取消按钮
};

#endif // SHOWPROGRESSDIALOG_H
