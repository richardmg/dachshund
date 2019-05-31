#include "itemfactory.h"
#include "widgetimageitem.h"

#include <QQmlContext>
#include <QQmlComponent>
#include <QQmlEngine>

#include <QQuickWindow>

#include <QDebug>

QUrl ItemFactory::styleUrl(const QString &name) const
{
    // Convert the name into the name of a qml style file
    return QUrl(QStringLiteral("qrc:/style/%1.qml").arg(name));
}

QUrl ItemFactory::styleUrlForWidget(const QWidget *widget) const
{
    // Convert the class name into the name of a qml style file
    const QString className(widget->metaObject()->className());
    return styleUrl(className.toLower());
}

QQuickItem *ItemFactory::createQuickItem(const QWidget *widget, QQuickItem *parentItem)
{
    if (!parentItem) {
        // Special case! Create root window and return content item
        QQuickWindow *window = new QQuickWindow();
        QQuickItem *contentItem = window->contentItem();
        window->show();
        return contentItem;
    }

    QQuickItem *item = nullptr;
    QUrl styleItemUrl = styleUrlForWidget(widget);
    if (!engine)
        engine = new QQmlEngine;
    QQmlComponent component(engine, styleItemUrl);

    if (component.status() == QQmlComponent::Ready) {
        // We found a QML file that matches the class name of the widget
        item = qobject_cast<QQuickItem *>(component.create());
        if (!item) {
            qWarning() << "Could not create control for widget:" << widget << component.errorString();
            return nullptr;
        }
        //            qDebug() << "create control for:" << widget;
        item->setParentItem(parentItem);
    } else {
        // We found no QML file that matches the class name of the widget. In
        // that case we take the fallback path, and draw the widget to a texture
        // in the scene graph instead.
        //            qDebug() << "create widget image for:" << widget;
        item = new WidgetImageItem(parentItem);
    }

    return item;
}
