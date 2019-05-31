#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H

#include <QWidget>
#include <QQuickItem>

class ItemFactory : public QObject
{
    Q_OBJECT

public:
    QQuickItem *createQuickItem(const QWidget *widget, QQuickItem *parentItem);

private:
    QUrl styleUrl(const QString &name) const;
    QUrl styleUrlForWidget(const QWidget *widget) const;

private:
    mutable QQmlEngine *engine = nullptr;
};

#endif // ITEMFACTORY_H
