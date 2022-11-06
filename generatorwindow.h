#ifndef GENERATORWINDOW_H
#define GENERATORWINDOW_H

#include <QMainWindow>
#include "eulerfunction.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GeneratorWindow; }
QT_END_NAMESPACE

class GeneratorWindow : public QMainWindow
{
    Q_OBJECT

public:
    GeneratorWindow(QWidget *parent = nullptr);
    ~GeneratorWindow();

private slots:
    //void on_tasksList_itemDoubleClicked(QListWidgetItem *item);

private:
    EulerFunction eulerFunction;

    Ui::GeneratorWindow *ui;
};
#endif // GENERATORWINDOW_H
