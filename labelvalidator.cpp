#include "labelvalidator.h"
#include <QStringList>
#include "labels.h"


QValidator::State LabelValidator::validate(QString &input, int &) const
{
    if (input.size() == 0)
    {
        return QValidator::Intermediate;
    }
    else if (input.size() == 1)
    {
        const QStringList &kLabels = GetAnnotationLabels();

        input = input.toUpper();
        if (kLabels.contains(input))
        {
            return QValidator::Acceptable;
        }
    }

    return QValidator::Invalid;
}
