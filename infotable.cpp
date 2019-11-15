#include "infotable.h"

InfoTable::InfoTable(QSize size, QList<QString> titles, QWidget *parent)
    : QWidget(parent), size(size), titles(titles)
{
    per_page_items = 15;
    InitTitle();

    QWidget *split = new QWidget();
    split->setFixedSize(size.width(), 1);
    split->setStyleSheet("background-color:#e4e4e4");

    QList<QString> items;
    items.append("523213");
    items.append("2019SW628086");
    items.append("张三");
    items.append("027-9128022");
    items.append("15956986328");
    items.append("6358974562@qq.com");
    items.append("6358974562");
    items.append("3C");
    items.append("品牌设计，策划等等等...");


    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->setContentsMargins(20,0,20,0);
    layout->addLayout(head_layout);
    layout->addWidget(split);
    for(int i=0;i<7;i++) {
        layout->addWidget(addOneItem(items, false),1,Qt::AlignVCenter);
        layout->addWidget(addOneItem(items, true),1,Qt::AlignVCenter);
    }
    layout->addWidget(addOneItem(items, false),1,Qt::AlignVCenter);



    this->setLayout(layout);
}

void InfoTable::setTableTitle(QList<QString> titles)
{
    this->titles = titles;
}

void InfoTable::setSize(QSize size)
{
    this->size = size;
    this->setSize(size);
}

void InfoTable::InitTitle()
{
    per_column_width = (size.width()-40)/(titles.size() + 1);
    per_row_heigh = size.height()/10;

    head_layout = new QHBoxLayout();
    head_layout->setMargin(0);
    head_layout->setSpacing(0);
    QCheckBox *box = new QCheckBox();
//    box->setFixedWidth(per_column_width);
    head_layout->addWidget(box);
    title_wgs.append(box);
//    box->setVisible(false);
    foreach(QString title, titles) {
        QLabel *label = new QLabel(title);
        label->setAlignment(Qt::AlignHCenter);

//        label->setFixedWidth(per_column_width);

        head_layout->addWidget(label);
        title_wgs.append(label);
    }

}

QWidget *InfoTable::addOneItem(QList<QString> items, bool isEven)
{
    QHBoxLayout *item_layout = new QHBoxLayout();
    item_layout->setMargin(0);
    item_layout->setSpacing(0);
    item_layout->setAlignment(Qt::AlignVCenter);

    QCheckBox *check_box = new QCheckBox();
//    check_box->setFixedWidth(per_column_width);
    item_layout->addWidget(check_box,0,Qt::AlignVCenter);
    int i = 1;
    foreach(QString item, items) {
        QLabel *label = new QLabel(item);
        label->setAlignment(Qt::AlignHCenter);

//        label->setFixedWidth(per_column_width);

        item_layout->addWidget(label,0,Qt::AlignVCenter);
        i++;
    }

    QPixmap map(":/image/change");
    map = map.scaled(20,20);
    QLabel *change_lb = new QLabel();
    change_lb->setPixmap(map);

//    change_lb->setFixedWidth(per_column_width);

    item_layout->addWidget(change_lb,0,Qt::AlignVCenter);

    QWidget *widget = new QWidget();
    widget->setLayout(item_layout);
    if(isEven) {
        widget->setStyleSheet("background-color:#ffffff;");
    } else {
        widget->setStyleSheet("background-color:#fcfcfc;");
    }
    return widget;

}
