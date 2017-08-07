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
            "S", "H", "Y", "C"
        };
        input = input.toUpper();
        if (kLabels.contains(input))
        {
            return QValidator::Acceptable;
        }
    }

    return QValidator::Invalid;
}
