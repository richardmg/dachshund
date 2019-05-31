#include "widgetimageitem.h"

#include <QtQuick/qsgimagenode.h>
#include <QtQuick/private/qquickitem_p.h>

WidgetImageItem::WidgetImageItem(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents);
}

QSGNode *WidgetImageItem::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *)
{
    if (!node) {
        QQuickItemPrivate *d = QQuickItemPrivate::get(this);
        node = d->sceneGraphContext()->createImageNode();
    }

    QSGImageNode *imageNode = static_cast<QSGImageNode *>(node);

    // TODO: can I reuse the same texture instead of recreating it all the time?
    if (auto texture = imageNode->texture())
        delete texture;

    imageNode->setTexture(window()->createTextureFromImage(m_image, QQuickWindow::TextureCanUseAtlas));
    imageNode->setRect(QRectF(0, 0, width(), height()));

    return imageNode;
}
