#ifndef ERRORS_H
#define ERRORS_H

#include <QString>

enum class ErrorType {
    CANNOT_OPEN_FILE, FILE_NOT_EXISTED
};

class ErrorManager {
private:
    ErrorType type;
    QString description;
public:
    explicit ErrorManager();

    void setError(ErrorType type) {this->type = type;}
    void getDescription();
};

#endif //ERRORS_H
