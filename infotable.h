#ifndef INFOTABLE_H
#define INFOTABLE_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>

class InfoTable : public QWidget
{
    Q_OBJECT
public:
    explicit InfoTable(QSize size, QList<QString> titles, QWidget *parent = nullptr);

public:
    void setTableTitle(QList<QString> titles);
    void setSize(QSize size);

private:
    void InitTitle();
    QWidget* addOneItem(QList<QString> items, bool isEven);

private:
    QList<QString> titles; // 界面的标题
    QSize size;            // 界面的大小
    int per_column_width;  // 每列的宽度
    int per_row_heigh;     // 每行的高度
    int per_page_items;    // 每页显示的条目数

    QList<QWidget*> title_wgs;

private:
    QHBoxLayout *head_layout;

signals:

public slots:
};

#endif // INFOTABLE_H
