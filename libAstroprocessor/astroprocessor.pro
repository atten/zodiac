#-------------------------------------------------
#
# Project created by QtCreator 2013-03-27T01:47:20
#
#-------------------------------------------------

QT += widgets

TARGET = astroprocessor
TEMPLATE = lib
DESTDIR = ../../bin
TRANSLATIONS = ../bin/i18n/astroprocessor_ru.ts \
               ../bin/i18n/astroprocessor_en.ts

SOURCES += \
    src/appsettings.cpp \
    src/astro-gui.cpp \
    src/astro-output.cpp \
    src/astro-data.cpp \
    src/astro-calc.cpp \
    src/csvreader.cpp

HEADERS +=\
    src/appsettings.h \
    src/astro-gui.h \
    src/astro-output.h \
    src/astro-data.h \
    src/astro-calc.h \
    include/Astroprocessor/Output \
    include/Astroprocessor/Gui \
    include/Astroprocessor/Data \
    include/Astroprocessor/Calc \
    src/csvreader.h


## library dependencies
INCLUDEPATH += ../libSwisseph

LIBS += -L../../bin
LIBS += -lswe


