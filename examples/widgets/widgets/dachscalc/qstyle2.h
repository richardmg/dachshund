#ifndef QSTYLE2_H
#define QSTYLE2_H

#include <QProxyStyle>
#include "itemfactory.h"

// Todo: create QStyleOption2 structs that contains all the information
// needed instad of relying on QWidget in the following API. QStyle2
// should in the end be completly independent of QtWidgets.

class WidgetImageItem;

class QStyle2CallbackInterface
{
public:
    virtual ~QStyle2CallbackInterface() {}
    virtual void paintControl(const void *srcControl, QPainter *painter, const QPoint &targetOffset, const QRegion &sourceRegion);
    virtual void controlClicked(void *srcControl);
};

class QStyle2 : public QObject
{

    Q_OBJECT

public:
    QStyle2(QStyle2CallbackInterface *callback);

    void *createControl(const QWidget *widget, void *parentControl);
    void syncControl(void *dstControl, const QWidget *widget);
    QSize controlSize(void *dstControl) const;
    void resizeControl(void *dstControl, QSize size);
    void moveControl(void *dstControl, QPoint pos);

private slots:
    void clicked();

private:
    ItemFactory itemFactory;
    QStyle2CallbackInterface *callback;

    void drawControl(WidgetImageItem *imageItem, const QWidget *widget) const;
};

#endif // QSTYLE2_H
