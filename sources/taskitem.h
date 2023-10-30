#ifndef TASKITEM_H
#define TASKITEM_H
#include "environment.h"

class TaskItem : public QListWidgetItem {
public:
    explicit TaskItem(const QString &label, QListWidget *parent = nullptr);
    explicit TaskItem(const QString &label, const QString &iconPath, QListWidget *parent = nullptr);
};

#endif // TASKITEM_H
