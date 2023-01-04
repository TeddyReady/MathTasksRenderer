#include "transpositiongroup.h"

TranspositionGroup::TranspositionGroup() {}

TranspositionGroup::TranspositionGroup(QVector<std::pair<int, int>> data){
    std::sort(data.begin(), data.end());
    QVector<int> multi, single;
    for (int i = 0; i < data.size(); i++) {
        bool access = true;
        if (data[i].first != data[i].second) {
            for (int K = 0; K < tp.size(); K++) {
                for (int k = 0; k < tp[K].size(); k++) {
                    if (tp[K][k] == data[i].second) {
                        access = false;
                        break;
                    }
                }
                if (!access) break;
            }
            if (access) {
                multi.push_back(data[i].second);
                for (int j = data[i].second;;) {
                    multi.push_back(data[j - 1].second);
                    if (data[j - 1].second == data[i].first) {
                        break;
                    } else {
                        j = data[j - 1].second;
                    }
                }
                tp.push_back(multi);
                multi.clear();
            }
        } else {
            single.push_back(data[i].second);
            tp.push_back(single);
            single.clear();
        }
    }
}

TranspositionGroup TranspositionGroup::operator *(TranspositionGroup &trans)
{
    QVector<std::pair<int, int>> result;
    for (int I = 0; I < tp.size(); I++) {
        for (int i = 0; i < tp[I].size(); i++) {
            for (int J = 0; J < trans.getTransposition().size(); J++) {
                for (int j = 0; j < trans.getTransposition()[J].size(); j++) {

                    if (tp[I].size() > 1) {
                        if (trans.getTransposition()[J].size() > 1) {
                            if (i + 1 != tp[I].size()) {
                                if (tp[I][i + 1] == trans.getTransposition()[J][j]) {
                                    if (j + 1 != trans.getTransposition()[J].size()) {
                                        result.push_back(std::make_pair(tp[I][i], trans.getTransposition()[J][j + 1]));
                                    } else {
                                        result.push_back(std::make_pair(tp[I][i], trans.getTransposition()[J][0]));
                                    }
                                }
                            } else {
                                if (tp[I][0] == trans.getTransposition()[J][j]) {
                                    if (j + 1 != trans.getTransposition()[J].size()) {
                                        result.push_back(std::make_pair(tp[I][i], trans.getTransposition()[J][j + 1]));
                                    } else {
                                        result.push_back(std::make_pair(tp[I][i], trans.getTransposition()[J][0]));
                                    }
                                }
                            }
                        } else {
                            if (i + 1 != tp[I].size()) {
                                if (tp[I][i + 1] == trans.getTransposition()[J][j])
                                    result.push_back(std::make_pair(tp[I][i], trans.getTransposition()[J][j]));
                            } else {
                                if (tp[I][0] == trans.getTransposition()[J][j])
                                    result.push_back(std::make_pair(tp[I][i], trans.getTransposition()[J][j]));
                            }
                        }
                    } else {
                        if (trans.getTransposition()[J].size() > 1) {
                            if (tp[I][i] == trans.getTransposition()[J][j]) {
                                if (j + 1 != trans.getTransposition()[J].size()) {
                                    result.push_back(std::make_pair(tp[I][i], trans.getTransposition()[J][j + 1]));
                                } else {
                                    result.push_back(std::make_pair(tp[I][i], trans.getTransposition()[J][0]));
                                }
                            }
                        } else {
                            if (tp[I][i] == trans.getTransposition()[J][j])
                                result.push_back(std::make_pair(tp[I][i], trans.getTransposition()[J][j]));
                        }
                    }

                }
            }
        }
    }
    TranspositionGroup tmp(result);
    return tmp;
}

TranspositionGroup TranspositionGroup::operator ~(){
    QVector<std::pair<int, int>> result;
    for (int I = 0; I < tp.size(); I++) {
        for (int i = 0; i < tp[I].size(); i++) {
            if (tp[I].size() > 1) {
                if (i + 1 != tp[I].size())
                    result.push_back(std::make_pair(tp[I][i], tp[I][i + 1]));
                else result.push_back(std::make_pair(tp[I][i], tp[I][0]));
            } else {
                result.push_back(std::make_pair(tp[I][i], tp[I][i]));
            }
        }
    }
    for (int i = 0; i < result.size(); i++) {
        std::swap(result[i].first, result[i].second);
    }
    TranspositionGroup tmp(result);
    return tmp;
}

