#ifndef QBETAKARTTAKUVA_H
#define QBETAKARTTAKUVA_H

#include <QQuickItem>

class QBetaKarttakuva : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(QBetaKarttakuva)

public:
    explicit QBetaKarttakuva(QQuickItem *parent = nullptr);
    ~QBetaKarttakuva() override;
};

#endif // QBETAKARTTAKUVA_H
