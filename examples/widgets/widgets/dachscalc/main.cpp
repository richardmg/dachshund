#include <QApplication>
#include <QQuickStyle>

#include "calculator.h"
#include "adaptorstyle.h"

/*
 * Issues:
 * - I start to think that what this really ends up being is an abstract backingstore
 *    - the idea is that the style also owns the backing store implementation, since
 * 	       a style will be tightly coupled with the control set scene graph.
 *    - since we'll need to have markDirty functions
 *    - and this will drive the control set to repaint/sync.
 *    - this will be how widgets already works, except that now the backing store will
 * 	       be a part of the style / pluggable.
 *    - this will also match better with QtQuicks polish/update system. a QuickControl
 *         will basically just forward an update call to the style.
 *    - Doing it this way will stop the recursive repaints from happending, since we
 *         never sync the backinstore.
 * - Since we use a separate window, we currently need to forward/handle mouse events
 * - Create a fall-back path for custom widgets that the style doesn't have controls for,
 * 		or that does more painting in paintEvent(). If we can detect those widgets, draw
 * 		them to a QImage to put inside an Image item (simple), or cooperate with the
 * 		widget backingstore to optimize drawing (dirty regions, obscured widgets, etc etc).
 */

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle(new AdaptorStyle);

    QQuickStyle::setStyle("Material");
//    QQuickStyle::setStyle("Universal");

    Calculator calc;
    calc.show();
    calc.raise();

    return app.exec();
}
