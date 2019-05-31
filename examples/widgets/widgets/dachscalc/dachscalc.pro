QT += widgets widgets-private
QT += quick quick-private quickcontrols2 quicktemplates2 quicktemplates2-private
QT += testlib

HEADERS       = button.h \
                adaptorstyle.h \
                calculator.h \
                itemfactory.h \
                qstyle2.h \
                widgetimageitem.h
SOURCES       = button.cpp \
                adaptorstyle.cpp \
                calculator.cpp \
                itemfactory.cpp \
                main.cpp \
                qstyle2.cpp \
                widgetimageitem.cpp

RESOURCES += 	style/widgetimage.qml \
                style/qlineedit.qml \
                style/button.qml
