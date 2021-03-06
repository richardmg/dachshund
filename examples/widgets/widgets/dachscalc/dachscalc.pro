QT += widgets widgets-private
QT += quick quick-private quickcontrols2 quicktemplates2 quicktemplates2-private
QT += testlib

HEADERS       = button.h \
                calculator.h \
                itemfactory.h \
                widgetimageitem.h \
                quickbackingstore.h \
                qquickcontrolsstyle.h
SOURCES       = button.cpp \
                calculator.cpp \
                itemfactory.cpp \
                main.cpp \
                widgetimageitem.cpp \
                quickbackingstore.cpp \
                qquickcontrolsstyle.cpp

RESOURCES += 	style/widgetimage.qml \
                style/qlineedit.qml \
                style/button.qml
# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/dachscalc
INSTALLS += target
