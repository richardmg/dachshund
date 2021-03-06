CONFIG += testcase
testcase.timeout = 800 # this test is slow
SOURCES  += ../tst_qudpsocket.cpp
INCLUDEPATH += ../../../../../shared/
QT = core network testlib

MOC_DIR=tmp

win32 {
  CONFIG(debug, debug|release) {
    DESTDIR = ../debug
} else {
    DESTDIR = ../release
  }
} else {
    DESTDIR = ../
}

TARGET = tst_qudpsocket

# Only on Linux until 'echo' has been added to docker-compose-for-{windows,macOS}.yml and tested
linux {
    QT_TEST_SERVER_LIST = danted echo
    include($$dirname(_QMAKE_CONF_)/tests/auto/testserver.pri)
}
