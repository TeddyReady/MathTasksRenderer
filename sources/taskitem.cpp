#include "taskitem.h"

TaskItem::TaskItem(const QString &label, QListWidget *parent)
    : QListWidgetItem(label, parent)
{

}


TaskItem::TaskItem(const QString &label, const QString &iconPath, QListWidget *parent)
    : QListWidgetItem(QIcon(iconPath), label, parent)
{
}
