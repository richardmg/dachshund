#include "qstyle2.h"

#include <QStyleOption>
#include <QLineEdit>
#include <QToolButton>
#include <QFocusFrame>

#include <QtWidgets/private/qwidget_p.h>
#include <QtWidgets/private/qwidgetwindow_p.h>

#include <QtQuick/qquickwindow.h>

#include <QtQuickTemplates2/private/qquicktoolbutton_p.h>
#include <QtQuickTemplates2/private/qquicktextfield_p.h>

#include "widgetimageitem.h"

#include <QDebug>

QStyle2::QStyle2(QStyle2CallbackInterface *callback)
    : callback(callback)
{
}

void *QStyle2::createControl(const QWidget *widget, void *parentControl)
{
    QQuickItem *item = itemFactory.createQuickItem(widget, static_cast<QQuickItem *>(parentControl));

    // a little hackish for now...
    void *vp = const_cast<QWidget *>(widget);
    item->setProperty("_qstyle2_src_control_", QVariant::fromValue<void *>(vp));
    if (item->metaObject()->indexOfSignal("clicked()") != -1)
        QObject::connect(item, SIGNAL(clicked()), this, SLOT(clicked()));

    return item;
}

void QStyle2::clicked()
{
    // We take a short-cut, and use the test lib to send a mouse click to the widget. In
    // the end, we should probably create QPA level events instead so that all our events
    // take the standard route through Qt, as any other QPA view.
    // Note that regardless of how we solve this, the styled control (be it controls2 or
    // native) will show that it's being clicked, even if the application chooses to filter
    // the events we produce here. We're not planning to suppress that, this is simply the
    // expected side effect of using native/scene graph/controls controls that live their
    // own life in the style. The logic of filtering a mouse click, OTOH, will work as
    // expected, meaning the widget will e.g not fire any signals or such.
    void *srcControl = qvariant_cast<void *>(sender()->property("_qstyle2_src_control_"));
    callback->controlClicked(srcControl);
}

void QStyle2::drawControl(WidgetImageItem *imageItem, const QWidget *widget) const
{
    QImage &backingstore = imageItem->m_image;
    if (backingstore.size() != widget->size()) {
        const auto window = QWidgetPrivate::get(widget->topLevelWidget())->windowHandle();
        const qreal ratio = window->devicePixelRatio();
        backingstore = QImage(widget->size() * ratio, QImage::Format_ARGB32);
        backingstore.setDevicePixelRatio(ratio);
    }
    QPainter p(&backingstore);
    callback->paintControl(widget, &p, QPoint(), widget->rect());
    imageItem->update();
}

void QStyle2::syncControl(void *dstControl, const QWidget *widget)
{
    // todo: the idea is that we pass a style options struct to this function, and
    // not a widget. And then we switch on the type to figure out how to sync.
    QQuickItem *item = static_cast<QQuickItem *>(dstControl);

    if (auto widgetButton = qobject_cast<const QToolButton *>(widget)) {
        auto button = static_cast<QQuickToolButton *>(item);
        button->setText(widgetButton->text());
    } else if (auto widgetLineEdit = qobject_cast<const QLineEdit *>(widget)) {
        auto textField = static_cast<QQuickTextField *>(item);
        textField->setText(widgetLineEdit->text());
    } else if (auto imageItem = qobject_cast<WidgetImageItem *>(item)) {
        drawControl(imageItem, widget);
    } else if (widget->isTopLevel()) {
        // The top level is currently just shown as a plain window
    } else {
        qWarning() << "Don't know how to sync:" << widget;
    }

    item->polish();
}

QSize QStyle2::controlSize(void *dstControl) const
{
    QQuickItem *item = static_cast<QQuickItem *>(dstControl);
    return QSizeF(item->implicitWidth(), item->implicitHeight()).toSize();
}

void QStyle2::resizeControl(void *dstControl, QSize size)
{
    QQuickItem *item = static_cast<QQuickItem *>(dstControl);
    if (!item->parentItem()) {
        QQuickWindow *window = item->window();
        window->resize(size);
    } else {
        item->setWidth(size.width());
        item->setHeight(size.height());
    }
    item->polish();
}

void QStyle2::moveControl(void *dstControl, QPoint pos)
{
    QQuickItem *item = static_cast<QQuickItem *>(dstControl);
    if (!item->parentItem()) {
        QQuickWindow *window = item->window();
        window->setPosition(pos);
    } else {
        item->setX(pos.x());
        item->setY(pos.y());
    }
    item->polish();
}

void QStyle2CallbackInterface::paintControl(const void *srcControl, QPainter *painter, const QPoint &targetOffset, const QRegion &sourceRegion)
{
    Q_UNUSED(srcControl);
    Q_UNUSED(painter);
    Q_UNUSED(targetOffset);
    Q_UNUSED(sourceRegion);
}

void QStyle2CallbackInterface::controlClicked(void *srcControl)
{
    Q_UNUSED(srcControl);
}
