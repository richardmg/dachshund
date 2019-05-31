#ifndef ADAPTORSTYLE_H
#define ADAPTORSTYLE_H

#include <QProxyStyle>
#include <QPointer>

#include "qquickcontrolsstyle.h"

class QuickBackingStore : public QProxyStyle, QStyle2CallbackInterface
{
    Q_OBJECT

public:

    QSize sizeFromContents(QStyle::ContentsType type, const QStyleOption *opt, const QSize &contentsSize,
                           const QWidget *widget = nullptr) const override;

    bool eventFilter(QObject *watched, QEvent *event) override;

    bool isNewAndFancy() override { return true; }
    void markDirty(const QWidget *widget, const QRegion &region, bool updateNow = false) override;

    // QStyle2CallbackInterface
    void paintControl(const void *srcControl, QPainter *painter, const QPoint &targetOffset, const QRegion &sourceRegion) override;
    void controlClicked(void *srcControl) override;

private:
    mutable QPointer<QQuickControlsStyle> controlsStyle = new QQuickControlsStyle(this);
    mutable QHash<const QWidget *, void *> knownWidgets;

    void *resolveControl(const QWidget *widget) const;
};

#endif // ADAPTORSTYLE_H
