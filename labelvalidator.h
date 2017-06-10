#ifndef LABELVALIDATOR_H
#define LABELVALIDATOR_H

#include <QValidator>


class LabelValidator : public QValidator
{
public:

    State validate(QString &, int &) const override;
};

#endif // LABELVALIDATOR_H
