#include "qquickscenegraphadaptor.h"

#include <QStyleOption>
#include <QLineEdit>
#include <QToolButton>
#include <QFocusFrame>

#include <QQuickStyle>
#include <QtQuickTemplates2/private/qquicktoolbutton_p.h>
#include <QtQuickTemplates2/private/qquicktextfield_p.h>

#include <QTest>

#include <QDebug>

void QQuickSceneGraphAdaptor::markDirty(const QWidget *widget, const QRegion &region, bool updateNow)
{
    Q_UNUSED(region);
    Q_UNUSED(updateNow);

    // Resolve the root control for the dirty widget
    const QWidget *controlWidget = widget;
    void *control = resolveControl(controlWidget);
    while (controlWidget && !control) {
        controlWidget = controlWidget->parentWidget();
        control = resolveControl(controlWidget);
    }

    if (!control)
        return;

    controlsStyle->syncControl(control, controlWidget);
}

void *QQuickSceneGraphAdaptor::resolveControl(const QWidget *widget) const
{
    if (!widget)
        return nullptr;

    if (knownWidgets.contains(widget))
        return knownWidgets.value(widget);

    // We only create controls for the window, and the children of the window (for now)
    if (widget->parentWidget() && !widget->parentWidget()->isTopLevel())
        return nullptr;

    // Ignore transparent widgets for now
    if (qobject_cast<const QFocusFrame *>(widget))
        return nullptr;

    void *control = controlsStyle->createControl(widget, resolveControl(widget->parentWidget()));
    if (!control)
        return nullptr;

    controlsStyle->resizeControl(control, widget->size());
    controlsStyle->moveControl(control, widget->pos());

    knownWidgets.insert(widget, control);
    const_cast<QWidget *>(widget)->installEventFilter(const_cast<QQuickSceneGraphAdaptor *>(this));

    return control;
}

bool QQuickSceneGraphAdaptor::eventFilter(QObject *watched, QEvent *event)
{
    QWidget *widget = qobject_cast<QWidget *>(watched);

    switch (event->type()) {
    case QEvent::Resize: {
        void *control = resolveControl(widget);
        controlsStyle->resizeControl(control, widget->size());
        break; }
    case QEvent::Move: {
        void *control = resolveControl(widget);
        if (widget->isTopLevel())
            controlsStyle->moveControl(control, widget->geometry().topLeft() + QPoint(widget->frameGeometry().width() + 50, 0));
        else
            controlsStyle->moveControl(control, widget->pos());
        break; }
    default:
        break;
    }

    return false;
}

void QQuickSceneGraphAdaptor::paintControl(const void *srcControl, QPainter *painter, const QPoint &targetOffset, const QRegion &sourceRegion)
{
    QWidget *w = static_cast<QWidget *>(const_cast<void *>(srcControl));
    w->setAttribute(Qt::WA_WState_Visible);
    w->render(painter, targetOffset, sourceRegion);
}

void QQuickSceneGraphAdaptor::controlClicked(void *srcControl)
{
    QWidget *widget = static_cast<QWidget *>(srcControl);
    QTest::mouseClick(widget, Qt::LeftButton);
}

void QQuickSceneGraphAdaptor::setStyle(const QString &style)
{
    // For now we mix QStyle and AbstactSceneGraph into one, just
    // so we don't need two plugins while prototyping
    QQuickStyle::setStyle(style);
}

QSize QQuickSceneGraphAdaptor::sizeFromContents(QStyle::ContentsType type, const QStyleOption *opt, const QSize &contentsSize, const QWidget *widget) const
{
    // Here we choose whether to use the size hint of the control or the widget. It
    // would probably make most sense to always use the size of the control, but
    // to make the UI more equal to widgets for demo purposes, we mix a little.
    QSize size;

    switch (type) {
    case CT_LineEdit: {
        if (void *control = resolveControl(widget))
            size = controlsStyle->controlSize(control);
        break; }
    default:
        size = QProxyStyle::sizeFromContents(type, opt, contentsSize, widget);
        break;
    }

    return size;
}
