#include "transpositiongroup.h"

TranspositionGroup::TranspositionGroup() {}

QString TranspositionGroup::writeToMode(ViewMode mode){
    QString result;
    switch (mode) {
    case ViewMode::Cycle:
        for (int i = 0; i < tp.size(); i++) {
            for (int j = 0; j < tp[i].size(); j++){
                if (tp[i].size() > 1) {
                    if (j == 0) result += "{" + QString::number(tp[i][j]) + ", ";
                    else if (j == tp[i].size() - 1)
                        result += QString::number(tp[i][j]) + "}";
                    else
                        result += QString::number(tp[i][j]) + ", ";
                }
                else result += "(" + QString::number(tp[i][j]) + ")";
            }
        } break;
    case ViewMode::Standart:
        int cnt = 0;
        for (int i = 0; i < tp.size(); i++) {
            cnt += tp[i].size();
        }
        QVector<int> tmp; tmp.resize(cnt);
        for (int i = 0; i < tp.size(); i++) {
            for (int j = 0; j < tp[i].size(); j++){
                if (tp[i].size() > 1) {
                    if (j != tp[i].size() - 1) {
                       tmp[tp[i][j] - 1] = tp[i][j + 1];
                    } else {
                        tmp[tp[i][j] - 1] = tp[i][0];
                    }
                } else {
                    tmp[tp[i][j] - 1] = tp[i][j];
                }
            }
        } result += "(";
        for (int i = 0; i < tmp.size(); i++) {
            if (i != tmp.size() - 1)
                result += QString::number(tmp[i]) + ", ";
            else result += QString::number(tmp[i]) + ")";
        }
    }
    return result;
}

void TranspositionGroup::setTask(int n, ViewMode mode)
{
    QRandomGenerator gen;
    int cnt = 0, curValue;
    QVector<int> sizes, id, tmp;
    //Рандомная инициализация весов
    while (cnt != n) {
        curValue = static_cast<int>(gen.bounded(1, (n + 1) - cnt));
        sizes.push_back(curValue);
        cnt += curValue;
    }
    //Тождественную перестановку
    for (int i = 1; i < n + 1; i++) {
        id.push_back(i);
    }
    tp.clear();
    for (int i = 0; i < sizes.size(); i++) {
        for (int j = 0; j < sizes[i]; j++) {
            curValue = static_cast<int>(gen.bounded(0, id.size()));
            tmp.push_back(id[curValue]);
            id.removeAt(curValue);
        }
        tp.push_back(tmp);
        tmp.clear();
    }
    this->mode = mode;
    return;
}
int TranspositionGroup::getTask(){
    int cnt = 0;
    for (int i = 0; i < tp.size(); i++) {
        cnt += tp[i].size();
    }
    return cnt;
}
ViewMode TranspositionGroup::getViewMode(){
    return mode;
}