QString TranspositionGroup::writeToMode(ViewMode mode){
    QString result;
    switch (mode) {
    case ViewMode::Cycle:
        //сортировка нужна!
        for (int i = 0; i < tp.size(); i++) {
            for (int j = 0; j < tp[i].size(); j++){
                if (tp[i].size() > 1) {
                    if (j == 0) result += "(" + QString::number(tp[i][j]) + ", ";
                    else if (j == tp[i].size() - 1)
                        result += QString::number(tp[i][j]) + ")";
                    else
                        result += QString::number(tp[i][j]) + ", ";
                }
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
        } result += "\\left(\\begin{smallmatrix}";
        for (int i = 0; i < tmp.size(); i++) {
            result += QString::number(i + 1);
            if (i + 1 != tmp.size()) result += "&";
            else result += "\\\\";
        }
        for (int i = 0; i < tmp.size(); i++) {
            if (i != tmp.size() - 1)
                result += QString::number(tmp[i]) + " & ";
            else result += QString::number(tmp[i]) + "\\end{smallmatrix}\\right)";
        }
    }
    return result;
}

void TranspositionGroup::setTask(int n, ViewMode mode)
{
    QRandomGenerator *gen = QRandomGenerator::global();
    int cnt = 0, curValue;
    QVector<int> sizes, id, tmp;
    //Рандомная инициализация весов
    while (cnt != n) {
        curValue = static_cast<int>(gen->bounded(1, (n + 1) - cnt));
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
            curValue = static_cast<int>(gen->bounded(0, id.size()));
            tmp.push_back(id[curValue]);
            id.removeAt(curValue);
        }
        tp.push_back(tmp);
        tmp.clear();
    }
    this->mode = mode;
    return;
}
int TranspositionGroup::getTask()
{
    int cnt = 0;
    for (int i = 0; i < tp.size(); i++) {
        cnt += tp[i].size();
    }
    return cnt;
}

QVector<QVector<int>>& TranspositionGroup::getTransposition()
{
    return tp;
}

ViewMode TranspositionGroup::getViewMode()
{
    return mode;
}

QString TranspositionGroup::cycleType(){
    int maxCycle = 0;
    for (int i = 0; i < tp.size(); i++) {
        if (maxCycle < tp[i].size())
            maxCycle = tp[i].size();
    }
    QMap<int, int> result;
    while (maxCycle != 0) {
        result.insert(maxCycle, 0);
        maxCycle--;
    }
    for (int i = 0; i < tp.size(); i++) {
        result[tp[i].size()]++;
    }
    QString str = "[";
    for (int i = 1; i <= result.size(); i++) {
        if (result[i] != 0) {
            if (i != result.size())
                str += QString::number(i) + "^" + QString::number(result[i]) + ",";
            else str += QString::number(i) + "^" + QString::number(result[i]);
        }
    }
    str += "]";
    return str;
}
int TranspositionGroup::getHaos(){
    QVector<std::pair<int, int>> result;
    for (int I = 0; I < tp.size(); I++) {
        for (int i = 0; i < tp[I].size(); i++) {
            if (tp[I].size() > 1) {
                if (i + 1 != tp[I].size())
                    result.push_back(std::make_pair(tp[I][i], tp[I][i + 1]));
                else result.push_back(std::make_pair(tp[I][i], tp[I][0]));
            } else {
                result.push_back(std::make_pair(tp[I][i], tp[I][i]));
            }
        }
    } std::sort(result.begin(), result.end());
    int count = 0;
    for (int i = 0; i < result.size(); i++) {
        for (int j = i; j < result.size() - 1; j++) {
            if (result[i].second > result[j + 1].second)
                count++;
        }
    }
    return count;
}

QString TranspositionGroup::getEven(){
    return (getHaos() % 2) ? "Нечетная" : "Четная";
}

int TranspositionGroup::getOrder(){
    int order = tp[0].size();
    for (int i = 0; i < tp.size() - 1; i++) {
        order = LCM(order, tp[i + 1].size());
    }
    return order;
}
