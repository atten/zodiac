QT += network widgets

TEMPLATE = lib
DESTDIR = ../../bin
TRANSLATIONS = ../bin/i18n/fileeditor_ru.ts \
               ../bin/i18n/fileeditor_en.ts

SOURCES += \
    src/fileeditor.cpp \
    src/geosearch.cpp
HEADERS += \
    src/fileeditor.h \
    ../libAstroprocessor/src/astro-gui.h \
    src/geosearch.h



# library dependencies
INCLUDEPATH += ../libAstroprocessor/include/

LIBS += -L../../bin
LIBS += -lastroprocessor

































