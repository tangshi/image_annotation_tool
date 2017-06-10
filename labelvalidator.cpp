#include "labelvalidator.h"
#include <QStringList>


QValidator::State LabelValidator::validate(QString &input, int &) const
{
    if (input.size() == 0)
    {
        return QValidator::Intermediate;
    }
    else if (input.size() == 1)
    {
        const static QStringList kLabels {
            "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
            "A", "B", "C", "D", "E", "F", "G",
            "H", "I", "J", "K", "L", "M", "N",
            "O", "P", "Q", "R", "S", "T",
            "U", "V", "W", "X", "Y", "Z"
        };
        input = input.toUpper();
        if (kLabels.contains(input))
        {
            return QValidator::Acceptable;
        }
    }

    return QValidator::Invalid;
}
