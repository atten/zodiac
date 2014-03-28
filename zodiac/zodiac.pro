#-------------------------------------------------
#
# Project created by QtCreator 2012-01-03T01:27:17
#
#-------------------------------------------------

QT += widgets network declarative

TARGET = zodiac
TEMPLATE = app
DESTDIR = ../../bin
TRANSLATIONS = ../bin/i18n/zodiac_ru.ts \
               ../bin/i18n/zodiac_en.ts

SOURCES += src/main.cpp \
       src/mainwindow.cpp \
    src/help.cpp \
    src/slidewidget.cpp

HEADERS  += src/mainwindow.h \
    src/help.h \
    src/slidewidget.h

## win icon, etc
win32: RC_FILE = app.rc

# library dependencies
INCLUDEPATH += ../libAstroprocessor/include/

LIBS += -L../../bin
LIBS += -lastroprocessor -lfileeditor -lplain -lcircle -lplanets
