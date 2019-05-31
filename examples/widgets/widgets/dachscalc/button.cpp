#include "button.h"

#include <QtWidgets>

//! [0]
Button::Button(const QString &text, QWidget *parent)
    : QToolButton(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setText(text);
}
//! [0]

//! [1]
QSize Button::sizeHint() const
//! [1] //! [2]
{
    QSize size = QToolButton::sizeHint();
    size.rheight() += 20;
    size.rwidth() = qMax(size.width(), size.height());
    return size;
}

void Button::paintEvent(QPaintEvent *e)
{
//    qDebug() << "paint button" << this;
    QToolButton::paintEvent(e);
}
//! [2]
