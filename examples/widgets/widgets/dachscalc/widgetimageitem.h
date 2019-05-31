#ifndef WIDGETIMAGEITEM_H
#define WIDGETIMAGEITEM_H

#include <QtQuick/qquickitem.h>
#include <QImage>

class WidgetImageItem : public QQuickItem
{
    Q_OBJECT

public:
    WidgetImageItem(QQuickItem *parent);

    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;

public:
    QImage m_image;
};

#endif // WIDGETIMAGEITEM_H
