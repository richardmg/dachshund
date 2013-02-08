CONFIG(release, debug|release):DEFINES *= NDEBUG
DEFINES += SQLITE_OMIT_LOAD_EXTENSION SQLITE_OMIT_COMPLETE SQLITE_ENABLE_FTS3 SQLITE_ENABLE_FTS3_PARENTHESIS SQLITE_ENABLE_RTREE
!contains(CONFIG, largefile):DEFINES += SQLITE_DISABLE_LFS
winrt: DEFINES += SQLITE_OS_WINRT
INCLUDEPATH +=  $$PWD/sqlite
SOURCES +=      $$PWD/sqlite/sqlite3.c

TR_EXCLUDE += $$PWD/*
